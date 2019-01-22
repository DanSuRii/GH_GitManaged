USE [TestDB]
GO

/****** Object:  StoredProcedure [dbo].[usp_Prepare_HOBA_OPOS]    Script Date: 15.03.2017 16:36:04 ******/
DROP PROCEDURE [dbo].[usp_Prepare_HOBA_OPOS]
GO

/****** Object:  StoredProcedure [dbo].[usp_Prepare_HOBA_OPOS]    Script Date: 15.03.2017 16:36:04 ******/
SET ANSI_NULLS ON
GO

SET QUOTED_IDENTIFIER ON
GO


CREATE PROC [dbo].[usp_Prepare_HOBA_OPOS]
AS
BEGIN

	SELECT *
	FROM INFORMATION_SCHEMA.TABLES

	DECLARE @TblName NVARCHAR(MAX)
		,	@Qry NVARCHAR(MAX)
	EXEC @TblName = dbo.ufn_GetOPOS_TblName

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
GO


