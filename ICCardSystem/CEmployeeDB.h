#pragma once
#include "CEmployee.h"
#include "ZSqlite3.h"
class CEmployeeDB
{
private:
	CString m_strPathDB;
	void CreateTable();
public:
	CEmployeeDB();
	virtual ~CEmployeeDB();
	bool Insert(const CEmployee & employee);
	bool Update(const CEmployee & employee);
	bool Select(const CString & strSql, std::vector<CEmployee> & vec_employee);//strSql��Ϊ��SELECT *ѡ��ȫ���в��ܷ��ض���
};

