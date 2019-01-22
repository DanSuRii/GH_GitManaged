﻿/*
Deployment script for TestDB

This code was generated by a tool.
Changes to this file may cause incorrect behavior and will be lost if
the code is regenerated.
*/

GO
SET ANSI_NULLS, ANSI_PADDING, ANSI_WARNINGS, ARITHABORT, CONCAT_NULL_YIELDS_NULL, QUOTED_IDENTIFIER ON;

SET NUMERIC_ROUNDABORT OFF;


GO
:setvar DatabaseName "TestDB"
:setvar DefaultFilePrefix "TestDB"
:setvar DefaultDataPath "C:\Program Files\Microsoft SQL Server\MSSQL13.SQLEXPRESS01\MSSQL\DATA\"
:setvar DefaultLogPath "C:\Program Files\Microsoft SQL Server\MSSQL13.SQLEXPRESS01\MSSQL\DATA\"

GO
:on error exit
GO
/*
Detect SQLCMD mode and disable script execution if SQLCMD mode is not supported.
To re-enable the script after enabling SQLCMD mode, execute the following:
SET NOEXEC OFF; 
*/
:setvar __IsSqlCmdEnabled "True"
GO
IF N'$(__IsSqlCmdEnabled)' NOT LIKE N'True'
    BEGIN
        PRINT N'SQLCMD mode must be enabled to successfully execute this script.';
        SET NOEXEC ON;
    END


GO
IF EXISTS (SELECT 1
           FROM   [master].[dbo].[sysdatabases]
           WHERE  [name] = N'$(DatabaseName)')
    BEGIN
        ALTER DATABASE [$(DatabaseName)]
            SET ANSI_NULLS ON,
                ANSI_PADDING ON,
                ANSI_WARNINGS ON,
                ARITHABORT ON,
                CONCAT_NULL_YIELDS_NULL ON,
                QUOTED_IDENTIFIER ON,
                ANSI_NULL_DEFAULT ON,
                CURSOR_DEFAULT LOCAL,
                RECOVERY FULL 
            WITH ROLLBACK IMMEDIATE;
    END


GO
IF EXISTS (SELECT 1
           FROM   [master].[dbo].[sysdatabases]
           WHERE  [name] = N'$(DatabaseName)')
    BEGIN
        ALTER DATABASE [$(DatabaseName)]
            SET PAGE_VERIFY NONE 
            WITH ROLLBACK IMMEDIATE;
    END


GO
ALTER DATABASE [$(DatabaseName)]
    SET TARGET_RECOVERY_TIME = 0 SECONDS 
    WITH ROLLBACK IMMEDIATE;


GO
IF EXISTS (SELECT 1
           FROM   [master].[dbo].[sysdatabases]
           WHERE  [name] = N'$(DatabaseName)')
    BEGIN
        ALTER DATABASE [$(DatabaseName)]
            SET QUERY_STORE (CLEANUP_POLICY = (STALE_QUERY_THRESHOLD_DAYS = 367)) 
            WITH ROLLBACK IMMEDIATE;
    END


GO
USE [$(DatabaseName)];


GO
PRINT N'Creating [dbo].[Tbl_COF_OP]...';


GO
CREATE TABLE [dbo].[Tbl_COF_OP] (
    [OPCLNR]     NVARCHAR (10)  NOT NULL,
    [OPDBNR]     NVARCHAR (15)  NOT NULL,
    [OPITMTYP]   NVARCHAR (5)   NOT NULL,
    [OPITMNR]    NVARCHAR (20)  NOT NULL,
    [OPITMNRREF] NVARCHAR (20)  NULL,
    [OPITMDTL]   DATE           NOT NULL,
    [OPITMDAT]   DATE           NOT NULL,
    [OPITMDTD]   DATE           NOT NULL,
    [OPAMT]      MONEY          NOT NULL,
    [OPCUR]      NVARCHAR (3)   NOT NULL,
    [OPAMTMN]    MONEY          NOT NULL,
    [OPCURMN]    NVARCHAR (3)   NOT NULL,
    [OPITMDLV]   BIT            NULL,
    [OPITMTAX]   NVARCHAR (3)   NULL,
    [OPITMDY1]   SMALLINT       NULL,
    [OPITMRD1]   INT            NULL,
    [OPITMDY2]   SMALLINT       NULL,
    [OPITMRD2]   INT            NULL,
    [OPITMDY3]   SMALLINT       NULL,
    [OPITMTXT]   NVARCHAR (254) NULL
);


GO
PRINT N'Creating [dbo].[Tbl_Coface_Variables]...';


GO
CREATE TABLE [dbo].[Tbl_Coface_Variables] (
    [SN]       BIGINT        IDENTITY (1, 1) NOT NULL,
    [Name]     NVARCHAR (20) NULL,
    [DataType] NVARCHAR (8)  NOT NULL,
    [Value1]   SQL_VARIANT   NULL,
    [Value2]   SQL_VARIANT   NULL,
    [Value3]   SQL_VARIANT   NULL,
    CONSTRAINT [PK_Coface_Variables] PRIMARY KEY CLUSTERED ([SN] ASC),
    CONSTRAINT [UK_Coface_Variables] UNIQUE NONCLUSTERED ([Name] ASC)
);


GO
PRINT N'Creating [dbo].[Tbl_Hoba_OPOS_20161130]...';


GO
CREATE TABLE [dbo].[Tbl_Hoba_OPOS_20161130] (
    [Konto ]         VARCHAR (50) NULL,
    [Name]           VARCHAR (50) NULL,
    [KD-WWS]         VARCHAR (50) NULL,
    [BelegNr]        VARCHAR (50) NULL,
    [BelegNr 2]      VARCHAR (50) NULL,
    [Typ]            VARCHAR (50) NULL,
    [BelegDat]       DATE         NULL,
    [Netto]          DATE         NULL,
    [MS]             VARCHAR (50) NULL,
    [M]              VARCHAR (50) NULL,
    [Tage]           VARCHAR (50) NULL,
    [Gesamt]         MONEY        NULL,
    [Offen]          MONEY        NULL,
    [Währung]        VARCHAR (50) NULL,
    [OP-Hinweistext] VARCHAR (50) NULL
);


GO
PRINT N'Creating [dbo].[Tbl_Hoba_OPOS_20161201]...';


GO
CREATE TABLE [dbo].[Tbl_Hoba_OPOS_20161201] (
    [Konto ]         VARCHAR (50) NULL,
    [Name]           VARCHAR (50) NULL,
    [KD-WWS]         VARCHAR (50) NULL,
    [BelegNr]        VARCHAR (50) NULL,
    [BelegNr 2]      VARCHAR (50) NULL,
    [Typ]            VARCHAR (50) NULL,
    [BelegDat]       DATE         NULL,
    [Netto]          DATE         NULL,
    [MS]             VARCHAR (50) NULL,
    [M]              VARCHAR (50) NULL,
    [Tage]           VARCHAR (50) NULL,
    [Gesamt]         MONEY        NULL,
    [Offen]          MONEY        NULL,
    [Währung]        VARCHAR (50) NULL,
    [OP-Hinweistext] VARCHAR (50) NULL
);


GO
PRINT N'Creating [dbo].[Tbl_Hoba_Stream]...';


GO
CREATE TABLE [dbo].[Tbl_Hoba_Stream] (
    [Konto ]         VARCHAR (50) NULL,
    [Name]           VARCHAR (50) NULL,
    [KD-WWS]         VARCHAR (50) NULL,
    [BelegNr]        VARCHAR (50) NULL,
    [BelegNr 2]      VARCHAR (50) NULL,
    [Typ]            VARCHAR (50) NULL,
    [BelegDat]       DATE         NULL,
    [Netto]          DATE         NULL,
    [MS]             VARCHAR (50) NULL,
    [M]              VARCHAR (50) NULL,
    [Tage]           VARCHAR (50) NULL,
    [Gesamt]         MONEY        NULL,
    [Offen]          MONEY        NULL,
    [WÃ¤hrung]       VARCHAR (50) NULL,
    [OP-Hinweistext] VARCHAR (50) NULL
);


GO
PRINT N'Creating [dbo].[Tbl_Hoba_Stream_UCS2]...';


GO
CREATE TABLE [dbo].[Tbl_Hoba_Stream_UCS2] (
    [Konto ]         NVARCHAR (50) NULL,
    [Name]           NVARCHAR (50) NULL,
    [KD-WWS]         NVARCHAR (50) NULL,
    [BelegNr]        NVARCHAR (50) NULL,
    [BelegNr 2]      NVARCHAR (50) NULL,
    [Typ]            NVARCHAR (50) NULL,
    [BelegDat]       DATE          NULL,
    [Netto]          DATE          NULL,
    [MS]             NVARCHAR (50) NULL,
    [M]              NVARCHAR (50) NULL,
    [Tage]           NVARCHAR (50) NULL,
    [Gesamt]         MONEY         NULL,
    [Offen]          MONEY         NULL,
    [Währung]        NVARCHAR (50) NULL,
    [OP-Hinweistext] NVARCHAR (50) NULL
);


GO
PRINT N'Creating [dbo].[Tbl_Hoba_Stream_UTF8]...';


GO
CREATE TABLE [dbo].[Tbl_Hoba_Stream_UTF8] (
    [Konto ]         VARCHAR (50) NULL,
    [Name]           VARCHAR (50) NULL,
    [KD-WWS]         VARCHAR (50) NULL,
    [BelegNr]        VARCHAR (50) NULL,
    [BelegNr 2]      VARCHAR (50) NULL,
    [Typ]            VARCHAR (50) NULL,
    [BelegDat]       DATE         NULL,
    [Netto]          DATE         NULL,
    [MS]             VARCHAR (50) NULL,
    [M]              VARCHAR (50) NULL,
    [Tage]           VARCHAR (50) NULL,
    [Gesamt]         MONEY        NULL,
    [Offen]          MONEY        NULL,
    [Währung]        VARCHAR (50) NULL,
    [OP-Hinweistext] VARCHAR (50) NULL
);


GO
PRINT N'Creating [dbo].[Tbl_HOBA_Xls]...';


GO
CREATE TABLE [dbo].[Tbl_HOBA_Xls] (
    [Konto ]         VARCHAR (50) NULL,
    [Name]           VARCHAR (50) NULL,
    [KD-WWS]         VARCHAR (50) NULL,
    [BelegNr]        VARCHAR (50) NULL,
    [BelegNr 2]      VARCHAR (50) NULL,
    [Typ]            VARCHAR (50) NULL,
    [BelegDat]       DATE         NULL,
    [Netto]          DATE         NULL,
    [MS]             VARCHAR (50) NULL,
    [M]              VARCHAR (50) NULL,
    [Tage]           VARCHAR (50) NULL,
    [Gesamt]         MONEY        NULL,
    [Offen]          MONEY        NULL,
    [Währung]        VARCHAR (50) NULL,
    [OP-Hinweistext] VARCHAR (50) NULL
);


GO
PRINT N'Creating [dbo].[Tbl_TestDatumDef]...';


GO
CREATE TABLE [dbo].[Tbl_TestDatumDef] (
    [FÄLLIGKIT] DATE NULL,
    [DATEN]     INT  NULL
);


GO
PRINT N'Creating [dbo].[Tbl_WWS_20160920-20160922]...';


GO
CREATE TABLE [dbo].[Tbl_WWS_20160920-20160922] (
    [RLKDNR] NUMERIC (7)     NOT NULL,
    [KDNAM]  VARCHAR (25)    NOT NULL,
    [KDNAM2] VARCHAR (25)    NOT NULL,
    [KDSTR]  VARCHAR (25)    NOT NULL,
    [KDPLZ]  DECIMAL (5)     NOT NULL,
    [KDORT]  VARCHAR (25)    NOT NULL,
    [RLRENR] DECIMAL (7)     NOT NULL,
    [RLDAVS] NUMERIC (8)     NOT NULL,
    [RLREDA] NUMERIC (8)     NOT NULL,
    [RLDANF] NUMERIC (8)     NOT NULL,
    [RLBETR] DECIMAL (13, 2) NOT NULL,
    [RLIKRN] DECIMAL (7)     NOT NULL,
    [RLVBNR] VARCHAR (7)     NOT NULL,
    [RLAD01] DECIMAL (7)     NOT NULL,
    [KDTXP2] VARCHAR (3)     NOT NULL
);


GO
PRINT N'Creating [dbo].[Tbl_WWS_20160928_20160929]...';


GO
CREATE TABLE [dbo].[Tbl_WWS_20160928_20160929] (
    [RLKDNR] NUMERIC (7)     NOT NULL,
    [KDNAM]  VARCHAR (25)    NOT NULL,
    [KDNAM2] VARCHAR (25)    NOT NULL,
    [KDSTR]  VARCHAR (25)    NOT NULL,
    [KDPLZ]  DECIMAL (5)     NOT NULL,
    [KDORT]  VARCHAR (25)    NOT NULL,
    [RLRENR] DECIMAL (7)     NOT NULL,
    [RLDAVS] NUMERIC (8)     NOT NULL,
    [RLREDA] NUMERIC (8)     NOT NULL,
    [RLDANF] NUMERIC (8)     NOT NULL,
    [RLBETR] DECIMAL (13, 2) NOT NULL,
    [RLIKRN] DECIMAL (7)     NOT NULL,
    [RLVBNR] VARCHAR (7)     NOT NULL,
    [RLAD01] DECIMAL (7)     NOT NULL,
    [KDTXP2] VARCHAR (3)     NOT NULL
);


GO
PRINT N'Creating [dbo].[Tbl_WWS_OP]...';


GO
CREATE TABLE [dbo].[Tbl_WWS_OP] (
    [RLKDNR] NUMERIC (7)     NOT NULL,
    [KDNAM]  VARCHAR (25)    NOT NULL,
    [KDNAM2] VARCHAR (25)    NOT NULL,
    [KDSTR]  VARCHAR (25)    NOT NULL,
    [KDPLZ]  DECIMAL (5)     NOT NULL,
    [KDORT]  VARCHAR (25)    NOT NULL,
    [RLRENR] DECIMAL (7)     NOT NULL,
    [RLDAVS] NUMERIC (8)     NOT NULL,
    [RLREDA] NUMERIC (8)     NOT NULL,
    [RLDANF] NUMERIC (8)     NOT NULL,
    [RLBETR] DECIMAL (13, 2) NOT NULL,
    [RLIKRN] DECIMAL (7)     NOT NULL,
    [RLVBNR] VARCHAR (7)     NOT NULL,
    [RLAD01] DECIMAL (7)     NOT NULL,
    [KDTXP2] VARCHAR (3)     NOT NULL
);


GO
PRINT N'Creating [dbo].[Tbl_WWS_Sample]...';


GO
CREATE TABLE [dbo].[Tbl_WWS_Sample] (
    [RLKDNR] NUMERIC (7)     NOT NULL,
    [KDNAM]  VARCHAR (25)    NOT NULL,
    [KDNAM2] VARCHAR (25)    NOT NULL,
    [KDSTR]  VARCHAR (25)    NOT NULL,
    [KDPLZ]  DECIMAL (5)     NOT NULL,
    [KDORT]  VARCHAR (25)    NOT NULL,
    [RLRENR] DECIMAL (7)     NOT NULL,
    [RLDAVS] NUMERIC (8)     NOT NULL,
    [RLREDA] NUMERIC (8)     NOT NULL,
    [RLDANF] NUMERIC (8)     NOT NULL,
    [RLBETR] DECIMAL (13, 2) NOT NULL,
    [RLIKRN] DECIMAL (7)     NOT NULL,
    [RLVBNR] VARCHAR (7)     NOT NULL,
    [RLAD01] DECIMAL (7)     NOT NULL,
    [KDTXP2] VARCHAR (3)     NOT NULL
);


GO
PRINT N'Creating [dbo].[test]...';


GO
CREATE TABLE [dbo].[test] (
    [Datum]        VARCHAR (50) NULL,
    [Artikel Nr]   VARCHAR (50) NULL,
    [Einspeichern] VARCHAR (50) NULL,
    [Verkauft]     VARCHAR (50) NULL,
    [Ort]          VARCHAR (50) NULL,
    [Artikel_Bez]  VARCHAR (50) NULL,
    [Menge]        VARCHAR (50) NULL,
    [Ertrag]       MONEY        NULL,
    [KW]           VARCHAR (50) NULL,
    [Begin_WT]     VARCHAR (50) NULL,
    [End_WT]       VARCHAR (50) NULL,
    [KW_Text]      VARCHAR (50) NULL,
    [Primary Key]  VARCHAR (50) NULL
);


GO
PRINT N'Creating unnamed constraint on [dbo].[Tbl_TestDatumDef]...';


GO
ALTER TABLE [dbo].[Tbl_TestDatumDef]
    ADD DEFAULT ('2099-12-31') FOR [FÄLLIGKIT];


GO
PRINT N'Creating [dbo].[usp_Backup_WWSOP]...';


GO

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
GO
PRINT N'Creating [dbo].[usp_COF_HOBA_DatenInput]...';


GO
CREATE PROCEDURE [dbo].[usp_COF_HOBA_DatenInput]
	@HobaOposTblName NVARCHAR(MAX)
AS
	


RETURN 0
GO
PRINT N'Creating [dbo].[usp_COF_Prepare_HOBAStream]...';


GO
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
GO
PRINT N'Creating [dbo].[usp_Create_CofaceWWS_OPTbl]...';


GO

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
GO
PRINT N'Creating [dbo].[usp_Prepare_HOBA_OPOS]...';


GO

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
GO
PRINT N'Creating [dbo].[usp_Prepare_WWS_OPTbl]...';


GO


CREATE PROC [dbo].[usp_Prepare_WWS_OPTbl]
AS
BEGIN
	EXEC dbo.usp_Backup_WWSOP
	
	TRUNCATE TABLE dbo.Tbl_WWS_OP

	RETURN 0

END
GO
PRINT N'Creating [dbo].[usp_PrepareCofaceTbl]...';


GO

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
GO
PRINT N'Creating [dbo].[usp_Set_WWSOP_Date]...';


GO


CREATE PROC [dbo].[usp_Set_WWSOP_Date]
	@DateBegin DATE
	, @DateEnd DATE
AS
BEGIN
	
	UPDATE [dbo].[Tbl_Coface_Variables]
	SET 
		[Value1] = @DateBegin
		, [Value2] = @DateEnd
	WHERE [Name] = N'RG Period'

END
GO
PRINT N'Update complete.';


GO