CREATE DATABASE IF NOT EXISTS db_gbilling;
use db_gbilling;

create table if not exitts gb_users(
	user_id	int		NOT NULL	PRIMARY KEY,
	user_code	varchar(13)	NOT NULL	UNIQUE KEY,
	user_name	varchar(40)	NOT NULL,
	user_pass	varchar(40)	NOT NULL,
	user_phone	varchar(10)	NOT NULL,
	user_email	varchar(60)	NOT NULL
);

create table if not exists gb_province(
	pro_int	int		NOT NULL 	PRIMARY KEY,
	pro_code	varchar(5)	NOT NULL	UNIQUE KEY, 
	Pro_name	varchar(50)	NOT NULL
);

create table if not exists gb_customers(
	cus_id 	int 		NOT NULL	PRIMARY KEY,
	cus_code	varchar(13)	NOT NULL	UNIQUE KEY, 
	cus_name 	varchar(50) 	NOT NULL,
	cus_addr1	varchar(70)	NOT NULL,
	cus_addr2	varchar(70)	NOT NULL,
	cus_pro_code	varchar(5)	NOT NULL,
	cus_zip_code	varchar(5)	NOT NULL,
	cus_phone	varchar(10)	NOT NULL,
);


/* In the statement below and the second LOAD TABLE statement 
   further in the file, note that we have put the fully qualified
   path name for the file.  This is necessary because MySQL will
   by default look in the database directory, not the current local
   directory or your project directory.  I would not recommend putting
   the text files in your database directory - you probably want to 
   keep that as clean as possible.

   Also note that the text files being imported are tab-delimited
   and have the same number of fields as the table has columns.
*/

LOAD DATA INFILE "/mnt/DOS_hda2/newriders/book/ch6/new_item_master.txt"
	INTO TABLE tbl_items;

/* Check for the success of the import. Should be approx 600 records */

select count(*) from tbl_items;

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

/*
LOAD DATA INFILE "/mnt/DOS_hda2/newriders/book/ch6/cust_mast.txt"
	INTO TABLE tbl_customers;

   Again, check the success of the import operation.  This time, 
   we are looking for approximately 170 records.
*/

/* Now alter tbl_customers to be auto_increment on the first column.
   The reason we do this now instead of at table creation is to avoid
   problems importing the data.  When we read the text file in for
   the initial data load, we dont want MySQL to try to auto_increment
   the first column because we already have values for that column, and
   they are not necessarily sequential or starting at 1.  Therefore, 
   we will now alter the table so that the first column becomes 
   our auto-incrememt-er from here forward.  Note that this is only
   good until the first add, edit, or delete to our new table.  From
   that point on, this procedure will no longer be valid.

   Also note that we dont need to restate PRIMARY KEY.  If we attempt
   to put it in the ALTER TABLE statement, MySQL will issue a duplicate
   primary key error message.

   One final comment.  If you end one of these files with a comment,
   it will produce an error when you attempt to feed it to MySQL.  
   Apparently, it is expecting a semi-colon as the last thing, so we
   will appease the software...


ALTER TABLE tbl_customers 
MODIFY num	SMALLINT 
		NOT NULL 
		AUTO_INCREMENT;

*/
