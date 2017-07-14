USE [DBSA]
GO
create TRIGGER [dbo].[tbKPIBulkInser] 
   ON  [dbo].[tbKPI]
   INSTEAD OF INSERT	
AS 
BEGIN
	--找出导入数据中需要Update操作的全部元组，并暂时存放在#UpdateTemp
	select A.* into #UpdateTemp
	from inserted as A , [tbKPI] as B
	where A.[起始时间] = B.[起始时间]
	and A.[网元名称] = B.[网元名称]
	and A.[小区名] = B.[小区名]

	--tbInserted与#UpdateTemp做集合差操作
	select A.* into #InsertTemp
	from ((select * from inserted) EXCEPT (select * from #UpdateTemp)) as A

	--将新的内容插入cell表
	INSERT INTO [dbo].[tbKPI]
	SELECT * FROM #InsertTemp

	--对已有的内容先删除后插入
	DELETE FROM [dbo].[tbKPI]
	WHERE [起始时间] = (select [起始时间]
						from #UpdateTemp as B
						)
		and [网元名称] = (select [网元名称]
						from #UpdateTemp as C
						where [起始时间] = C.[起始时间]
						and [网元名称] = C.[网元名称]
						)
		and [小区名] = (select [小区名]
						from #UpdateTemp as D
						where [起始时间] = D.[起始时间]
						and [网元名称] = D.[网元名称]
						and [小区名] = D.[小区名]
						)

	INSERT INTO [dbo].[tbKPI]
	SELECT * FROM #UpdateTemp
END
