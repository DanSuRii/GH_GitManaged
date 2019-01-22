using System;

namespace DanUtility
{
    class Util
    {
        public static string GetCurrentDateStringAsFormat()
        {
            DateTime dtCurrent = DateTime.Now;
            return dtCurrent.ToString("yyyyMMdd");
        }
    }
}