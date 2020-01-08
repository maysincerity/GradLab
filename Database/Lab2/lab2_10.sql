begin tran
	insert into THUMB (UID, BID) values (1, 7);
	insert into THUMB (UID, BID) values (1, 1);
if(@@ERROR >0)
	begin rollback tran end
else
	begin commit tran end