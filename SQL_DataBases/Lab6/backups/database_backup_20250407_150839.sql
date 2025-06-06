PGDMP  '                    }            Lido    16.2    16.2 J               0    0    ENCODING    ENCODING        SET client_encoding = 'UTF8';
                      false                        0    0 
   STDSTRINGS 
   STDSTRINGS     (   SET standard_conforming_strings = 'on';
                      false            !           0    0 
   SEARCHPATH 
   SEARCHPATH     8   SELECT pg_catalog.set_config('search_path', '', false);
                      false            "           1262    16548    Lido    DATABASE     ~   CREATE DATABASE "Lido" WITH TEMPLATE = template0 ENCODING = 'UTF8' LOCALE_PROVIDER = libc LOCALE = 'Belarusian_Belarus.1251';
    DROP DATABASE "Lido";
                postgres    false            �            1255    17055    update_order_total()    FUNCTION     �  CREATE FUNCTION public.update_order_total() RETURNS trigger
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
 +   DROP FUNCTION public.update_order_total();
       public          postgres    false            �            1259    16953    Order    TABLE     �  CREATE TABLE public."Order" (
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
    DROP TABLE public."Order";
       public         heap    postgres    false            �            1259    16952    Order_orderid_seq    SEQUENCE     �   CREATE SEQUENCE public."Order_orderid_seq"
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 *   DROP SEQUENCE public."Order_orderid_seq";
       public          postgres    false    228            #           0    0    Order_orderid_seq    SEQUENCE OWNED BY     K   ALTER SEQUENCE public."Order_orderid_seq" OWNED BY public."Order".orderid;
          public          postgres    false    227            �            1259    16930    dish    TABLE     �  CREATE TABLE public.dish (
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
    DROP TABLE public.dish;
       public         heap    postgres    false            �            1259    16929    dish_dishid_seq    SEQUENCE     �   CREATE SEQUENCE public.dish_dishid_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 &   DROP SEQUENCE public.dish_dishid_seq;
       public          postgres    false    222            $           0    0    dish_dishid_seq    SEQUENCE OWNED BY     C   ALTER SEQUENCE public.dish_dishid_seq OWNED BY public.dish.dishid;
          public          postgres    false    221            �            1259    16964    dishingredient    TABLE     �   CREATE TABLE public.dishingredient (
    dishid integer NOT NULL,
    ingredientid integer NOT NULL,
    quantity numeric(10,2)
);
 "   DROP TABLE public.dishingredient;
       public         heap    postgres    false            �            1259    16923    employee    TABLE     ^  CREATE TABLE public.employee (
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
    DROP TABLE public.employee;
       public         heap    postgres    false            �            1259    16922    employee_employeeid_seq    SEQUENCE     �   CREATE SEQUENCE public.employee_employeeid_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 .   DROP SEQUENCE public.employee_employeeid_seq;
       public          postgres    false    220            %           0    0    employee_employeeid_seq    SEQUENCE OWNED BY     S   ALTER SEQUENCE public.employee_employeeid_seq OWNED BY public.employee.employeeid;
          public          postgres    false    219            �            1259    16959    employeeposition    TABLE     k   CREATE TABLE public.employeeposition (
    employeeid integer NOT NULL,
    positionid integer NOT NULL
);
 $   DROP TABLE public.employeeposition;
       public         heap    postgres    false            �            1259    16937 
   ingredient    TABLE     L  CREATE TABLE public.ingredient (
    ingredientid integer NOT NULL,
    name character varying(100) NOT NULL,
    unitofmeasurement character varying(50),
    instock numeric(10,2) NOT NULL,
    expirationdate date,
    supplierid integer,
    CONSTRAINT name_check CHECK (((name)::text ~ '^[a-zA-Zа-яА-Я0-9 ,."-]+$'::text))
);
    DROP TABLE public.ingredient;
       public         heap    postgres    false            �            1259    16936    ingredient_ingredientid_seq    SEQUENCE     �   CREATE SEQUENCE public.ingredient_ingredientid_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 2   DROP SEQUENCE public.ingredient_ingredientid_seq;
       public          postgres    false    224            &           0    0    ingredient_ingredientid_seq    SEQUENCE OWNED BY     [   ALTER SEQUENCE public.ingredient_ingredientid_seq OWNED BY public.ingredient.ingredientid;
          public          postgres    false    223            �            1259    16969 	   orderdish    TABLE     �   CREATE TABLE public.orderdish (
    orderid integer NOT NULL,
    dishid integer NOT NULL,
    count integer DEFAULT 1 NOT NULL
);
    DROP TABLE public.orderdish;
       public         heap    postgres    false            �            1259    16914    position    TABLE     m  CREATE TABLE public."position" (
    positionid integer NOT NULL,
    name character varying(100) NOT NULL,
    salary numeric(10,2) NOT NULL,
    workschedule character varying(100),
    responsibilities text,
    CONSTRAINT name_check CHECK (((name)::text ~ '^[a-zA-Zа-яА-Я0-9 ,."-]+$'::text)),
    CONSTRAINT salary_check CHECK ((salary >= (0)::numeric))
);
    DROP TABLE public."position";
       public         heap    postgres    false            �            1259    16913    position_positionid_seq    SEQUENCE     �   CREATE SEQUENCE public.position_positionid_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 .   DROP SEQUENCE public.position_positionid_seq;
       public          postgres    false    218            '           0    0    position_positionid_seq    SEQUENCE OWNED BY     U   ALTER SEQUENCE public.position_positionid_seq OWNED BY public."position".positionid;
          public          postgres    false    217            �            1259    16907    supplier    TABLE     �  CREATE TABLE public.supplier (
    supplierid integer NOT NULL,
    companyname character varying(100) NOT NULL,
    contactinformation character varying(255),
    reliabilityrating integer,
    productcategory character varying(100),
    CONSTRAINT companyname_check CHECK (((companyname)::text ~ '^[a-zA-Zа-яА-Я0-9 ,."-]+$'::text)),
    CONSTRAINT reliabilityrating_check CHECK (((reliabilityrating >= 1) AND (reliabilityrating <= 10)))
);
    DROP TABLE public.supplier;
       public         heap    postgres    false            �            1259    16906    supplier_supplierid_seq    SEQUENCE     �   CREATE SEQUENCE public.supplier_supplierid_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 .   DROP SEQUENCE public.supplier_supplierid_seq;
       public          postgres    false    216            (           0    0    supplier_supplierid_seq    SEQUENCE OWNED BY     S   ALTER SEQUENCE public.supplier_supplierid_seq OWNED BY public.supplier.supplierid;
          public          postgres    false    215            �            1259    16944    visitor    TABLE     �  CREATE TABLE public.visitor (
    visitorid integer NOT NULL,
    name character varying(200) NOT NULL,
    birthdate character varying(5),
    phonenumber character varying(15),
    preferences text,
    CONSTRAINT birthdate_format CHECK (((birthdate)::text ~ '^(0[1-9]|[12][0-9]|3[01])-(0[1-9]|1[0-2])$'::text)),
    CONSTRAINT name_check CHECK (((name)::text ~ '^[a-zA-Zа-яА-ЯёЁ0-9 ,."-]+$'::text)),
    CONSTRAINT phonenumber_check CHECK (((phonenumber)::text ~ '^\+375[0-9]{9}$'::text))
);
    DROP TABLE public.visitor;
       public         heap    postgres    false            �            1259    16943    visitor_visitorid_seq    SEQUENCE     �   CREATE SEQUENCE public.visitor_visitorid_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 ,   DROP SEQUENCE public.visitor_visitorid_seq;
       public          postgres    false    226            )           0    0    visitor_visitorid_seq    SEQUENCE OWNED BY     O   ALTER SEQUENCE public.visitor_visitorid_seq OWNED BY public.visitor.visitorid;
          public          postgres    false    225            K           2604    16956    Order orderid    DEFAULT     r   ALTER TABLE ONLY public."Order" ALTER COLUMN orderid SET DEFAULT nextval('public."Order_orderid_seq"'::regclass);
 >   ALTER TABLE public."Order" ALTER COLUMN orderid DROP DEFAULT;
       public          postgres    false    228    227    228            H           2604    16933    dish dishid    DEFAULT     j   ALTER TABLE ONLY public.dish ALTER COLUMN dishid SET DEFAULT nextval('public.dish_dishid_seq'::regclass);
 :   ALTER TABLE public.dish ALTER COLUMN dishid DROP DEFAULT;
       public          postgres    false    221    222    222            G           2604    16926    employee employeeid    DEFAULT     z   ALTER TABLE ONLY public.employee ALTER COLUMN employeeid SET DEFAULT nextval('public.employee_employeeid_seq'::regclass);
 B   ALTER TABLE public.employee ALTER COLUMN employeeid DROP DEFAULT;
       public          postgres    false    219    220    220            I           2604    16940    ingredient ingredientid    DEFAULT     �   ALTER TABLE ONLY public.ingredient ALTER COLUMN ingredientid SET DEFAULT nextval('public.ingredient_ingredientid_seq'::regclass);
 F   ALTER TABLE public.ingredient ALTER COLUMN ingredientid DROP DEFAULT;
       public          postgres    false    223    224    224            F           2604    16917    position positionid    DEFAULT     |   ALTER TABLE ONLY public."position" ALTER COLUMN positionid SET DEFAULT nextval('public.position_positionid_seq'::regclass);
 D   ALTER TABLE public."position" ALTER COLUMN positionid DROP DEFAULT;
       public          postgres    false    218    217    218            E           2604    16910    supplier supplierid    DEFAULT     z   ALTER TABLE ONLY public.supplier ALTER COLUMN supplierid SET DEFAULT nextval('public.supplier_supplierid_seq'::regclass);
 B   ALTER TABLE public.supplier ALTER COLUMN supplierid DROP DEFAULT;
       public          postgres    false    216    215    216            J           2604    16947    visitor visitorid    DEFAULT     v   ALTER TABLE ONLY public.visitor ALTER COLUMN visitorid SET DEFAULT nextval('public.visitor_visitorid_seq'::regclass);
 @   ALTER TABLE public.visitor ALTER COLUMN visitorid DROP DEFAULT;
       public          postgres    false    225    226    226                      0    16953    Order 
   TABLE DATA           s   COPY public."Order" (orderid, status, totalcost, numberofguests, orderdatetime, visitorid, employeeid) FROM stdin;
    public          postgres    false    228   �_                 0    16930    dish 
   TABLE DATA           V   COPY public.dish (dishid, name, weight, nutritionalvalue, cost, category) FROM stdin;
    public          postgres    false    222   �a                 0    16964    dishingredient 
   TABLE DATA           H   COPY public.dishingredient (dishid, ingredientid, quantity) FROM stdin;
    public          postgres    false    230   ii                 0    16923    employee 
   TABLE DATA           g   COPY public.employee (employeeid, fullname, phonenumber, email, hiredate, contractenddate) FROM stdin;
    public          postgres    false    220   @k                 0    16959    employeeposition 
   TABLE DATA           B   COPY public.employeeposition (employeeid, positionid) FROM stdin;
    public          postgres    false    229   �o                 0    16937 
   ingredient 
   TABLE DATA           p   COPY public.ingredient (ingredientid, name, unitofmeasurement, instock, expirationdate, supplierid) FROM stdin;
    public          postgres    false    224   4p                 0    16969 	   orderdish 
   TABLE DATA           ;   COPY public.orderdish (orderid, dishid, count) FROM stdin;
    public          postgres    false    231   �r                 0    16914    position 
   TABLE DATA           ^   COPY public."position" (positionid, name, salary, workschedule, responsibilities) FROM stdin;
    public          postgres    false    218   xt                 0    16907    supplier 
   TABLE DATA           s   COPY public.supplier (supplierid, companyname, contactinformation, reliabilityrating, productcategory) FROM stdin;
    public          postgres    false    216   Kx                 0    16944    visitor 
   TABLE DATA           W   COPY public.visitor (visitorid, name, birthdate, phonenumber, preferences) FROM stdin;
    public          postgres    false    226   �~       *           0    0    Order_orderid_seq    SEQUENCE SET     B   SELECT pg_catalog.setval('public."Order_orderid_seq"', 37, true);
          public          postgres    false    227            +           0    0    dish_dishid_seq    SEQUENCE SET     ?   SELECT pg_catalog.setval('public.dish_dishid_seq', 204, true);
          public          postgres    false    221            ,           0    0    employee_employeeid_seq    SEQUENCE SET     F   SELECT pg_catalog.setval('public.employee_employeeid_seq', 32, true);
          public          postgres    false    219            -           0    0    ingredient_ingredientid_seq    SEQUENCE SET     J   SELECT pg_catalog.setval('public.ingredient_ingredientid_seq', 47, true);
          public          postgres    false    223            .           0    0    position_positionid_seq    SEQUENCE SET     F   SELECT pg_catalog.setval('public.position_positionid_seq', 30, true);
          public          postgres    false    217            /           0    0    supplier_supplierid_seq    SEQUENCE SET     F   SELECT pg_catalog.setval('public.supplier_supplierid_seq', 46, true);
          public          postgres    false    215            0           0    0    visitor_visitorid_seq    SEQUENCE SET     D   SELECT pg_catalog.setval('public.visitor_visitorid_seq', 45, true);
          public          postgres    false    225            k           2606    16958    Order Order_pkey 
   CONSTRAINT     W   ALTER TABLE ONLY public."Order"
    ADD CONSTRAINT "Order_pkey" PRIMARY KEY (orderid);
 >   ALTER TABLE ONLY public."Order" DROP CONSTRAINT "Order_pkey";
       public            postgres    false    228            e           2606    16935    dish dish_pkey 
   CONSTRAINT     P   ALTER TABLE ONLY public.dish
    ADD CONSTRAINT dish_pkey PRIMARY KEY (dishid);
 8   ALTER TABLE ONLY public.dish DROP CONSTRAINT dish_pkey;
       public            postgres    false    222            o           2606    16968 "   dishingredient dishingredient_pkey 
   CONSTRAINT     r   ALTER TABLE ONLY public.dishingredient
    ADD CONSTRAINT dishingredient_pkey PRIMARY KEY (dishid, ingredientid);
 L   ALTER TABLE ONLY public.dishingredient DROP CONSTRAINT dishingredient_pkey;
       public            postgres    false    230    230            c           2606    16928    employee employee_pkey 
   CONSTRAINT     \   ALTER TABLE ONLY public.employee
    ADD CONSTRAINT employee_pkey PRIMARY KEY (employeeid);
 @   ALTER TABLE ONLY public.employee DROP CONSTRAINT employee_pkey;
       public            postgres    false    220            m           2606    16963 &   employeeposition employeeposition_pkey 
   CONSTRAINT     x   ALTER TABLE ONLY public.employeeposition
    ADD CONSTRAINT employeeposition_pkey PRIMARY KEY (employeeid, positionid);
 P   ALTER TABLE ONLY public.employeeposition DROP CONSTRAINT employeeposition_pkey;
       public            postgres    false    229    229            g           2606    16942    ingredient ingredient_pkey 
   CONSTRAINT     b   ALTER TABLE ONLY public.ingredient
    ADD CONSTRAINT ingredient_pkey PRIMARY KEY (ingredientid);
 D   ALTER TABLE ONLY public.ingredient DROP CONSTRAINT ingredient_pkey;
       public            postgres    false    224            q           2606    16973    orderdish orderdish_pkey 
   CONSTRAINT     c   ALTER TABLE ONLY public.orderdish
    ADD CONSTRAINT orderdish_pkey PRIMARY KEY (orderid, dishid);
 B   ALTER TABLE ONLY public.orderdish DROP CONSTRAINT orderdish_pkey;
       public            postgres    false    231    231            a           2606    16921    position position_pkey 
   CONSTRAINT     ^   ALTER TABLE ONLY public."position"
    ADD CONSTRAINT position_pkey PRIMARY KEY (positionid);
 B   ALTER TABLE ONLY public."position" DROP CONSTRAINT position_pkey;
       public            postgres    false    218            _           2606    16912    supplier supplier_pkey 
   CONSTRAINT     \   ALTER TABLE ONLY public.supplier
    ADD CONSTRAINT supplier_pkey PRIMARY KEY (supplierid);
 @   ALTER TABLE ONLY public.supplier DROP CONSTRAINT supplier_pkey;
       public            postgres    false    216            i           2606    16951    visitor visitor_pkey 
   CONSTRAINT     Y   ALTER TABLE ONLY public.visitor
    ADD CONSTRAINT visitor_pkey PRIMARY KEY (visitorid);
 >   ALTER TABLE ONLY public.visitor DROP CONSTRAINT visitor_pkey;
       public            postgres    false    226            {           2620    17056    orderdish order_total_update    TRIGGER     �   CREATE TRIGGER order_total_update AFTER INSERT OR UPDATE ON public.orderdish FOR EACH ROW EXECUTE FUNCTION public.update_order_total();
 5   DROP TRIGGER order_total_update ON public.orderdish;
       public          postgres    false    231    232            s           2606    17000    Order Order_employeeid_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public."Order"
    ADD CONSTRAINT "Order_employeeid_fkey" FOREIGN KEY (employeeid) REFERENCES public.employee(employeeid);
 I   ALTER TABLE ONLY public."Order" DROP CONSTRAINT "Order_employeeid_fkey";
       public          postgres    false    220    228    4707            t           2606    16995    Order Order_visitorid_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public."Order"
    ADD CONSTRAINT "Order_visitorid_fkey" FOREIGN KEY (visitorid) REFERENCES public.visitor(visitorid);
 H   ALTER TABLE ONLY public."Order" DROP CONSTRAINT "Order_visitorid_fkey";
       public          postgres    false    4713    226    228            w           2606    17015 )   dishingredient dishingredient_dishid_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.dishingredient
    ADD CONSTRAINT dishingredient_dishid_fkey FOREIGN KEY (dishid) REFERENCES public.dish(dishid);
 S   ALTER TABLE ONLY public.dishingredient DROP CONSTRAINT dishingredient_dishid_fkey;
       public          postgres    false    4709    230    222            x           2606    17020 /   dishingredient dishingredient_ingredientid_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.dishingredient
    ADD CONSTRAINT dishingredient_ingredientid_fkey FOREIGN KEY (ingredientid) REFERENCES public.ingredient(ingredientid);
 Y   ALTER TABLE ONLY public.dishingredient DROP CONSTRAINT dishingredient_ingredientid_fkey;
       public          postgres    false    224    4711    230            u           2606    17005 1   employeeposition employeeposition_employeeid_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.employeeposition
    ADD CONSTRAINT employeeposition_employeeid_fkey FOREIGN KEY (employeeid) REFERENCES public.employee(employeeid);
 [   ALTER TABLE ONLY public.employeeposition DROP CONSTRAINT employeeposition_employeeid_fkey;
       public          postgres    false    220    4707    229            v           2606    17010 1   employeeposition employeeposition_positionid_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.employeeposition
    ADD CONSTRAINT employeeposition_positionid_fkey FOREIGN KEY (positionid) REFERENCES public."position"(positionid);
 [   ALTER TABLE ONLY public.employeeposition DROP CONSTRAINT employeeposition_positionid_fkey;
       public          postgres    false    4705    218    229            r           2606    16986 %   ingredient ingredient_supplierid_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.ingredient
    ADD CONSTRAINT ingredient_supplierid_fkey FOREIGN KEY (supplierid) REFERENCES public.supplier(supplierid);
 O   ALTER TABLE ONLY public.ingredient DROP CONSTRAINT ingredient_supplierid_fkey;
       public          postgres    false    224    4703    216            y           2606    17030    orderdish orderdish_dishid_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.orderdish
    ADD CONSTRAINT orderdish_dishid_fkey FOREIGN KEY (dishid) REFERENCES public.dish(dishid);
 I   ALTER TABLE ONLY public.orderdish DROP CONSTRAINT orderdish_dishid_fkey;
       public          postgres    false    4709    222    231            z           2606    17025     orderdish orderdish_orderid_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.orderdish
    ADD CONSTRAINT orderdish_orderid_fkey FOREIGN KEY (orderid) REFERENCES public."Order"(orderid);
 J   ALTER TABLE ONLY public.orderdish DROP CONSTRAINT orderdish_orderid_fkey;
       public          postgres    false    4715    228    231                       6104    17058    name    PUBLICATION     \   CREATE PUBLICATION name FOR ALL TABLES WITH (publish = 'insert, update, delete, truncate');
    DROP PUBLICATION name;
                postgres    false               �  x��U�m1=KU�)��-"���W}�-5A9�mG&�����u����!��2��?��ח�sȜ*���3<d�\@�R�svJ�p�l�ϑ��� ]^��ܣ툞
�(hՕ"�(gyR�~�u���}�I �D`י�`N=� e2P6��c�Dh�cGPO�#�M��R�ާ!�����T�!xɨ�w���e�-D3�]����m�"�[$����5~�����Զ�P��A�>��3^���y���R-�_�掠��'�2�P�pA�0��A����jM�g��7��'
rF��b5
��Y{���I�u��M��ǵ������"��)\�/�㨩��o
'���oձd�2ѹ��,|r�[��GO�l[X�u�꠿:E���47��)��uԵ7��T������=^;nr;^H��^+��ht'�v�_�Ia��OܗJ��A�"��FB�p��}~J1�w�1�"         �  x����n�F��� t����\�U�0�$)b�ih����M"זl����'�?CR\J#�f�+[���f�����$��U5�����i8K\��eI�>��2,�u��촘�4\~��pӬzA��rZ���u.�*|j�Tz�����¼��yu�ǟ��>�NRL8��wX�AuX׏��ԥ�-��_�ܧ��M�J4��W�}��_�)��B��:�a�� �qg�gU=a�y�M�.���p�o�u�L��~�Aj75#!�V��y�h�u5L@t ���}JȘW����t).US]lA��A��������.d6k ��hi�A�Ք�f��%����i9 �b�(��@�	��	���i��N0���3Bf��M̬6��Q���Ű�y�'��>�,���m@���`���1K����6�K�*�xu6ͷ��iw�D��?ǣ�MYt�ʩ�e��M֪�	�[���]V/çp��
�!@�	34�jY�bS-E̦\��_�M�<��Ҵh�Wvڮ,*	1�s||м�7b�eP@k}���d��D��&��Ō�S�Mi�6�_��z�b��p�ƜZ�DN�<O��=�|Y'2=�&�N�T{�ls��țS�=�(�-h�\��0M��K�{+�����>Ѯ�l5��JR��xO8U{U����L%��Et�����^�9���AvK����M+��ᔒ�� ���\t((��ub��1������I+gQ=�g;�S
�ٽ�\��g��|��F"��TL�lHl9�Oh����g��l/��'�y�����������5�O�k��� ����hQ3I����ʹX�u]�|�aa������E�sIw/q���-�(w��&�����^�E�0��~�D�	���J�!X'G�n;$ݵ�#�|~ҩ7�3���Ɛ�x�7~�%/��sS�"k���z���U���P��}w�0"�|8��=�`-h���N�]O!���>J�u�������eml�RSoyh�g���P�P���fF��(22R�tQֽc۽j���ˌ'��p�p,��K�dȑ�50�L��Y��Td���Q>ҹ�Dץ���d�Qn��TR<cE���i��E�wT�(Y�ມR���������N�/�K�A��/j;�٠#2�Թh�����hۦU̒�N���b3Ru���L7K�.�w`iI@��_�!�qg��Q�����jZF4c����d2�v#z��i�^w0WĒ�������Q�7�N�z���#f\�_4�"�R����������
���$%�� � ����}p��We�M�ѕ��*�\�]ܨG��;:Ѽ�8�(���a�ľR!I�2�p�S/�R�������xV��9�o�	���wvܴ�H���zY'$G�S���RnPL�
�(����3����L:��$\�.o�\�Đ��P{sl�YE���wN�nXj�z� �rg5��G�����E�H�9�Z��U$ ���Xҿ��A����u;pw�D�o{�n�Y���K:k;�U7V�4E�Bj��~иY+�2А�p�wRܑp��M�&�7M����K��W(ץ!�ߩ�-
�5"z�c��V�7qLG�v����3��/)�;�Ի�C�r��f�Y��Y�V�4]dd��;��H@0;Έ�M6��L6�>��Tֵb�7s[P`��.�u*5�/�.JK���!q����F�Jd�d�M=��+N�d(�ѷ�7�^�"Ӝ(+qN|��Z�5��$5&6Ȧ\3z6Z-�*m�(bM��Vn����NY��c�f~P-�3��?#6X��KJF���q�^�/|�ǓdM�k	>ʁ���1j�6�����(u�ꊏ���l=����UxJ�5��|Y��۽��v�HA#��C\��U�Q���m������޿�n��         �  x�U��q@!C��b2`6���_G�`Dno4�0fik�&�����5Kk�19�r:Vi��f0�e2O������:�<�,(e����f*�얟�vl�Xv-D�u�Лr�tkQv��+B�.�:��G�h9��1�0���LCL��f���3��x�&�V�6��e+�uҷ��p��q�F��:�:�ܷq���U������Rc*�2s�N��K�vh�u���u����}0�#���2�.�+����j�����/�	�F#����}�j��R� N'��	��p�My�8���ȍ�I�e�#A���uM�!_F��9e�az�E�O��]�÷�~/�����
^��G���t빆��/���^>i�2��Fq&�ȧ%��Ώ|���ە��#�W3���j�qA��ӓḎ^8�zy��u�_]^��?�m������?����         Z  x��V�n�F�?�ծ�C|��,L821\%��O�jI��v���Mժ��Jti��W�Q�762�o�E��3���9gt&�񐜋H�b*�ˊ�Y,���Ĥ".��,9s|���3�arK�Fͬ����?h��w�M����*7�T�'��z��8��X*8��۹�_�������(�y��5��7����{@c�o��H��8�r���v�,-�R��<q�TRE܊�dV�Tc-���lia��<9M^RK�f���$&H�#��|Ef����A��'g9ͭ�&c���rIU���ոz���5T���b�O�IE�QY	�t��PO������o��L��?�=�f�D����^{G��Y�-&~�ɐI�I2����dkЖ+0��a��EbR����^߳���H%�iz���$�ɋUm��ld�>�9������>Q�i�R���3�0�D����J��x�x�q*��g��v��tNl�Ĝ�&��%�$n�i��3��8M��<��k�!	�͏dkżK'DI`� ��
�\/C��W���}ҬGd.+k���;�x�Gݶ����Q0%�f$����1�e/����h~#�����uֆ���]�2!gˬ-�Lub:rwL��+���!~�1O^�#��-���`���]��{%���$����e���(��K�&��Y���}��uz*��[D� H^'�Ʌ���ѧL�źNgz
�+��G���������]�:���3���F��,��"�ğsi3%��0ݣ�SN�Nt,�Af�;�I��|fGJ�z��c�R����Pّh�d�~��`~��S�BPp8����7���s�.Jox��>��;a�p�EM�{�T�����q�����+�7|�,��E�"~��Uj�{\/C\
�n���[�Z��(�"ohq�Up��bz�~������ ��yUX��&�'��H�t#�˞�~�-��]v:J�$ح�U��q�$Xv�*�
�`�� �}Z��,�ζCt�/�]�����.	n�n:*�TnVHg�B�����Z�Y~��� �5ݲ,f��nɬ~�TӴ� =T�4         z   x�ϹDA��Y���ￎe���B� :�pғ���Y����N6o����Y.*�z9�Y.��.4��t�f�hլ��s��Yi�O7z��'���a��Ϟ��V���*�\���?�3"         �  x�}U[r�@��9��}H���0��UN�$���J��'`q���g%�"�������R��ɿ�B�+?U6IΒD�Ĥ��m������y����%o���> k3�Z҉���� T��W(��l�6Zҹ�{����#`NzH����2��@Ӹ�1Rs�g��?|�������\�+_�w�Q[ڑ�X?���� X��Bށ��<�~ms2�⏐�Qzt�d �����p$GI	����x��g	X��+?�p�ֆ8 6(��M�~�U|�Ɲ(�Ա�6#�K���F�����޽����m$�\�� �����R��e�� �-�߄�f���A����5�\
Ŧ������e�I�?o����Aʛ��˽w���x'��n�4B9rH�/p��V��@vhF2�C��~7J݆l=���-:M��1Gm�%�����x-����b������Vʃ�a���bF�4B�ؽ�>h,�[���,>���1#
�-5zZ��
� S�y	Av�W���[|5�ƾK�qO�ܟʤNɀ�'�Ey|Rs�l�%d��#{pH��[!ۋ�z�?�;�a��{e�cҸxw�hA�$6��L�V�˾H8��>��E�&Y�aU���e�몧\B.Ȳ��rJ��z��t�{CrIw)w��8��e�挈�����         x  x�M�Q�� D��a�T�.s�s,���|���`K��s?����k��
=/���?�w��R	=Q�Qw���\|o�zμ��T*�6��oQ��s�;�Aw�c~C���U�SZu�/�q��h6�4x�mx��m�50B� �	����_o;}�*�^@���.���]vɡ_���c�P),/��p.����R�,�-���OH�6X�@��tҨ�c ��i��m�kt��}�*u�*-Ah#c��D �	�RY��yxv�,&�8�T��	��,�t�iq���Cx8��?)�¤Z5��%HG����K��6�2�V�`j�8i��|׬��T��q���V��i�N��y�?����!��         �  x��V]N�@~�=�PHv�r�� U�k���E�*!�U�>U��$Ĺ�������!8i_�Y�|3;�}3n*�M�;ڡ)tG��(۬�w�uek6�����}īC6Q���h$����u�)���>M(�G�Rt�z;�c+[�����؝F�e!��{��gg6dw�:��p]�#�	N��4vg�jSW��(��e����2m��k<��^$4q]e6\��x����c���-���~!g��CeL5�["�#I\\��P��R<�h�0�@[T�'M�z�S������MmbE��������@ʋt'�g�Q�&��w��#��)I1fy��f����`�N=�� Td>Q{��W��:��.���g�Dbf�3�°�R\kx��2ڂwc����s�<9�ԗf�k3��҄�\h��f�}	�:B}�8 rm��o�U���Ϥ2� �����L���X����D�א��!����*�|�#�+��f2tRh�7���Q�G
��&�So=���M�����u��@�<+g"nc�Ȑ�L|'JŽ@���|���������|�s1�Mxk-[���aѸy���3��O��04h�mk6o%�rڮ�~y�r>S��,m�L�\���ƍ�;�`qbA�DJt��ެ�b5�-{�L�p4�A�b&
�Sw����K?1d��v��/��.���g.A�=9g�a�p�&�S}>����6R|��j����-���ѯW�f���j�g��0M&<�XLs�6�EYq򚺡k���#T8��u�w�Z��yA4�8	%�����p�9=D�=rw�Nԓ
��0^����O9e�� mVW&��U�Q{�52��ϕ��`܁�ǲ��+��s,�������˨lR��+��E?J��MO�0���~�m̟bV��|��ϯ�l�?�����g�0T�9[ ��}s����q��j�����         �  x��W[OW~>�籕Xgo���JU���}iU�r��Z!�rHQ� $�H�m�b��B%
�Ő`���?�7��6�`���������fK�Ui�r�fP�JT���OMfn&'���L�������?>�⊖��ԡ��b�U�Z����mMOqp��8���IM,�bz�u|��pU˒��^ǆ��g�Y�@���\�,j�'m`��4����Z�}oޛ���hq�K�t��Co�0��1�V�zs��A����0�/�о�@��y���T��,������6�������J�F�m�1/;�cx��-R[�-��E;�-ST�}������T��y���1%z	4
X�1��[�aF��� �����R�m�c1\��\�h�����w_c�8��=x����+v�߈ �E�q�y�R7]��<��\�?��:��[�y��5 �8��4���%װTb�a�R���	]���k��g�=ȱ�m���-u0�g���BL罇=�;�����KX����ڠ�!|�ii��U*�p���l`�����5��"���b�d��p�r��V����Cj*ji�1i�YV��m��q�"��c->뫼��m����Q�ƹ毷 ��?;��c4a�Y���ہw]�#9x��N���Q�|���I���x���1c����q�������_�~������>`[��X�k��)��\OreIS[B�&����UH��Ԅ;>�����b�B�yɅ@���t3˭��@�̜2�-�ô��c8�#c��i阗,�UE��,������i�?㄂�'_�b�YNb��b��o`Z	�{ ��j�䦏���	bX�����+�F�`���t�l,���G٦�$j���Ȧp������S��v��8u;V��	I�y��H��shNX�i��P��� ���:�D4��TuM-e�%�U�cl�r2@�����`��� ��[?r!qP��JY{9��!J�ޒ�k���	#7LKY����rp���P_�.333�������4��V%;X�\j����y���2��[BY��VǕ�`����pO!e�S�d��*�I��Qfm�`塜�GN
���]��2i�9�8Wb�֦P\�� i�Wm���ϒ�������nn\}q�K�O��t�s�v4�������#%ϲ+]�颤~��%� $J����ɕpU_����+��?�@%90:��LLS�oH}lH�,�g@w�=1�F�÷,�-"vaȡ�I�1Fc���^��3���L��ͅ���%�r��SVMX/����G�2�_|C�Jf�G�Ғg?�! �iY1Ƽ.�F�D��a�+�_�~ϦS�������n��,ޡ���ց�	w@�)؂A�f:��wD�p�f�OT8W��� M�E�L��nMr킨.(i�~�9f"����Z����T� �p9��F�=�TD��
�k���d&ҷ��F��e�=^�� ���.f�r���{���PzڶϦ~�-=}�V&��H�����*���/gS3��=�/vB[1�H�F�~����;us2�;�D�O�>���E#,�=(�� ߽ [l�W��5@q]ճ��p�����稜`�C�+e��KZ�0�N���'�q7�J1 5iw���K�;ɉh�����h$��s�         @  x��X�R�P��"�[;�	��}Fp����֎�֎N;!��@�+�}���x#4*�"��'�g�좔C?iH#�Oc}���9s�r�GC�v�'��)����O9�f��~+��fˡ_X�Ҁ��� �:S�;.�������t�FxJ]���c�?���睮n�4�]���yD�5�;t	���c�)v�9�{� R�1�P"+0�p��_PG�,A=�&���ܛ� F�tB�8xO��� ��T�;e���>�b�'��1s�3bh��x���]����G1���?b&��x2��OH`~xo��k���>�����K�X��i���}3���w�6�%�'�Z�8\�#�ÌС�)�r����N��16�3��C����:��� .��>\����X��SƆ��☁�s<�Q���D��˝��ϘmV1��x�#��1`A��t|تY�kuRni���CW e���GK�Gv���Ż\���b�ty�,����1�������nٙ3�f��1ͼ�{1'�#�Θr�/����
	ƸH�������Ň<��`�`yA��Uj� �4\��>T錳�[9��#��_��F,בH��>T���,ű�>X8�l�Geq�>�9�&���\cp�众�,s:�����d$��� ��]������#�⸐�������.6�D���+<�r�@�P�fu�@�&'��l���,��&�6iW��ư��95b-��il��87	JX�XӶ��elnq��s��1�D2U6���W(����QfreS^Q�����L���D�))Q�(-��U[�}և�a��6�7H#)��y�>a���r��'˨Sa@ۙrKq.��k�~��T�Ժ&4���U�x�*V�)�ԻѪ^B��9���07�������MS,�YѰj�_2gi��ȱ������q �z��o���/�H,)�wݹLi����a�<�������Zhѷ�R<n1v>�_��|�X4Z��m�Xsn%��&�h�]�E�+6to�@Օ��F�9��j�l�ߦN�ܭ���$�&Z�����T_��6�LG���V���$�     