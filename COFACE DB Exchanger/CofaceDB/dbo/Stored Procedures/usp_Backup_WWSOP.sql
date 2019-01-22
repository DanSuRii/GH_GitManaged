
CREATE PROC [dbo].[usp_Backup_WWSOP]
AS
BEGIN

	IF NOT EXISTS( SELECT TOP 1 * FROM [dbo].[Tbl_WWS_OP] )
		RETURN

	DECLARE @WWSTbl_Begin	DATE = '1900-01-01',
			@WWSTbl_End		DATE = '1900-01-01'

	SELECT @WWSTbl_Begin = CONVERT(DATE, Value1),
			@WWSTbl_End = CONVERT(DATE, Value2)
	FROM  [dbo].[Tbl_Coface_Variables]
	WHERE Name = N'RG Period'

	/*
	SELECT @WWSTbl_Begin, @WWSTbl_End,
			CASE WHEN N'1900-01-01' = @WWSTbl_Begin THEN 1 ELSE 0 END
	 */

	IF @WWSTbl_Begin = N'1900-01-01'
		RETURN



	DECLARE @TblName NVARCHAR(MAX) = 'Tbl_WWS_' + CONVERT(NVARCHAR(MAX), @WWSTbl_Begin,112) +'_'+ CONVERT(NVARCHAR(MAX),@WWSTbl_End,112)
	DECLARE @TblTarget NVARCHAR(MAX) = 'dbo.' + @TblName
	DECLARE @Qry NVARCHAR(MAX) =
	N'
	SELECT  *
	 INTO '+ @TblTarget +'
	 FROM dbo.Tbl_WWS_OP '

	IF EXISTS( 
		SELECT *
		FROM INFORMATION_SCHEMA.TABLES
		WHERE TABLE_NAME = @TblName )
		RETURN

	EXEC sp_executesql @Qry

	IF NOT EXISTS(
		SELECT *
		FROM INFORMATION_SCHEMA.TABLES
		WHERE TABLE_NAME = @TblName
	)
		RAISERROR(N'Table Backup Failure ', 11, 1);
END
