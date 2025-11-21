import psycopg2
from psycopg2.extras import RealDictCursor

def get_db_connection(user, password):
    return psycopg2.connect(
        dbname="Lido",
        user=user,
        password=password,
        host="localhost",
        port="5432",
        cursor_factory=RealDictCursor
    )