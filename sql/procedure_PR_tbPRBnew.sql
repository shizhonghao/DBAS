
create procedure PR_tbPRBnew
as
begin
	declare @sql varchar(8000)
	declare @count int
	set @count = 0
	set @sql = 'select ��ʼʱ��,����,��Ԫ����,С����'
	while @count < 100
	begin
		set @sql = @sql + ',avg(PRB' + CONVERT(varchar(3),@count) + ')as PRB' + CONVERT(varchar(3),@count)
		set @count = @count + 1
	end
	set @sql = @sql + ' into DBSA.dbo.tbPRBnew from DBSA.dbo.tbPRBtemp group by ��ʼʱ��,����,��Ԫ����,С����'
	drop table DBSA.dbo.tbPRBnew
	select * into DBSA.dbo.tbPRBtemp from DBSA.dbo.tbPRB
	update DBSA.dbo.tbPRBtemp
	set ��ʼʱ�� = SUBSTRING(CONVERT(varchar(63),��ʼʱ��),1,14) + '00:00'
	where SUBSTRING(CONVERT(varchar(63),��ʼʱ��),15,2) != '00'
	exec(@sql)
	drop table DBSA.dbo.tbPRBtemp
end
