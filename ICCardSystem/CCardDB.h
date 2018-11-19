#pragma once
#include "CCard.h"
#include "ZSqlite3.h"
class CCardDB
{
private:
	CString m_strPathDB;
	void CreateTable();
public:
	CCardDB();
	virtual ~CCardDB();
	bool Insert(const CCard & card);
	bool UpdateCloseCard(const CCard & card);
	bool Select(const CString & strSql, std::vector<CCard> & vec_card);//strSql��Ϊ��SELECT *ѡ��ȫ���в��ܷ��ض���
};

