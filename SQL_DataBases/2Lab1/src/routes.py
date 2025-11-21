from flask import Blueprint, jsonify, request, send_file, g
from db import get_db_connection
from auth import auth
import psycopg2
import json
import os
import subprocess
from datetime import datetime
import logging

# Настройка логирования
logger = logging.getLogger(__name__)

# Кастомный jsonify с UTF-8
def custom_jsonify(**kwargs):
    kwargs.setdefault('ensure_ascii', False)
    return jsonify(**kwargs)  # Only keyword arguments

# Создание Blueprint'ов для каждой таблицы
orders = Blueprint('orders', __name__)
dishes = Blueprint('dishes', __name__)
dishingredients = Blueprint('dishingredients', __name__)
employees = Blueprint('employees', __name__)
employeepositions = Blueprint('employeepositions', __name__)
ingredients = Blueprint('ingredients', __name__)
orderdishes = Blueprint('orderdishes', __name__)
positions = Blueprint('positions', __name__)
suppliers = Blueprint('suppliers', __name__)
visitors = Blueprint('visitors', __name__)
queries = Blueprint('queries', __name__)
admin = Blueprint('admin', __name__)

# Список справочных таблиц, доступных только для admin
REFERENCE_TABLES = ['position', 'supplier', 'dish']

# Вспомогательная функция для проверки роли
def is_admin():
    return auth.current_user() in ['admin', 'manager1']

# Универсальная функция для фильтрации (с нормализацией UTF-8)
def apply_filters(table_name, params):
    query = f'SELECT * FROM public."{table_name}" WHERE 1=1'
    values = []
    for key, value in params.items():
        # Нормализуем значение для UTF-8 (исправляем возможные искажения)
        if isinstance(value, str):
            value = value.encode('utf-8', errors='ignore').decode('utf-8')
        if key in ['status', 'name', 'category', 'companyname', 'productcategory', 'phonenumber', 'email', 'workschedule', 'responsibilities', 'preferences', 'unitofmeasurement']:
            query += f' AND {key} ILIKE %s'
            values.append(f'%{value}%')
        elif key in ['totalcost', 'weight', 'cost', 'quantity', 'instock', 'salary', 'count', 'numberofguests', 'reliabilityrating']:
            query += f' AND {key} = %s'
            values.append(value)
        elif key in ['orderdatetime', 'hiredate', 'contractenddate', 'expirationdate', 'birthdate']:
            query += f' AND {key}::text ILIKE %s'
            values.append(f'%{value}%')
    return query, values

# Универсальная функция для обработки CRUD
def create_crud_routes(bp, table_name, primary_key, fields, is_reference=False):
    @bp.route('/', methods=['GET'])
    @auth.login_required
    def get_all():
        user = auth.username()
        role = auth.current_user()
        logger.info(f"User {user} (role: {role}) accessed endpoint: GET /{table_name}")
        try:
            conn = get_db_connection(auth.username(), g.password)
            cur = conn.cursor()
            query, values = apply_filters(table_name, request.args)
            cur.execute(query + ';', values)
            data = cur.fetchall()
            cur.close()
            conn.close()
            logger.info(f"User {user} (role: {role}) retrieved {len(data)} records from {table_name}")
            return custom_jsonify(data=data)  # Кастомный jsonify с UTF-8
        except psycopg2.Error as e:
            logger.error(f"User {user} (role: {role}) failed to retrieve records from {table_name}: {str(e)}")
            return custom_jsonify(error=str(e)), 400

    @bp.route('/<id>', methods=['GET'])
    @auth.login_required
    def get_by_id(id):
        user = auth.username()
        role = auth.current_user()
        logger.info(f"User {user} (role: {role}) accessed endpoint: GET /{table_name}/{id}")
        try:
            conn = get_db_connection(auth.username(), g.password)
            cur = conn.cursor()
            cur.execute(f'SELECT * FROM public."{table_name}" WHERE {primary_key} = %s;', (id,))
            data = cur.fetchone()
            cur.close()
            conn.close()
            if data:
                logger.info(f"User {user} (role: {role}) retrieved record {id} from {table_name}")
                return custom_jsonify(data=data)
            logger.warning(f"User {user} (role: {role}) failed to find record {id} in {table_name}")
            return custom_jsonify({'error': 'Not found'}), 404
        except psycopg2.Error as e:
            logger.error(f"User {user} (role: {role}) failed to retrieve record {id} from {table_name}: {str(e)}")
            return custom_jsonify(error=str(e)), 400

    @bp.route('/', methods=['POST'])
    @auth.login_required
    def create():
        user = auth.username()
        role = auth.current_user()
        logger.info(f"User {user} (role: {role}) accessed endpoint: POST /{table_name}")
        if is_reference and not is_admin():
            logger.warning(f"User {user} (role: {role}) attempted to modify reference table {table_name}")
            return custom_jsonify({'error': 'Only admin can modify reference tables'}), 403
        try:
            data = request.get_json()
            conn = get_db_connection(auth.username(), g.password)
            cur = conn.cursor()
            columns = ', '.join(data.keys())
            placeholders = ', '.join(['%s'] * len(data))
            query = f'INSERT INTO public."{table_name}" ({columns}) VALUES ({placeholders}) RETURNING *;'
            cur.execute(query, list(data.values()))
            conn.commit()
            result = cur.fetchone()
            cur.close()
            conn.close()
            logger.info(f"User {user} (role: {role}) created record in {table_name}: {result}")
            return custom_jsonify(result=result), 201
        except psycopg2.Error as e:
            conn.rollback()
            logger.error(f"User {user} (role: {role}) failed to create record in {table_name}: {str(e)}")
            return custom_jsonify(error=str(e)), 400

    @bp.route('/<id>', methods=['PUT'])
    @auth.login_required
    def update(id):
        user = auth.username()
        role = auth.current_user()
        logger.info(f"User {user} (role: {role}) accessed endpoint: PUT /{table_name}/{id}")
        if is_reference and not is_admin():
            logger.warning(f"User {user} (role: {role}) attempted to modify reference table {table_name}")
            return custom_jsonify({'error': 'Only admin can modify reference tables'}), 403
        try:
            data = request.get_json()
            conn = get_db_connection(auth.username(), g.password)
            cur = conn.cursor()
            updates = ', '.join([f'{k} = %s' for k in data.keys()])
            query = f'UPDATE public."{table_name}" SET {updates} WHERE {primary_key} = %s RETURNING *;'
            cur.execute(query, list(data.values()) + [id])
            conn.commit()
            result = cur.fetchone()
            cur.close()
            conn.close()
            if result:
                logger.info(f"User {user} (role: {role}) updated record {id} in {table_name}: {result}")
                return custom_jsonify(result=result)
            logger.warning(f"User {user} (role: {role}) failed to find record {id} in {table_name}")
            return custom_jsonify({'error': 'Not found'}), 404
        except psycopg2.Error as e:
            conn.rollback()
            logger.error(f"User {user} (role: {role}) failed to update record {id} in {table_name}: {str(e)}")
            return custom_jsonify(error=str(e)), 400

    @bp.route('/<id>', methods=['DELETE'])
    @auth.login_required
    def delete(id):
        user = auth.username()
        role = auth.current_user()
        logger.info(f"User {user} (role: {role}) accessed endpoint: DELETE /{table_name}/{id}")
        if is_reference and not is_admin():
            logger.warning(f"User {user} (role: {role}) attempted to modify reference table {table_name}")
            return custom_jsonify({'error': 'Only admin can modify reference tables'}), 403
        try:
            conn = get_db_connection(auth.username(), g.password)
            cur = conn.cursor()
            cur.execute(f'DELETE FROM public."{table_name}" WHERE {primary_key} = %s RETURNING *;', (id,))
            conn.commit()
            result = cur.fetchone()
            cur.close()
            conn.close()
            if result:
                logger.info(f"User {user} (role: {role}) deleted record {id} from {table_name}")
                return custom_jsonify(result=result)
            logger.warning(f"User {user} (role: {role}) failed to find record {id} in {table_name}")
            return custom_jsonify({'error': 'Not found'}), 404
        except psycopg2.Error as e:
            conn.rollback()
            logger.error(f"User {user} (role: {role}) failed to delete record {id} from {table_name}: {str(e)}")
            return custom_jsonify(error=str(e)), 400

# Определение маршрутов для каждой таблицы
create_crud_routes(orders, 'Order', 'orderid', [
    'status', 'totalcost', 'numberofguests', 'orderdatetime', 'visitorid', 'employeeid'])
create_crud_routes(dishes, 'dish', 'dishid', [
    'name', 'weight', 'nutritionalvalue', 'cost', 'category'], is_reference=True)
create_crud_routes(dishingredients, 'dishingredient', 'dishid,ingredientid', [
    'dishid', 'ingredientid', 'quantity'])
create_crud_routes(employees, 'employee', 'employeeid', [
    'fullname', 'phonenumber', 'email', 'hiredate', 'contractenddate'])
create_crud_routes(employeepositions, 'employeeposition', 'employeeid,positionid', [
    'employeeid', 'positionid'])
create_crud_routes(ingredients, 'ingredient', 'ingredientid', [
    'name', 'unitofmeasurement', 'instock', 'expirationdate', 'supplierid'])
create_crud_routes(orderdishes, 'orderdish', 'orderid,dishid', [
    'orderid', 'dishid', 'count'])
create_crud_routes(positions, 'position', 'positionid', [
    'name', 'salary', 'workschedule', 'responsibilities'], is_reference=True)
create_crud_routes(suppliers, 'supplier', 'supplierid', [
    'companyname', 'contactinformation', 'reliabilityrating', 'productcategory'], is_reference=True)
create_crud_routes(visitors, 'visitor', 'visitorid', [
    'name', 'birthdate', 'phonenumber', 'preferences'])

# Маршрут для специальных запросов и сохранения результатов
@queries.route('/execute', methods=['POST'])
@auth.login_required
def execute_query():
    user = auth.username()
    role = auth.current_user()
    logger.info(f"User {user} (role: {role}) accessed endpoint: POST /queries/execute")
    try:
        data = request.get_json()
        query = data.get('query')
        save_to_file = data.get('save_to_file', False)
        if not query:
            logger.warning(f"User {user} (role: {role}) sent empty query")
            return custom_jsonify({'error': 'Query is required'}), 400
        conn = get_db_connection(auth.username(), g.password)
        cur = conn.cursor()
        cur.execute(query)
        if query.strip().upper().startswith('SELECT'):
            results = cur.fetchall()
            logger.info(f"User {user} (role: {role}) executed SELECT query, retrieved {len(results)} records")
        else:
            conn.commit()
            results = {'message': 'Query executed successfully'}
            logger.info(f"User {user} (role: {role}) executed non-SELECT query")
        cur.close()
        conn.close()
        if save_to_file:
            filename = f'query_result_{datetime.now().strftime("%Y%m%d_%H%M%S")}.json'
            with open(filename, 'w', encoding='utf-8') as f:  # Явная UTF-8 кодировка
                json.dump(results, f, indent=2, ensure_ascii=False, default=str)
            logger.info(f"User {user} (role: {role}) saved query results to {filename}")
            return send_file(filename, as_attachment=True, mimetype='application/json; charset=utf-8')
        return custom_jsonify(results=results)
    except psycopg2.Error as e:
        logger.error(f"User {user} (role: {role}) failed to execute query: {str(e)}")
        return custom_jsonify(error=str(e)), 400

# Маршрут для создания бэкапа (только для admin)
@admin.route('/backup', methods=['POST'])
@auth.login_required
def create_backup():
    user = auth.username()
    role = auth.current_user()
    logger.info(f"User {user} (role: {role}) accessed endpoint: POST /admin/backup")
    if not is_admin():
        logger.warning(f"User {user} (role: {role}) attempted to create backup without admin privileges")
        return custom_jsonify({'error': 'Only admin can create backups'}), 403
    try:
        data = request.get_json()
        admin_password = data.get('admin_password')
        if not admin_password:
            logger.warning(f"User {user} (role: {role}) did not provide admin password")
            return custom_jsonify({'error': 'Admin password is required'}), 400
        # Проверяем пароль администратора
        try:
            conn = get_db_connection('manager1', admin_password)
            conn.close()
        except psycopg2.Error:
            logger.error(f"User {user} (role: {role}) provided invalid admin password")
            return custom_jsonify({'error': 'Invalid admin password'}), 401
        backup_file = f'backup_{datetime.now().strftime("%Y%m%d_%H%M%S")}.sql'
        subprocess.run([
            'pg_dump',
            '-U', 'manager1',
            '-h', 'localhost',
            '-p', '5432',
            '-F', 'p',
            '-f', backup_file,
            'Lido'  # Изменено на 'Lido' из вашего config
        ], env={'PGPASSWORD': admin_password}, check=True)
        logger.info(f"User {user} (role: {role}) created backup: {backup_file}")
        return send_file(backup_file, as_attachment=True, mimetype='application/sql; charset=utf-8')
    except subprocess.CalledProcessError:
        logger.error(f"User {user} (role: {role}) failed to create backup")
        return custom_jsonify({'error': 'Failed to create backup'}), 500
    except Exception as e:
        logger.error(f"User {user} (role: {role}) encountered error during backup: {str(e)}")
        return custom_jsonify(error=str(e)), 400