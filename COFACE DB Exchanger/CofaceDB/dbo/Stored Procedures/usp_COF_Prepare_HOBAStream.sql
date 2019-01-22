CREATE PROCEDURE [dbo].[usp_COF_Prepare_HOBAStream]
	@TblName NVARCHAR(MAX)
AS

	TRUNCATE TABLE dbo.Tbl_Hoba_Stream
	
	
	IF NOT EXISTS ( SELECT * FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME = @TblName )
	BEGIN
		RAISERROR(  'Die Table ist besteht nicht ' ,-1, -1   )
		return 
	END

	DECLARE @Qry NVARCHAR(MAX)

	SET @Qry = '
		INSERT INTO dbo.Tbl_Hoba_Stream 
			(	Konto , Name, KD-WWS, BelegNr, BelegNr 2, Typ, BelegDat, Netto, MS, M, Tage, Gesamt, Offen, Währung, OP-Hinweistext)
		SELECT	Konto , Name, KD-WWS, BelegNr, BelegNr 2, Typ, BelegDat, Netto, MS, M, Tage, Gesamt, Offen, Währung, OP-Hinweistext
		FROM ' + @TblName + '
	'
	EXEC sp_executesql @Qry