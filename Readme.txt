1.
install unixodbc unixodbc-dev odbcinst 
install freetds-bin freetds-common freetds-dev tdsodbc (for MSSQL)
install libmyodbc (for MySQL)

2.(for MSSQL)
設定：
tsql -C（確認freetds.conf位置）
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
odbinst -j（確認(dbcinst.ini & odbc.ini)位置）
{
	odbcinst.ini
	{
		[FreeTDS]
		Description = TDS driver (Sybase/MS SQL)
		Driver = /usr/lib/arm-linux-gnueabihf/odbc/libtdsodbc.so
		Setup = /usr/lib/arm-linux-gnueabihf/odbc/libtdsS.so
		CPTimeout =
		CPReuse =
		FileUsage = 1

		[MySQL]
		Description     = ODBC driver for mariaDB
		Driver          = /usr/lib/arm-linux-gnueabihf/odbc/libmyodbc.so
	}

	odbc.ini
	{
		[MSSQL]
		Driver = FreeTDS
		Description = connection to MSSQL
		Trace = No
		Servername = MSSQL
		UID = sa
		Port = 1433
		Database = WebAccessPdata
		TDS_Version = auto

		[mariadb-connector]
		Description = connection to  MySQL
		Driver      = MySQL
		Database    = test
		Server      = 127.0.0.1
		Port        = 3306
	}
}

測試：
isql -v <servername> <username> <password>
//isql -v MSSQL sa '!QAZ2wsx'

4.
gcc -o XXXXX XXXXX.c -lodbc
//gcc -o test odbc_OK.c -lodbc