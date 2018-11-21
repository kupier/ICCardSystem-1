#include "stdafx.h"
#include "CCard.h"


CCard::CCard()
	:m_nID(0)
	,m_nState(STATE_UNOPEN)
{
}


CCard::CCard(int nID, int nUserID, int nNo,int nType, CString strDeadline, int nDeposit,int nState, int nEmployeeIDOpenCard, CString strTimeOpenCard, int nEmployeeIDCloseCard, CString strTimeCloseCard)
	: m_nID(nID)
	, m_nUserID(nUserID)
	, m_nNo(nNo)
	, m_nType(nType)
	, m_strDeadline(strDeadline)
	, m_nDeposit(nDeposit)
	, m_nState(STATE_UNOPEN)
	, m_nEmployeeIDOpenCard(nEmployeeIDOpenCard)
	, m_strTimeOpenCard(strTimeOpenCard)
	, m_nEmployeeIDCloseCard(nEmployeeIDCloseCard)
	, m_strTimeCloseCard(strTimeCloseCard)
{
	if (nState < STATE_ERROR)
		m_nState = nState;
}


CCard::~CCard()
{
}


int CCard::GetID() const
{
	return m_nID;
}


int CCard::GetUserID() const
{
	return m_nUserID;
}


int CCard::GetNo() const
{
	return m_nNo;
}


int CCard::GetType() const
{
	return m_nType;
}


CString CCard::GetDeadline() const
{
	return m_strDeadline;
}


int CCard::GetDeposit() const
{
	return m_nDeposit;
}


int CCard::GetState() const
{
	return m_nState;
}


int CCard::GetEmployeeIDOpenCard() const
{
	return m_nEmployeeIDOpenCard;
}


CString CCard::GetTimeOpenCard() const
{
	return m_strTimeOpenCard;
}


int CCard::GetEmployeeIDCloseCard() const
{
	return m_nEmployeeIDCloseCard;
}


CString CCard::GetTimeCloseCard() const
{
	return m_strTimeCloseCard;
}


void CCard::SetID(int nID)
{
	m_nID = nID;
}


void CCard::SetUserID(int nUserID)
{
	m_nUserID = nUserID;
}


void CCard::SetNo(int nNo)
{
	m_nNo = nNo;
}


void CCard::SetType(int nType)
{
	m_nType = nType;
}


void CCard::SetDeadline(CString strDeadline)
{
	m_strDeadline = strDeadline;
}


void CCard::SetDeposit(int nDeposit)
{
	m_nDeposit = nDeposit;
}


void CCard::SetState(int nState)
{
	if (nState < STATE_ERROR)
		m_nState = nState;
}


void CCard::SetEmployeeIDOpenCard(int nEmployeeIDOpenCard)
{
	m_nEmployeeIDOpenCard = nEmployeeIDOpenCard;
}


void CCard::SetTimeOpenCard(CString strTimeOpenCard)
{
	m_strTimeOpenCard = strTimeOpenCard;
}


void CCard::SetEmployeeIDCloseCard(int nEmployeeIDCloseCard)
{
	m_nEmployeeIDCloseCard = nEmployeeIDCloseCard;
}


void CCard::SetTimeCloseCard(CString strTimeCloseCard)
{
	m_strTimeCloseCard = strTimeCloseCard;
}