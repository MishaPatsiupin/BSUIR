-- Создание роли для обычного пользователя (user)
CREATE ROLE "user" WITH
    LOGIN
    PASSWORD 'user'
    NOSUPERUSER
    NOCREATEDB
    NOCREATEROLE
    INHERIT
    NOREPLICATION;

-- Создание роли для администратора (admin)
CREATE ROLE admin WITH
    LOGIN
    PASSWORD 'admin'
    NOSUPERUSER
    NOCREATEDB
    NOCREATEROLE
    INHERIT
    NOREPLICATION;

-- Предоставление прав на просмотр (SELECT) для всех таблиц для роли user
GRANT SELECT ON TABLE public."Order", public.dish, public.dishingredient, public.employee,
    public.employeeposition, public.ingredient, public.orderdish, public.position,
    public.supplier, public.visitor TO "user";

-- Предоставление прав на создание, обновление и удаление (INSERT, UPDATE, DELETE) для несправочных таблиц для user
GRANT INSERT, UPDATE, DELETE ON TABLE public."Order", public.dishingredient, public.employee,
    public.employeeposition, public.ingredient, public.orderdish, public.visitor TO "user";

-- Предоставление прав на использование последовательностей для несправочных таблиц для user
GRANT USAGE, SELECT ON SEQUENCE public."Order_orderid_seq", public.employee_employeeid_seq,
    public.ingredient_ingredientid_seq, public.visitor_visitorid_seq TO "user";

-- Предоставление всех прав для admin на все таблицы
GRANT ALL PRIVILEGES ON TABLE public."Order", public.dish, public.dishingredient, public.employee,
    public.employeeposition, public.ingredient, public.orderdish, public.position,
    public.supplier, public.visitor TO admin;

-- Предоставление прав на использование всех последовательностей для admin
GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO admin;

-- Создание пользователей
CREATE USER waiter1 WITH PASSWORD 'waiter1';
CREATE USER manager1 WITH PASSWORD 'manager1';

-- Назначение ролей пользователям
GRANT "user" TO waiter1;
GRANT admin TO manager1;

-- Установка схемы по умолчанию для ролей
ALTER ROLE "user" SET search_path TO public;
ALTER ROLE admin SET search_path TO public;