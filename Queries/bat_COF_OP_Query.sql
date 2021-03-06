/****** Script for SelectTopNRows command from SSMS  ******/
--USE [TestDB]

SET NOCOUNT ON

DECLARE @FLITNR INT
		, @OPAMTMN MONEY

SELECT @FLITNR = COUNT(1)
FROM [TestDB].[dbo].[Tbl_COF_OP]

SELECT @OPAMTMN = SUM([OPAMTMN])
FROM [TestDB].[dbo].[Tbl_COF_OP]


SELECT '1' AS FLNUM, FORMAT( GetDate(), 'dd.MM.yyyy' ) AS FLDATE, N'Günther Hellriegel GmbH' AS CLNAME1
	, '' AS CLNAME2, 'Kronberg' AS CLPLACE, '61476' AS CLZIP
	, 'In der Schneithohl 3' AS CLSTREET, '2016/053' AS CLNR, @FLITNR  AS FLBELANZ, 
	FORMAT(@OPAMTMN, '0.00', 'de-de') AS FLBELSUM


SELECT [OPCLNR]
      ,[OPDBNR]
      ,[OPITMTYP]
      ,[OPITMNR]
      ,[OPITMNRREF]      
	  ,FORMAT([OPITMDTL], 'dd.MM.yyyy') AS [OPITMDTL] 
      ,FORMAT([OPITMDAT], 'dd.MM.yyyy') AS [OPITMDAT]
      ,FORMAT([OPITMDTD], 'dd.MM.yyyy') AS [OPITMDTD]      
	  --,FORMAT([OPAMT], 'N', 'de-de') AS [OPAMT]
	  ,FORMAT([OPAMT], '0.00', 'de-de') AS [OPAMT]
      ,[OPCUR]
      ,FORMAT([OPAMTMN],'0.00', 'de-de') AS [OPAMTMN]
      ,[OPCURMN]
      ,[OPITMDLV]
      ,[OPITMTAX]
      ,[OPITMDY1]
      ,[OPITMRD1]
      ,[OPITMDY2]
      ,[OPITMRD2]
      ,[OPITMDY3]
      ,[OPITMTXT]
  FROM [TestDB].[dbo].[Tbl_COF_OP]