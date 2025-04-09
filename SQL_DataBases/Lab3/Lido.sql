--
-- PostgreSQL database dump
--

-- Dumped from database version 16.2
-- Dumped by pg_dump version 17.2

-- Started on 2025-03-11 19:20:59

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET transaction_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- TOC entry 4 (class 2615 OID 2200)
-- Name: public; Type: SCHEMA; Schema: -; Owner: pg_database_owner
--

CREATE SCHEMA public;


ALTER SCHEMA public OWNER TO pg_database_owner;

--
-- TOC entry 4898 (class 0 OID 0)
-- Dependencies: 4
-- Name: SCHEMA public; Type: COMMENT; Schema: -; Owner: pg_database_owner
--

COMMENT ON SCHEMA public IS 'standard public schema';


--
-- TOC entry 232 (class 1255 OID 17055)
-- Name: update_order_total(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.update_order_total() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
    UPDATE public."Order" 
    SET totalcost = (
        SELECT COALESCE(SUM(d.cost * od.count), 0)  -- Используем COALESCE для обработки NULL
        FROM public."orderdish" od
        JOIN public."dish" d ON od.dishid = d.dishid
        WHERE od.orderid = NEW.orderid
    )
    WHERE orderid = NEW.orderid;

    RETURN NEW;
END;
$$;


ALTER FUNCTION public.update_order_total() OWNER TO postgres;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- TOC entry 228 (class 1259 OID 16953)
-- Name: Order; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."Order" (
    orderid integer NOT NULL,
    status character varying(50) NOT NULL,
    totalcost numeric(10,2) NOT NULL,
    numberofguests integer,
    orderdatetime timestamp without time zone NOT NULL,
    visitorid integer,
    employeeid integer NOT NULL,
    CONSTRAINT numberofguests_check CHECK ((numberofguests >= 0)),
    CONSTRAINT totalcost_check CHECK ((totalcost >= (0)::numeric))
);


ALTER TABLE public."Order" OWNER TO postgres;

--
-- TOC entry 227 (class 1259 OID 16952)
-- Name: Order_orderid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public."Order_orderid_seq"
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public."Order_orderid_seq" OWNER TO postgres;

--
-- TOC entry 4899 (class 0 OID 0)
-- Dependencies: 227
-- Name: Order_orderid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public."Order_orderid_seq" OWNED BY public."Order".orderid;


--
-- TOC entry 222 (class 1259 OID 16930)
-- Name: dish; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.dish (
    dishid integer NOT NULL,
    name character varying(100) NOT NULL,
    weight numeric(10,2) NOT NULL,
    nutritionalvalue character varying(100),
    cost numeric(10,2),
    category character varying(100),
    CONSTRAINT cost_check CHECK ((cost > (0)::numeric)),
    CONSTRAINT name_check CHECK (((name)::text ~ '^[a-zA-Zа-яА-Я0-9 ,."-]+$'::text)),
    CONSTRAINT weight_check CHECK ((weight > (0)::numeric))
);


ALTER TABLE public.dish OWNER TO postgres;

--
-- TOC entry 221 (class 1259 OID 16929)
-- Name: dish_dishid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.dish_dishid_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.dish_dishid_seq OWNER TO postgres;

--
-- TOC entry 4900 (class 0 OID 0)
-- Dependencies: 221
-- Name: dish_dishid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.dish_dishid_seq OWNED BY public.dish.dishid;


--
-- TOC entry 230 (class 1259 OID 16964)
-- Name: dishingredient; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.dishingredient (
    dishid integer NOT NULL,
    ingredientid integer NOT NULL,
    quantity numeric(10,2)
);


ALTER TABLE public.dishingredient OWNER TO postgres;

--
-- TOC entry 220 (class 1259 OID 16923)
-- Name: employee; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.employee (
    employeeid integer NOT NULL,
    fullname character varying(200) NOT NULL,
    phonenumber character varying(15),
    email character varying(100),
    hiredate date,
    contractenddate date,
    CONSTRAINT contractenddate_check CHECK ((contractenddate >= hiredate)),
    CONSTRAINT email_check CHECK (((email)::text ~* '^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$'::text)),
    CONSTRAINT fullname_check CHECK (((fullname)::text ~ '^[a-zA-Zа-яА-Я0-9 ,."-]+$'::text)),
    CONSTRAINT phonenumber_check CHECK (((phonenumber)::text ~ '^\+375[0-9]{9}$'::text))
);


ALTER TABLE public.employee OWNER TO postgres;

--
-- TOC entry 219 (class 1259 OID 16922)
-- Name: employee_employeeid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.employee_employeeid_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.employee_employeeid_seq OWNER TO postgres;

--
-- TOC entry 4901 (class 0 OID 0)
-- Dependencies: 219
-- Name: employee_employeeid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.employee_employeeid_seq OWNED BY public.employee.employeeid;


--
-- TOC entry 229 (class 1259 OID 16959)
-- Name: employeeposition; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.employeeposition (
    employeeid integer NOT NULL,
    positionid integer NOT NULL
);


ALTER TABLE public.employeeposition OWNER TO postgres;

--
-- TOC entry 224 (class 1259 OID 16937)
-- Name: ingredient; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.ingredient (
    ingredientid integer NOT NULL,
    name character varying(100) NOT NULL,
    unitofmeasurement character varying(50),
    instock numeric(10,2) NOT NULL,
    expirationdate date,
    supplierid integer,
    CONSTRAINT name_check CHECK (((name)::text ~ '^[a-zA-Zа-яА-Я0-9 ,."-]+$'::text))
);


ALTER TABLE public.ingredient OWNER TO postgres;

--
-- TOC entry 223 (class 1259 OID 16936)
-- Name: ingredient_ingredientid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.ingredient_ingredientid_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.ingredient_ingredientid_seq OWNER TO postgres;

--
-- TOC entry 4902 (class 0 OID 0)
-- Dependencies: 223
-- Name: ingredient_ingredientid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.ingredient_ingredientid_seq OWNED BY public.ingredient.ingredientid;


--
-- TOC entry 231 (class 1259 OID 16969)
-- Name: orderdish; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.orderdish (
    orderid integer NOT NULL,
    dishid integer NOT NULL,
    count integer DEFAULT 1 NOT NULL
);


ALTER TABLE public.orderdish OWNER TO postgres;

--
-- TOC entry 218 (class 1259 OID 16914)
-- Name: position; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."position" (
    positionid integer NOT NULL,
    name character varying(100) NOT NULL,
    salary numeric(10,2) NOT NULL,
    workschedule character varying(100),
    responsibilities text,
    CONSTRAINT name_check CHECK (((name)::text ~ '^[a-zA-Zа-яА-Я0-9 ,."-]+$'::text)),
    CONSTRAINT salary_check CHECK ((salary >= (0)::numeric))
);


ALTER TABLE public."position" OWNER TO postgres;

--
-- TOC entry 217 (class 1259 OID 16913)
-- Name: position_positionid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.position_positionid_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.position_positionid_seq OWNER TO postgres;

--
-- TOC entry 4903 (class 0 OID 0)
-- Dependencies: 217
-- Name: position_positionid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.position_positionid_seq OWNED BY public."position".positionid;


--
-- TOC entry 216 (class 1259 OID 16907)
-- Name: supplier; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.supplier (
    supplierid integer NOT NULL,
    companyname character varying(100) NOT NULL,
    contactinformation character varying(255),
    reliabilityrating integer,
    productcategory character varying(100),
    CONSTRAINT companyname_check CHECK (((companyname)::text ~ '^[a-zA-Zа-яА-Я0-9 ,."-]+$'::text)),
    CONSTRAINT reliabilityrating_check CHECK (((reliabilityrating >= 1) AND (reliabilityrating <= 10)))
);


ALTER TABLE public.supplier OWNER TO postgres;

--
-- TOC entry 215 (class 1259 OID 16906)
-- Name: supplier_supplierid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.supplier_supplierid_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.supplier_supplierid_seq OWNER TO postgres;

--
-- TOC entry 4904 (class 0 OID 0)
-- Dependencies: 215
-- Name: supplier_supplierid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.supplier_supplierid_seq OWNED BY public.supplier.supplierid;


--
-- TOC entry 226 (class 1259 OID 16944)
-- Name: visitor; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.visitor (
    visitorid integer NOT NULL,
    name character varying(200) NOT NULL,
    birthdate character varying(5),
    phonenumber character varying(15),
    preferences text,
    CONSTRAINT birthdate_format CHECK (((birthdate)::text ~ '^(0[1-9]|[12][0-9]|3[01])-(0[1-9]|1[0-2])$'::text)),
    CONSTRAINT name_check CHECK (((name)::text ~ '^[a-zA-Zа-яА-ЯёЁ0-9 ,."-]+$'::text)),
    CONSTRAINT phonenumber_check CHECK (((phonenumber)::text ~ '^\+375[0-9]{9}$'::text))
);


ALTER TABLE public.visitor OWNER TO postgres;

--
-- TOC entry 225 (class 1259 OID 16943)
-- Name: visitor_visitorid_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.visitor_visitorid_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.visitor_visitorid_seq OWNER TO postgres;

--
-- TOC entry 4905 (class 0 OID 0)
-- Dependencies: 225
-- Name: visitor_visitorid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.visitor_visitorid_seq OWNED BY public.visitor.visitorid;


--
-- TOC entry 4683 (class 2604 OID 16956)
-- Name: Order orderid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Order" ALTER COLUMN orderid SET DEFAULT nextval('public."Order_orderid_seq"'::regclass);


--
-- TOC entry 4680 (class 2604 OID 16933)
-- Name: dish dishid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.dish ALTER COLUMN dishid SET DEFAULT nextval('public.dish_dishid_seq'::regclass);


--
-- TOC entry 4679 (class 2604 OID 16926)
-- Name: employee employeeid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.employee ALTER COLUMN employeeid SET DEFAULT nextval('public.employee_employeeid_seq'::regclass);


--
-- TOC entry 4681 (class 2604 OID 16940)
-- Name: ingredient ingredientid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.ingredient ALTER COLUMN ingredientid SET DEFAULT nextval('public.ingredient_ingredientid_seq'::regclass);


--
-- TOC entry 4678 (class 2604 OID 16917)
-- Name: position positionid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."position" ALTER COLUMN positionid SET DEFAULT nextval('public.position_positionid_seq'::regclass);


--
-- TOC entry 4677 (class 2604 OID 16910)
-- Name: supplier supplierid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.supplier ALTER COLUMN supplierid SET DEFAULT nextval('public.supplier_supplierid_seq'::regclass);


--
-- TOC entry 4682 (class 2604 OID 16947)
-- Name: visitor visitorid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.visitor ALTER COLUMN visitorid SET DEFAULT nextval('public.visitor_visitorid_seq'::regclass);


--
-- TOC entry 4889 (class 0 OID 16953)
-- Dependencies: 228
-- Data for Name: Order; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."Order" (orderid, status, totalcost, numberofguests, orderdatetime, visitorid, employeeid) FROM stdin;
18	Закрыт	49.50	4	2025-03-09 19:30:00	12	19
2	Закрыт	15.70	1	2025-03-10 12:30:00	13	12
11	Закрыт	13.00	1	2025-03-10 10:15:00	15	20
5	Закрыт	13.00	1	2025-03-09 09:00:00	17	19
13	Закрыт	19.40	2	2025-03-10 13:30:00	19	20
12	Закрыт	4.70	1	2025-03-10 11:00:00	21	20
7	Закрыт	25.05	3	2025-03-09 11:45:00	29	19
9	Закрыт	55.91	5	2025-03-09 14:00:00	31	19
15	Закрыт	25.05	3	2025-03-09 16:15:00	32	19
16	Закрыт	39.95	3	2025-03-09 17:30:00	33	19
3	Закрыт	23.10	2	2025-03-10 12:30:00	36	14
14	Закрыт	33.75	2	2025-03-10 15:00:00	37	20
17	Закрыт	27.70	4	2025-03-09 18:00:00	38	19
10	Закрыт	29.00	5	2025-03-10 09:30:00	42	18
6	Закрыт	34.40	2	2025-03-09 10:30:00	22	19
8	Закрыт	77.60	4	2025-03-09 12:15:00	25	19
30	Закрыт	34.30	5	2025-03-10 19:00:00	14	18
20	Закрыт	65.00	5	2025-03-10 09:45:00	16	18
32	Закрыт	28.80	2	2025-03-10 21:00:00	18	20
25	Закрыт	20.10	3	2025-03-09 14:30:00	20	19
22	Закрыт	7.90	1	2025-03-10 11:15:00	23	20
27	Закрыт	33.40	4	2025-03-09 16:45:00	11	19
33	Закрыт	23.80	5	2025-03-10 09:15:00	26	20
19	Закрыт	69.25	5	2025-03-09 20:00:00	27	19
23	Закрыт	14.00	2	2025-03-10 12:00:00	28	20
28	Закрыт	65.80	4	2025-03-09 17:00:00	30	19
29	Закрыт	40.56	5	2025-03-09 18:15:00	24	19
26	Закрыт	30.90	3	2025-03-09 15:30:00	34	19
34	Закрыт	43.95	5	2025-03-10 10:45:00	35	20
21	Закрыт	9.50	1	2025-03-10 10:30:00	39	20
24	Закрыт	7.80	2	2025-03-10 13:45:00	40	20
31	Закрыт	1.20	1	2025-03-10 20:00:00	41	20
\.


--
-- TOC entry 4883 (class 0 OID 16930)
-- Dependencies: 222
-- Data for Name: dish; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.dish (dishid, name, weight, nutritionalvalue, cost, category) FROM stdin;
164	Горчица	60.00	Белки –7,1 г; Жиры – 8,7 г; Углеводы – 12,0 г; Энергетическая ценность – 157,6 Ккал/659 кДж	2.00	Соусы
165	Блины с картофелем и грибами	135.00	Белки –6,9 г; Жиры – 9,7 г; Углеводы – 26,3 г; Энергетическая ценность – 221 Ккал/925 кДж	3.00	Блины
166	Суп сырный с грибами	280.00	Белки – 1,27г; Жиры – 2,3; Углеводы – 5,0 г; Энергетическая ценность – 49 Ккал/205 кДж	2.40	Супы
167	Суп куриный	280.00	Белки – 3,6 г; Жиры – 2,2 г; Углеводы – 3,8 г; Энергетическая ценность – 50 Ккал/209 кДж	2.80	Супы
168	Солянка мясная	280.00	Белки – 1,9 г; Жиры – 6,9 г; Углеводы – 2,7 г; Энергетическая ценность – 84 Ккал/351 кДж	4.50	Супы
169	Колбаса "Куриная с майораном" жареная	100.00	Белки – 20,5 г; Жиры – 33,3 г; Углеводы – 0,9 г; Энергетическая ценность – 387 Ккал/1618 кДж	5.30	Горячие блюда
170	Скумбрия гриль	100.00	Белки – 21,3 г; Жиры – 16,1 г; Углеводы – 0,9 г; Энергетическая ценность – 235 Ккал/983 кДж	6.00	Гриль
171	Шашлык из свинины "Фирменный"	150.00	Белки – 19,4 г; Жиры – 45,5 г; Углеводы – 0,9 г; Энергетическая ценность – 496 Ккал/2075 кДж	13.50	Гриль
172	Шашлык из птицы "Оригинальный"	160.00	Белки – 29,1 г; Жиры – 6,6 г; Углеводы – 2,1 г; Энергетическая ценность – 183 Ккал/766 кДж	9.50	Гриль
173	Окорочка "Ароматные"	100.00	Белки – 26,0 г; Жиры – 21,0 г; Углеводы – 4,1 г; Энергетическая ценность – 305 Ккал/1275 кДж	3.80	Горячие блюда
174	Хлеб "Мамма Миа"	60.00	 Белки –11,0 г; Жиры – 4,0 г; Углеводы – 46 г; Энергетическая ценность – 260 Ккал/1120 кДж	1.00	Выпечка
175	Чиабатта	100.00	Белки – 8 г; Жиры – 2 г; Углеводы – 52 г; Энергетическая ценность – 260 Ккал/1100 кДж	1.00	Выпечка
176	Хлеб	35.00	Белки – 6,0 г; Жиры – 0,5 г; Углеводы – 48,0 г; Энергетическая ценность – 220 Ккал/950 кДж	0.13	Выпечка
177	Напиток из шиповника и боярышника	250.00	Белки – 0,3 г; Жиры – 0 г; Углеводы – 8,6 г; Энергетическая ценность – 35 Ккал/146 кДж	1.90	Напитки
178	Топпинг клубничный	60.00	Белки – 0,0 г; Жиры – 0,0 г; Углеводы – 69,0 г; Энергетическая ценность – 264 Ккал/1105 кДж	2.50	Соусы
179	Аджика "Домашняя"	60.00	Белки – 1 г; Жиры – 4,6 г; Углеводы – 9,1 г	0.90	Соусы
180	Десерт "Вишневый пломбир"	155.00	Белки –2,7 г; Жиры – 12,1 г; Углеводы – 33,8 г; Энергетическая ценность – 261 Ккал/1092 кДж	6.00	Десерты
181	Фруктовый коктейль	170.00	Белки –0,3 г; Жиры – 0,1 г; Углеводы – 11,7 г; Энергетическая ценность – 48 Ккал/202 кДж	7.00	Напитки
182	Сметана	55.00	Белки –2,6 г; Жиры – 18,0 г; Углеводы – 2,9 г; Энергетическая ценность – 184 Ккал/770,4 кДж	0.90	Молочные продукты
184	Йогурт питьевой	250.00	Белки –2,9 г; Жиры – 1,0 г; Углеводы – 11,5 г; Энергетическая ценность – 66,6 Ккал/281,8 кДж	1.20	Молочные продукты
185	Блины с начинкой из вишни	135.00	Белки – 5,6 г; Жиры – 6,3 г; Углеводы – 37,5 г; Энергетическая ценность – 226 Ккал/945 кДж	3.50	Блины
186	Блины с творогом	135.00	Белки –10 г; Жиры – 7,4 г; Углеводы – 28,1 г; Энергетическая ценность – 216 Ккал/904 кДж	2.70	Блины
187	Блины с мясом цыпленка	135.00	Белки –12,3 г; Жиры – 9 г; Углеводы – 23,5 г; Энергетическая ценность – 222 Ккал/929 кДж	3.50	Блины
188	Блины с сыром и ветчиной	135.00	Белки –14,2 г; Жиры – 14,1 г; Углеводы – 22,2 г; Энергетическая ценность – 271 Ккал/1134 кДж	4.00	Блины
189	Блины с яблоками и клюквой	135.00	Белки –5,3 г; Жиры – 6,3 г; Углеводы – 32,7 г; Энергетическая ценность – 207 Ккал/866 кДж	2.70	Блины
190	Шампиньоны "Ароматные"	100.00	Белки – 6,4 г; Жиры – 10,3 г; Углеводы – 0,1 г; Энергетическая ценность – 123 Ккал/515 кДж	4.30	Овощи
191	Блины, 2 шт	180.00	Белки – 7,9 г; Жиры – 4,5 г; Углеводы – 33,2 г; Энергетическая ценность – 201 Ккал/841 кДж	2.20	Блины
192	Стручковая фасоль с морковью и кукурузой	150.00	Белки – 1,8 г; Жиры – 10,2 г; Углеводы – 7,1 г; Энергетическая ценность – 132 Ккал/552 кДж	5.25	Овощи
193	Микс овощной	150.00	Белки – 1,0 г; Жиры – 0 г; Углеводы – 3,3 г; Энергетическая ценность – 19 Ккал/79 кДж	6.45	Овощи
194	Картофель отварной обжаренный	150.00	Белки – 2,3 г; Жиры – 14,1 г; Углеводы – 18,1 г; Энергетическая ценность – 219 Ккал/916 кДж	2.25	Картофель
195	Картофель в кожуре обжаренный	150.00	Белки – 2,3 г; Жиры – 14,1 г; Углеводы – 22,6 г; Энергетическая ценность – 222 Ккал/929 кДж	1.80	Картофель
196	Рис с куркумой	150.00	Белки – 2,7 г; Жиры – 3,9 г; Углеводы – 26,5 г; Энергетическая ценность – 154 Ккал/644 кДж	1.80	Гарниры
197	Гречка отварная	150.00	Белки – 5,7 г; Жиры – 3,4 г; Углеводы – 26,1 г; Энергетическая ценность – 160 Ккал/669 кДж	1.20	Гарниры
198	Драники по-домашнему	200.00	Белки – 4,8 г; Жиры – 15,7 г; Углеводы – 34,9 г; Энергетическая ценность – 293 Ккал/1225 кДж	4.50	Горячие блюда
199	Картофельное пюре	200.00	Белки – 2,1 г; Жиры – 2,6 г; Углеводы – 16,7 г; Энергетическая ценность – 94 Ккал/393 кДж	2.00	Гарниры
200	Картофель жареный соломкой	150.00	Белки – 3,9 г; Жиры – 10,9 г; Углеводы – 30,5 г; Энергетическая ценность – 250 Ккал/1046 кДж	3.00	Картофель
201	Картофель отварной с укропом	150.00	Белки – 1,9 г; Жиры – 0,4 г; Углеводы – 18,6 г; Энергетическая ценность – 80 Ккал/334 кДж	1.25	Картофель
202	Котлета из свинины	95.00	Белки – 12,2 г; Жиры – 39,1 г; Углеводы – 9,8 г; Энергетическая ценность – 446 Ккал/1864 кДж	4.20	Мясо
203	Говядина с грибами под сыром	140.00	Белки – 16,6 г; Жиры – 26,8 г; Углеводы – 5,7 г; Энергетическая ценность – 331 Ккал/1384 кДж	9.30	Мясо
183	Кефир	240.00	Белки –3,1 г; Жиры – 2,6 г; Углеводы – 4,0 г; Энергетическая ценность – 50 Ккал/220 кДж	1.20	Молочные продукты
204	Пиво, в ассартименте	0.50	Белки – 0 г; Жиры – 0 г; Углеводы – 12 г; Энергетическая ценность – 220 Ккал	4.50	Напитки
\.


--
-- TOC entry 4891 (class 0 OID 16964)
-- Dependencies: 230
-- Data for Name: dishingredient; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.dishingredient (dishid, ingredientid, quantity) FROM stdin;
164	19	0.03
165	11	0.03
165	12	0.02
165	4	0.05
165	1	0.03
166	18	0.15
166	12	0.05
166	21	0.10
166	13	0.03
167	7	0.20
167	13	0.05
167	14	0.03
167	21	0.10
168	7	0.05
168	8	0.05
168	9	0.05
168	15	0.03
168	16	0.02
168	21	0.10
169	7	0.08
169	17	0.02
170	22	0.15
170	17	0.01
171	9	0.20
171	17	0.02
172	7	0.20
172	17	0.02
173	7	0.10
173	17	0.02
174	1	0.06
174	6	0.01
174	4	0.02
174	3	0.00
174	5	0.01
175	1	0.10
175	6	0.02
175	4	0.04
175	3	0.00
175	5	0.01
176	1	0.04
176	6	0.01
176	4	0.01
176	3	0.00
176	5	0.00
177	24	0.02
177	23	0.02
177	2	0.02
178	25	0.06
179	26	0.06
180	27	0.10
180	28	0.05
181	30	0.02
181	27	0.02
181	29	0.02
181	31	0.11
182	32	0.06
184	31	0.25
183	33	0.24
165	2	0.01
165	3	0.01
165	5	1.00
185	1	0.03
185	2	0.01
185	3	0.01
185	4	0.05
185	5	1.00
185	27	0.35
186	1	0.03
186	2	0.01
186	3	0.01
186	4	0.05
186	5	1.00
186	34	0.35
187	1	0.03
187	2	0.01
187	3	0.01
187	4	0.05
187	5	1.00
187	7	0.35
188	1	0.03
188	2	0.01
188	3	0.01
188	4	0.05
188	5	1.00
188	18	0.02
188	35	0.15
189	1	0.03
189	2	0.01
189	3	0.01
189	4	0.05
189	5	1.00
189	37	0.02
189	36	0.15
190	12	0.10
190	16	0.01
191	1	0.05
191	2	0.01
191	3	0.01
191	4	0.10
191	5	2.00
192	38	0.10
192	13	0.03
192	39	0.03
193	39	0.05
193	13	0.05
193	12	0.05
193	16	0.01
193	6	0.01
194	11	0.20
194	6	0.01
195	11	0.20
195	6	0.01
196	40	0.06
196	17	0.01
197	6	0.01
197	41	0.06
198	11	0.25
198	3	0.01
198	5	1.00
198	14	0.02
199	11	0.18
199	4	0.02
199	6	0.01
200	11	0.20
200	6	0.01
201	11	0.20
201	6	0.01
201	45	0.01
202	9	0.10
202	6	0.01
202	3	0.01
202	5	1.00
202	46	0.01
203	8	0.10
203	6	0.01
203	3	0.01
203	5	1.00
203	46	0.01
203	12	0.10
203	18	0.10
204	47	0.50
\.


--
-- TOC entry 4881 (class 0 OID 16923)
-- Dependencies: 220
-- Data for Name: employee; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.employee (employeeid, fullname, phonenumber, email, hiredate, contractenddate) FROM stdin;
1	Бакунович Никита Андреевич	+375291234501	bakunovich@lido.by	2025-03-01	2026-03-01
2	Велич Никита Олегович	+375291234502	velich@lido.by	2025-03-02	2026-03-02
3	Георгиев Никита Димитров	+375291234503	georgiev@lido.by	2025-03-03	2027-03-03
4	Говор Павел Сергеевич	+375291234504	govor@lido.by	2025-03-04	2025-09-04
5	Горчаков Никита Сергеевич	+375291234505	gorchakov@lido.by	2025-03-05	2026-03-05
6	Гусаков Святослав	+375291234506	gusakov@lido.by	2025-03-01	2027-03-01
7	Демидович Руслан Сергеевич	+375291234507	demidovich@lido.by	2025-03-02	2025-09-02
8	Дылевский Егор Олегович	+375291234508	dylevski@lido.by	2025-03-03	2026-03-03
9	Жуковская Вероника Кирилловна	+375291234509	zhukovskaya@lido.by	2025-03-04	2027-03-04
10	Золотницкий Алексей Андреевич	+375291234510	zolotnitski@lido.by	2025-03-05	2025-09-05
11	Каражан Ксения Александровна	+375291234511	karajan@lido.by	2025-03-01	2026-03-01
12	Коледа Анна Валерьевна	+375291234512	koleda@lido.by	2025-03-02	2027-03-02
13	Корзун Ксения Игоревна	+375291234513	korzun@lido.by	2025-03-03	2025-09-03
14	Кузьмин Дмитрий Сергеевич	+375291234514	kuzmin@lido.by	2025-03-04	2026-03-04
15	Курдеко Никита Андреевич	+375291234515	kurdeko@lido.by	2025-03-05	2027-03-05
16	Курчицкий Глеб Анатольевич	+375291234516	kurchitski@lido.by	2025-03-01	2025-09-01
17	Лемешонок Дмитрий Андреевич	+375291234517	lemeshonok@lido.by	2025-03-02	2026-03-02
18	Мащенко Артур Витальевич	+375291234518	mashchenko@lido.by	2025-03-03	2027-03-03
19	Мелюх Евгений Сергеевич	+375291234519	melyukh@lido.by	2025-03-04	2025-09-04
20	Можейко Дмитрий Олегович	+375291234520	mozheiko@lido.by	2025-03-05	2026-03-05
21	Мынзул Александр Эдуардович	+375291234521	minzul@lido.by	2025-03-01	2027-03-01
22	Николаев Иван Ярославович	+375291234522	nikolaev@lido.by	2025-03-02	2025-09-02
23	Осинин Иван Николаевич	+375291234523	osinin@lido.by	2025-03-03	2026-03-03
24	Патюпин Михаил Сергеевич	+375291234524	patyupin@lido.by	2025-03-04	2027-03-04
25	Приловский Виктор Витальевич	+375291234525	prilovski@lido.by	2025-03-05	2025-09-05
26	Рыбак Феона Игоревна	+375291234526	rybak@lido.by	2025-03-01	2026-03-01
27	Савинич Тимофей Максимович	+375291234527	savinich@lido.by	2025-03-02	2027-03-02
28	Хорошун Никита Андреевич	+375291234528	khoroshun@lido.by	2025-03-03	2025-09-03
29	Черняк Станислав Сергеевич	+375291234529	chernyak@lido.by	2025-03-04	2026-03-04
30	Шагун Денис Валерьевич	+375291234530	shagun@lido.by	2025-03-05	2027-03-05
31	Шах Евгений Дмитриевич	+375291234531	shakh@lido.by	2025-03-01	2025-09-01
32	Абуям Токимаци	+375298041999	abuym@lido.by	2025-03-05	2025-09-05
\.


--
-- TOC entry 4890 (class 0 OID 16959)
-- Dependencies: 229
-- Data for Name: employeeposition; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.employeeposition (employeeid, positionid) FROM stdin;
1	1
2	1
3	2
3	3
3	4
4	5
5	5
5	17
6	6
7	6
8	7
9	7
10	8
10	9
11	25
11	26
12	11
13	11
14	11
15	11
16	12
17	12
18	14
19	14
20	15
20	16
21	15
21	16
22	18
22	21
23	19
23	20
24	22
25	23
26	24
27	24
28	27
28	30
29	28
30	28
31	29
31	31
31	32
32	10
\.


--
-- TOC entry 4885 (class 0 OID 16937)
-- Dependencies: 224
-- Data for Name: ingredient; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.ingredient (ingredientid, name, unitofmeasurement, instock, expirationdate, supplierid) FROM stdin;
5	Яйца	шт	300.00	2025-04-30	14
8	Говядина	кг	100.00	2025-12-31	2
10	Рыба	кг	100.00	2025-12-31	12
15	Оливки	кг	30.00	2025-12-31	13
17	Специи	кг	10.00	2025-12-31	17
19	Горчица	кг	15.00	2025-12-31	13
22	Скумбрия	кг	15.00	2025-04-30	12
24	Боярешник	кг	5.00	2028-12-31	4
25	Топпинг клубничный	кг	5.00	2028-12-31	37
26	Аджика	кг	5.00	2028-12-31	13
29	Мед	кг	10.00	2028-12-31	5
30	Банан	кг	10.00	2025-04-30	13
31	Йогурт	кг	3.00	2025-04-30	14
32	Сметана	кг	3.00	2025-04-30	14
33	Кефир	л	5.00	2025-04-30	36
37	Яблоко	кг	10.00	2025-04-30	35
38	Стручковая фасоль	кг	5.00	2025-12-31	33
39	Кукуруза консервированая	шт	10.00	2025-12-31	13
40	Рис	кг	20.00	2025-12-31	11
41	Гречка	кг	20.00	2025-12-31	11
6	Масло	кг	50.00	2025-12-31	14
45	Укроп	кг	1.00	2025-03-28	33
46	Панировочные сухари	кг	2.00	2025-12-31	4
47	Пиво	л	30.00	2025-12-31	38
35	Ветчина	кг	5.00	2025-04-30	3
7	Курица	кг	150.00	2025-12-31	6
12	Грибы	кг	50.00	2025-12-31	1
9	Свинина	кг	100.00	2025-12-31	7
1	Мука	кг	100.00	2025-12-31	8
2	Сахар	кг	50.00	2025-12-31	9
3	Соль	кг	20.00	2025-12-31	10
28	Мороженное пломбир	кг	5.00	2025-12-31	15
21	Бульен	л	30.00	2025-12-31	16
13	Морковь	кг	50.00	2025-12-31	32
11	Картофель	кг	200.00	2025-12-31	34
18	Сыр	кг	50.00	2025-12-31	39
14	Лук	кг	50.00	2025-03-31	44
36	Клюква	кг	5.00	2025-04-30	42
16	Лимон	кг	20.00	2025-06-30	43
34	Творог	кг	5.00	2025-04-30	40
4	Молоко	л	50.00	2025-04-30	41
23	Шиповник	кг	5.00	2028-12-31	45
27	Вишня	кг	5.00	2025-12-31	46
\.


--
-- TOC entry 4892 (class 0 OID 16969)
-- Dependencies: 231
-- Data for Name: orderdish; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.orderdish (orderid, dishid, count) FROM stdin;
2	168	1
2	174	1
2	183	1
3	177	2
3	169	2
3	194	1
3	193	1
5	180	1
5	181	1
6	172	2
6	177	1
2	204	2
6	204	2
6	194	2
7	177	3
7	202	3
7	194	3
8	171	4
8	177	4
8	200	4
8	174	2
8	175	2
9	177	5
9	169	5
9	164	2
9	176	2
9	175	2
9	194	5
9	183	2
10	185	1
10	186	1
10	187	1
10	188	1
10	189	1
10	177	3
10	182	5
10	183	1
10	184	1
11	180	1
11	181	1
12	167	1
12	177	1
13	170	2
13	177	2
13	196	2
14	172	2
14	177	2
14	194	2
14	193	1
15	202	3
15	177	3
15	194	3
16	171	1
16	177	3
16	200	2
16	201	1
16	202	1
16	203	1
17	191	4
17	178	2
17	179	2
17	177	4
17	194	2
18	169	1
18	170	1
18	171	1
18	172	1
18	177	4
18	196	2
18	199	2
19	198	1
19	199	1
19	200	1
19	201	1
19	202	1
19	203	1
19	168	5
19	204	5
20	180	5
20	181	5
21	181	1
21	178	1
22	180	1
22	177	1
23	181	2
24	186	2
24	183	2
25	185	3
25	182	3
25	177	3
25	184	1
26	190	3
26	192	2
26	195	1
26	177	3
27	193	4
27	177	4
28	192	4
28	203	4
28	177	4
29	198	5
29	167	2
29	179	3
29	176	2
29	177	5
30	165	6
30	191	2
30	177	5
30	183	2
31	184	1
32	175	2
32	171	1
32	172	1
32	177	2
33	186	3
33	187	2
33	183	5
33	182	3
34	175	2
34	173	5
34	201	5
34	193	2
34	177	2
\.


--
-- TOC entry 4879 (class 0 OID 16914)
-- Dependencies: 218
-- Data for Name: position; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."position" (positionid, name, salary, workschedule, responsibilities) FROM stdin;
5	Шеф-повар	2500.00	2/2 по 12 часов	Руководство кухней
6	Су-шеф	2000.00	2/2 по 12 часов	Помощь шеф-повару
7	Повар	1500.00	2/2 по 12 часов	Приготовление блюд
10	Бариста	1200.00	2/2 по 12 часов	Приготовление кофе
11	Официант	1000.00	2/2 по 12 часов	Обслуживание клиентов
12	Хостес	1100.00	2/2 по 12 часов	Встреча гостей
13	Администратор	1500.00	2/2 по 12 часов	Управление персоналом
14	Кассир	900.00	2/2 по 12 часов	Работа на кассе
15	Уборщик	800.00	2/2 по 12 часов	Уборка помещений
16	Мойщик посуды	700.00	2/2 по 12 часов	Мытье посуды
18	Маркетолог	2500.00	4/3 по 6 часов	Маркетинговые исследования
24	Охранник	1000.00	2/2 по 12 часов	Охрана территории
27	Заведующий складом	1400.00	2/2 по 12 часов	Управление складом
28	Курьер	1000.00	2/2 по 12 часов	Доставка заказов
1	Менеджер	2000.00	2/2 по 12 часов	Управление проектами
17	Закупщик	400.00	1/6 по 6 часов	Закупка продуктов
19	Бухгалтер	1500.00	4/3 по 6 часов	Ведение бухгалтерии
21	HR-менеджер	1500.00	2/6 по 6 часов	Управление персоналом
25	Сомелье	1500.00	7 по 4 часа	Подбор вин
26	Бармен	1200.00	7 по 4 часа	Приготовление напитков
29	Дизайнер	400.00	1/6 по 6 часов	Дизайн интерьера
30	PR-менеджер	400.00	1/6 по 6 часов	Связи с общественностью
32	Видеограф	200.00	1/6 по 6 часов	Создание контента для социальных сетей, рекламы
31	SMM специалист	400.00	2/6 по 6 часов	Ведение социальных сетей
2	Разработчик	2000.00	2/5 по 6 часов	Разработка ПО
3	Тестировщик	1200.00	2/5 по 6 часов	Тестирование ПО
4	Аналитик	2500.00	2/5 по 6 часов	Анализ данных
8	Кондитер	1400.00	4/3 по 4 часа	Приготовление десертов
9	Пекарь	1300.00	4/3 по 4 часа	Выпечка хлебобулочных изделий
20	Юрист	2000.00	3/4 по 6 часов	Юридическое сопровождение
23	Водитель	1200.00	7/0 по 4 часа	Доставка продуктов
22	Техник	1400.00	5/2 по 8 часов	Обслуживание оборудования
\.


--
-- TOC entry 4877 (class 0 OID 16907)
-- Dependencies: 216
-- Data for Name: supplier; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.supplier (supplierid, companyname, contactinformation, reliabilityrating, productcategory) FROM stdin;
1	ЛОГАЛ-БИО ООО	logal-bio.by, info@logal-bio.by	8	Грибы
4	ИМПЕРИЯ ЗЛАКОВ ТМ СМОРГОНСКИЙ КОМБИНАТ ХЛЕБОПРОДУКТОВ УПП	 Беларусь, Гродненская область, Сморгонский район, Сморгонь, Комсомольский переулок, 20	7	Пищевые добавки
5	ВИГОЛ ООО	tastee.by	8	Tastee
6	ПИЩЕВОЙ КОМБИНАТ БЕЛКООПСОЮЗА ЧУП	220075, город Минск, пр Партизанский 168	6	Продукты питаня
7	ДОБРЫЙ ДЕНЬ ООО	220140, город Минск, Лещинского 8/4-1	9	Продукты питаня
11	БАКАЛЕЯ МОГИЛЕВ ОАО	212040, город Могилев, Залуцкого 25	9	Бакалея
12	ИВАСИ-ТОРГ ЧАСТНОЕ ТОРГОВОЕ УНИТАРНОЕ ПРЕДПРИЯТИЕ	211388, город Орша, Ленина 230Ц	7	Морепродукты
14	ВЕРХНИЙ ЛУГ СООО	211622, Верхнедвинский район, Партизанская 1, д Янино	6	Молочные продукты
16	ЛЕБУР-ПРОДУКТ-ПЛЮС ООО	223050, Минский район, Базовская 1Б-2, пос Сухорукие	7	Мясо
17	ЛИДАПИЩЕКОНЦЕНТРАТЫ ОАО	wed4@lidkon.by	5	Брикеты супа
33	КОМВАК ЗАО	220018, город Минск, Якубовского 30/1	5	Овощи
35	ТРЕЙДХОЛЛПРОЕКТ ООО	 212039, город Могилев, Ровчакова 5 офис 1	9	Фрукты
36	ЛЕПЕЛЬСКИЙ МОЛОЧНОКОНСЕРВНЫЙ КОМБИНАТ ФИЛИАЛОАО ВИТЕБСКИЙ МЯСОКОМБИНАТ	 211099, город Лепель, Борисовский тракт 120	4	Молочные продукты
37	МАГВЮС ООО	223053, город Минский р-н, 40 лет Победы 23А-113, пос Боровляны	9	Напитки
38	Завод Аливарыя	8 017 239-58-01	10	Напитки
2	АМИФРУТ ООО	www.amifruit.by	7	Мясо
13	МИРАНА ООО	224005, город Брест, Комсомольская 23/1	8	Консервы
45	Вальдерольд ООО	ул. Радиальная 52, Минск, Минская область 220070	9	Продукты питаня
3	ЕВРОФОЛИЯ ЧТПУП	220018, город Минск, Шаранговича 7	9	Мясо
8	ABC ПЛЮС ФИРМА ООО ПРЕДСТАВИТЕЛЬСТВО	212011, город Могилев, пер Березовский 1	7	Продукты питаня
9	КИТАЙСКИЙ ЧАЙ PEKOE.BY МАГАЗИН ЧТУП АДС-ВОСТОК	210015, город Витебск, Генерала Белобородова 3-65а (ТДЦ Беларусь)	8	Продукты питаня
10	ОРЕХОВАЯ КОМПАНИЯ ООО	212040, город Могилев, пер 1-й Южный 21 комн 1	6	Продукты питаня
15	КРАСНЫЙ ПИЩЕВИК ОАО	priemnaya@zefir.by	9	Продукты питаня
32	ABC ПЛЮС ФИРМА ООО ПРЕДСТАВИТЕЛЬСТВО	220094, город Минск, пер 2-й Велосипедный 30-312	8	Овощи
34	ОВОЩИФРЕШ ЧТУП	220095, город Минск, Плеханова 56/1	8	Овощи
39	Лиско-плюс ООО	 ул. Левкова 24, Минск	9	Молочные продукты
40	Zavodik	пер. Домашевский 9, Минск, Минская область	9	Молочные продукты
41	Рэчыцкае	@rechitskoe_by	9	Продукты питаня
42	Mad Frog Brewery	8 029 151-94-84	9	Продукты питаня
43	Jungle Brewery	пер. Первомайский 1, Юзуфово, Минская область	9	Цитрусовые
44	Первая ферма HOPS	улица Колесникова 38, Минск, Минская область	9	Овощи
46	Сады Придоронья	sad.ru	9	Фрукты
\.


--
-- TOC entry 4887 (class 0 OID 16944)
-- Dependencies: 226
-- Data for Name: visitor; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.visitor (visitorid, name, birthdate, phonenumber, preferences) FROM stdin;
11	Никульшин Борис Викторович	01-01	+375172932379	Предпочитает вегетарианские блюда, любит столик у окна
12	Куприянова Диана Вячеславовна	15-02	+375172938617	Предпочитает блюда с морепродуктами, любит столик на террасе
13	Перцев Дмитрий Юрьевич	10-03	+375172938039	Предпочитает блюда с низким содержанием жира, любит столик в углу
14	Татур Михаил Михайлович	25-04	+375172938564	Предпочитает мясные блюда, любит столик у камина
15	Старовойтов Валерий Васильевич	30-05	\N	Предпочитает морепродукты, любит столик на террасе
16	Воронов Александр	05-06	\N	Предпочитает блюда с высоким содержанием белка, любит столик у окна
17	Кобяк Игорь	20-07	+375172938569	Предпочитает блюда с высоким содержанием белка, любит столик в углу
18	Луцик Юрий	15-08	+375172938697	Предпочитает блюда без сахара, любит столик у камина
19	Одинец Дмитрий	10-09	+375172932389	Предпочитает блюда без сахара, любит столик у окна
20	Селезнёв Игорь	25-10	+375172938569	Предпочитает блюда с низким содержанием углеводов, любит столик на террасе
21	Фролов Игорь	30-11	\N	Предпочитает блюда с низким содержанием углеводов, любит столик в углу
22	Байрак Сергей	05-12	+375172932389	Предпочитает веганские блюда, любит столик у камина
23	Глецевич Иван	10-01	+375172938587	Предпочитает веганские блюда, любит столик у окна
24	Ковальчук Анна	20-02	+375172938697	Предпочитает блюда с высоким содержанием клетчатки, любит столик на террасе
25	Лукьянова Ирина	15-03	+375172938697	Предпочитает блюда с высоким содержанием клетчатки, любит столик в углу
26	Поденок Леонид	10-04	+375172938039	Предпочитает блюда с низким содержанием жира, любит столик у камина
27	Смирнов Юрий	05-05	+375172938597	Предпочитает блюда с низким содержанием жира, любит столик у окна
28	Андриевский Егор	20-06	+375172938039	Предпочитает блюда с высоким содержанием белка, любит столик на террасе
29	Богдан Евгений	15-07	+375172938039	Предпочитает блюда без сахара, любит столик в углу
30	Внук Ольга	10-08	+375172938840	Предпочитает блюда с низким содержанием углеводов, любит столик у камина
31	Жалейко Дмитрий	05-09	+375172932222	Предпочитает блюда с низким содержанием углеводов, любит столик у окна
32	Желтко Юлия	20-10	+375172938039	Предпочитает веганские блюда, любит столик на террасе
33	Ковшер Елизавета	15-11	+375172938697	Предпочитает веганские блюда, любит столик в углу
34	Крюков Алексей	10-12	+375172938697	Предпочитает блюда с высоким содержанием клетчатки, любит столик у камина
35	Марцинкевич Владислав	25-01	+375172938691	Предпочитает блюда с высоким содержанием клетчатки, любит столик у окна
36	Протько Мария	05-02	\N	Предпочитает блюда с низким содержанием жира, любит столик на террасе
37	Савчиц Андрей	10-03	\N	Предпочитает блюда с низким содержанием жира, любит столик в углу
38	Селезнёв Александр	15-04	+375172938569	Предпочитает блюда с высоким содержанием белка, любит столик у камина
39	Скиба Ирина	20-05	+375172938698	Предпочитает блюда без сахара, любит столик у окна
40	Стракович Андрей	25-06	+375172938589	Предпочитает блюда с низким содержанием углеводов, любит столик на террасе
41	Тарасюк Игорь	30-07	+375172932222	Предпочитает блюда с низким содержанием углеводов, любит столик в углу
42	Туровец Николай	05-08	+375172938589	Предпочитает веганские блюда, любит столик у камина
\.


--
-- TOC entry 4906 (class 0 OID 0)
-- Dependencies: 227
-- Name: Order_orderid_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public."Order_orderid_seq"', 34, true);


--
-- TOC entry 4907 (class 0 OID 0)
-- Dependencies: 221
-- Name: dish_dishid_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.dish_dishid_seq', 204, true);


--
-- TOC entry 4908 (class 0 OID 0)
-- Dependencies: 219
-- Name: employee_employeeid_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.employee_employeeid_seq', 32, true);


--
-- TOC entry 4909 (class 0 OID 0)
-- Dependencies: 223
-- Name: ingredient_ingredientid_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.ingredient_ingredientid_seq', 47, true);


--
-- TOC entry 4910 (class 0 OID 0)
-- Dependencies: 217
-- Name: position_positionid_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.position_positionid_seq', 30, true);


--
-- TOC entry 4911 (class 0 OID 0)
-- Dependencies: 215
-- Name: supplier_supplierid_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.supplier_supplierid_seq', 46, true);


--
-- TOC entry 4912 (class 0 OID 0)
-- Dependencies: 225
-- Name: visitor_visitorid_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.visitor_visitorid_seq', 45, true);


--
-- TOC entry 4715 (class 2606 OID 16958)
-- Name: Order Order_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Order"
    ADD CONSTRAINT "Order_pkey" PRIMARY KEY (orderid);


--
-- TOC entry 4709 (class 2606 OID 16935)
-- Name: dish dish_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.dish
    ADD CONSTRAINT dish_pkey PRIMARY KEY (dishid);


--
-- TOC entry 4719 (class 2606 OID 16968)
-- Name: dishingredient dishingredient_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.dishingredient
    ADD CONSTRAINT dishingredient_pkey PRIMARY KEY (dishid, ingredientid);


--
-- TOC entry 4707 (class 2606 OID 16928)
-- Name: employee employee_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.employee
    ADD CONSTRAINT employee_pkey PRIMARY KEY (employeeid);


--
-- TOC entry 4717 (class 2606 OID 16963)
-- Name: employeeposition employeeposition_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.employeeposition
    ADD CONSTRAINT employeeposition_pkey PRIMARY KEY (employeeid, positionid);


--
-- TOC entry 4711 (class 2606 OID 16942)
-- Name: ingredient ingredient_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.ingredient
    ADD CONSTRAINT ingredient_pkey PRIMARY KEY (ingredientid);


--
-- TOC entry 4721 (class 2606 OID 16973)
-- Name: orderdish orderdish_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.orderdish
    ADD CONSTRAINT orderdish_pkey PRIMARY KEY (orderid, dishid);


--
-- TOC entry 4705 (class 2606 OID 16921)
-- Name: position position_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."position"
    ADD CONSTRAINT position_pkey PRIMARY KEY (positionid);


--
-- TOC entry 4703 (class 2606 OID 16912)
-- Name: supplier supplier_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.supplier
    ADD CONSTRAINT supplier_pkey PRIMARY KEY (supplierid);


--
-- TOC entry 4713 (class 2606 OID 16951)
-- Name: visitor visitor_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.visitor
    ADD CONSTRAINT visitor_pkey PRIMARY KEY (visitorid);


--
-- TOC entry 4731 (class 2620 OID 17056)
-- Name: orderdish order_total_update; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER order_total_update AFTER INSERT OR UPDATE ON public.orderdish FOR EACH ROW EXECUTE FUNCTION public.update_order_total();


--
-- TOC entry 4723 (class 2606 OID 17000)
-- Name: Order Order_employeeid_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Order"
    ADD CONSTRAINT "Order_employeeid_fkey" FOREIGN KEY (employeeid) REFERENCES public.employee(employeeid);


--
-- TOC entry 4724 (class 2606 OID 16995)
-- Name: Order Order_visitorid_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Order"
    ADD CONSTRAINT "Order_visitorid_fkey" FOREIGN KEY (visitorid) REFERENCES public.visitor(visitorid);


--
-- TOC entry 4727 (class 2606 OID 17015)
-- Name: dishingredient dishingredient_dishid_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.dishingredient
    ADD CONSTRAINT dishingredient_dishid_fkey FOREIGN KEY (dishid) REFERENCES public.dish(dishid);


--
-- TOC entry 4728 (class 2606 OID 17020)
-- Name: dishingredient dishingredient_ingredientid_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.dishingredient
    ADD CONSTRAINT dishingredient_ingredientid_fkey FOREIGN KEY (ingredientid) REFERENCES public.ingredient(ingredientid);


--
-- TOC entry 4725 (class 2606 OID 17005)
-- Name: employeeposition employeeposition_employeeid_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.employeeposition
    ADD CONSTRAINT employeeposition_employeeid_fkey FOREIGN KEY (employeeid) REFERENCES public.employee(employeeid);


--
-- TOC entry 4726 (class 2606 OID 17010)
-- Name: employeeposition employeeposition_positionid_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.employeeposition
    ADD CONSTRAINT employeeposition_positionid_fkey FOREIGN KEY (positionid) REFERENCES public."position"(positionid);


--
-- TOC entry 4722 (class 2606 OID 16986)
-- Name: ingredient ingredient_supplierid_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.ingredient
    ADD CONSTRAINT ingredient_supplierid_fkey FOREIGN KEY (supplierid) REFERENCES public.supplier(supplierid);


--
-- TOC entry 4729 (class 2606 OID 17030)
-- Name: orderdish orderdish_dishid_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.orderdish
    ADD CONSTRAINT orderdish_dishid_fkey FOREIGN KEY (dishid) REFERENCES public.dish(dishid);


--
-- TOC entry 4730 (class 2606 OID 17025)
-- Name: orderdish orderdish_orderid_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.orderdish
    ADD CONSTRAINT orderdish_orderid_fkey FOREIGN KEY (orderid) REFERENCES public."Order"(orderid);


-- Completed on 2025-03-11 19:20:59

--
-- PostgreSQL database dump complete
--

