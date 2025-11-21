from flask import Flask, jsonify, g
from flasgger import Swagger
from db import get_db_connection
import psycopg2
from auth import auth
import logging
from logging.handlers import RotatingFileHandler
from flask_cors import CORS
import yaml

from routes import (
    orders, dishes, dishingredients, employees, employeepositions,
    ingredients, orderdishes, positions, suppliers, visitors, queries, admin
)

# Настройка логирования
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)
handler = RotatingFileHandler('app.log', maxBytes=10_000_000, backupCount=5)
formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')
handler.setFormatter(formatter)
logger.addHandler(handler)
console_handler = logging.StreamHandler()
console_handler.setFormatter(formatter)
logger.addHandler(console_handler)



app = Flask(__name__)
# Загружаем swagger спецификацию из yaml файла
try:
    with open('src/swagger.yaml', 'r', encoding='utf-8') as f:
        swagger_template = yaml.safe_load(f)
except FileNotFoundError:
    with open('swagger.yaml', 'r', encoding='utf-8') as f:
        swagger_template = yaml.safe_load(f)

# Конфигурация Swagger UI
swagger_config = {
    "headers": [],
    "specs": [
        {
            "endpoint": 'apispec_1',
            "route": '/apispec_1.json',
            "rule_filter": lambda rule: True,
            "model_filter": lambda tag: True,
        }
    ],
    "static_url_path": "/flasgger_static",
    "swagger_ui": True,
    "specs_route": "/swagger/"
}

swagger = Swagger(app, template=swagger_template, config=swagger_config)
app.url_map.strict_slashes = False

CORS(app, resources={
    r"/*": {
        "origins": "http://localhost:5173",
        "methods": ["GET", "POST", "PUT", "DELETE", "OPTIONS"],
        "allow_headers": ["Content-Type", "Authorization"],
        "expose_headers": ["Content-Disposition"],
        "supports_credentials": False
    }
})

# Кастомный jsonify с поддержкой UTF-8
def custom_jsonify(**kwargs):
    kwargs.setdefault('ensure_ascii', False)
    return jsonify(**kwargs)  # Передаем только именованные аргументы

@app.route('/', methods=['GET'])
@auth.login_required
def home():
    user = auth.username()
    role = auth.current_user()
    logger.info(f"User {user} (role: {role}) accessed endpoint: GET /")
    try:
        conn = get_db_connection(user, g.password)
        cur = conn.cursor()
        cur.execute('SELECT * FROM public."Order";')
        data = cur.fetchall()
        cur.close()
        conn.close()
        logger.info(f"User {user} (role: {role}) successfully retrieved {len(data)} orders")
        return custom_jsonify(data=data)  # Передаем как именованный аргумент
    except psycopg2.Error as e:
        logger.error(f"User {user} (role: {role}) failed to retrieve orders: {str(e)}")
        return custom_jsonify(error=str(e)), 400

# Регистрация Blueprint'ов
app.register_blueprint(orders, url_prefix='/api/orders')
app.register_blueprint(dishes, url_prefix='/api/dishes')
app.register_blueprint(dishingredients, url_prefix='/api/dishingredients')
app.register_blueprint(employees, url_prefix='/api/employees')
app.register_blueprint(employeepositions, url_prefix='/api/employeepositions')
app.register_blueprint(ingredients, url_prefix='/api/ingredients')
app.register_blueprint(orderdishes, url_prefix='/api/orderdishes')
app.register_blueprint(positions, url_prefix='/api/positions')
app.register_blueprint(suppliers, url_prefix='/api/suppliers')
app.register_blueprint(visitors, url_prefix='/api/visitors')
app.register_blueprint(queries, url_prefix='/api/queries')
app.register_blueprint(admin, url_prefix='/api/admin')

if __name__ == '__main__':
    logger.info("Starting Flask application")
    app.run(debug=True, host='0.0.0.0', port=5000)