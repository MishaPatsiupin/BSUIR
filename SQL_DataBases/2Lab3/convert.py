import psycopg2
import json
import bsddb3
from psycopg2.extras import RealDictCursor

# Конфигурация для вашего Docker контейнера
DB_CONFIG = {
    "dbname": "Lido",  # Оставлю как было, но вы можете поменять на имя вашей БД
    "user": "postgres", 
    "password": "1234",
    "host": "localhost",
    "port": "5432"
}

# Таблицы из вашей БД ресторана
TABLES = {
    "Order": "orderid",
    "dish": "dishid", 
    "dishingredient": ["dishid", "ingredientid"],
    "employee": "employeeid",
    "employeeposition": ["employeeid", "positionid"],
    "ingredient": "ingredientid",
    "orderdish": ["orderid", "dishid"],
    "position": "positionid",
    "supplier": "supplierid",
    "visitor": "visitorid"
}

def convert_postgres_to_berkeleydb():
    """Основная функция конвертации данных из PostgreSQL в BerkeleyDB"""
    
    try:
        conn = psycopg2.connect(**DB_CONFIG)
        cursor = conn.cursor(cursor_factory=RealDictCursor)
        
        for table_name, primary_key in TABLES.items():
            print(f"Конвертация таблицы: {table_name}")
            
            # Для таблицы "Order" используем кавычки, так как это зарезервированное слово
            if table_name == "Order":
                cursor.execute(f'SELECT * FROM "{table_name}"')
            else:
                cursor.execute(f'SELECT * FROM {table_name}')
                
            rows = cursor.fetchall()
            
            db = bsddb3.btopen(f"{table_name}.db", "c")
            
            for row in rows:
                row_dict = dict(row)
                
                if isinstance(primary_key, list):
                    key_parts = [str(row_dict[col]) for col in primary_key]
                    key_value = "_".join(key_parts)
                else:
                    key_value = str(row_dict[primary_key])
                
                json_value = json.dumps(row_dict, ensure_ascii=False, default=str)
                db[key_value.encode('utf-8')] = json_value.encode('utf-8')
            
            db.close()
            print(f"Таблица {table_name} конвертирована. Записей: {len(rows)}")
        
        cursor.close()
        conn.close()
        print("Конвертация завершена успешно!")
        
    except Exception as e:
        print(f"Ошибка при конвертации: {str(e)}")

if __name__ == "__main__":
    convert_postgres_to_berkeleydb()
