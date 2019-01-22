using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace COFACE_DB_Exchanger
{
    class CSVFormatter
    {
        public static void DoFormat(string strCsvFile, string strToExport)
        {
            if (false == File.Exists(strCsvFile))
                return;

            string strFilePath = Path.GetDirectoryName(strCsvFile);
            string strFileName = Path.GetFileNameWithoutExtension(strCsvFile);
            string strFilePathName = strFilePath + @"\"  +strFileName;

            var readedLine = new List<string>(File.ReadAllLines(strCsvFile));
            readedLine.RemoveAt(4);
            readedLine.RemoveAt(1);
                        
            //StreamReader reader = File.OpenText(strCsvFile);
            string strTxt = string.Join("\r\n",readedLine);
            //string strTxt = File.ReadAllText(strCsvFile);
            strTxt = strTxt.Replace("NULL", "");
                        
            File.WriteAllText(strToExport, strTxt, Encoding.UTF8);
            //File.WriteAllText( strFilePathName + "_removeNULL.csv", strTxt, Encoding.UTF8);
        }
    }
}
