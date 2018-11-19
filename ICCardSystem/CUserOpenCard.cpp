// CUserOpenCard.cpp: 实现文件
//

#include "stdafx.h"
#include "ICCardSystem.h"
#include "CUserOpenCard.h"
#include "afxdialogex.h"


// CUserOpenCard 对话框

IMPLEMENT_DYNAMIC(CUserOpenCard, CDialogEx)

CUserOpenCard::CUserOpenCard(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_USEROPENCARD, pParent)
{

}

CUserOpenCard::~CUserOpenCard()
{
}

void CUserOpenCard::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CONDITION, m_comboCondition);
	DDX_Control(pDX, IDC_LIST_USER, m_listctrlUser);
	DDX_Control(pDX, IDC_EDIT_KEYWORD, m_editKeyword);
	DDX_Control(pDX, IDC_EDIT_ID, m_editID);
	DDX_Control(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Control(pDX, IDC_EDIT_IDNO, m_editIDNo);
	DDX_Control(pDX, IDC_EDIT_PHONENO, m_editPhoneNo);
	DDX_Control(pDX, IDC_EDIT_ADDR, m_editAddr);
	DDX_Control(pDX, IDC_EDIT_LICENSE, m_editLicense);
	DDX_Control(pDX, IDC_EDIT_STATE, m_editState);
	DDX_Control(pDX, IDC_EDIT_CARDNO, m_editCardNo);
	DDX_Control(pDX, IDC_EDIT_DEPOSIT, m_editDeposit);
}


BEGIN_MESSAGE_MAP(CUserOpenCard, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CUserOpenCard::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_READLICENSECARD, &CUserOpenCard::OnBnClickedButtonReadlicensecard)
	ON_BN_CLICKED(IDC_BUTTON_OPENCARD, &CUserOpenCard::OnBnClickedButtonOpencard)
	ON_BN_CLICKED(IDC_BUTTON_PRESTORE, &CUserOpenCard::OnBnClickedButtonPrestore)
	ON_NOTIFY(NM_CLICK, IDC_LIST_USER, &CUserOpenCard::OnNMClickListUser)
	ON_MESSAGE(WM_MSGRECVPRO, &CUserOpenCard::OnMsgrecvpro)
END_MESSAGE_MAP()


// CUserOpenCard 消息处理程序


BOOL CUserOpenCard::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitUI();
	g_sz_p_wnd[WND_USEROPENCARD] = this;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CUserOpenCard::OnBnClickedButtonSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strKeyword, strSql;
	m_editKeyword.GetWindowText(strKeyword);
	std::vector<CUser> vec_user;
	if (strKeyword.IsEmpty())
		strSql.Format(_T("SELECT * FROM User ORDER BY ID"));
	else
	{
		switch (m_comboCondition.GetCurSel())
		{
		case 0:strSql.Format(_T("SELECT * FROM User WHERE ID=%d ORDER BY ID"), _ttoi(strKeyword)); break;
		case 1:strSql.Format(_T("SELECT * FROM User WHERE Name LIKE '%%%s%%'ORDER BY ID"), strKeyword); break;
		case 2:strSql.Format(_T("SELECT * FROM User WHERE PhoneNo LIKE '%%%s%%'ORDER BY ID"), strKeyword); break;
		case 3:strSql.Format(_T("SELECT * FROM User WHERE Addr LIKE '%%%s%%'ORDER BY ID"), strKeyword); break;
		case 4:strSql.Format(_T("SELECT * FROM User WHERE License=%d ORDER BY ID"), _ttoi(strKeyword)); break;
		}
	}
	if (m_comboCondition.GetCurSel() != -1)
	{
		m_udb.Select(strSql, vec_user);
		UpdateList(vec_user);
	}
}


void CUserOpenCard::OnBnClickedButtonReadlicensecard()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strCard, strError;
	if (CCardOperator::ReadCard(strCard, strError, true))
	{
		int nCardNo, nLicenseCard, nCardType;
		CCardOperator::DecodeCard(strCard, nCardNo, nLicenseCard, nCardType);
		if (nCardType == CCardOperator::TYPE_LICENSE)
		{
			CCardOperator::DecodeLicenseCard(strCard, nCardNo, nLicenseCard, nCardType);
			SetDlgItemInt(IDC_EDIT_LICENSECARD, nLicenseCard);
		}
		else
			AfxMessageBox(_T("此卡非授权卡!"));
	}
	else
		AfxMessageBox(_T("读卡失败!\r\n") + strError);
}


void CUserOpenCard::OnBnClickedButtonOpencard()
{
	// TODO: 在此添加控件通知处理程序代码
	int nLicense, nLicenseCard;
	nLicense = GetDlgItemInt(IDC_EDIT_LICENSE);
	nLicenseCard = GetDlgItemInt(IDC_EDIT_LICENSECARD);
	if (!nLicenseCard)
	{
		AfxMessageBox(_T("请读取授权卡!"));
		return;
	}
	if (nLicense&&nLicense != nLicenseCard)
	{
		AfxMessageBox(_T("此授权卡非该客户所有!"));
		return;
	}
	nLicense = nLicenseCard;
	m_editCardNo.SetWindowText(_T(""));
	CString strCard, strCardNo, strError;
	if (CCardOperator::ReadCardNo(strCardNo, strError))
	{
		m_editCardNo.SetWindowText(strCardNo);
	}
	else
	{
		OnBnClickedButtonSearch();
		AfxMessageBox(_T("开卡失败!\r\n") + strError);
		return;
	}
	CString strDeposit;
	m_editDeposit.GetWindowText(strDeposit);
	int nIDCard;
	std::vector<CCard> vec_card;
	m_cdb.Select(_T("SELECT * FROM Card ORDER BY ID DESC LIMIT 1"), vec_card);
	if (vec_card.size() == 1)
		nIDCard = vec_card[0].GetID() + 1;
	else
		nIDCard = 1;
	m_SelectCard = CCard(nIDCard, m_SelectUser.GetID(), _ttoi(strCardNo), CCard::TYPE_RECHARGE, _T(""), _ttoi(strDeposit)*g_nINT100, g_employee.GetID(), _T(""), g_employee.GetID(), _T(""));
	m_SelectUser.SetLicense(nLicense);
	m_SelectUser.SetState(CUser::STATE_OPEN);
	strCard = CCardOperator::EncodeUserCard(m_SelectCard.GetNo(), m_SelectUser.GetLicense(), CCardOperator::TYPE_USER, m_SelectUser.GetID(), 0x00);
	if (CCardOperator::WriteCard(strCard, strError, true))
	{
		if (m_udb.UpdateOpenCard(m_SelectUser))
		{
			m_cdb.Insert(m_SelectCard);
			m_editDeposit.SetWindowText(_T(""));
			OnBnClickedButtonSearch();
			AfxMessageBox(_T("开卡成功!"));
			return;
		}
		else
			CCardOperator::ClearUserCard();
	}
	else
	{
		OnBnClickedButtonSearch();
		AfxMessageBox(_T("开卡失败!\r\n") + strError);
	}
}


void CUserOpenCard::OnBnClickedButtonPrestore()
{
	// TODO: 在此添加控件通知处理程序代码
	if (g_sz_p_wnd[WND_USERPRESTORE])
		g_sz_p_wnd[WND_USERPRESTORE]->PostMessage(WM_MSGRECVPRO, 0, MSGUSER_SHOWPRESTORE);
}


void CUserOpenCard::OnNMClickListUser(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	int & nItem = pNMItemActivate->iItem;
	if (nItem != -1)
	{
		int nID = _ttoi(m_listctrlUser.GetItemText(nItem, 0));
		InsertEditByID(nID);
	}
}


void CUserOpenCard::InitUI()
{
	// TODO: 在此处添加实现代码.
	CString sz_strCondition[] = { _T("客户编号"),_T("客户名称"), _T("客户手机号"), _T("客户地址"), _T("授权码") };
	for (int i = 0; i < sizeof(sz_strCondition) / sizeof(CString); ++i)
		m_comboCondition.AddString(sz_strCondition[i]);
	m_comboCondition.SetCurSel(0);
	const int nMax = 13;
	CRect rc;
	m_listctrlUser.GetClientRect(&rc);
	m_listctrlUser.SetExtendedStyle(m_listctrlUser.GetExtendedStyle() | LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listctrlUser.InsertColumn(0, _T("客户编号"), LVCFMT_LEFT, rc.Width() * 2 / nMax, 0);
	m_listctrlUser.InsertColumn(1, _T("客户名称"), LVCFMT_LEFT, rc.Width() * 2 / nMax, 1);
	m_listctrlUser.InsertColumn(2, _T("手机"), LVCFMT_LEFT, rc.Width() * 3 / nMax, 2);
	m_listctrlUser.InsertColumn(3, _T("地址"), LVCFMT_LEFT, rc.Width() * 3 / nMax, 3);
	m_listctrlUser.InsertColumn(4, _T("状态"), LVCFMT_LEFT, rc.Width() * 2 / nMax, 4);
}


void CUserOpenCard::UpdateList(const std::vector<CUser> & vec_user)
{
	m_listctrlUser.DeleteAllItems();
	int nSize = vec_user.size();
	for (int i = 0; i < nSize; ++i)
	{
		CString strID, strState;
		strID.Format(_T("%d"), vec_user[i].GetID());
		CString sz_strState[] = { _T("0-未开卡"),_T("1-已开卡"), _T("2-已退卡") };
		if (vec_user[i].GetState() < CUser::STATE_ERROR)
			strState = sz_strState[vec_user[i].GetState()];
		else
			strState = sz_strState[0];
		m_listctrlUser.InsertItem(i, strID);
		m_listctrlUser.SetItemText(i, 1, vec_user[i].GetName());
		m_listctrlUser.SetItemText(i, 2, vec_user[i].GetPhoneNo());
		m_listctrlUser.SetItemText(i, 3, vec_user[i].GetAddr());
		m_listctrlUser.SetItemText(i, 4, strState);
	}
}


void CUserOpenCard::InsertEdit(const CUser & user)
{
	CString str;
	str.Format(_T("%d"), user.GetID());
	m_editID.SetWindowText(str);
	m_editName.SetWindowText(user.GetName());
	m_editIDNo.SetWindowText(user.GetIDNo());
	m_editPhoneNo.SetWindowText(user.GetPhoneNo());
	m_editAddr.SetWindowText(user.GetAddr());
	if (user.GetLicense())
		str.Format(_T("%d"), user.GetLicense());
	else
		str.Empty();
	m_editLicense.SetWindowText(str);
	CString sz_strState[] = { _T("0-未开卡"),_T("1-已开卡"), _T("2-已退卡") };
	if (user.GetState() < CUser::STATE_ERROR)
		str = sz_strState[user.GetState()];
	else
		str = sz_strState[0];
	m_editState.SetWindowText(str);
}

void CUserOpenCard::InsertEditByID(int nID)
{
	std::vector<CUser> vec_user;
	CString strSql;
	strSql.Format(_T("SELECT * FROM User WHERE ID=%d ORDER BY ID"), nID);
	m_udb.Select(strSql, vec_user);
	if (vec_user.size() == 1)
	{
		InsertEdit(vec_user[0]);
		m_SelectUser = vec_user[0];
	}
}

afx_msg LRESULT CUserOpenCard::OnMsgrecvpro(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case MSGUSER_SHOWOPENCARD:
	{
		InsertEditByID(wParam);
		ShowWindow(SW_SHOW);
	}
	break;
	}
	return 0;
}
