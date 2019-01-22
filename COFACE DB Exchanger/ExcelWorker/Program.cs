using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Excel = Microsoft.Office.Interop.Excel;

namespace ExcelWorker
{
    public enum ErrorCodes
    {
        Success = 0,
        FileDoesNotExists   = -1,
        ArgmentLess         = -2,            
        FileIsNot_Hoba_OPOS       = -3,
        InvalidExportPath   = -4,
        IsNotExcelWorkerErrcode = -5,
    }

    class CondExiter
    {
        int _errCode = 0;

        public int ErrCode
        {
            get
            {
                return _errCode;
            }            
        }

        public bool Check(bool Cond, string ErrMsg)
        {
            //--_errCode;
            if (false == Cond)
            {
                Console.WriteLine(ErrMsg);
                return false;
            }
            return true;
        }
    }


    class Program
    {
        static int Main(string[] args)
        {
            //System.Diagnostics.Debugger.Launch();


            CondExiter condExiter = new CondExiter();

            //Step Error Decrease

            //Check Condition 1

            if(false == condExiter.Check( 1 < args.Length, "Arguments less then 2"))
                return (int)ErrorCodes.ArgmentLess;

            string pathXls = args[0];
            if (false == condExiter.Check(System.IO.File.Exists(pathXls), "File ("+ pathXls +  ") does not exist") )
                return (int)ErrorCodes.FileDoesNotExists;

            string fileToExport = args[1];
              /*
          if (false == condExiter.Check(System.Uri.IsWellFormedUriString(fileToExport, UriKind.Absolute), "Invalid Export file path"))
                return (int)ErrorCodes.InvalidExportPath;
                */
            /*
            if (1 > args.Length)
            {
                //Send Error Message
                Console.WriteLine();
                //Return with Error Code
                return (int)ErrorCodes.FileDoesNotExists;
            }

            string pathXls = args[0];

            if ( false == System.IO.File.Exists( pathXls ) )
            {
                Console.WriteLine();
                return (int)ErrorCodes.ArgmentLess;
            }
            */

            Excel._Application oXL = new Excel.Application();
            Excel.Workbooks oWBs = oXL.Workbooks;
            Excel._Workbook oWB = oWBs.Open(pathXls);

            Excel.Sheets oWSs = oWB.Worksheets;
            Excel.Worksheet oWS = oWSs[1];

            Excel.Range oRangeHeader = oWS.get_Range("A7");
            string strFomula = oRangeHeader.Formula;
            if ("Konto " != strFomula)
            {
                string strMsg = String.Format("{0}\n Die Excel Tabelle ist kein Hoba Offeneposten", pathXls);
                Console.WriteLine(strMsg);

                return (int)ErrorCodes.FileIsNot_Hoba_OPOS;
            }

            // 이 날짜를 기준으로 
            Excel.Range oRangeDate = oWS.get_Range("M1");
            string strDatum = oRangeDate.Formula;

            Excel.Range oRemoveRange = oWS.get_Range("1:6");
            oRemoveRange.Delete();

            oXL.DisplayAlerts = false;
            oWB.SaveAs(fileToExport, Excel.XlFileFormat.xlOpenXMLWorkbook);

            oXL.Quit();

            return (int)ErrorCodes.Success;
        }
    }
}
