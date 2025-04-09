--
-- PostgreSQL database dump
--

-- Dumped from database version 16.2
-- Dumped by pg_dump version 16.2

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
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
-- Name: Order_orderid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public."Order_orderid_seq" OWNED BY public."Order".orderid;


--
-- Name: Order orderid; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Order" ALTER COLUMN orderid SET DEFAULT nextval('public."Order_orderid_seq"'::regclass);


--
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
38	Ожидает	2.80	2	2025-04-02 11:39:05	\N	12
\.


--
-- Name: Order_orderid_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public."Order_orderid_seq"', 39, true);


--
-- Name: Order Order_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Order"
    ADD CONSTRAINT "Order_pkey" PRIMARY KEY (orderid);


--
-- Name: Order Order_employeeid_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Order"
    ADD CONSTRAINT "Order_employeeid_fkey" FOREIGN KEY (employeeid) REFERENCES public.employee(employeeid);


--
-- Name: Order Order_visitorid_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."Order"
    ADD CONSTRAINT "Order_visitorid_fkey" FOREIGN KEY (visitorid) REFERENCES public.visitor(visitorid);


--
-- PostgreSQL database dump complete
--

