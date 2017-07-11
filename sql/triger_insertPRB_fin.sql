USE [DBSA]
GO
alter TRIGGER [dbo].[tbPRBBulkInser] 
   ON  [dbo].[tbPRB]
   INSTEAD OF INSERT	
AS 
BEGIN
--找出导入数据中需要Update操作的全部元组，并暂时存放在#UpdateTemp
	select A.* into #UpdateTemp
	from inserted as A , [tbPRB] as B
	where A.[起始时间] = B.[起始时间]
	and A.[小区名] = B.[小区名]

	--tbInserted与#UpdateTemp做集合差操作
	select A.* into #InsertTemp
	from ((select * from inserted) EXCEPT (select * from #UpdateTemp)) as A

	--将新的内容插入cell表
	INSERT INTO [dbo].[tbPRB]
	SELECT * FROM #InsertTemp

	--对已有的内容先删除后插入
	DELETE FROM [dbo].[tbPRB]
	WHERE [起始时间] = (select [起始时间]
						from #UpdateTemp as B
						)
		and [小区名] = (select [小区名]
						from #UpdateTemp as C
						where [起始时间] = C.[起始时间]
						and [小区名] = C.[小区名]
						)

	INSERT INTO [dbo].[tbPRB]
	SELECT * FROM #UpdateTemp
END
