1.
install unixodbc unixodbc-dev freetds-bin freetds-common freetds-dev tdsodbc odbcinst

2.
設定：
tsql -C（確認Version & freetds.conf位置）
{
	freetds.conf
	{
		[MSSQL]
		host = 192.168.10.1
		port = 1433
	}
}

測試：
tsql -S <servername> -U <username> -P <password>（確認freetds.conf設定正確）
//tsql -S MSSQL -U sa -P '!QAZ2wsx'

3.
設定：
odbinst -j（確認Version & (dbcinst.ini & odbc.ini)位置）
{
	dbcinst.ini
	{
		[FreeTDS]
		Description = TDS driver (Sybase/MS SQL)
		Driver = /usr/lib/arm-linux-gnueabihf/odbc/libtdsodbc.so
		Setup = /usr/lib/arm-linux-gnueabihf/odbc/libtdsS.so
		CPTimeout =
		CPReuse =
		FileUsage = 1
	}

	odbc.ini
	{
		[MSSQL]
		Driver = FreeTDS
		Description = ODBC connection via FreeTDS
		Trace = No
		Servername = MSSQL
		UID = sa
		Port = 1433
		Database = WebAccessPdata
		TDS_Version = auto
	}
}

測試：
isql -v <servername> <username> <password>
//isql -v MSSQL sa '!QAZ2wsx'

4.
gcc -o XXXXX XXXXX.c -lodbc
//gcc -o test odbc_OK.c -lodbc