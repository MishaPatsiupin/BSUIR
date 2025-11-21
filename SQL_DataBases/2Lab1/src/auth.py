from flask_httpauth import HTTPBasicAuth
from db import get_db_connection
import psycopg2
import logging
from flask import g

auth = HTTPBasicAuth()
logger = logging.getLogger(__name__)

@auth.verify_password
def verify_password(username, password):
    logger.info(f"Authentication attempt for user: {username}")
    try:
        # Проверяем, можно ли подключиться к базе данных с указанными учетными данными
        conn = get_db_connection(username, password)
        cur = conn.cursor()
        # Получаем текущую роль пользователя
        cur.execute('SELECT current_user;')
        role = cur.fetchone()['current_user']
        cur.close()
        conn.close()
        # Проверяем, является ли роль допустимой (user, admin или пользователи, унаследовавшие эти роли)
        if role in ['user', 'admin', 'waiter1', 'manager1']:
            # Сохраняем пароль в контексте запроса
            g.password = password
            logger.info(f"User {username} authenticated successfully with role: {role}")
            return role
        logger.warning(f"User {username} provided invalid role: {role}")
        return None
    except psycopg2.Error as e:
        logger.error(f"Authentication failed for user {username}: {str(e)}")
        return None

@auth.get_user_roles
def get_user_roles(user):
    # Если пользователь waiter1 или другой с ролью user, возвращаем 'user'
    # Если пользователь manager1 или другой с ролью admin, возвращаем 'admin'
    if user in ['admin', 'manager1']:
        return 'admin'
    return 'user'

@auth.get_password
def get_password(username):
    # Пароль будет проверяться через psycopg2.connect, поэтому возвращаем None
    return None