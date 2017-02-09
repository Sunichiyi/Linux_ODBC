#include <stdlib.h>
#include <stdio.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

#define dns "MSSQL"
#define uid "sa"
#define pid "!QAZ2wsx"

SQLHENV			V_OD_Env;     // Handle ODBC environment
long			V_OD_erg;     // result of functions
SQLHDBC			V_OD_hdbc;    // Handle connection
SQLHSTMT		V_OD_hstmt;
char			V_OD_stat[10]; // Status SQL
SQLINTEGER		V_OD_err;
SQLSMALLINT		V_OD_mlen;

SQLINTEGER		V_OD_id;
char			V_OD_msg[200],V_OD_buffer[200],V_OD_email[200];

void Connect2ODBC(void);
void Disconnect2ODBC(void);

int main(int argc,char *argv[])
{
	Connect2ODBC();//--------連線--------

	while(1)
	{
		V_OD_erg=SQLAllocHandle(SQL_HANDLE_STMT, V_OD_hdbc, &V_OD_hstmt);
		if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
		{
			printf("Fehler im AllocStatement %d\n",V_OD_erg);
			SQLGetDiagRec(SQL_HANDLE_DBC, V_OD_hdbc,1, V_OD_stat,&V_OD_err,V_OD_msg,100,&V_OD_mlen);
			printf("%s (%d)\n",V_OD_msg,V_OD_err);
        	SQLDisconnect(V_OD_hdbc);
        	SQLFreeHandle(SQL_HANDLE_DBC,V_OD_hdbc);
			SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
			exit(0);
		}
		//--------合併陣列&宣告類型--------
		SQLBindCol(V_OD_hstmt,1,SQL_C_ULONG,&V_OD_id,150,&V_OD_err);
		SQLBindCol(V_OD_hstmt,2,SQL_C_CHAR, &V_OD_buffer,150,&V_OD_err);
		SQLBindCol(V_OD_hstmt,3,SQL_C_CHAR, &V_OD_email,150,&V_OD_err);
		V_OD_erg=SQLExecDirect(V_OD_hstmt,"SELECT TOP 1 CO2,TEMP,WET FROM Sensor3in1 ORDER BY DateTime DESC",SQL_NTS);   
		if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
		{
			printf("Error in Select %d\n",V_OD_erg);
	   		SQLGetDiagRec(SQL_HANDLE_DBC, V_OD_hdbc,1, V_OD_stat,&V_OD_err,V_OD_msg,100,&V_OD_mlen);
	   		printf("%s (%d)\n",V_OD_msg,V_OD_err);
	   		SQLFreeHandle(SQL_HANDLE_STMT,V_OD_hstmt);
	   		SQLDisconnect(V_OD_hdbc);
	   		SQLFreeHandle(SQL_HANDLE_DBC,V_OD_hdbc);
	   		SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
	   		exit(0);
		}
		
		//--------顯示查詢結果--------
		printf("SELECT CO2,TEMP,WET FROM Sensor3in1\n---------------------------------\n");
		V_OD_erg=SQLFetch(V_OD_hstmt);  
		while(V_OD_erg != SQL_NO_DATA)
		{
	 		printf("Result: %d\t%s\t%s\n",V_OD_id,V_OD_buffer,V_OD_email);
	 		V_OD_erg=SQLFetch(V_OD_hstmt);  
		};
	}

	Disconnect2ODBC();

}

void Connect2ODBC(void)
{
	// 1. allocate Environment handle and register version 
	V_OD_erg=SQLAllocHandle(SQL_HANDLE_ENV,SQL_NULL_HANDLE,&V_OD_Env);
	if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
	{
		printf("Error AllocHandle\n");
		exit(0);
	}
	V_OD_erg=SQLSetEnvAttr(V_OD_Env, SQL_ATTR_ODBC_VERSION,(void*)SQL_OV_ODBC3, 0); 
	if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
	{
		printf("Error SetEnv\n");
		SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
		exit(0);
	}

	// 2. allocate connection handle, set timeout
	V_OD_erg = SQLAllocHandle(SQL_HANDLE_DBC, V_OD_Env, &V_OD_hdbc); 
	if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
	{
		printf("Error AllocHDB %d\n",V_OD_erg);
		SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
		exit(0);
	}
	SQLSetConnectAttr(V_OD_hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER *)5, 0);

	// 3. Connect to the datasource  
	V_OD_erg = SQLConnect(V_OD_hdbc, dns, SQL_NTS,
                                     uid, SQL_NTS,
                                     pid, SQL_NTS);
	if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
	{
		printf("Error SQLConnect %d\n",V_OD_erg);
		SQLGetDiagRec(SQL_HANDLE_DBC, V_OD_hdbc,1, 
		              V_OD_stat, &V_OD_err,V_OD_msg,100,&V_OD_mlen);
		printf("%s (%d)\n",V_OD_msg,V_OD_err);
		SQLFreeHandle(SQL_HANDLE_DBC, V_OD_hdbc);
		SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
		exit(0);
	}
	printf("Connected !\n");
}

void Disconnect2ODBC(void)
{
	//--------斷開連線--------
	SQLFreeHandle(SQL_HANDLE_STMT,V_OD_hstmt);
	SQLDisconnect(V_OD_hdbc);
	SQLFreeHandle(SQL_HANDLE_DBC,V_OD_hdbc);
	SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
}

	/*
	SQLSMALLINT		V_OD_colanz;
	//--------計算有幾個欄位--------
	V_OD_erg=SQLNumResultCols(V_OD_hstmt,&V_OD_colanz);
	if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
	{
    	SQLFreeHandle(SQL_HANDLE_STMT,V_OD_hstmt);
    	SQLDisconnect(V_OD_hdbc);
    	SQLFreeHandle(SQL_HANDLE_DBC,V_OD_hdbc);
    	SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
    	exit(0);
	}
	printf("Number of Columns %d\n",V_OD_colanz);
	*/

	/*
	SQLINTEGER		V_OD_rowanz;
	//--------計算有幾個紀錄--------
	V_OD_erg=SQLRowCount(V_OD_hstmt,&V_OD_rowanz);
	if ((V_OD_erg != SQL_SUCCESS) && (V_OD_erg != SQL_SUCCESS_WITH_INFO))
	{
  		printf("Number ofRowCount %d\n",V_OD_erg);
  		SQLFreeHandle(SQL_HANDLE_STMT,V_OD_hstmt);
  		SQLDisconnect(V_OD_hdbc);
  		SQLFreeHandle(SQL_HANDLE_DBC,V_OD_hdbc);
 		SQLFreeHandle(SQL_HANDLE_ENV, V_OD_Env);
  		exit(0);
	}
	printf("Number of Rows %d\n",V_OD_rowanz);
	*/