--insert into mblog
insert into MBLOG (TITLE, UID, PYEAR, PMONTH, PDAY, CONT) values(
'李敏镐退伍', 1, 2019, 4, 25, '李敏镐退伍');

--delete a raw from mblog
delete from MBLOG
where bid = 9;

--update a raw from mblog
update MBLOG
set CONT = '李敏镐退伍啦！'
where bid = 9;
