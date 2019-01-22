
-- Batch submitted through debugger: SQLQuery2.sql|7|0|C:\Users\AA229~1.ROE\AppData\Local\Temp\~vsBDBE.sql
CREATE PROC [dbo].[usp_Create_CofaceWWS_OPTbl]
	@TblName NVARCHAR(MAX)
AS
BEGIN

DECLARE @Qry NVARCHAR(MAX) =
	'
	IF NOT EXISTS( SELECT * FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME = '''+ @TblName +''' )
	SELECT * INTO [dbo].['+ @TblName +'] FROM [dbo].[Tbl_WWS_Sample] WHERE 0<>0
	ELSE
	TRUNCATE TABLE [dbo].['+ @TblName +']
	'

EXEC sp_executesql @Qry
	

END


