

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
