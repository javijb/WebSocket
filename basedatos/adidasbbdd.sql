--
-- PostgreSQL database dump
--

-- Dumped from database version 10.10
-- Dumped by pg_dump version 10.10

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

SET default_with_oids = false;

--
-- Name: compras; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.compras (
    idcompra integer NOT NULL,
    iduser integer,
    idproducto integer,
    fecha_comp date
);


ALTER TABLE public.compras OWNER TO postgres;

--
-- Name: compras_idcompra_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.compras_idcompra_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.compras_idcompra_seq OWNER TO postgres;

--
-- Name: compras_idcompra_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.compras_idcompra_seq OWNED BY public.compras.idcompra;


--
-- Name: productos; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.productos (
    idproducto integer NOT NULL,
    nombre character varying NOT NULL,
    categoria character varying NOT NULL,
    modelo character varying NOT NULL,
    stock integer NOT NULL,
    precio money
);


ALTER TABLE public.productos OWNER TO postgres;

--
-- Name: productos_idproducto_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.productos_idproducto_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.productos_idproducto_seq OWNER TO postgres;

--
-- Name: productos_idproducto_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.productos_idproducto_seq OWNED BY public.productos.idproducto;


--
-- Name: user; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public."user" (
    iduser integer NOT NULL,
    nombre character varying NOT NULL,
    apellidos character varying NOT NULL,
    email character varying NOT NULL,
    telefono character varying NOT NULL,
    password character varying NOT NULL,
    fecha_nac character varying NOT NULL,
    genero character varying NOT NULL,
    nacionalidad character varying NOT NULL,
    provincia character varying NOT NULL,
    direccion character varying NOT NULL
);


ALTER TABLE public."user" OWNER TO postgres;

--
-- Name: user_iduser_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.user_iduser_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.user_iduser_seq OWNER TO postgres;

--
-- Name: user_iduser_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.user_iduser_seq OWNED BY public."user".iduser;


--
-- Name: compras idcompra; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.compras ALTER COLUMN idcompra SET DEFAULT nextval('public.compras_idcompra_seq'::regclass);


--
-- Name: productos idproducto; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.productos ALTER COLUMN idproducto SET DEFAULT nextval('public.productos_idproducto_seq'::regclass);


--
-- Name: user iduser; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."user" ALTER COLUMN iduser SET DEFAULT nextval('public.user_iduser_seq'::regclass);


--
-- Data for Name: compras; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.compras (idcompra, iduser, idproducto, fecha_comp) FROM stdin;
\.


--
-- Data for Name: productos; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.productos (idproducto, nombre, categoria, modelo, stock, precio) FROM stdin;
2	Chaqueta Adidas Sportive Black	Chaquetas	Adidas Original Sportive	6	109,99 €
4	Adidas SST TP	Zapatos	Adidas Original	3	55,96 €
3	Adidas California Blue	Camisetas	Adidas Original	5	22,70 €
\.


--
-- Data for Name: user; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public."user" (iduser, nombre, apellidos, email, telefono, password, fecha_nac, genero, nacionalidad, provincia, direccion) FROM stdin;
1	holahola	 lncfeqw	jkbdsced@gmail.com	987654321	hola	1997-02-20	hjcewb	wkcmekc	wcemknbc	ewjncwjenck
2	eewew	ewc	edeeqqd@gmail.com	987654321	asdqd	20/12/1998	wegggw	evdevewvw	provincia	direccion
3	fedwrfeawrfea	tyduyuyuyf	kkryyk@gmail.com	987654321	12354	2020-02-19	Masculino	”España”	jngcfdcfd	hgvcgytyftyf
4	Javier	Jaume Belda	javierjaume@estudiante.edib.es	659933958	holahola1	1998-12-20	Masculino	”España”	Islas Baleares	Calle Caro, 76
\.


--
-- Name: compras_idcompra_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.compras_idcompra_seq', 3, true);


--
-- Name: productos_idproducto_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.productos_idproducto_seq', 4, true);


--
-- Name: user_iduser_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.user_iduser_seq', 4, true);


--
-- Name: compras compras_idproducto_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.compras
    ADD CONSTRAINT compras_idproducto_key UNIQUE (idproducto);


--
-- Name: compras compras_iduser_key; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.compras
    ADD CONSTRAINT compras_iduser_key UNIQUE (iduser);


--
-- Name: compras compras_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.compras
    ADD CONSTRAINT compras_pkey PRIMARY KEY (idcompra);


--
-- Name: productos productos_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.productos
    ADD CONSTRAINT productos_pkey PRIMARY KEY (idproducto);


--
-- Name: user user_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public."user"
    ADD CONSTRAINT user_pkey PRIMARY KEY (iduser);


--
-- Name: compras fk1_idproducto; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.compras
    ADD CONSTRAINT fk1_idproducto FOREIGN KEY (idproducto) REFERENCES public.productos(idproducto) ON UPDATE CASCADE;


--
-- Name: compras fk1_iduser; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.compras
    ADD CONSTRAINT fk1_iduser FOREIGN KEY (iduser) REFERENCES public."user"(iduser) ON UPDATE CASCADE;


--
-- PostgreSQL database dump complete
--

