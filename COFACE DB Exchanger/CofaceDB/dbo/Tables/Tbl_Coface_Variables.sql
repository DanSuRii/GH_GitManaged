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

