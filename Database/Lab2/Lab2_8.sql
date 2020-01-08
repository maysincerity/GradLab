USE [DBlab2]
GO
/****** Object:  UserDefinedFunction [dbo].[toptimes100]    Script Date: 2019/5/6 0:45:50 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
ALTER FUNCTION [dbo].[toptimes100]
(	
	-- Add the parameters for the function here
	@topdays int = 100
)
RETURNS TABLE 
AS
RETURN 
(
	-- Add the SELECT statement with parameter references here
	SELECT MBLOG.UID,toptoday.BID
	from MBLOG,toptoday,CUSTOMER
	where MBLOG.BID = toptoday.BID and MBLOG.UID = CUSTOMER.UID and CUSTOMER.BYAER >= 2000
	group by MBLOG.UID,toptoday.BID
	having count(toptoday.BID) >= @topdays
)