#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <iostream>

static void extract_error(const char *fn, SQLHANDLE handle, SQLSMALLINT type)
{
	SQLINTEGER i = 0;
	SQLINTEGER  NativeError;
	SQLTCHAR SQLState[7];
	SQLTCHAR MessageText[256];
	SQLSMALLINT TextLength;
	SQLRETURN ret;
	do
	{
		ret = SQLGetDiagRec(type, handle, ++i, SQLState, &NativeError,
			MessageText, sizeof(MessageText), &TextLength);
		std::wstring msg = MessageText;
		setlocale(LC_ALL, "korean");
		if (SQL_SUCCEEDED(ret)) {
			std::wcout << msg.c_str() << std::endl;
		}
	} while (ret == SQL_SUCCESS);
}
#define CHECK_ERROR(e, s, h, t) {if (e!=SQL_SUCCESS && e != SQL_SUCCESS_WITH_INFO) {extract_error(s, h, t);}}

using RECORD = std::vector<std::wstring>;

class C_ODBC
{
public:
	SQLHENV		m_hEnv;
	SQLHDBC		m_hDbc;
	SQLHSTMT	m_hStmt;
	SQLHSTMT	m_hCreateAccount;
	SQLHSTMT	LoginUserCheck;
	SQLHSTMT	m_hCreateAccountImage;
	SQLHSTMT	m_hAllUserList;
	SQLHSTMT	m_hUpdateStmt;
	BYTE*		m_pFileBuffer;

	TCHAR		m_szID[32];
	TCHAR		m_szPS[32];
	TCHAR		m_szNAME[32];
	int			m_iOutRet;
	int			m_iPrice;
	int			m_iRet;
	SQLINTEGER	m_iDataLength;
	SQLLEN		m_cbColumn;

	//void ImageLoadDirectSQL(const TCHAR* id);
	bool FileLoad(std::wstring name);
	bool ProcedurePrepare();
	//bool CreateAccountImage(byte* pBuffer);
	int  AccountUserImage(const TCHAR* id, const TCHAR* ps,
		std::wstring filename);
	int  ExecProc(SQLHSTMT stmt);
public:
	struct ColDescription
	{
		SQLUSMALLINT       icol;
		SQLWCHAR		   szColName[80];
		SQLSMALLINT        cchColNameMax;
		SQLSMALLINT        pcchColName;
		SQLSMALLINT        pfSqlType;
		SQLULEN            pcbColDef;
		SQLSMALLINT        pibScale;
		SQLSMALLINT        pfNullable;
		SQLWCHAR		   bindData[80];
		//TargetValuePtr 버퍼에서 리턴할 수 있는 바이트 수를 표시하는 값(또는 값 배열)의 포인터
		SQLLEN			   byteSize; // bite
	};
	std::vector< ColDescription> m_ColumnList;
	RECORD					m_ColumnResultList;
	std::vector<RECORD>		m_RecordResultList;

	bool		Init();
	//bool		ConnectAccess(const TCHAR* loadDB);
	bool		ConnectSQL();
	bool		ConnectDSN(const TCHAR* loadDB);
	bool		ConnectSQL(const TCHAR* dsn,
		const TCHAR* id, const TCHAR* ps);
	bool		Release();
	void		Print();
	void		Check(SQLHSTMT stmt);
	bool		Prepare(SQLHSTMT& stmt, const TCHAR* sql, int iMaker = 0);
	bool		Exec(const TCHAR* sql);
	bool		ExecReturn(const TCHAR* sql);
	bool		Exec(SQLHSTMT stmt);
	bool		ExecReturn(SQLHSTMT stmt);
	void		GetDescribeCol(SQLHSTMT stmt);
	bool		GetRecordData(SQLHSTMT stmt);
public:
	bool		UpdateRecord();
	bool		ReadRecord();
	bool		AddRecord();
	bool		DeleteRecord();
public:
	C_ODBC();
	~C_ODBC();
};

