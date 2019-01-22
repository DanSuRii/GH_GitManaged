using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;


namespace COFACE_DB_Exchanger
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        string WorkDB
        {
            get { return cofAccessor.TargetDB; }
        }

        public string ExcelPath
        {
            get
            {
                return _excelPath;
            }

            private set
            {
                _excelPath = value;
            }
        }

        public static string ExportPath
        {
            get
            {
                return _exportPath;
            }
        }

        string _excelPath;
        const string _exportPath = @"C:\temp\CofaceExcelTemp.xlsx";
        //차후에는 이 배치파일에 export 위치 인자를 주도록 할 것이고, 배치파일이 있는 위치도 세팅등의 파일에서 조정 할 수 있도록...
        const string _sqlExportBatch = @"C:\Users\A.Roennburg\Documents\GH_GitManaged\Batches\OPOS_CSV_Export.bat";

        private void ExcelPathTryGetNewerOPOSListe()
        {
            System.IO.DirectoryInfo di = new System.IO.DirectoryInfo(ExcelPath);
            if (null == di)
                return;

            System.IO.FileInfo[] files = di.GetFiles("HOBA_OP_*.xlsx");
            if (null == files)
                return;

            if (0 >= files.Length)
                return;

            var orderedFiles = files.OrderBy(f => f.CreationTime).ToList();
            var firstFile = orderedFiles.First();
            var lastFile = orderedFiles.Last();

            ExcelPath = lastFile.FullName;
        }

        private void InitExcelPathInfo()
        {
            ExcelPath = @"O:\_Dahn\_Post Eingang\";
            ExcelPathTryGetNewerOPOSListe();

            //ExcelPath = @"O:\_Dahn\_Post Eingang\HOBA_OP_" + DanUtility.GetCurrentDateStringAsFormat() + ".xlsx";
        }

        public MainWindow()
        {
            InitializeComponent();

            InitExcelPathInfo();
            
            this.DataContext = this;
        }

        CofaceDB_Accessor.CofDBAccessor cofAccessor = new CofaceDB_Accessor.CofDBAccessor();

        private void btn_OpenExcel_Click(object sender, RoutedEventArgs e)
        {
            var dlgOpenFile = new Microsoft.Win32.OpenFileDialog();
            dlgOpenFile.InitialDirectory = System.IO.Path.GetDirectoryName(ExcelPath);
            dlgOpenFile.Multiselect = false;
            dlgOpenFile.DefaultExt = ".xlsx";
            dlgOpenFile.Filter = "Excel Files (*.xlsx)|*.xlsx";

            if (true == dlgOpenFile.ShowDialog())
            {
                ExcelPath = dlgOpenFile.FileName;
            }
        }

        static class ToQuotenArg
        {          
            public static string GetQuotenString(string strArg)
            {
                return '\"' + strArg + '\"';
            }
        }


        private bool _doExcelWork()
        {
            if (String.IsNullOrEmpty(ExcelPath))
            {
                MessageBox.Show("File was not selected.");
                return false;
            }

            ExcelWorker.ErrorCodes errCode = ExcelWorker.ErrorCodes.IsNotExcelWorkerErrcode;

            System.Diagnostics.Process procExcel = new System.Diagnostics.Process
            {
                StartInfo = {
                    FileName    = "ExcelWorker.exe",
                    Arguments   = ToQuotenArg.GetQuotenString( ExcelPath )
                                    + ' ' 
                                    + ToQuotenArg.GetQuotenString(ExportPath),
                },
            };

            procExcel.Start();


            procExcel.WaitForExit();
            int ret = procExcel.ExitCode;

            if (Enum.IsDefined(typeof(ExcelWorker.ErrorCodes), ret))
                errCode = (ExcelWorker.ErrorCodes)ret;

            if (errCode != ExcelWorker.ErrorCodes.Success)
            {
                string strErrMsg =
                (errCode == ExcelWorker.ErrorCodes.IsNotExcelWorkerErrcode) ?
                    String.Format("Excel work failure with ({0}), retCode({1})", errCode.ToString(), ret) :
                    String.Format("Excel work failure with ({0})", errCode.ToString());

                MessageBox.Show(strErrMsg);
                return false;
            }

            return true;
        }

        private bool _doTableExportByBatch()
        {
            System.Diagnostics.Process procBatch = new System.Diagnostics.Process
            {
                StartInfo =
                {
                    FileName = _sqlExportBatch
                },
            };

            procBatch.Start();
            procBatch.WaitForExit();

            int ret = procBatch.ExitCode;

            return true;
        }

         private void _doSaveFileFormatted()
        {
            string strDefaultFilePath = @"O:\coface\Einreichung\OP_2016053_EUR_" + DanUtility.Util.GetCurrentDateStringAsFormat() + ".csv";
            var saveDlg = new Microsoft.Win32.SaveFileDialog
            {
                InitialDirectory = @"O:\coface\Einreichung",
                DefaultExt = ".csv",
                FileName = "OP_2016053_EUR_" + DanUtility.Util.GetCurrentDateStringAsFormat(),
            };

            if(true == saveDlg.ShowDialog())
            {
                //CSV format for regular
                CSVFormatter.DoFormat(@"C:\Temp\firstexport.csv", saveDlg.FileName);
                MessageBox.Show("Daten hat am (" + saveDlg.FileName + ") gespeichert");
            }
            else
            {
                CSVFormatter.DoFormat(@"C:\Temp\firstexport.csv", strDefaultFilePath);
                MessageBox.Show("File was successfully saved at: ("+ strDefaultFilePath +")");
            }
        }
        private void btnToCSV_Click(object sender, RoutedEventArgs e)
        {


            //cofAccessor.InsertExcelIntoSQLBulk("", "", "");
            if (false == _doExcelWork())
                return;

            //string strTblName = System.IO.Path.GetFileName(ExcelPath);            

            
            //Todo - Table name and sheet name wird als Varaiable
            cofAccessor.InsertExcelIntoSQLBulk(ExportPath, "Tabelle1", "[dbo].[Tbl_Hoba_OPOS_"+ DanUtility.Util.GetCurrentDateStringAsFormat()  +"]");
            cofAccessor.DBMoveHOBAtoCOF();
            //call batch for create DB to csv
            if (false == _doTableExportByBatch())
            {
                MessageBox.Show("Failed to DBTable export to csv automatic. See TestDB\\Tbl_COF_OP manual to csv");

                return;
            }

            _doSaveFileFormatted();
        }

    }
}
