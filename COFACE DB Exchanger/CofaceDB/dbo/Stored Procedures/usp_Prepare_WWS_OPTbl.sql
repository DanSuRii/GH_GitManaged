

CREATE PROC [dbo].[usp_Prepare_WWS_OPTbl]
AS
BEGIN
	EXEC dbo.usp_Backup_WWSOP
	
	TRUNCATE TABLE dbo.Tbl_WWS_OP

	RETURN 0

END
