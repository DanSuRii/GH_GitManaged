USE [TestDB]

--SELECT CONVERT( NVARCHAR(256), GETDATE(), 112)

GO

CREATE PROCEDURE dbo.usp_OPOS_TblHoba_to_TblCOF

AS

DECLARE @l_CurDate NVARCHAR(256)
DECLARE @l_BkUp_Tbl_Name NVARCHAR(256)
DECLARE @l_BkUpTbl_LastName NVARCHAR(256)
DECLARE @l_BkUpTbl_Prefix NVARCHAR(256)
DECLARE @qry NVARCHAR(MAX)

DECLARE @l_HOBA_OPOS_TblName NVARCHAR(256)

SELECT @l_CurDate = CONVERT( NVARCHAR(256), GETDATE(), 112)


/***********************************************************
	CHECK CURRENT HOBA VALIDATE 
***********************************************************/

EXEC @l_HOBA_OPOS_TblName = dbo.ufn_GetOPOS_TblName

/***********************************************************
	CREATE BACKUP
***********************************************************/

SET @l_BkUpTbl_Prefix = 'ZBK_Tbl_COF_OP_' + @l_CurDate
SET @l_BkUp_Tbl_Name =  @l_BkUpTbl_Prefix + '_000'

IF EXISTS ( SELECT 1 FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME LIKE @l_BkUpTbl_Prefix +'_%' )
BEGIN
	SELECT @l_BkUpTbl_LastName = MAX(TABLE_NAME) FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME LIKE @l_BkUpTbl_Prefix +'_%'
	SELECT @l_BkUp_Tbl_Name = @l_BkUpTbl_Prefix + '_' +FORMAT(CONVERT(INT, RIGHT(@l_BkUpTbl_LastName, 3)) + 1, '000' )
END

SET @qry =
'
	SELECT *
	INTO [dbo].['+@l_BkUp_Tbl_Name+']
	FROM [dbo].[Tbl_COF_OP]
'
EXEC sp_executesql @qry

--CLEAR DESTINATION TABLE
TRUNCATE TABLE [dbo].[Tbl_COF_OP]


-- INPUT
SET @qry =
'
	INSERT [dbo].[Tbl_COF_OP]
			([OPCLNR], [OPDBNR], [OPITMTYP],[OPITMNR],[OPITMDTL],[OPITMDAT],[OPITMDTD],[OPAMT],[OPCUR],[OPAMTMN],[OPCURMN])
	SELECT	''2016/053'' AS [OPCLNR],
			[KD-WWS], [CofaTyp],[BelegNr],
	
			[BelegDat] AS LieferDatum,
			[BelegDat] AS BelDatum,
			[Netto] AS FälligkiteDatum,
	
	
			--CONVERT(DATE, [BelegDat]) AS LieferDatum,
			--CONVERT(DATE,[BelegDat]) AS BelDatum,
			[Offen],[Währung] AS GWährung,[Offen],[Währung] AS OWährung
		--INTO Tbl_COF_OP_Test
	FROM [dbo].['+@l_HOBA_OPOS_TblName+'] AS OP
		JOIN [dbo].[Tbl_Typ_Lookup] AS TY
		ON OP.[Typ] = TY.[HobaTyp]
'
EXEC sp_executesql @qry

--Prüfung
/*
SELECT *
FROM dbo.Tbl_COF_OP
WHERE OPDBNR NOT IN (
	SELECT DBNR FROM dbo.Tbl_COF_DebitStamm
	 )


SELECT OPDBNR, COUNT(1) AS COUNT
FROM dbo.Tbl_COF_OP
WHERE OPDBNR NOT IN (
	SELECT DBNR FROM dbo.Tbl_COF_DebitStamm
	 )
GROUP BY OPDBNR
*/


UPDATE [dbo].[Tbl_COF_OP]
SET OPDBNR = 1022430
WHERE OPDBNR = 14840
	--AND OPITMNR LIKE 'BA%'

--SELECT '54mal Edeka Sammel', @@ROWCOUNT

UPDATE [dbo].[Tbl_COF_OP]
SET OPDBNR = 1053150
WHERE OPDBNR = 16830

--SELECT '0mal Freisch Paradies', @@ROWCOUNT

--EXEC [dbo].[usp_Coface_OP_Stammdaten]


--COMMIT
--ROLLBACK
--DBCC OPENTRAN