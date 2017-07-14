
create procedure PR_tbPRBnew
as
begin
	declare @sql varchar(8000)
	declare @count int
	set @count = 0
	set @sql = 'select 起始时间,周期,网元名称,小区名'
	while @count < 100
	begin
		set @sql = @sql + ',avg(PRB' + CONVERT(varchar(3),@count) + ')as PRB' + CONVERT(varchar(3),@count)
		set @count = @count + 1
	end
	set @sql = @sql + ' into DBSA.dbo.tbPRBnew from DBSA.dbo.tbPRBtemp group by 起始时间,周期,网元名称,小区名'
	drop table DBSA.dbo.tbPRBnew
	select * into DBSA.dbo.tbPRBtemp from DBSA.dbo.tbPRB
	update DBSA.dbo.tbPRBtemp
	set 起始时间 = SUBSTRING(CONVERT(varchar(63),起始时间),1,14) + '00:00'
	where SUBSTRING(CONVERT(varchar(63),起始时间),15,2) != '00'
	exec(@sql)
	drop table DBSA.dbo.tbPRBtemp
end
