class DanUtility
{
    public static string GetCurrentDateStringAsFormat()
    {
        DateTime dtCurrent = DateTime.Now;
        return dtCurrent.GetString("yyyyMMdd");
    }
}