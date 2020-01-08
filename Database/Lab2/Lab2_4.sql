--task 1
insert into CUSTOMER (USERNAME,SEX,BYAER,CITY) values(
'张三','男',1993,'南京');

insert into FOLLOW (UID, UIDFLED) values(
13,2),(
13,3),(
13,5),(
13,1),(
13,7);

select CUSTOMER.UID, CUSTOMER.USERNAME, CUSTOMER.SEX, CUSTOMER.BYAER, CUSTOMER.CITY
from CUSTOMER inner join FOLLOW
on FOLLOW.UID = 13 and FOLLOW.UIDFLED = CUSTOMER.UID
Group by CUSTOMER.UID, CUSTOMER.USERNAME, CUSTOMER.SEX, CUSTOMER.BYAER, CUSTOMER.CITY
order by CUSTOMER.BYAER DESC, CUSTOMER.UID

select * from FOLLOW
where UID = 13;

--task 2
select MBLOG.BID, MBLOG.TITLE, CUSTOMER.USERNAME
from MBLOG , CUSTOMER , thumb as p
where MBLOG.BID not in (select thumb.BID from thumb) and MBLOG.UID = CUSTOMER.UID
Group by MBLOG.BID, MBLOG.TITLE, CUSTOMER.USERNAME
order by MBLOG.TITLE

select * from THUMB;
select * from MBLOG;

--task3
insert into MBLOG (TITLE, UID, PYEAR, PMONTH, PDAY, CONT) values(
'复联4上映，校长包场', 12, 2019, 4, 26, '复联4上映，校长包场');

insert into toptoday (TYEAR, TMONTH, TDAY, BID, TNO) values(
2019, 4, 26 ,12, 1);

select * from toptoday;

select * from MBLOG;

select * from CUSTOMER;

select MBLOG.BID
from MBLOG,CUSTOMER,toptoday
where CUSTOMER.BYAER >= 2000 and MBLOG.UID = CUSTOMER.UID and MBLOG.BID in (select toptoday.BID from toptoday) and CUSTOMER.CITY = '武汉'
group by MBLOG.BID;

--task4
select * from sub;

select sub.UID
from sub
group by sub.UID
having count(sub.LID) = (select count(*) from LABEL);

--task5
insert into CUSTOMER (USERNAME,SEX,BYAER,CITY) values(
'李春生','女',1966,'长春');
insert into CUSTOMER (USERNAME,SEX,BYAER,CITY) values(
'李昂','男',2012,'昆明');

select * from CUSTOMER;

select CUSTOMER.UID, CUSTOMER.BYAER, CUSTOMER.CITY
from CUSTOMER
where CUSTOMER.BYAER not between 1970 and 2010;

--task6
select count(CUSTOMER.UID) as population, CUSTOMER.CITY
from CUSTOMER
Group by CUSTOMER.CITY;

--task7
insert into CUSTOMER (USERNAME,SEX,BYAER,CITY) values(
'李武','男',1986,'武汉');

select * from CUSTOMER;

select CUSTOMER.CITY, count(CUSTOMER.UID) as population, CUSTOMER.BYAER
from CUSTOMER
Group by CUSTOMER.CITY , CUSTOMER.BYAER
order by CUSTOMER.CITY ,count(CUSTOMER.UID) desc;

--task 8
insert into thumb (UID, BID) values(
1,8),(
7,8),(
2,8),(
3,8),(
4,8),(
5,8),(
6,8),(
8,8),(
9,8),(
10,8),(
11,8);

select * from THUMB;

select thumb.BID
from thumb
group by thumb.BID
having count(thumb.BID) > 10;

--task9
insert into CUSTOMER (USERNAME,SEX,BYAER,CITY) values(
'林兴钰','男',2001,'武汉'),(
'林龙','男',2002,'上海'),(
'林芯诗','女',2003,'北京'),(
'林鸢玲','女',2004,'深圳'),(
'谢灵彤','女',2005,'广州'),(
'谢凤丹','女',2006,'成都'),(
'潘辰光','男',2002,'长沙'),(
'潘东','男',2003,'长沙'),(
'宋雯萱','女',2009,'广州'),(
'宋子辰','男',2010,'苏州');

insert into thumb (UID, BID) values(
12,1),(
15,1),(
17,1),(
18,1),(
19,1),(
20,1),(
21,1),(
22,1),(
23,1),(
24,1),(
25,1),(
26,1),(
12,2),(
15,2),(
17,2),(
18,2),(
19,2),(
20,2),(
21,2),(
22,2),(
23,2),(
24,2),(
25,2),(
26,2),(
12,8),(
15,8),(
17,8),(
18,8),(
19,8),(
20,8),(
21,8),(
22,8),(
23,8),(
24,8),(
25,8),(
26,8);

insert into toptoday (TYEAR, TMONTH, TDAY, BID, TNO) values(
2019, 4, 26 ,2, 3);

go
CREATE VIEW thumb_ex_10_by_00 AS
select thumb.BID
from thumb,CUSTOMER
where CUSTOMER.BYAER >= 2000 and CUSTOMER.UID = THUMB.UID
group by thumb.BID
having count(thumb.UID) > 10;
go

select BID from thumb_ex_10_by_00;


--task10
go
CREATE VIEW thumb_ex_10_by_00 AS
select thumb.BID
from thumb,CUSTOMER
where CUSTOMER.BYAER >= 2000 and CUSTOMER.UID = THUMB.UID
group by thumb.BID
having count(thumb.UID) > 10;
go

select toptoday.BID ,count(*) as topnums
from toptoday
where toptoday.BID in (select thumb_ex_10_by_00.BID from thumb_ex_10_by_00)
group by toptoday.BID;

--task11
insert into label (LNAME) values (
'文学'),(
'艺术'),(
'哲学'),(
'音乐');

insert into sub (UID, LID) values(
1,8),(
1,9),(
2,8),(
7,9),(
11,10),(
9,11);

select CUSTOMER.UID
from CUSTOMER,sub
where CUSTOMER.UID = sub.UID and sub.LID in (8,9,10,11)
group by CUSTOMER.UID;

--task12
insert into MBLOG (TITLE, UID, PYEAR, PMONTH, PDAY, CONT) values(
'华中科技大学最多地铁站', 1, 2019, 4, 20, '华中科技大学在2号线上拥有最多地铁站');
insert into MBLOG (TITLE, UID, PYEAR, PMONTH, PDAY, CONT) values(
'华中科技大学开学', 1, 2019, 4, 20, '华中科技大学在2号线上拥有最多地铁站');

select * from MBLOG
where MBLOG.TITLE like '%最多地铁站%' and MBLOG.TITLE like '%华中科技大学%';

--task13

select FOLLOW.UID, FOLLOW1.UID   
from FOLLOW,FOLLOW as FOLLOW1  
where FOLLOW.UID = FOLLOW1.UIDFLED and FOLLOW.UIDFLED = FOLLOW1.UID and FOLLOW.UID < FOLLOW1.UID;  

--task14
insert into FRIENDS (UID, FUID) values(
5,3),(
5,6),(
5,7),(
1,7),(
1,6);

select * from FRIENDS;

select uid
from FRIENDS
where FRIENDS.FUID in (select fuid from FRIENDS where FRIENDS.UID = 5) and FRIENDS.uid != 5
group by FRIENDS.uid
having count(FRIENDS.UID) = (select count(FRIENDS.UID) from FRIENDS where FRIENDS.UID = 5);

--task15
select * from B_L;

select MBLOG.BID, MBLOG.TITLE,
	case when MBLOG.BID in (select B_L.BID from B_L) then (select B_L.LID from B_L where MBLOG.BID = B_L.BID)
	else NULL
	end BlogLid
from MBLOG,B_L
where MBLOG.PYEAR = 2019 AND MBLOG.PMONTH = 4 AND MBLOG.PDAY = 20
group by MBLOG.BID, MBLOG.TITLE;

--task16
select * from FRIENDS;

select FRIENDS.uid, FRIENDS1.uid --, count(FRIENDS.FUID) as num
from FRIENDS, FRIENDS as FRIENDS1
where FRIENDS.UID != FRIENDS1.UID and FRIENDS.FUID = FRIENDS1.FUID and FRIENDS.UID < FRIENDS1.UID
group by FRIENDS.UID, FRIENDS1.uid
having count(FRIENDS.FUID) >=3

--task17
insert into toptoday (TYEAR, TMONTH, TDAY, BID, TNO) values(
2019, 4, 27 ,1, 1),(
2019, 4, 27 ,2, 2),(
2019, 4, 27 ,3, 3),(
2019, 4, 27 ,4, 4),(
2019, 4, 27 ,5, 5),(
2019, 4, 27 ,6, 6),(
2019, 4, 27 ,7, 7),(
2019, 4, 27 ,8, 8),(
2019, 4, 27 ,10, 9),(
2019, 4, 27 ,11, 10),(
2019, 4, 26 ,12, 11);

select * from toptoday;
go
CREATE VIEW top10blogtoday AS
select MBLOG.BID, MBLOG.TITLE, MBLOG.UID, CUSTOMER.USERNAME, 
		case when (select COUNT(thumb.uid) from THUMB where toptoday.BID = THUMB.BID) >=0 then (select COUNT(thumb.uid) from THUMB where toptoday.BID = THUMB.BID)
		else 0
		end thumbs
from toptoday,MBLOG,CUSTOMER,THUMB
where toptoday.TYEAR=year(GETDATE()) and toptoday.TMONTH=month(GETDATE()) and toptoday.TDAY=day(GETDATE()) and toptoday.BID=MBLOG.BID and MBLOG.UID = CUSTOMER.UID and toptoday.TNO<=10
group by MBLOG.BID, MBLOG.TITLE, MBLOG.UID, CUSTOMER.USERNAME, toptoday.BID, toptoday.TNO
go

select * from top10blogtoday;

