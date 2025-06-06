PGDMP      &                 }            Lido    16.2    17.2 J               0    0    ENCODING    ENCODING        SET client_encoding = 'UTF8';
                           false                        0    0 
   STDSTRINGS 
   STDSTRINGS     (   SET standard_conforming_strings = 'on';
                           false            !           0    0 
   SEARCHPATH 
   SEARCHPATH     8   SELECT pg_catalog.set_config('search_path', '', false);
                           false            "           1262    16548    Lido    DATABASE     ~   CREATE DATABASE "Lido" WITH TEMPLATE = template0 ENCODING = 'UTF8' LOCALE_PROVIDER = libc LOCALE = 'Belarusian_Belarus.1251';
    DROP DATABASE "Lido";
                     postgres    false            �            1255    17055    update_order_total()    FUNCTION     �  CREATE FUNCTION public.update_order_total() RETURNS trigger
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
       public               postgres    false            �            1259    16953    Order    TABLE     �  CREATE TABLE public."Order" (
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
       public         heap r       postgres    false            �            1259    16952    Order_orderid_seq    SEQUENCE     �   CREATE SEQUENCE public."Order_orderid_seq"
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 *   DROP SEQUENCE public."Order_orderid_seq";
       public               postgres    false    228            #           0    0    Order_orderid_seq    SEQUENCE OWNED BY     K   ALTER SEQUENCE public."Order_orderid_seq" OWNED BY public."Order".orderid;
          public               postgres    false    227            �            1259    16930    dish    TABLE     �  CREATE TABLE public.dish (
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
       public         heap r       postgres    false            �            1259    16929    dish_dishid_seq    SEQUENCE     �   CREATE SEQUENCE public.dish_dishid_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 &   DROP SEQUENCE public.dish_dishid_seq;
       public               postgres    false    222            $           0    0    dish_dishid_seq    SEQUENCE OWNED BY     C   ALTER SEQUENCE public.dish_dishid_seq OWNED BY public.dish.dishid;
          public               postgres    false    221            �            1259    16964    dishingredient    TABLE     �   CREATE TABLE public.dishingredient (
    dishid integer NOT NULL,
    ingredientid integer NOT NULL,
    quantity numeric(10,2)
);
 "   DROP TABLE public.dishingredient;
       public         heap r       postgres    false            �            1259    16923    employee    TABLE     ^  CREATE TABLE public.employee (
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
       public         heap r       postgres    false            �            1259    16922    employee_employeeid_seq    SEQUENCE     �   CREATE SEQUENCE public.employee_employeeid_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 .   DROP SEQUENCE public.employee_employeeid_seq;
       public               postgres    false    220            %           0    0    employee_employeeid_seq    SEQUENCE OWNED BY     S   ALTER SEQUENCE public.employee_employeeid_seq OWNED BY public.employee.employeeid;
          public               postgres    false    219            �            1259    16959    employeeposition    TABLE     k   CREATE TABLE public.employeeposition (
    employeeid integer NOT NULL,
    positionid integer NOT NULL
);
 $   DROP TABLE public.employeeposition;
       public         heap r       postgres    false            �            1259    16937 
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
       public         heap r       postgres    false            �            1259    16936    ingredient_ingredientid_seq    SEQUENCE     �   CREATE SEQUENCE public.ingredient_ingredientid_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 2   DROP SEQUENCE public.ingredient_ingredientid_seq;
       public               postgres    false    224            &           0    0    ingredient_ingredientid_seq    SEQUENCE OWNED BY     [   ALTER SEQUENCE public.ingredient_ingredientid_seq OWNED BY public.ingredient.ingredientid;
          public               postgres    false    223            �            1259    16969 	   orderdish    TABLE     �   CREATE TABLE public.orderdish (
    orderid integer NOT NULL,
    dishid integer NOT NULL,
    count integer DEFAULT 1 NOT NULL
);
    DROP TABLE public.orderdish;
       public         heap r       postgres    false            �            1259    16914    position    TABLE     m  CREATE TABLE public."position" (
    positionid integer NOT NULL,
    name character varying(100) NOT NULL,
    salary numeric(10,2) NOT NULL,
    workschedule character varying(100),
    responsibilities text,
    CONSTRAINT name_check CHECK (((name)::text ~ '^[a-zA-Zа-яА-Я0-9 ,."-]+$'::text)),
    CONSTRAINT salary_check CHECK ((salary >= (0)::numeric))
);
    DROP TABLE public."position";
       public         heap r       postgres    false            �            1259    16913    position_positionid_seq    SEQUENCE     �   CREATE SEQUENCE public.position_positionid_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 .   DROP SEQUENCE public.position_positionid_seq;
       public               postgres    false    218            '           0    0    position_positionid_seq    SEQUENCE OWNED BY     U   ALTER SEQUENCE public.position_positionid_seq OWNED BY public."position".positionid;
          public               postgres    false    217            �            1259    16907    supplier    TABLE     �  CREATE TABLE public.supplier (
    supplierid integer NOT NULL,
    companyname character varying(100) NOT NULL,
    contactinformation character varying(255),
    reliabilityrating integer,
    productcategory character varying(100),
    CONSTRAINT companyname_check CHECK (((companyname)::text ~ '^[a-zA-Zа-яА-Я0-9 ,."-]+$'::text)),
    CONSTRAINT reliabilityrating_check CHECK (((reliabilityrating >= 1) AND (reliabilityrating <= 10)))
);
    DROP TABLE public.supplier;
       public         heap r       postgres    false            �            1259    16906    supplier_supplierid_seq    SEQUENCE     �   CREATE SEQUENCE public.supplier_supplierid_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 .   DROP SEQUENCE public.supplier_supplierid_seq;
       public               postgres    false    216            (           0    0    supplier_supplierid_seq    SEQUENCE OWNED BY     S   ALTER SEQUENCE public.supplier_supplierid_seq OWNED BY public.supplier.supplierid;
          public               postgres    false    215            �            1259    16944    visitor    TABLE     �  CREATE TABLE public.visitor (
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
       public         heap r       postgres    false            �            1259    16943    visitor_visitorid_seq    SEQUENCE     �   CREATE SEQUENCE public.visitor_visitorid_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 ,   DROP SEQUENCE public.visitor_visitorid_seq;
       public               postgres    false    226            )           0    0    visitor_visitorid_seq    SEQUENCE OWNED BY     O   ALTER SEQUENCE public.visitor_visitorid_seq OWNED BY public.visitor.visitorid;
          public               postgres    false    225            K           2604    16956    Order orderid    DEFAULT     r   ALTER TABLE ONLY public."Order" ALTER COLUMN orderid SET DEFAULT nextval('public."Order_orderid_seq"'::regclass);
 >   ALTER TABLE public."Order" ALTER COLUMN orderid DROP DEFAULT;
       public               postgres    false    228    227    228            H           2604    16933    dish dishid    DEFAULT     j   ALTER TABLE ONLY public.dish ALTER COLUMN dishid SET DEFAULT nextval('public.dish_dishid_seq'::regclass);
 :   ALTER TABLE public.dish ALTER COLUMN dishid DROP DEFAULT;
       public               postgres    false    221    222    222            G           2604    16926    employee employeeid    DEFAULT     z   ALTER TABLE ONLY public.employee ALTER COLUMN employeeid SET DEFAULT nextval('public.employee_employeeid_seq'::regclass);
 B   ALTER TABLE public.employee ALTER COLUMN employeeid DROP DEFAULT;
       public               postgres    false    219    220    220            I           2604    16940    ingredient ingredientid    DEFAULT     �   ALTER TABLE ONLY public.ingredient ALTER COLUMN ingredientid SET DEFAULT nextval('public.ingredient_ingredientid_seq'::regclass);
 F   ALTER TABLE public.ingredient ALTER COLUMN ingredientid DROP DEFAULT;
       public               postgres    false    223    224    224            F           2604    16917    position positionid    DEFAULT     |   ALTER TABLE ONLY public."position" ALTER COLUMN positionid SET DEFAULT nextval('public.position_positionid_seq'::regclass);
 D   ALTER TABLE public."position" ALTER COLUMN positionid DROP DEFAULT;
       public               postgres    false    218    217    218            E           2604    16910    supplier supplierid    DEFAULT     z   ALTER TABLE ONLY public.supplier ALTER COLUMN supplierid SET DEFAULT nextval('public.supplier_supplierid_seq'::regclass);
 B   ALTER TABLE public.supplier ALTER COLUMN supplierid DROP DEFAULT;
       public               postgres    false    216    215    216            J           2604    16947    visitor visitorid    DEFAULT     v   ALTER TABLE ONLY public.visitor ALTER COLUMN visitorid SET DEFAULT nextval('public.visitor_visitorid_seq'::regclass);
 @   ALTER TABLE public.visitor ALTER COLUMN visitorid DROP DEFAULT;
       public               postgres    false    225    226    226                      0    16953    Order 
   TABLE DATA           s   COPY public."Order" (orderid, status, totalcost, numberofguests, orderdatetime, visitorid, employeeid) FROM stdin;
    public               postgres    false    228   Ia                 0    16930    dish 
   TABLE DATA           V   COPY public.dish (dishid, name, weight, nutritionalvalue, cost, category) FROM stdin;
    public               postgres    false    222   c                 0    16964    dishingredient 
   TABLE DATA           H   COPY public.dishingredient (dishid, ingredientid, quantity) FROM stdin;
    public               postgres    false    230   �j                 0    16923    employee 
   TABLE DATA           g   COPY public.employee (employeeid, fullname, phonenumber, email, hiredate, contractenddate) FROM stdin;
    public               postgres    false    220   wl                 0    16959    employeeposition 
   TABLE DATA           B   COPY public.employeeposition (employeeid, positionid) FROM stdin;
    public               postgres    false    229   �p                 0    16937 
   ingredient 
   TABLE DATA           p   COPY public.ingredient (ingredientid, name, unitofmeasurement, instock, expirationdate, supplierid) FROM stdin;
    public               postgres    false    224   kq                 0    16969 	   orderdish 
   TABLE DATA           ;   COPY public.orderdish (orderid, dishid, count) FROM stdin;
    public               postgres    false    231   't                 0    16914    position 
   TABLE DATA           ^   COPY public."position" (positionid, name, salary, workschedule, responsibilities) FROM stdin;
    public               postgres    false    218   �u                 0    16907    supplier 
   TABLE DATA           s   COPY public.supplier (supplierid, companyname, contactinformation, reliabilityrating, productcategory) FROM stdin;
    public               postgres    false    216   xy                 0    16944    visitor 
   TABLE DATA           W   COPY public.visitor (visitorid, name, birthdate, phonenumber, preferences) FROM stdin;
    public               postgres    false    226   �       *           0    0    Order_orderid_seq    SEQUENCE SET     B   SELECT pg_catalog.setval('public."Order_orderid_seq"', 34, true);
          public               postgres    false    227            +           0    0    dish_dishid_seq    SEQUENCE SET     ?   SELECT pg_catalog.setval('public.dish_dishid_seq', 204, true);
          public               postgres    false    221            ,           0    0    employee_employeeid_seq    SEQUENCE SET     F   SELECT pg_catalog.setval('public.employee_employeeid_seq', 32, true);
          public               postgres    false    219            -           0    0    ingredient_ingredientid_seq    SEQUENCE SET     J   SELECT pg_catalog.setval('public.ingredient_ingredientid_seq', 47, true);
          public               postgres    false    223            .           0    0    position_positionid_seq    SEQUENCE SET     F   SELECT pg_catalog.setval('public.position_positionid_seq', 30, true);
          public               postgres    false    217            /           0    0    supplier_supplierid_seq    SEQUENCE SET     F   SELECT pg_catalog.setval('public.supplier_supplierid_seq', 46, true);
          public               postgres    false    215            0           0    0    visitor_visitorid_seq    SEQUENCE SET     D   SELECT pg_catalog.setval('public.visitor_visitorid_seq', 45, true);
          public               postgres    false    225            k           2606    16958    Order Order_pkey 
   CONSTRAINT     W   ALTER TABLE ONLY public."Order"
    ADD CONSTRAINT "Order_pkey" PRIMARY KEY (orderid);
 >   ALTER TABLE ONLY public."Order" DROP CONSTRAINT "Order_pkey";
       public                 postgres    false    228            e           2606    16935    dish dish_pkey 
   CONSTRAINT     P   ALTER TABLE ONLY public.dish
    ADD CONSTRAINT dish_pkey PRIMARY KEY (dishid);
 8   ALTER TABLE ONLY public.dish DROP CONSTRAINT dish_pkey;
       public                 postgres    false    222            o           2606    16968 "   dishingredient dishingredient_pkey 
   CONSTRAINT     r   ALTER TABLE ONLY public.dishingredient
    ADD CONSTRAINT dishingredient_pkey PRIMARY KEY (dishid, ingredientid);
 L   ALTER TABLE ONLY public.dishingredient DROP CONSTRAINT dishingredient_pkey;
       public                 postgres    false    230    230            c           2606    16928    employee employee_pkey 
   CONSTRAINT     \   ALTER TABLE ONLY public.employee
    ADD CONSTRAINT employee_pkey PRIMARY KEY (employeeid);
 @   ALTER TABLE ONLY public.employee DROP CONSTRAINT employee_pkey;
       public                 postgres    false    220            m           2606    16963 &   employeeposition employeeposition_pkey 
   CONSTRAINT     x   ALTER TABLE ONLY public.employeeposition
    ADD CONSTRAINT employeeposition_pkey PRIMARY KEY (employeeid, positionid);
 P   ALTER TABLE ONLY public.employeeposition DROP CONSTRAINT employeeposition_pkey;
       public                 postgres    false    229    229            g           2606    16942    ingredient ingredient_pkey 
   CONSTRAINT     b   ALTER TABLE ONLY public.ingredient
    ADD CONSTRAINT ingredient_pkey PRIMARY KEY (ingredientid);
 D   ALTER TABLE ONLY public.ingredient DROP CONSTRAINT ingredient_pkey;
       public                 postgres    false    224            q           2606    16973    orderdish orderdish_pkey 
   CONSTRAINT     c   ALTER TABLE ONLY public.orderdish
    ADD CONSTRAINT orderdish_pkey PRIMARY KEY (orderid, dishid);
 B   ALTER TABLE ONLY public.orderdish DROP CONSTRAINT orderdish_pkey;
       public                 postgres    false    231    231            a           2606    16921    position position_pkey 
   CONSTRAINT     ^   ALTER TABLE ONLY public."position"
    ADD CONSTRAINT position_pkey PRIMARY KEY (positionid);
 B   ALTER TABLE ONLY public."position" DROP CONSTRAINT position_pkey;
       public                 postgres    false    218            _           2606    16912    supplier supplier_pkey 
   CONSTRAINT     \   ALTER TABLE ONLY public.supplier
    ADD CONSTRAINT supplier_pkey PRIMARY KEY (supplierid);
 @   ALTER TABLE ONLY public.supplier DROP CONSTRAINT supplier_pkey;
       public                 postgres    false    216            i           2606    16951    visitor visitor_pkey 
   CONSTRAINT     Y   ALTER TABLE ONLY public.visitor
    ADD CONSTRAINT visitor_pkey PRIMARY KEY (visitorid);
 >   ALTER TABLE ONLY public.visitor DROP CONSTRAINT visitor_pkey;
       public                 postgres    false    226            {           2620    17056    orderdish order_total_update    TRIGGER     �   CREATE TRIGGER order_total_update AFTER INSERT OR UPDATE ON public.orderdish FOR EACH ROW EXECUTE FUNCTION public.update_order_total();
 5   DROP TRIGGER order_total_update ON public.orderdish;
       public               postgres    false    231    232            s           2606    17000    Order Order_employeeid_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public."Order"
    ADD CONSTRAINT "Order_employeeid_fkey" FOREIGN KEY (employeeid) REFERENCES public.employee(employeeid);
 I   ALTER TABLE ONLY public."Order" DROP CONSTRAINT "Order_employeeid_fkey";
       public               postgres    false    220    228    4707            t           2606    16995    Order Order_visitorid_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public."Order"
    ADD CONSTRAINT "Order_visitorid_fkey" FOREIGN KEY (visitorid) REFERENCES public.visitor(visitorid);
 H   ALTER TABLE ONLY public."Order" DROP CONSTRAINT "Order_visitorid_fkey";
       public               postgres    false    4713    226    228            w           2606    17015 )   dishingredient dishingredient_dishid_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.dishingredient
    ADD CONSTRAINT dishingredient_dishid_fkey FOREIGN KEY (dishid) REFERENCES public.dish(dishid);
 S   ALTER TABLE ONLY public.dishingredient DROP CONSTRAINT dishingredient_dishid_fkey;
       public               postgres    false    4709    230    222            x           2606    17020 /   dishingredient dishingredient_ingredientid_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.dishingredient
    ADD CONSTRAINT dishingredient_ingredientid_fkey FOREIGN KEY (ingredientid) REFERENCES public.ingredient(ingredientid);
 Y   ALTER TABLE ONLY public.dishingredient DROP CONSTRAINT dishingredient_ingredientid_fkey;
       public               postgres    false    224    4711    230            u           2606    17005 1   employeeposition employeeposition_employeeid_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.employeeposition
    ADD CONSTRAINT employeeposition_employeeid_fkey FOREIGN KEY (employeeid) REFERENCES public.employee(employeeid);
 [   ALTER TABLE ONLY public.employeeposition DROP CONSTRAINT employeeposition_employeeid_fkey;
       public               postgres    false    220    4707    229            v           2606    17010 1   employeeposition employeeposition_positionid_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.employeeposition
    ADD CONSTRAINT employeeposition_positionid_fkey FOREIGN KEY (positionid) REFERENCES public."position"(positionid);
 [   ALTER TABLE ONLY public.employeeposition DROP CONSTRAINT employeeposition_positionid_fkey;
       public               postgres    false    4705    218    229            r           2606    16986 %   ingredient ingredient_supplierid_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.ingredient
    ADD CONSTRAINT ingredient_supplierid_fkey FOREIGN KEY (supplierid) REFERENCES public.supplier(supplierid);
 O   ALTER TABLE ONLY public.ingredient DROP CONSTRAINT ingredient_supplierid_fkey;
       public               postgres    false    224    4703    216            y           2606    17030    orderdish orderdish_dishid_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.orderdish
    ADD CONSTRAINT orderdish_dishid_fkey FOREIGN KEY (dishid) REFERENCES public.dish(dishid);
 I   ALTER TABLE ONLY public.orderdish DROP CONSTRAINT orderdish_dishid_fkey;
       public               postgres    false    4709    222    231            z           2606    17025     orderdish orderdish_orderid_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.orderdish
    ADD CONSTRAINT orderdish_orderid_fkey FOREIGN KEY (orderid) REFERENCES public."Order"(orderid);
 J   ALTER TABLE ONLY public.orderdish DROP CONSTRAINT orderdish_orderid_fkey;
       public               postgres    false    4715    228    231                       6104    17058    name    PUBLICATION     \   CREATE PUBLICATION name FOR ALL TABLES WITH (publish = 'insert, update, delete, truncate');
    DROP PUBLICATION name;
                     postgres    false               �  x��T�q1|�Q(�p<6���8��nF"��٣����j�p���?����ǯ�ϐ{2
9����S�~S���=�	`Ku|z��X6@&2B!�nl������1���i���29(t�'��|U��B�s���b�,��߲��>g�'�,uv �bPv�=EY>�G��DhOu�V��I41����8G�@�� l˨�,:U��g��F4�A��3o���v��"��gܓ�@�)hy���L��s��B�0�:��L�b_�x6d7
���!��ܦ�����B��p�m�����K�:E��Ѫ�exo�Z�v�8�Wv��4��!���\p��4��O��%��:���*�*S�۶���!Mw�8l��#�)��سV��U�����3�E�M2&�����0����}N�0���)��	$դ         �  x����n�F��� t��{�D_�#�A�"F��A�izJ���!rmɖ_a�
}��3$ť4bm&��MQ��yf��gV{��Ӱ�fճp]=g�W�J%��0˰��?��|��p�M~�K��=K�i^?�=\��y����P���?�+܅y5�;��_����:I1��a�au\5˧>o靰��ge�_߄����~��Gx�Ş�-t���Rw�yV�����+���6\'�fې~ZnC�C��O�HHct�X��E�ͺ& zWG�>%dL���Y�L���ɷ���� �FG͕�QY2� ��[��@g5e�ٱzK���z�;$vZ��T�\�(8ò:���vn��|G#���Y�t3�M�Gc�ð�n1�~��(�Jx�N��6 ��H�[�rC‟�%�N�pE��%^�$^���6���n��U��x4�-�Y{]��پU������y
�e�2|
g{:g1A���1C�����.6�R�lڷ
�eٌ�"�(l��ye��Z�����<ǟ/���F�
h������Z����$љ����mS���뗥t���^�Vm9mcN#p"�i�����_�����&�N�Ԕ���[�7�{�Q[������´Ix�i�0�w�+��v%f#��dW��V�{©ۧ�MeU�����aVo@��?x!�{��-I�;�z6i�Hڇ�Zbs�h~���ѡ��Q����k��cp|�7�'��E�S��O!g��3�e���c�~�D�G��TjH\1�O����KW��n/��'�y�������?���k ��ה�+�# �y�<1�*I����ʹX��]�J�aa������C�sIo/q���-�(wJ�� �/��л^�E�0��~�D�	���J�!X'G�n7$ݵ�#�����SogN���!)=�}6��_/yI�4���Y۶�70C���w��G5�V�黃���>��#8�#ւ���������Y�CXP���XC弬��P
�-M�,��"���z3���؁�"#�Je�;�ݫf��ɼ�xbG
��xtI��X���K����HE����e#!��KtS�i�I�y�V�N%�3V4Jʛ��]4qGՎ�U�*5l>�ۘ�t����e{�7�0�Em�0tD�т:��mی�Y���9�`�7#U���H�t�4��rx��d����"wV��!*6CT7XVM��fl�v�L��nDo۹!m4�����X�u]}��7*�M��ޤ�uÈl���������5�{���+�i+'II?2{ �㪩q\$y���j�I1��3Q�鬋�(޳6|G'Z�#�)ʂ�yu�h���K��9����SQ�Nw�1��3_��`N��G}­&�7�%�C~�:�^�	�����뤔S��B�ʨ||�l$)Yf������	׮�[5��1$;>���p�Q6/;��65{=t�_9�����ΣV`]pȈ�����C-�,u$ �-2 K�!��3�����5ftݸ���z�xaL~���u	��-��Y*�5v��G��:���w�Ot�$!�4y�6k:�D����@C>ˏW���}�7ʛ@0%o���5��_�4^��t�U%F<Ԝ����-�u��ӥ�;&:ֲ��_Y
���¥�K
�B#�k�Pѯ�h?RƷ)���Dm��(i�E&ͽ]]!���g�bNm9��[���K���T�Fn�����	R:�gܭ�g�1(ડ��Ƿڊ��H_��(�wex�����JRJ��6Ru`ˡà�r.*�P�F���u� �[�����I��Yw§�(J�c��KZ�b���c7��qd�5c�F�eԅ���E�)1�����č��.��k��J��+���b��M`��֖S���У^��"���}��ԣ냨a���Y�����޿ixǵ         �  x�U��q@!C��b2`6���_G�`Dno4�0fik�&�����5Kk�19�r:Vi��f0�e2O������:�<�,(e����f*�얟�vl�Xv-D�u�Лr�tkQv��+B�.�:��G�h9��1�0���LCL��f���3��x�&�V�6��e+�uҷ��p��q�F��:�:�ܷq���U������Rc*�2s�N��K�vh�u���u����}0�#���2�.�+����j�����/�	�F#����}�j��R� N'��	��p�My�8���ȍ�I�e�#A���uM�!_F��9e�az�E�O��]�÷�~/�����
^��G���t빆��/���^>i�2��Fq&�ȧ%��Ώ|���ە��#�W3���j�qA��ӓḎ^8�zy��u�_]^��?�m������?����         Z  x��V�n�F�?�ծ�C|��,L821\%��O�jI��v���Mժ��Jti��W�Q�762�o�E��3���9gt&�񐜋H�b*�ˊ�Y,���Ĥ".��,9s|���3�arK�Fͬ����?h��w�M����*7�T�'��z��8��X*8��۹�_�������(�y��5��7����{@c�o��H��8�r���v�,-�R��<q�TRE܊�dV�Tc-���lia��<9M^RK�f���$&H�#��|Ef����A��'g9ͭ�&c���rIU���ոz���5T���b�O�IE�QY	�t��PO������o��L��?�=�f�D����^{G��Y�-&~�ɐI�I2����dkЖ+0��a��EbR����^߳���H%�iz���$�ɋUm��ld�>�9������>Q�i�R���3�0�D����J��x�x�q*��g��v��tNl�Ĝ�&��%�$n�i��3��8M��<��k�!	�͏dkżK'DI`� ��
�\/C��W���}ҬGd.+k���;�x�Gݶ����Q0%�f$����1�e/����h~#�����uֆ���]�2!gˬ-�Lub:rwL��+���!~�1O^�#��-���`���]��{%���$����e���(��K�&��Y���}��uz*��[D� H^'�Ʌ���ѧL�źNgz
�+��G���������]�:���3���F��,��"�ğsi3%��0ݣ�SN�Nt,�Af�;�I��|fGJ�z��c�R����Pّh�d�~��`~��S�BPp8����7���s�.Jox��>��;a�p�EM�{�T�����q�����+�7|�,��E�"~��Uj�{\/C\
�n���[�Z��(�"ohq�Up��bz�~������ ��yUX��&�'��H�t#�˞�~�-��]v:J�$ح�U��q�$Xv�*�
�`�� �}Z��,�ζCt�/�]�����.	n�n:*�TnVHg�B�����Z�Y~��� �5ݲ,f��nɬ~�TӴ� =T�4         z   x�ϹDA��Y���ￎe���B� :�pғ���Y����N6o����Y.*�z9�Y.��.4��t�f�hլ��s��Yi�O7z��'���a��Ϟ��V���*�\���?�3"         �  x�}U[r�@��9��}H���0��UN�$���J��'`q���g%�"�������R��ɿ�B�+?U6IΒD�Ĥ��m������y����%o���> k3�Z҉���� T��W(��l�6Zҹ�{����#`NzH����2��@Ӹ�1Rs�g��?|�������\�+_�w�Q[ڑ�X?���� X��Bށ��<�~ms2�⏐�Qzt�d �����p$GI	����x��g	X��+?�p�ֆ8 6(��M�~�U|�Ɲ(�Ա�6#�K���F�����޽����m$�\�� �����R��e�� �-�߄�f���A����5�\
Ŧ������e�I�?o����Aʛ��˽w���x'��n�4B9rH�/p��V��@vhF2�C��~7J݆l=���-:M��1Gm�%�����x-����b������Vʃ�a���bF�4B�ؽ�>h,�[���,>���1#
�-5zZ��
� S�y	Av�W���[|5�ƾK�qO�ܟʤNɀ�'�Ey|Rs�l�%d��#{pH��[!ۋ�z�?�;�a��{e�cҸxw�hA�$6��L�V�˾H8��>��E�&Y�aU���e�몧\B.Ȳ��rJ��z��t�{CrIw)w��8��e�挈�����         n  x�M�Q�� D��a�T�.s�s,���|���`c��s?����k��
=/���?�w��R	=Q�Qw���\|o�zμ��T*�6��oQ��s�;�Aw�c~C���U�SZu�/�q��h6�4x�mx��m�50B� �	����_o;}�*�^@���.���]vɡ_���c�P),/��p.����R�ƥ�B�� ��U����� ]}��a�4h�Uq�ǝP'}.@p^^�Yh;���8��ݼ	������� �Dؕ�b��&�e1��H�ҍM87 �����m������)l�I�&ժi�x�tD�M��߈aC+l�> ��D�I+��?���*���         �  x��V]N�@~�=�PHv�r�� U�k���E�*!�U�>U��$Ĺ�������!8i_�Y�|3;�}3n*�M�;ڡ)tG��(۬�w�uek6�����}īC6Q���h$����u�)���>M(�G�Rt�z;�c+[�����؝F�e!��{��gg6dw�:��p]�#�	N��4vg�jSW��(��e����2m��k<��^$4q]e6\��x����c���-���~!g��CeL5�["�#I\\��P��R<�h�0�@[T�'M�z�S������MmbE��������@ʋt'�g�Q�&��w��#��)I1fy��f����`�N=�� Td>Q{��W��:��.���g�Dbf�3�°�R\kx��2ڂwc����s�<9�ԗf�k3��҄�\h��f�}	�:B}�8 rm��o�U���Ϥ2� �����L���X����D�א��!����*�|�#�+��f2tRh�7���Q�G
��&�So=���M�����u��@�<+g"nc�Ȑ�L|'JŽ@���|���������|�s1�Mxk-[���aѸy���3��O��04h�mk6o%�rڮ�~y�r>S��,m�L�\���ƍ�;�`qbA�DJt��ެ�b5�-{�L�p4�A�b&
�Sw����K?1d��v��/��.���g.A�=9g�a�p�&�S}>����6R|��j����-���ѯW�f���j�g��0M&<�XLs�6�EYq򚺡k���#T8��u�w�Z��yA4�8	%�����p�9=D�=rw�Nԓ
��0^����O9e�� mVW&��U�Q{�52��ϕ��`܁�ǲ��+��s,�������˨lR��+��E?J��MO�0���~�m̟bV��|��ϯ�l�?�����g�0T�9[ ��}s����q��j�����         �  x��W]OW}�����X�_���JU���}iU�r��Z!�rHQ� $�H�m�b���$JC�����gfwm����ޯ�{朙�R�NZ�,���"U4��SS雉)�z*=z�ވNM�HO����>u護q���J��mG�S����kjPYS*�^�s_%,\Ӳ�,�7����v0P�q5<Wi��I��*��6���w�[��'#Z���.b�Л��.k���s޼,���b�;���:��1Ф}:��7�����<C�'��tDm,m{�ҥ�m�j\�ˎ����%jk��~4���"QEyܗ�X�IܙI&}`���S��@#�5�*��uf��x��YM�)eۦ9��Ʌ ����qy�5�1��ރ�}L���b���� \�w��W.Gp�U�!P���U��A��K���g�_p��oLc��]rKŇ�,�L���eG�V �ك,{`��i�RC|F��M�t�{�����9����E�9�ǩu*�Gp����]��Ǚ���&f��b8Xü. ��/L� W-';�j����>T�6�������eeY��8B�\fk�X_����m=��e��6��3�E��.��9K�	+��<�	��2ga��{�u��S��@�T3��?���z�E������c���V�/�|�#�1��#�����m��[fUG�qv��=ɕ$Mm��z�W!�f����Tj�Vz�%A�$ M�*��,7#���2sJL��/ӊ�-�ഀ�^ȧ�c^��Vq�p"�¦58��
~>�|�F��f!8��P{���xӢH���nN�&�|�$�Nê����^7*P��{fco�>�ȶD�EQ�@�%E6�#'��Ol��Yf���X1G\&$��d�#I���м���G���#��B��S$uΈh�W���Z2�<K��t��ed�>cs�A-�`��� l�[?r!qP��JY{Y��!JK޲�i���q#3LKY���Ӳp��kP_�.�����۩���&��V%;X\j����y���2��YBY��Vǔ�`�������2ɩ�	2S[���$��Qfm�`塜�G����]��2i�9�8Wb�֖P\�� i�Wm���%�R-KM���ܘ��򗺟 ��r�~��KI	5���5GJ�e��t�Z�����wL�40�!W�H�`��N��k�ڕo�^���T�C�S(��4����Ǻ��|t'ؓ�m��>|+R�!b���`O��c,�?���U�>�+~�d;��0�T�dTN�;aʪ
�%�a!�`�h[��o�_�,��YZ��:��:-+ʘפ��h�9,re�+��T��t�^b���T��;�����:�?��j
�`�����d�� �4�Y�c�UA/�2HSz�#�o���*�vQT��1?���s!���րG�8�3H3\NZ��h��IE$<�p���)�Gz2uK�h�r���o��k0�	��b��-׼��G(��m��L���R3wn���\�$���KL�2��r&9�����bǵ���k���g8꛻�7���z@T����A��\4كr�����&}�K^$�U=����鯯^�^j	�=�R8Q���#��>1*}��w��R�Fq7�+��D�F'�q�:�d�<�D��bu.         @  x��X�R�P��"�[;�	��}Fp����֎�֎N;!��@�+�}���x#4*�"��'�g�좔C?iH#�Oc}���9s�r�GC�v�'��)����O9�f��~+��fˡ_X�Ҁ��� �:S�;.�������t�FxJ]���c�?���睮n�4�]���yD�5�;t	���c�)v�9�{� R�1�P"+0�p��_PG�,A=�&���ܛ� F�tB�8xO��� ��T�;e���>�b�'��1s�3bh��x���]����G1���?b&��x2��OH`~xo��k���>�����K�X��i���}3���w�6�%�'�Z�8\�#�ÌС�)�r����N��16�3��C����:��� .��>\����X��SƆ��☁�s<�Q���D��˝��ϘmV1��x�#��1`A��t|تY�kuRni���CW e���GK�Gv���Ż\���b�ty�,����1�������nٙ3�f��1ͼ�{1'�#�Θr�/����
	ƸH�������Ň<��`�`yA��Uj� �4\��>T錳�[9��#��_��F,בH��>T���,ű�>X8�l�Geq�>�9�&���\cp�众�,s:�����d$��� ��]������#�⸐�������.6�D���+<�r�@�P�fu�@�&'��l���,��&�6iW��ư��95b-��il��87	JX�XӶ��elnq��s��1�D2U6���W(����QfreS^Q�����L���D�))Q�(-��U[�}և�a��6�7H#)��y�>a���r��'˨Sa@ۙrKq.��k�~��T�Ժ&4���U�x�*V�)�ԻѪ^B��9���07�������MS,�YѰj�_2gi��ȱ������q �z��o���/�H,)�wݹLi����a�<�������Zhѷ�R<n1v>�_��|�X4Z��m�Xsn%��&�h�]�E�+6to�@Օ��F�9��j�l�ߦN�ܭ���$�&Z�����T_��6�LG���V���$�     