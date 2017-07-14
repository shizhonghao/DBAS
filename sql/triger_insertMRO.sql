USE [DBSA]
GO

ALTER TRIGGER [dbo].[tbMROBulkInser] 
   ON  [dbo].[tbMROData]
   INSTEAD OF INSERT	
AS 
BEGIN
	--�ҳ�������������ҪUpdate������ȫ��Ԫ�飬����ʱ�����#UpdateTemp
	select A.* into #UpdateTemp
	from inserted AS A,[dbo].[tbMROData] As B
	where A.[TimeStamp] = B.[TimeStamp]
		and A.[ServingSector]=B.[ServingSector]
		and A.[InterferingSector] = B.[InterferingSector]

	--tbInserted��#UpdateTemp�����ϲ����
	--select A.* into #InsertTemp
	--from ((select * from inserted) EXCEPT (select * from #UpdateTemp)) as A
	--tbInserted��#UpdateTemp�����ϲ����

	select A.* into #InsertTemp
	from inserted AS A left join #UpdateTemp As B 
	on(A.[TimeStamp] = B.[TimeStamp]
		and A.[ServingSector] =B.[ServingSector]
		and A.[InterferingSector] = B.[InterferingSector]
		and A.[LteScRSRP] = B.[LteScRSRP]
		and A.[LteNcRSRP] = B.[LteNcRSRP]
		and A.[LteNcPci] = B.[LteNcPci])
	where B.[TimeStamp] is null

	DELETE FROM [dbo].[tbMROData]
	where [TimeStamp] = (
				select B.[TimeStamp]
				from #UpdateTemp AS B
				where [TimeStamp] = B.[TimeStamp]
				and [ServingSector] =B.[ServingSector]
				and [InterferingSector] = B.[InterferingSector])
		and [ServingSector] = (
				select B.[ServingSector]
				from #UpdateTemp AS B
				where [TimeStamp] = B.[TimeStamp]
				and [ServingSector] =B.[ServingSector]
				and [InterferingSector] = B.[InterferingSector])
		and [InterferingSector] = (
				select B.[InterferingSector]
				from #UpdateTemp AS B
				where [TimeStamp] = B.[TimeStamp]
				and [ServingSector] =B.[ServingSector]
				and [InterferingSector] = B.[InterferingSector])

	--���µ����ݲ���cell��
	INSERT INTO [dbo].[tbMROData]
	SELECT * FROM #UpdateTemp

	--���µ����ݲ���cell��
	INSERT INTO [dbo].[tbMROData]
	SELECT * FROM #InsertTemp
	
END
