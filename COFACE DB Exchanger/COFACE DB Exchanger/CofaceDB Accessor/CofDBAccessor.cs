using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Data.OleDb;
using System.Data.SqlClient;

namespace COFACE_DB_Exchanger.CofaceDB_Accessor
{
    public class CofDBAccessor
    {
        /// <summary>
        /// 전의 DB랑 비교해서 만드는 Report, Coface규약에 어울리는 Csv를 만들어낸다.
        /// Stammdaten 은 여전히 수동으로 유지.
        /// </summary>
        public CofDBAccessor()
        {
#if DEBUG
            //이 프로퍼티는 UI에서 바인딩해서 현재 어느 DB에서 일 하고 있는지를 표시 하도록 한다.
            _targetDB = "TestDB";
#else
             // 우선은 완전히 모든게 확실 해 질 때 까지(최소 3회 테스트) TestDB를 Release 버전에서도 사용하도록 한다.
            _targetDB = "TestDB";
            //_targetDB = "COFACE";
#endif
        }

        private void PrepareDBTable()
        {
            string sql = "EXEC dbo.usp_Prepare_HOBA_OPOS";
            using (SqlConnection dbCon =
                    new SqlConnection(
                        @"Server=.\SQLEXPRESS;Database="+ TargetDB +";Trusted_Connection=True"
                        ))
            {
                dbCon.Open();

                using (SqlCommand cmd = new SqlCommand(sql, dbCon))
                {
                    cmd.ExecuteNonQuery();
                }
            }
        }

        /// <summary>
        /// 일단은... 파일이 존재하는지, 파일 내에 시트가 존재하는지는 검증을 해서 굴려야 되지만, 이렇게 처리하도록 한다.
        /// 왜냐하면 현재 DB엑세서에서는 Excel COM+ Object에 대한 참조를 만들 생각이 전혀 없이 때문이다.
        /// </summary>
        /// <param name="xlsFilePath"></param>
        /// <param name="sheetName"></param>
        /// <param name="tblName"></param>
        public void InsertExcelIntoSQLBulk(string xlsFilePath, string sheetName, string tblName)
        {
            if (false == System.IO.File.Exists(xlsFilePath))
            {
                System.Diagnostics.Debug.WriteLine("File(" + xlsFilePath + ") does not exists");
                return;
            }

            //추후에는 테이블 이름을 db와 교환하는 방법을 개발 해야 한다. StoredProcedure로 부터 return값으로 받는게 최고다.
            PrepareDBTable();

            //GetFirstSheetName From 

            // 차후에 TargetDB Property Update Cahin 에 연결해서 맴버 변수로 만든다.
            string strConnection = @"Server=.\SQLEXPRESS;Database=" + TargetDB +
                ";Trusted_Connection=True";

            string qry = "SELECT * FROM ["+ sheetName + "$]";

            using (OleDbConnection exlCon = new OleDbConnection(
                "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=" + xlsFilePath +
                ";Extended Properties=\"Excel 12.0 Xml\""))
            using (OleDbCommand cmd = new OleDbCommand(qry, exlCon))
            {
                exlCon.Open();
                using (OleDbDataReader dReader = cmd.ExecuteReader())
                using (SqlBulkCopy sqlBulk = new SqlBulkCopy(strConnection))
                {
                    sqlBulk.DestinationTableName = tblName;
                    sqlBulk.WriteToServer(dReader);
                }
            }
        }

        public void DBMoveHOBAtoCOF()
        {
            string strConnection = @"Provider=SQLOLEDB;Server=.\SQLEXPRESS;Database=" + TargetDB +
                ";Trusted_Connection=yes";

            string strQry = "EXEC [dbo].[usp_OPOS_TblHoba_to_TblCOF]";

            using (OleDbConnection con = new OleDbConnection(strConnection))
            using (OleDbCommand cmd = new OleDbCommand(strQry, con))
            {
                con.Open();
                cmd.ExecuteNonQuery();
            }
        }



        public string TargetDB
        {
            get
            {
                return _targetDB;
            }

            private set
            {
                _targetDB = value;
            }
        }
        private string _targetDB;
        private string _strConnection;
    }
}
