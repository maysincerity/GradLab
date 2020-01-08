select MBLOG.BID, count(THUMB.UID)
from MBLOG,THUMB
where MBLOG.PYEAR = year(GETDATE()) and MBLOG.PMONTH = month(GETDATE()) and MBLOG.PDAY = day(GETDATE()) and MBLOG.BID = THUMB.BID
group by MBLOG.BID
order by count(THUMB.UID);


select * from thumbToday;



go
create procedure updatethumbtoday
as
begin
	truncate table thumbtoday;
	insert into thumbtoday (bid, thumbnums)
	select top 10 MBLOG.BID, count(THUMB.UID)
	from MBLOG,THUMB
	where MBLOG.PYEAR = year(GETDATE()) and MBLOG.PMONTH = month(GETDATE()) and MBLOG.PDAY = day(GETDATE()) and MBLOG.BID = THUMB.BID
	group by MBLOG.BID
	order by count(THUMB.UID);
end
