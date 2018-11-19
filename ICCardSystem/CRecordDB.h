#pragma once
#include "CRecord.h"
#include "ZSqlite3.h"
class CRecordDB
{
private:
	CString m_strTableName;
	CString m_strPathDB;
	void CreateTable();
public:
	CRecordDB(const CString & strTableName);
	virtual ~CRecordDB();
	bool Insert(const CRecord & record);
	bool Select(const CString & strSql, std::vector<CRecord> & vec_record);//strSql��Ϊ��SELECT *ѡ��ȫ���в��ܷ��ض���
};

