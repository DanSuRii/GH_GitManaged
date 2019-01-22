﻿CREATE TABLE [dbo].[Tbl_COF_OP] (
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

