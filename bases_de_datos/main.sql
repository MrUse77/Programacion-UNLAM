#create database example_db;

use example_db

create table example_table (
    id int  primary key,
    name varchar(255) not null,
    age int not null
)

select * from example_table


drop table if exists example_table;

