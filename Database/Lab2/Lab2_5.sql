CREATE TABLE fans_3(UID int NOT NULL PRIMARY KEY, USERNAME char(20) NOT NULL, SEX char(2), BYAER int NOT NULL, CITY char(20));


insert into fans_3 (UID, USERNAME, SEX, BYAER, CITY) 
select UID, USERNAME, SEX, BYAER, CITY from CUSTOMER
where uid in (select uid from Follow
				where UIDFLED = 3);

select * from fans_3;

