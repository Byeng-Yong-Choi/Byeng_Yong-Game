#include "C_ODBC.h"

//void C_ODBC::ImageLoadDirectSQL(const TCHAR* id)
//{
//	SQLRETURN ret;
//	SQLHSTMT  stmt;
//	ret = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc,&stmt);
//
//	CHAR smallbufer[1000] = { 0, };
//	TCHAR sql[MAX_PATH] = L"{call GetImageStreaming(?,?)}";
//
//	TCHAR szSelectName[32] = { 0, };
//	_tcscpy(szSelectName, id);
//	SQLINTEGER strLength = sizeof(szSelectName);
//	SQLLEN m_cbColumn = SQL_NTS;
//	ret = SQLBindParameter(stmt, 1, SQL_PARAM_INPUT,SQL_UNICODE, SQL_VARCHAR, strLength,0, szSelectName, strLength, &m_cbColumn);
//
//	SQLLEN cbPutSize = 0;
//	ret = SQLBindParameter(stmt,2, SQL_PARAM_OUTPUT_STREAM,SQL_C_BINARY, SQL_VARBINARY, 0, 0,(SQLPOINTER)2,0, &cbPutSize);
//	if (ret != SQL_SUCCESS && ret != SQL_NEED_DATA)
//	{
//		Check(stmt);
//	}
//
//	ret = SQLPrepare(stmt, sql, SQL_NTS);
//	if (ret != SQL_SUCCESS)
//	{
//		Check(stmt);
//		return;
//	}
//
//	///////////////  실행하기 ///////////////////
//
//	ret = SQLExecute(stmt);
//	if (ret != SQL_PARAM_DATA_AVAILABLE)
//	{
//		Check(stmt);
//		return;
//	}
//
//	DWORD dwTrans;
//	LARGE_INTEGER filesize;
//
//	std::wstring fileImage = szSelectName;
//	fileImage += L".png";
//
//	FILE* fp = _wfopen(fileImage.c_str(), L"w+b");
//
//	if (fp != NULL)
//	{
//		SQLPOINTER token;
//		SQLLEN cb;
//		ret = SQLParamData(stmt, &token);
//		if (ret == SQL_PARAM_DATA_AVAILABLE)
//		{
//			do {
//				ret = SQLGetData(stmt,(UWORD)token, SQL_C_BINARY,smallbufer, sizeof(smallbufer),&cb);
//				if (ret == SQL_NULL_DATA)
//				{
//					break;
//				}
//				if (cb > sizeof(smallbufer))
//					fwrite(smallbufer, sizeof(char), sizeof(smallbufer), fp);
//				else
//					fwrite(smallbufer, sizeof(char), cb, fp);
//			} while (ret == SQL_SUCCESS_WITH_INFO);
//		}
//
//	}
//	fflush(fp);
//	fclose(fp);
//
//	SQLCloseCursor(stmt);
//	SQLFreeStmt(stmt, SQL_CLOSE);
//}
bool C_ODBC::FileLoad(std::wstring name)
{
	DWORD dwTrans;
	LARGE_INTEGER filesize;
	HANDLE hReadFile = CreateFile(name.c_str(), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hReadFile != INVALID_HANDLE_VALUE)
	{
		GetFileSizeEx(hReadFile, &filesize);
		m_pFileBuffer = new BYTE[filesize.LowPart];
		ReadFile(hReadFile, m_pFileBuffer,
			filesize.LowPart, &dwTrans, NULL);
	}
	else
	{
		return false;
	}
	CloseHandle(hReadFile);
	return true;
}
int C_ODBC::AccountUserImage(const TCHAR* id,
	const TCHAR* ps,
	std::wstring filename)
{
	m_iRet = -1;
	SQLHSTMT  stmt;
	SQLRETURN hr = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc,
		&stmt);

	std::wstring filepath = L"../../data/ui/";
	filepath += filename;
	if (FileLoad(filepath) == false)
	{
		return m_iRet;
	}

	TCHAR sql[MAX_PATH] = { 0, };
	wsprintf(sql,
		L"INSERT INTO USERLIST( USERID, USERPS, BlobData) VALUES('%s', '%s',?)",
		id, ps);

	// rerd
	SQLLEN lBytes = _msize(m_pFileBuffer);
	SQLLEN cbPutSize = SQL_LEN_DATA_AT_EXEC(lBytes);
	hr = SQLBindParameter(stmt, 1,SQL_PARAM_INPUT,SQL_C_BINARY,SQL_LONGVARBINARY,lBytes,0,(SQLPOINTER)3,lBytes,&cbPutSize);
	if (hr != SQL_SUCCESS)
	{
		Check(stmt);
		return false;
	}


	hr = SQLExecDirect(stmt, (SQLTCHAR*)sql, SQL_NTS);
	if (hr != SQL_SUCCESS && hr != SQL_NEED_DATA)
	{
		Check(stmt);
		return m_iRet;
	}
	/// 이미지 전송 시작
	SQLPOINTER pParamID;
	SQLParamData(stmt, &pParamID);

	lBytes = _msize(m_pFileBuffer);
	SQLLEN pos = 0;
	if (hr == SQL_NEED_DATA)
	{
		SWORD cbBatch = 1000;
		while (lBytes > cbBatch)
		{
			hr = SQLPutData(stmt, &m_pFileBuffer[pos], cbBatch);
			lBytes -= cbBatch;
			pos += cbBatch;
		}
	}
	hr = SQLPutData(stmt, &m_pFileBuffer[pos], lBytes);
	// 최종 전송 알림.
	hr = SQLParamData(stmt, &pParamID);
	if (hr != SQL_SUCCESS &&
		hr != SQL_SUCCESS_WITH_INFO)
	{
		Check(stmt);
		return m_iRet;
	}
	delete m_pFileBuffer;
	m_pFileBuffer = 0;

	while (hr = SQLMoreResults(stmt) != SQL_NO_DATA);
	SQLCloseCursor(stmt);
	SQLFreeStmt(stmt, SQL_CLOSE);
	return m_iRet;
}
void C_ODBC::Check(SQLHSTMT stmt)
{
	SQLTCHAR szSQLState[SQL_SQLSTATE_SIZE + 1];
	SQLTCHAR errorBuffer[SQL_MAX_MESSAGE_LENGTH + 1];
	SQLINTEGER iSQLCode;
	SQLSMALLINT length;
	SQLError(m_hEnv, m_hDbc,
		stmt,
		szSQLState,
		&iSQLCode,
		errorBuffer,
		sizeof(errorBuffer),
		&length);
	MessageBox(NULL, errorBuffer, szSQLState, MB_OK);
}
bool C_ODBC::Init()
{
	if (SQLAllocHandle(SQL_HANDLE_ENV,
		SQL_NULL_HANDLE, &m_hEnv) != SQL_SUCCESS)
	{
		return false;
	}
	if (SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION,
		(SQLPOINTER)SQL_OV_ODBC3_80,
		SQL_IS_INTEGER) != SQL_SUCCESS)
	{
		return false;
	}
	if (SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc) != SQL_SUCCESS)
	{
		return false;
	}
	return true;
}
//bool C_ODBC::CreateAccountImage(byte* pBuffer)
//{
//	SQLRETURN hr;
//	// CreateAccountUser
//	{
//		hr = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc,
//			&m_hCreateAccountImage);
//		hr = SQLPrepare(m_hCreateAccountImage,
//			(SQLTCHAR*)_T("{?=CALL CreateAccountImage(?,?,?,?)}"), SQL_NTS);
//		// 반환값
//		hr = SQLBindParameter(m_hCreateAccountImage, 1, SQL_PARAM_OUTPUT, SQL_C_ULONG,
//			SQL_INTEGER, 0, 0, &m_iRet, 0, NULL);
//		if (hr != SQL_SUCCESS)
//		{
//			Check(m_hCreateAccountImage);
//			return false;
//		}
//		SQLLEN cbColumn = SQL_NTS;
//		ZeroMemory(m_szID, sizeof(m_szID));
//		m_iDataLength = sizeof(m_szPS);
//		m_cbColumn = SQL_NTS;
//
//		// ID
//		hr = SQLBindParameter(m_hCreateAccountImage, 2,
//			SQL_PARAM_INPUT, SQL_UNICODE, SQL_VARCHAR,
//			m_iDataLength, 0, m_szID, m_iDataLength, &m_cbColumn);
//		if (hr != SQL_SUCCESS)
//		{
//			Check(m_hCreateAccountImage);
//			return false;
//		}
//		//PS
//		ZeroMemory(m_szPS, sizeof(m_szPS));
//		m_iDataLength = sizeof(m_szPS);
//		m_cbColumn = SQL_NTS;
//
//		hr = SQLBindParameter(m_hCreateAccountImage, 3,
//			SQL_PARAM_INPUT,
//			SQL_UNICODE,
//			SQL_VARCHAR,
//			m_iDataLength,
//			0,
//			m_szPS,
//			m_iDataLength,
//			&m_cbColumn);
//		if (hr != SQL_SUCCESS)
//		{
//			Check(m_hCreateAccount);
//			return false;
//		}
//		// rerd
//		SQLLEN lBytes = 0;
//		SQLLEN cbPutSize = SQL_LEN_DATA_AT_EXEC(0);
//		hr = SQLBindParameter(m_hCreateAccountImage, 4,
//			SQL_PARAM_INPUT,
//			SQL_C_BINARY,
//			SQL_VARBINARY,
//			0,
//			0,
//			m_pFileBuffer,
//			0,
//			&cbPutSize);
//		if (hr != SQL_SUCCESS)
//		{
//			Check(m_hCreateAccount);
//			return false;
//		}
//	}
//}
bool C_ODBC::ProcedurePrepare()
{
	// all user list
	SQLRETURN hr;
	{
		hr = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hAllUserList);
		hr = SQLPrepare(m_hAllUserList, (SQLTCHAR*)_T("{?=CALL AllUserList}"), SQL_NTS);
		// 반환값
		hr = SQLBindParameter(m_hAllUserList, 1, SQL_PARAM_OUTPUT, SQL_C_ULONG,
			SQL_INTEGER, 0, 0, &m_iRet, 0, NULL);
		if (hr != SQL_SUCCESS)
		{
			Check(m_hAllUserList);
			return false;
		}
	}

	// LoginUser
	{
		hr = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &LoginUserCheck);
		hr = SQLPrepare(LoginUserCheck, (SQLTCHAR*)_T("{?=CALL LoginUserCheck(?,?,?)}"), SQL_NTS);
		// 반환값
		hr = SQLBindParameter(LoginUserCheck, 1, SQL_PARAM_OUTPUT, SQL_C_ULONG,
			SQL_INTEGER, 0, 0, &m_iRet, 0, NULL);
		if (hr != SQL_SUCCESS)
		{
			Check(LoginUserCheck);
			return false;
		}
		SQLLEN cbColumn = SQL_NTS;
		ZeroMemory(m_szID, sizeof(m_szID));
		m_iDataLength = sizeof(m_szID);
		m_cbColumn = SQL_NTS;

		// ID
		hr = SQLBindParameter(LoginUserCheck, 2,
			SQL_PARAM_INPUT, SQL_UNICODE, SQL_VARCHAR,
			m_iDataLength, 0, m_szID, m_iDataLength, &m_cbColumn);
		if (hr != SQL_SUCCESS)
		{
			Check(LoginUserCheck);
			return false;
		}
		//PS
		ZeroMemory(m_szPS, sizeof(m_szPS));
		m_iDataLength = sizeof(m_szPS);
		m_cbColumn = SQL_NTS;

		hr = SQLBindParameter(LoginUserCheck, 3,
			SQL_PARAM_INPUT,
			SQL_UNICODE,
			SQL_VARCHAR,
			m_iDataLength,
			0,
			m_szPS,
			m_iDataLength,
			&m_cbColumn);
		if (hr != SQL_SUCCESS)
		{
			Check(LoginUserCheck);
			return false;
		}
		// rerd
		hr = SQLBindParameter(LoginUserCheck, 4, SQL_PARAM_OUTPUT, SQL_C_ULONG,
			SQL_INTEGER, 0, 0, &m_iOutRet, 0, NULL);
		if (hr != SQL_SUCCESS)
		{
			Check(LoginUserCheck);
			return false;
		}
	}

	// CreateAccountUser
	{
		hr = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hCreateAccount);
		hr = SQLPrepare(m_hCreateAccount, (SQLTCHAR*)_T("{?=CALL CreateAccountUser(?,?,?)}"), SQL_NTS);
		// 반환값
		hr = SQLBindParameter(m_hCreateAccount, 1, SQL_PARAM_OUTPUT, SQL_C_ULONG,
			SQL_INTEGER, 0, 0, &m_iRet, 0, NULL);
		if (hr != SQL_SUCCESS)
		{
			Check(m_hCreateAccount);
			return false;
		}
		SQLLEN cbColumn = SQL_NTS;
		ZeroMemory(m_szID, sizeof(m_szID));
		m_iDataLength = sizeof(m_szPS);
		m_cbColumn = SQL_NTS;

		// ID
		hr = SQLBindParameter(m_hCreateAccount, 2,
			SQL_PARAM_INPUT, SQL_UNICODE, SQL_VARCHAR,
			m_iDataLength, 0, m_szID, m_iDataLength, &m_cbColumn);
		if (hr != SQL_SUCCESS)
		{
			Check(m_hCreateAccount);
			return false;
		}
		//PS
		ZeroMemory(m_szPS, sizeof(m_szPS));
		m_iDataLength = sizeof(m_szPS);
		m_cbColumn = SQL_NTS;

		hr = SQLBindParameter(m_hCreateAccount, 3,
			SQL_PARAM_INPUT,
			SQL_UNICODE,
			SQL_VARCHAR,
			m_iDataLength,
			0,
			m_szPS,
			m_iDataLength,
			&m_cbColumn);
		if (hr != SQL_SUCCESS)
		{
			Check(m_hCreateAccount);
			return false;
		}
		// rerd
		hr = SQLBindParameter(m_hCreateAccount, 4, SQL_PARAM_OUTPUT, SQL_C_ULONG,
			SQL_INTEGER, 0, 0, &m_iOutRet, 0, NULL);
		if (hr != SQL_SUCCESS)
		{
			Check(m_hCreateAccount);
			return false;
		}
	}
}
int C_ODBC::ExecProc(SQLHSTMT stmt)
{
	m_iRet = -1;
	SQLRETURN hr = SQLExecute(stmt);
	if (hr != SQL_SUCCESS)
	{
		Check(stmt);
		return m_iRet;
	}
	if (m_iRet < 3)
	{
		GetDescribeCol(stmt);
		if (GetRecordData(stmt) == false)
		{
			return m_iRet;
		}
	}

	while (hr = SQLMoreResults(stmt) != SQL_NO_DATA);
	SQLCloseCursor(stmt);
	SQLFreeStmt(stmt, SQL_CLOSE);
	return m_iRet;
}
//bool C_ODBC::ConnectAccess(const TCHAR* loadDB)
//{
//	TCHAR Dir[MAX_PATH] = { 0, };
//	GetCurrentDirectory(MAX_PATH, Dir);
//
//
//	TCHAR InCon[MAX_PATH] = { 0, };
//	_stprintf(InCon,_T("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=%s\\%s;"),
//		Dir, loadDB);
//	SQLRETURN hr = SQLDriverConnect(m_hDbc, NULL,
//		(SQLTCHAR*)InCon, _countof(InCon),
//		NULL, NULL, NULL,
//		SQL_DRIVER_NOPROMPT);
//	if (hr != SQL_SUCCESS)
//	{
//
//	}
//
//	SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt);
//
//	return true;
//}
bool C_ODBC::ConnectSQL()
{
	TCHAR InCon[MAX_PATH] = { 0, };
	_stprintf(InCon, _T("%s"), _T("Driver={SQL Server};Server=shader.kr;Address=192.168.0.49,1433;Network=dbmssocn;Database=WildPlanetDB;Uid=sa;Pwd=kgca1234;"));
	SQLRETURN hr = SQLDriverConnect(m_hDbc, NULL,(SQLTCHAR*)InCon, _countof(InCon),NULL, NULL, NULL,SQL_DRIVER_NOPROMPT);
	if (hr != SQL_SUCCESS && hr != SQL_SUCCESS_WITH_INFO)
	{
		Check(m_hDbc);
	}

	SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt);

	return true;
}
bool C_ODBC::ConnectDSN(const TCHAR* dsn)
{
	TCHAR InCon[MAX_PATH] = { 0, };
	TCHAR id[MAX_PATH] = L"sa";
	TCHAR ps[MAX_PATH] = L"kgca1234";
	_stprintf(InCon, _T("FileDsn=%s"), dsn);
	SQLRETURN hr = SQLDriverConnect(m_hDbc, NULL,
		(SQLTCHAR*)InCon, _countof(InCon),
		NULL, NULL, NULL,
		SQL_DRIVER_NOPROMPT);

	if (hr != SQL_SUCCESS && hr != SQL_SUCCESS_WITH_INFO)
	{
		CHECK_ERROR(hr, "SQLDriverConnect(DATASOURCE)", m_hDbc, SQL_HANDLE_DBC);
	}
	SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt);

	return true;
}
bool C_ODBC::ConnectSQL(const TCHAR* dsn,const TCHAR* id, const TCHAR* ps)
{
	TCHAR InCon[MAX_PATH] = { 0, };
	_stprintf(InCon, _T("FileDsn=%s"), dsn);
	SQLRETURN hr = SQLConnect(m_hDbc,(SQLTCHAR*)dsn, SQL_NTS,(SQLTCHAR*)id, SQL_NTS,(SQLTCHAR*)ps, SQL_NTS);

	if (hr != SQL_SUCCESS && hr != SQL_SUCCESS_WITH_INFO)
	{
		CHECK_ERROR(hr, "SQLDriverConnect(DATASOURCE)", m_hDbc, SQL_HANDLE_DBC);
	}

	SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt);

	return true;
}
void C_ODBC::Print()
{
	setlocale(LC_ALL, "korean");
	for (auto& data : m_RecordResultList)
	{
		for (auto& col : data)
		{
			std::wcout << col << " ";
		}
		std::wcout << std::endl;
	}
}
bool C_ODBC::Release()
{
	if (m_hEnv) SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
	SQLDisconnect(m_hDbc);

	if (m_hDbc) SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
	if (m_hEnv) SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
	return true;
}
bool C_ODBC::Prepare(SQLHSTMT& stmt, const TCHAR* sql, int iMaker)
{
	SQLRETURN hr;
	hr = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &stmt);
	hr = SQLPrepare(stmt, (SQLTCHAR*)sql, SQL_NTS);

	// 반환값
	hr = SQLBindParameter(stmt, 1, SQL_PARAM_OUTPUT, SQL_C_ULONG,
		SQL_INTEGER, 0, 0, &m_iRet, 0, NULL);
	if (hr != SQL_SUCCESS)
	{
		Check(stmt);
		return false;
	}

	if (iMaker > 0)
	{
		SQLLEN cbColumn = SQL_NTS;
		ZeroMemory(m_szPS, sizeof(m_szPS));
		m_iDataLength = sizeof(m_szPS);
		m_cbColumn = SQL_NTS;

		// ID
		hr = SQLBindParameter(stmt, 2,SQL_PARAM_INPUT, SQL_UNICODE, SQL_VARCHAR,m_iDataLength, 0, m_szPS, m_iDataLength, &m_cbColumn);
		if (hr != SQL_SUCCESS)
		{
			Check(stmt);
			return false;
		}
		//PS
		ZeroMemory(m_szID, sizeof(m_szID));
		m_iDataLength = sizeof(m_szID);
		m_cbColumn = SQL_NTS;

		hr = SQLBindParameter(stmt, 3,SQL_PARAM_INPUT,SQL_UNICODE,SQL_VARCHAR,m_iDataLength,0,m_szID,m_iDataLength,&m_cbColumn);
		if (hr != SQL_SUCCESS)
		{
			Check(stmt);
			return false;
		}
		// rerd
		hr = SQLBindParameter(stmt, 4, SQL_PARAM_OUTPUT, SQL_C_ULONG,
			SQL_INTEGER, 0, 0, &m_iOutRet, 0, NULL);
		if (hr != SQL_SUCCESS)
		{
			Check(stmt);
			return false;
		}
	}
	return true;
}
void C_ODBC::GetDescribeCol(SQLHSTMT stmt)
{
	SQLRETURN hr;

	m_ColumnList.clear();
	m_ColumnResultList.clear();
	m_RecordResultList.clear();

	ColDescription col;
	col.icol = 1;
	while (1)
	{
		hr = SQLDescribeCol(stmt,
			col.icol,
			col.szColName,
			sizeof(col.szColName),
			&col.pcchColName,
			&col.pfSqlType,
			&col.pcbColDef,
			&col.pibScale,
			&col.pfNullable);
		if (hr != SQL_SUCCESS)
		{
			break;
		}
		m_ColumnList.push_back(col);
		col.icol++;
	}
}
bool C_ODBC::GetRecordData(SQLHSTMT stmt)
{
	SQLRETURN hr;
	if (m_ColumnList.size() <= 0) return true;

	while (hr = SQLFetch(stmt) != SQL_NO_DATA)
	{
		RECORD record;
		for (int iCol = 0;
			iCol < m_ColumnList.size();
			iCol++)
		{
			std::wstring retData;
			if (m_ColumnList[iCol].pfSqlType != SQL_VARBINARY)
			{
				hr = SQLGetData(stmt,
					m_ColumnList[iCol].icol,
					SQL_WCHAR,
					m_ColumnList[iCol].bindData,
					sizeof(m_ColumnList[iCol].bindData),
					NULL);
				if (hr == SQL_SUCCESS)
				{
					retData = m_ColumnList[iCol].bindData;
					record.push_back(retData);
				}
			}
		}
		m_RecordResultList.push_back(record);
	}
	return true;
}
bool C_ODBC::Exec(const TCHAR* sql)
{
	SQLRETURN hr = SQLExecDirect(m_hStmt,
		(SQLTCHAR*)sql,
		SQL_NTS);
	if (hr != SQL_SUCCESS)
	{
		return false;
	}
	GetDescribeCol(m_hStmt);
	return GetRecordData(m_hStmt);
}
bool C_ODBC::ExecReturn(const TCHAR* sql)
{
	SQLRETURN hr = SQLExecDirect(m_hStmt,
		(SQLTCHAR*)sql,
		SQL_NTS);
	if (hr != SQL_SUCCESS)
	{
		return false;
	}

	while (hr = SQLMoreResults(m_hStmt) != SQL_NO_DATA);
	SQLCloseCursor(m_hStmt);
	SQLFreeStmt(m_hStmt, SQL_CLOSE);
	return true;
}
bool C_ODBC::Exec(SQLHSTMT stmt)
{
	SQLRETURN hr = SQLExecute(stmt);
	if (hr != SQL_SUCCESS)
	{
		Check(stmt);
		return false;
	}
	GetDescribeCol(stmt);

	if (GetRecordData(stmt) == false)
	{
		return false;
	}
	return true;
}
bool C_ODBC::ExecReturn(SQLHSTMT stmt)
{
	SQLRETURN hr = SQLExecute(stmt);
	if (hr != SQL_SUCCESS)
	{
		Check(stmt);
		return false;
	}
	while (hr = SQLMoreResults(m_hStmt) != SQL_NO_DATA);
	SQLCloseCursor(m_hStmt);
	SQLFreeStmt(m_hStmt, SQL_CLOSE);
	return true;
}
bool C_ODBC::ReadRecord()
{
	SQLRETURN hr;
	m_ColumnList.clear();
	m_ColumnResultList.clear();
	m_RecordResultList.clear();

	hr = SQLExecDirect(m_hStmt,
		(SQLTCHAR*)_T("select * from tblCigar"),
		SQL_NTS);

	ColDescription col;
	col.icol = 1;
	while (1)
	{
		hr = SQLDescribeCol(m_hStmt,
			col.icol,
			col.szColName,
			sizeof(col.szColName),
			&col.pcchColName,
			&col.pfSqlType,
			&col.pcbColDef,
			&col.pibScale,
			&col.pfNullable);
		if (hr != SQL_SUCCESS)
		{
			break;
		}
		m_ColumnList.push_back(col);
		col.icol++;
	}

	while (hr = SQLFetch(m_hStmt) != SQL_NO_DATA)
	{
		RECORD record;
		for (int iCol = 0;
			iCol < m_ColumnList.size();
			iCol++)
		{
			std::wstring retData;
			hr = SQLGetData(m_hStmt,
				m_ColumnList[iCol].icol,
				SQL_WCHAR,
				m_ColumnList[iCol].bindData,
				sizeof(m_ColumnList[iCol].bindData),
				NULL);
			if (hr == SQL_SUCCESS)
			{
				retData = m_ColumnList[iCol].bindData;
				record.push_back(retData);
			}
		}
		m_RecordResultList.push_back(record);
	}
	/*


		TCHAR Name[21] = { 0, };
		SQLLEN lName;
		int Price = { 0, };
		SQLLEN lPrice;
		BOOL Kor = { 0, };
		SQLLEN lKor;
		SQLBindCol(m_hStmt, 1, SQL_UNICODE,
			Name, sizeof(Name), &lName);
		SQLBindCol(m_hStmt, 2, SQL_C_ULONG,
			&Price, 0, &lPrice);
		SQLBindCol(m_hStmt, 3, SQL_C_ULONG,
			&Kor, 0, &lKor);

		hr = SQLExecDirect(m_hStmt,
			(SQLTCHAR*)_T("select name,price,korean from tblCigar"),
			SQL_NTS);

		if (hr != SQL_SUCCESS)
		{
			return false;
		}*/

	setlocale(LC_ALL, "korean");
	for (auto& data : m_RecordResultList)
	{
		for (auto& col : data)
		{
			std::wcout << col << " ";
		}
		std::wcout << std::endl;
	}

	while (hr = SQLMoreResults(m_hStmt) != SQL_NO_DATA);
	SQLCloseCursor(m_hStmt);
	SQLFreeStmt(m_hStmt, SQL_CLOSE);
	return true;
}
bool C_ODBC::UpdateRecord()
{
	TCHAR Name[] = L"디스";
	SQLLEN lName;
	int Price = 2000;
	SQLLEN lPrice;
	BOOL Kor = 1;
	SQLLEN lKor;

	TCHAR szSQL[MAX_PATH] = { 0, };
	wsprintf(szSQL,
		//_T("update tblCigar set name='%s', price=%d, korean=%d where name = '%s'"),
		_T("update tblCigar set price=%d where name = '%s'"),
		Price, Name);

	SQLRETURN hr = SQLExecDirect(m_hStmt,
		(SQLTCHAR*)szSQL,
		SQL_NTS);

	if (hr != SQL_SUCCESS)
	{
		return false;
	}
	SQLCloseCursor(m_hStmt);
	return true;
}
bool C_ODBC::AddRecord()
{
	TCHAR Name[] = L"디스플러스2";
	SQLLEN lName;
	int Price = 4100;
	SQLLEN lPrice;
	BOOL Kor = 1;
	SQLLEN lKor;

	TCHAR szSQL[MAX_PATH] = { 0, };
	wsprintf(szSQL,
		_T("insert into tblCigar (name,price,korean) values ('%s',%d,%d)"),
		Name, Price, Kor);

	SQLRETURN hr = SQLExecDirect(m_hStmt,
		(SQLTCHAR*)szSQL,
		SQL_NTS);

	if (hr != SQL_SUCCESS)
	{
		return false;
	}
	SQLCloseCursor(m_hStmt);
	return true;
}
bool C_ODBC::DeleteRecord()
{
	TCHAR Name[] = L"디스플러스";
	TCHAR szSQL[MAX_PATH] = { 0, };
	wsprintf(szSQL,
		_T("delete from tblCigar where name='%s'"),
		Name);
	SQLRETURN hr = SQLExecDirect(m_hStmt,
		(SQLTCHAR*)szSQL,
		SQL_NTS);
	if (hr != SQL_SUCCESS)
	{
		return false;
	}
	SQLCloseCursor(m_hStmt);
	return true;
}


C_ODBC::C_ODBC()
{
	m_iRet = -999;
}


C_ODBC::~C_ODBC()
{
}
