create table test1 (uid int not null primary key, uname char(20) not null);

create table test2 (uid int not null, lid int not null
					foreign key (uid) references test1 (uid));



