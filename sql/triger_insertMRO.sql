USE [DBSA]
GO

ALTER TRIGGER [dbo].[tbMROBulkInser] 
   ON  [dbo].[tbMRODataNew]
   INSTEAD OF INSERT	
AS 
BEGIN
	--找出导入数据中需要Update操作的全部元组，并暂时存放在#UpdateTemp
	select A.* into #UpdateTemp
	from inserted AS A,[dbo].[tbMRODataNew] As B
	where A.[TimeStamp] = B.[TimeStamp]
		and A.[ServingSector]=B.[ServingSector]
		and A.[InterferingSector] = B.[InterferingSector]

	--tbInserted与#UpdateTemp做集合差操作
	--select A.* into #InsertTemp
	--from ((select * from inserted) EXCEPT (select * from #UpdateTemp)) as A
	--tbInserted与#UpdateTemp做集合差操作

	select A.* into #InsertTemp
	from inserted AS A left join #UpdateTemp As B 
	on(A.[TimeStamp] = B.[TimeStamp]
		and A.[ServingSector] =B.[ServingSector]
		and A.[InterferingSector] = B.[InterferingSector]
		and A.[LteScRSRP] = B.[LteScRSRP]
		and A.[LteNcRSRP] = B.[LteNcRSRP]
		and A.[LteNcPci] = B.[LteNcPci])
	where B.[TimeStamp] is null

	DELETE FROM [dbo].[tbMRODataNew]
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

	--将新的内容插入cell表
	INSERT INTO [dbo].[tbMRODataNew]
	SELECT * FROM #UpdateTemp

	--将新的内容插入cell表
	INSERT INTO [dbo].[tbMRODataNew]
	SELECT * FROM #InsertTemp
	
END
