sqlcmd -S .\SQLEXPRESS -d COFACE -E -s";" -o C:\temp\firstexport.csv -W -f 65001 -i "C:\Users\A.Roennburg\Documents\GH_GitManaged\Queries\bat_COF_OP_Query.sql"
