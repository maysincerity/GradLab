go
create trigger thumb_control
on thumb
after insert
as
	if exists(select thumb.UID from THUMB,MBLOG where THUMB.BID = MBLOG.BID and THUMB.UID = MBLOG.UID)
	begin
		rollback transaction
	end
go


insert into THUMB (uid,bid) values (1,1);






ALTER TRIGGER [dbo].[thumb_update]
   ON  [dbo].[THUMB] 
   AFTER update
AS 
BEGIN
	begin tran
	if exists(select thumb.UID from THUMB,MBLOG where THUMB.BID = MBLOG.BID and THUMB.UID = MBLOG.UID)
	begin
		print'点赞失败，不能给自己点赞！'
		rollback transaction
	end
	else
	begin
		print'点赞成功！'
		commit tran
	end
END

