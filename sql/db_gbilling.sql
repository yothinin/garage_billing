CREATE DATABASE IF NOT EXISTS db_gbilling;
use db_gbilling;

create table if not exists gb_users(
	user_id	int	NOT NULL	PRIMARY KEY,
	user_code	varchar(13)	NOT NULL	UNIQUE KEY,
	user_name	varchar(40)	NOT NULL,
	user_pass	varchar(40)	NOT NULL,
	user_phone	varchar(10)	NOT NULL,
	user_email	varchar(60)	NOT NULL
);

create table if not exists gb_province(
	pro_int	int	NOT NULL 	PRIMARY KEY,
	pro_code	varchar(5)	NOT NULL	UNIQUE KEY,
	pro_name	varchar(50)	NOT NULL
);

create table if not exists gb_customers(
	cus_id 	int 	NOT NULL	PRIMARY KEY,
	cus_code	varchar(13)	NOT NULL	UNIQUE KEY,
	cus_name 	varchar(50) 	NOT NULL,
	cus_addr1	varchar(70)	NOT NULL,
	cus_addr2	varchar(70)	NOT NULL,
	cus_pro_code	varchar(5)	NOT NULL,
	cus_zip_code	varchar(5)	NOT NULL,
	cus_phone	varchar(10)	NOT NULL
);

drop table if exists tbl_customers;
create table tbl_customers
(
	num		smallint	NOT NULL	PRIMARY KEY,
	name		varchar(100)	NOT NULL,
	ship_to_addr1	varchar(100),
	ship_to_addr2	varchar(100),
	ship_to_city	varchar(35),
	ship_to_state	char(2),
	ship_to_zip	varchar(10),
	bill_to_addr1	varchar(100),
	bill_to_addr2	varchar(100),
	bill_to_city	varchar(35),
	bill_to_state	char(2),
	bill_to_zip	varchar(10),
	contact_first	varchar(50),
	contact_last	varchar(50),
	phone		varchar(12),
	title		varchar(50),
	comments	varchar(255)
);
