
/****** Object:  StoredProcedure [dbo].[usp_PrepareCofaceTbl]    Script Date: 09.09.2016 10:21:37 ******/

CREATE PROC [dbo].[usp_PrepareCofaceTbl]
AS
BEGIN

	IF NOT EXISTS ( SELECT TOP 1 * FROM [dbo].[Tbl_COF_OP])
		RETURN;


	DECLARE @TableName NVARCHAR(50)
			,@Qry NVARCHAR(MAX)
			,@TblNr BIGINT

	SET @TblNr = 0

	SELECT @TableName = MAX(TABLE_NAME)
	FROM INFORMATION_SCHEMA.TABLES
	WHERE TABLE_NAME LIKE 'Tbl_COF_'+ CONVERT( NVARCHAR(50), GETDATE(),112) +'_OP%'


	IF(@TableName IS NOT NULL)
	BEGIN
		SELECT @TblNr = CONVERT(BIGINT, RIGHT(@TableName, 3))+1	
		--그 이름의 끝에 번호를 붙이던지 
	END	

	SELECT @TableName = 'dbo.Tbl_COF_'+ CONVERT( NVARCHAR(50), GETDATE(),112) +'_OP_' + RIGHT('000'+ISNULL( CONVERT(NVARCHAR(50),@TblNr) ,''),3)


	SET @Qry = 'SELECT * INTO ' + @TableName + ' FROM [dbo].[Tbl_COF_OP]'
	EXEC sp_executesql @Qry

	TRUNCATE TABLE [dbo].[Tbl_COF_OP]
END

