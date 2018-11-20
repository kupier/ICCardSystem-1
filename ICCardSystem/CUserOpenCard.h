#pragma once


// CUserOpenCard 对话框

class CUserOpenCard : public CDialogEx
{
	DECLARE_DYNAMIC(CUserOpenCard)

public:
	CUserOpenCard(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CUserOpenCard();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USEROPENCARD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_comboCondition;
	CListCtrl m_listctrlUser;
	CEdit m_editKeyword;
	CEdit m_editID;
	CEdit m_editName;
	CEdit m_editIDNo;
	CEdit m_editPhoneNo;
	CEdit m_editAddr;
	CEdit m_editLicense;
	CEdit m_editState;
	CEdit m_editCardNo;
	CEdit m_editDeposit;
	CUser m_SelectUser;
	CUserDB m_udb;
	CCard m_SelectCard;
	CCardDB m_cdb;
	CReturnDepositRecordDB m_rdrdb;
protected:
	afx_msg LRESULT OnMsgrecvpro(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedButtonReadlicensecard();
	afx_msg void OnBnClickedButtonOpencard();
	afx_msg void OnBnClickedButtonPrestore();
	afx_msg void OnNMClickListUser(NMHDR *pNMHDR, LRESULT *pResult);
	void InitUI();
	void UpdateList(const std::vector<CUser> & vec_user);
	void InsertEdit(const CUser & user);
	void InsertEditByID(int nID);
};
