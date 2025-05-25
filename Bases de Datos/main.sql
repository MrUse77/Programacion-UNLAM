use AdventureWorks2022

SELECT top 10 PostalCode CP, addressline1
from [Person].[Address]
where city='Paris' and (PostalCode='75017' or PostalCode='75008')
order by PostalCode asc, addressline1 desc

SELECT DISTINCT PostalCode, addressline1
from [Person].[Address]
where AddressLine1 like '%boulevard%'
order by PostalCode

SELECT *
from [Person].[Address]
where PostalCode IN('1002','1597','14227')
order by PostalCode

SELECT *
from [Person].[Address]
where AddressID Between 444 and 555 /*Extremos incluidos*/
order by AddressID asc