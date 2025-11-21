from flask import Flask, jsonify, request, render_template, send_from_directory
import bsddb3
import json
import os
from typing import Dict, List, Any
from flask_swagger_ui import get_swaggerui_blueprint

app = Flask(__name__)

# Конфигурация Berkeley DB
DB_PATH = "/home/bear/db-lab/Lab4/db"  # Путь к файлам .db

# Конфигурация Swagger
SWAGGER_URL = '/api/docs'  # URL для доступа к Swagger UI
API_URL = '/static/swagger.json'  # URL нашего API спецификации

# Инициализация Swagger UI
swaggerui_blueprint = get_swaggerui_blueprint(
    SWAGGER_URL,
    API_URL,
    config={
        'app_name': "Restaurant DB API"
    }
)

app.register_blueprint(swaggerui_blueprint, url_prefix=SWAGGER_URL)

# Список таблиц (баз данных)
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

def get_db_connection(table_name: str):
    """Получение соединения с Berkeley DB"""
    db_path = os.path.join(DB_PATH, f"{table_name}.db")
    if not os.path.exists(db_path):
        return None
    return bsddb3.btopen(db_path, "r")

def decode_unicode_escapes(text: str) -> str:
    """Декодирование Unicode escape-последовательностей"""
    try:
        if '\\' in text:
            return text.encode('latin1').decode('unicode_escape').encode('latin1').decode('utf-8')
        return text
    except:
        return text

def process_record_value(value: bytes) -> Dict[str, Any]:
    """Обработка значения записи из Berkeley DB"""
    try:
        value_str = value.decode('utf-8')
        data = json.loads(value_str)
        
        # Декодируем все строковые значения
        decoded_data = {}
        for k, v in data.items():
            if isinstance(v, str):
                decoded_data[k] = decode_unicode_escapes(v)
            else:
                decoded_data[k] = v
        
        return decoded_data
    except Exception as e:
        return {"error": f"Failed to decode: {str(e)}", "raw_value": value.decode('utf-8', errors='ignore')}

# Static files for Swagger
@app.route('/static/<path:path>')
def send_static(path):
    return send_from_directory('static', path)

# API Routes
@app.route('/')
def index():
    """Главная страница"""
    return render_template('index.html')

@app.route('/api/databases')
def get_databases():
    """
    Получение списка доступных баз данных
    ---
    tags:
      - Databases
    responses:
      200:
        description: Список доступных баз данных
        schema:
          type: array
          items:
            type: string
        examples:
          application/json: ["Order", "dish", "employee", "visitor"]
    """
    databases = []
    for table in TABLES.keys():
        db_path = os.path.join(DB_PATH, f"{table}.db")
        if os.path.exists(db_path):
            databases.append(table)
    return jsonify(databases)

@app.route('/api/<table_name>')
def get_table_data(table_name: str):
    """
    Получение всех данных из указанной таблицы
    ---
    tags:
      - Data
    parameters:
      - name: table_name
        in: path
        type: string
        required: true
        enum: [Order, dish, dishingredient, employee, employeeposition, ingredient, orderdish, position, supplier, visitor]
        description: Название таблицы
    responses:
      200:
        description: Данные из таблицы
        schema:
          type: array
          items:
            type: object
        examples:
          application/json: [{"id": 1, "name": "Example", "value": "Test"}]
      404:
        description: Таблица не найдена
      500:
        description: Ошибка сервера
    """
    if table_name not in TABLES:
        return jsonify({"error": f"Table {table_name} not found"}), 404
    
    db = get_db_connection(table_name)
    if not db:
        return jsonify({"error": f"Database file for {table_name} not found"}), 404
    
    try:
        data = []
        for key, value in db.items():
            key_str = key.decode('utf-8')
            record_data = process_record_value(value)
            record_data['_key'] = key_str  # Добавляем ключ в данные
            data.append(record_data)
        
        db.close()
        return jsonify(data)
    except Exception as e:
        if db:
            db.close()
        return jsonify({"error": str(e)}), 500

@app.route('/api/<table_name>/<key>')
def get_record(table_name: str, key: str):
    """
    Получение конкретной записи по ключу
    ---
    tags:
      - Data
    parameters:
      - name: table_name
        in: path
        type: string
        required: true
        enum: [Order, dish, dishingredient, employee, employeeposition, ingredient, orderdish, position, supplier, visitor]
        description: Название таблицы
      - name: key
        in: path
        type: string
        required: true
        description: Ключ записи
    responses:
      200:
        description: Данные записи
        schema:
          type: object
      404:
        description: Запись не найдена
      500:
        description: Ошибка сервера
    """
    if table_name not in TABLES:
        return jsonify({"error": f"Table {table_name} not found"}), 404
    
    db = get_db_connection(table_name)
    if not db:
        return jsonify({"error": f"Database file for {table_name} not found"}), 404
    
    try:
        key_bytes = key.encode('utf-8')
        if key_bytes in db:
            value = db[key_bytes]
            record_data = process_record_value(value)
            record_data['_key'] = key
            db.close()
            return jsonify(record_data)
        else:
            db.close()
            return jsonify({"error": "Record not found"}), 404
    except Exception as e:
        if db:
            db.close()
        return jsonify({"error": str(e)}), 500

@app.route('/api/<table_name>', methods=['POST'])
def create_record(table_name: str):
    """
    Создание новой записи
    ---
    tags:
      - Data
    parameters:
      - name: table_name
        in: path
        type: string
        required: true
        enum: [Order, dish, dishingredient, employee, employeeposition, ingredient, orderdish, position, supplier, visitor]
        description: Название таблицы
      - name: body
        in: body
        required: true
        schema:
          type: object
          properties:
            # Динамические свойства в зависимости от таблицы
          example:
            name: "New Record"
            value: "Example Value"
    responses:
      200:
        description: Запись успешно создана
        schema:
          type: object
          properties:
            success:
              type: boolean
            key:
              type: string
      400:
        description: Неверные данные
      404:
        description: Таблица не найдена
      500:
        description: Ошибка сервера
    """
    if table_name not in TABLES:
        return jsonify({"error": f"Table {table_name} not found"}), 404
    
    data = request.get_json()
    if not data:
        return jsonify({"error": "No data provided"}), 400
    
    db_path = os.path.join(DB_PATH, f"{table_name}.db")
    db = bsddb3.btopen(db_path, "w")
    
    try:
        # Определяем ключ
        primary_key = TABLES[table_name]
        if isinstance(primary_key, list):
            # Составной ключ
            key_parts = [str(data.get(col, "")) for col in primary_key]
            key_value = "_".join(key_parts)
        else:
            # Простой ключ
            key_value = str(data.get(primary_key, ""))
        
        if not key_value:
            return jsonify({"error": "Primary key not provided"}), 400
        
        # Сохраняем данные
        json_value = json.dumps(data, ensure_ascii=False, default=str)
        db[key_value.encode('utf-8')] = json_value.encode('utf-8')
        
        db.close()
        return jsonify({"success": True, "key": key_value})
    except Exception as e:
        if db:
            db.close()
        return jsonify({"error": str(e)}), 500

@app.route('/api/<table_name>/<key>', methods=['PUT'])
def update_record(table_name: str, key: str):
    """
    Обновление записи
    ---
    tags:
      - Data
    parameters:
      - name: table_name
        in: path
        type: string
        required: true
        enum: [Order, dish, dishingredient, employee, employeeposition, ingredient, orderdish, position, supplier, visitor]
        description: Название таблицы
      - name: key
        in: path
        type: string
        required: true
        description: Ключ записи
      - name: body
        in: body
        required: true
        schema:
          type: object
          properties:
            # Динамические свойства в зависимости от таблицы
    responses:
      200:
        description: Запись успешно обновлена
      404:
        description: Запись не найдена
      500:
        description: Ошибка сервера
    """
    if table_name not in TABLES:
        return jsonify({"error": f"Table {table_name} not found"}), 404
    
    data = request.get_json()
    if not data:
        return jsonify({"error": "No data provided"}), 400
    
    db_path = os.path.join(DB_PATH, f"{table_name}.db")
    db = bsddb3.btopen(db_path, "w")
    
    try:
        key_bytes = key.encode('utf-8')
        if key_bytes in db:
            json_value = json.dumps(data, ensure_ascii=False, default=str)
            db[key_bytes] = json_value.encode('utf-8')
            db.close()
            return jsonify({"success": True})
        else:
            db.close()
            return jsonify({"error": "Record not found"}), 404
    except Exception as e:
        if db:
            db.close()
        return jsonify({"error": str(e)}), 500

@app.route('/api/<table_name>/<key>', methods=['DELETE'])
def delete_record(table_name: str, key: str):
    """
    Удаление записи
    ---
    tags:
      - Data
    parameters:
      - name: table_name
        in: path
        type: string
        required: true
        enum: [Order, dish, dishingredient, employee, employeeposition, ingredient, orderdish, position, supplier, visitor]
        description: Название таблицы
      - name: key
        in: path
        type: string
        required: true
        description: Ключ записи
    responses:
      200:
        description: Запись успешно удалена
      404:
        description: Запись не найдена
      500:
        description: Ошибка сервера
    """
    if table_name not in TABLES:
        return jsonify({"error": f"Table {table_name} not found"}), 404
    
    db_path = os.path.join(DB_PATH, f"{table_name}.db")
    db = bsddb3.btopen(db_path, "w")
    
    try:
        key_bytes = key.encode('utf-8')
        if key_bytes in db:
            del db[key_bytes]
            db.close()
            return jsonify({"success": True})
        else:
            db.close()
            return jsonify({"error": "Record not found"}), 404
    except Exception as e:
        if db:
            db.close()
        return jsonify({"error": str(e)}), 500

@app.route('/api/health')
def health_check():
    """
    Проверка здоровья API
    ---
    tags:
      - System
    responses:
      200:
        description: API работает нормально
        schema:
          type: object
          properties:
            status:
              type: string
            timestamp:
              type: string
    """
    return jsonify({
        "status": "healthy",
        "timestamp": datetime.now().isoformat()
    })

# Генерация Swagger спецификации
@app.route('/api/swagger.json')
def swagger_spec():
    """Генерация Swagger спецификации"""
    from flask import jsonify
    import datetime
    
    spec = {
        "swagger": "2.0",
        "info": {
            "title": "Restaurant DB API",
            "description": "API для работы с Berkeley DB базами данных ресторана",
            "version": "1.0.0",
            "contact": {
                "name": "API Support",
                "url": "http://localhost:5000/api/docs"
            }
        },
        "host": "localhost:5000",
        "basePath": "/api",
        "schemes": ["http", "https"],
        "consumes": ["application/json"],
        "produces": ["application/json"],
        "paths": {},
        "definitions": {
            "Error": {
                "type": "object",
                "properties": {
                    "error": {
                        "type": "string",
                        "description": "Описание ошибки"
                    }
                }
            },
            "Success": {
                "type": "object",
                "properties": {
                    "success": {
                        "type": "boolean",
                        "description": "Статус операции"
                    }
                }
            }
        }
    }
    
    return jsonify(spec)

if __name__ == '__main__':
    # Создаем папки если их нет
    os.makedirs('templates', exist_ok=True)
    os.makedirs('static', exist_ok=True)
    
    # Импортируем здесь чтобы избежать циклического импорта
    from datetime import datetime
    
    app.run(host='0.0.0.0', port=5000, debug=True)
