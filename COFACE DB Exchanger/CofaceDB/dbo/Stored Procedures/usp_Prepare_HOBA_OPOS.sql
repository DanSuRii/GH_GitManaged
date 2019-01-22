
CREATE PROC dbo.usp_Prepare_HOBA_OPOS
AS
BEGIN

	SELECT *
	FROM INFORMATION_SCHEMA.TABLES

	DECLARE @TblName NVARCHAR(MAX)
		,	@Qry NVARCHAR(MAX)
	SELECT @TblName = 'Tbl_Hoba_OPOS_' + CONVERT( NVARCHAR(256), GETDATE(), 112)

	IF EXISTS( SELECT * FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME = @TblName )
		SET @Qry = 'TRUNCATE TABLE ' + @TblName
	ELSE
		SET @Qry = '
		CREATE TABLE [dbo].['+ @TblName +'](
			[Konto ] [varchar](50) NULL,
			[Name] [varchar](50) NULL,
			[KD-WWS] [varchar](50) NULL,
			[BelegNr] [varchar](50) NULL,
			[BelegNr 2] [varchar](50) NULL,
			[Typ] [varchar](50) NULL,
			[BelegDat] [date] NULL,
			[Netto] [date] NULL,
			[MS] [varchar](50) NULL,
			[M] [varchar](50) NULL,
			[Tage] [varchar](50) NULL,
			[Gesamt] [money] NULL,
			[Offen] [money] NULL,
			[Währung] [varchar](50) NULL,
			[OP-Hinweistext] [varchar](50) NULL
		)'

	exec sp_executesql @qry

END