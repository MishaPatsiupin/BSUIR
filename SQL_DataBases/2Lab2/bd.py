import os
import json
import ctypes
from ctypes import c_char_p, c_void_p
from http.server import BaseHTTPRequestHandler, HTTPServer
from urllib.parse import urlparse, parse_qs

os.system("chcp 65001")  # UTF-8 для консоли

# ------------------ libpq setup ------------------
libpq = ctypes.cdll.LoadLibrary(r"C:\Program Files\PostgreSQL\16\bin\libpq.dll")
libpq.PQconnectdb.argtypes = [c_char_p]; libpq.PQconnectdb.restype = c_void_p
libpq.PQerrorMessage.argtypes = [c_void_p]; libpq.PQerrorMessage.restype = c_char_p
libpq.PQexec.argtypes = [c_void_p, c_char_p]; libpq.PQexec.restype = c_void_p
libpq.PQresultStatus.argtypes = [c_void_p]; libpq.PQresultStatus.restype = ctypes.c_int
libpq.PQntuples.argtypes = [c_void_p]; libpq.PQntuples.restype = ctypes.c_int
libpq.PQnfields.argtypes = [c_void_p]; libpq.PQnfields.restype = ctypes.c_int
libpq.PQgetvalue.argtypes = [c_void_p, ctypes.c_int, ctypes.c_int]; libpq.PQgetvalue.restype = c_char_p
libpq.PQstatus.argtypes = [c_void_p]; libpq.PQstatus.restype = ctypes.c_int
libpq.PQfinish.argtypes = [c_void_p]; libpq.PQfinish.restype = None
libpq.PQfname.argtypes = [c_void_p, ctypes.c_int]; libpq.PQfname.restype = c_char_p

# ------------------ Подключение ------------------
password = os.getenv("PGPASSWORD") or ""
conninfo = b"dbname=hospital user=postgres password=" + password.encode("utf-8") + b" host=127.0.0.1 port=5432"
conn = libpq.PQconnectdb(conninfo)
if not conn or libpq.PQstatus(conn) != 0:
    errmsg = libpq.PQerrorMessage(conn) if conn else b"Failed to allocate connection"
    print("Connection failed:", errmsg.decode("cp1251", errors="replace"))
    if conn:
        libpq.PQfinish(conn)
    exit(1)

# ------------------ Общие функции ------------------
def exec_sql(sql):
    res = libpq.PQexec(conn, sql.encode("utf-8"))
    if libpq.PQresultStatus(res) not in (1, 2):
        errmsg = libpq.PQerrorMessage(conn)
        raise Exception(errmsg.decode("utf-8", errors="replace"))
    return res

def fetchall(sql):
    res = exec_sql(sql)
    rows = libpq.PQntuples(res)
    cols = libpq.PQnfields(res)
    col_names = [libpq.PQfname(res, i).decode("utf-8") for i in range(cols)]
    result = []
    for i in range(rows):
        row = {}
        for j in range(cols):
            val = libpq.PQgetvalue(res, i, j)
            row[col_names[j]] = val.decode("utf-8", errors="replace")
        result.append(row)
    return result

def insert(table, data: dict):
    keys = ','.join(data.keys())
    values = ','.join(f"'{v}'" for v in data.values())
    exec_sql(f"INSERT INTO {table} ({keys}) VALUES ({values});")

def update(table, pk_name, record_id, data: dict):
    set_str = ','.join(f"{k}='{v}'" for k, v in data.items())
    exec_sql(f"UPDATE {table} SET {set_str} WHERE {pk_name}={record_id};")

def delete(table, pk_name, record_id):
    exec_sql(f"DELETE FROM {table} WHERE {pk_name}={record_id};")

def check_admin_password(password: str) -> bool:
    """
    Try to connect to DB with provided password — if connects, password is valid.
    """
    try:
        conninfo = f"dbname=hospital user=postgres password={password} host=127.0.0.1 port=5432".encode("utf-8")
        test_conn = libpq.PQconnectdb(conninfo)
        if not test_conn or libpq.PQstatus(test_conn) != 0:
            if test_conn:
                libpq.PQfinish(test_conn)
            return False
        libpq.PQfinish(test_conn)
        return True
    except Exception:
        return False

# ------------------ Таблицы ------------------
TABLES = {
    "patients": ("hospital.patients", "patient_id"),
    "appointments": ("hospital.appointments", "appointment_id"),
    "billing": ("hospital.billing", "bill_id"),
    "departments": ("hospital.departments", "department_id"),
    "doctors": ("hospital.doctors", "doctor_id"),
    "rooms": ("hospital.rooms", "room_id"),
    "doctor_department": ("hospital.doctor_department", "doctor_department_id"),
    "doctor_patient": ("hospital.doctor_patient", "doctor_patient_id"),
    "patient_room": ("hospital.patient_room", "patient_room_id"),
    "medical_records": ("hospital.medical_records", "record_id")
}

LOOKUP_TABLES = {"departments", "rooms"}

# ------------------ HTTP сервер ------------------
class Handler(BaseHTTPRequestHandler):
    def _send_json(self, data, code=200):
        self.send_response(code)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.end_headers()
        self.wfile.write(json.dumps(data, ensure_ascii=False).encode("utf-8"))

    def _extract_table_key(self, path):
        # robust extraction of table key: '/api/appointments' -> 'appointments'
        if path.startswith('/api/'):
            return path[len('/api/'):]
        return path.lstrip('/')

    def check_admin(self):
        super_pass = self.headers.get("X-Superpass")
        if not super_pass:
            return False
        return check_admin_password(super_pass)

    def do_GET(self):
        parsed = urlparse(self.path)
        table_key = self._extract_table_key(parsed.path)
        if table_key in TABLES:
            try:
                filters = parse_qs(parsed.query)
                where_clauses = [f"{k}='{v[0]}'" for k, v in filters.items() if k not in ("limit", "offset")]
                limit = int(filters.get("limit", [50])[0])
                offset = int(filters.get("offset", [0])[0])
                where_sql = f" WHERE {' AND '.join(where_clauses)}" if where_clauses else ""
                table, _ = TABLES[table_key]
                sql = f"SELECT * FROM {table}{where_sql} LIMIT {limit} OFFSET {offset};"
                data = fetchall(sql)
                self._send_json(data)
            except Exception as e:
                self._send_json({"error": str(e)}, 500)
        elif table_key == "backup":
            if not self.check_admin():
                self._send_json({"error": "Permission denied"}, 403)
                return
            os.system("pg_dump -U postgres -F c hospital > backup.dump")
            self._send_json({"status": "ok"})
        else:
            self._send_json({"error": "Unknown endpoint"}, 404)

    def do_POST(self):
        parsed = urlparse(self.path)
        table_key = self._extract_table_key(parsed.path)
        length = int(self.headers.get("Content-Length", 0))
        body = self.rfile.read(length) if length else b"{}"
        try:
            data = json.loads(body)
        except Exception:
            data = {}
        try:
            # new endpoint: check_admin
            if table_key == "check_admin":
                pw = data.get("password") if isinstance(data, dict) else None
                if not pw:
                    self._send_json({"error": "password missing"}, 400)
                    return
                ok = check_admin_password(pw)
                if ok:
                    self._send_json({"ok": True})
                else:
                    self._send_json({"error": "invalid password"}, 403)
                return

            if table_key in TABLES:
                if table_key in LOOKUP_TABLES and not self.check_admin():
                    self._send_json({"error": "Permission denied"}, 403)
                    return
                table, _ = TABLES[table_key]
                insert(table, data)
                self._send_json({"status": "ok"})
            elif table_key == "restore":
                if not self.check_admin():
                    self._send_json({"error": "Permission denied"}, 403)
                    return
                os.system("pg_restore -U postgres -d hospital backup.dump")
                self._send_json({"status": "ok"})
            else:
                self._send_json({"error": "Unknown endpoint"}, 404)
        except Exception as e:
            self._send_json({"error": str(e)}, 500)

    def do_PUT(self):
        parsed = urlparse(self.path)
        table_key = self._extract_table_key(parsed.path)
        length = int(self.headers.get("Content-Length", 0))
        body = self.rfile.read(length) if length else b"{}"
        try:
            data = json.loads(body)
        except Exception:
            data = {}
        try:
            if table_key in TABLES:
                table, pk = TABLES[table_key]
                if table_key in LOOKUP_TABLES and not self.check_admin():
                    self._send_json({"error": "Permission denied"}, 403)
                    return
                record_id = data.pop(pk)
                update(table, pk, record_id, data)
                self._send_json({"status": "ok"})
            else:
                self._send_json({"error": "Unknown endpoint"}, 404)
        except Exception as e:
            self._send_json({"error": str(e)}, 500)

    def do_DELETE(self):
        parsed = urlparse(self.path)
        table_key = self._extract_table_key(parsed.path)
        length = int(self.headers.get("Content-Length", 0))
        body = self.rfile.read(length) if length else b"{}"
        try:
            data = json.loads(body)
        except Exception:
            data = {}
        try:
            if table_key in TABLES:
                table, pk = TABLES[table_key]
                if table_key in LOOKUP_TABLES and not self.check_admin():
                    self._send_json({"error": "Permission denied"}, 403)
                    return
                record_id = data.get(pk)
                delete(table, pk, record_id)
                self._send_json({"status": "ok"})
            else:
                self._send_json({"error": "Unknown endpoint"}, 404)
        except Exception as e:
            self._send_json({"error": str(e)}, 500)

# ------------------ Запуск ------------------
if __name__ == "__main__":
    try:
        server = HTTPServer(("0.0.0.0", 8000), Handler)
        print("Server running on port 8000")
        server.serve_forever()
    finally:
        if conn:
            libpq.PQfinish(conn)
