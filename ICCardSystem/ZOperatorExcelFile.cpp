#include "stdafx.h"
#include "ZOperatorExcelFile.h"

COleVariant ZOperatorExcelFile::m_covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

ZOperatorExcelFile::ZOperatorExcelFile()
	: m_strFileName(_T(""))
	, m_nIsInitCount(0)
	, m_nIsPreloadSucceed(FALSE)
{
}


ZOperatorExcelFile::~ZOperatorExcelFile()
{
	CloseExcelFile();
}


void ZOperatorExcelFile::InitOle()
{
	CoInitialize(NULL);
}


void ZOperatorExcelFile::UninitOle()
{
	CoUninitialize();
}


BOOL ZOperatorExcelFile::InitExcel(CString * p_strError)
{
	//����Excel ������(����Excel) 
	if (!m_nIsInitCount)
	{
		if (!m_excelapplication.CreateDispatch(_T("Excel.Application"), NULL))
		{
			if (p_strError)
				*p_strError = _T("����Excel����ʧ��,�����û�а�װEXCEL������!");
			return FALSE;
		}
		m_excelapplication.put_DisplayAlerts(FALSE);
		++m_nIsInitCount;
	}
	else
		++m_nIsInitCount;
	return TRUE;
}


void ZOperatorExcelFile::ReleaseExcel()
{
	if (m_nIsInitCount)
	{
		--m_nIsInitCount;
		if (!m_nIsInitCount)
		{
			m_excelapplication.Quit();
			m_excelapplication.ReleaseDispatch();
			m_excelapplication = NULL;
		}
	}
}


BOOL ZOperatorExcelFile::OpenExcelFile(const CString & strFileName)
{
	CFileFind finder;
	if (!finder.FindFile(strFileName))
	{
		finder.Close();
		return FALSE;
	}
	//�ȹر�
	CloseExcelFile();
	//����ģ���ļ��������ĵ� 
	m_excelbooks.AttachDispatch(m_excelapplication.get_Workbooks(), TRUE);
	LPDISPATCH lpDis = NULL;
	lpDis = m_excelbooks.Add(COleVariant(strFileName));
	if (lpDis)
	{
		m_excelworkbook.AttachDispatch(lpDis);
		//�õ�Worksheets 
		m_excelsheets.AttachDispatch(m_excelworkbook.get_Worksheets(), TRUE);
		//��¼�򿪵��ļ�����
		m_strFileName = strFileName;
		return TRUE;
	}
	return FALSE;
}


void ZOperatorExcelFile::CloseExcelFile(BOOL nIsSave)
{
	//����Ѿ��򿪣��ر��ļ�
	if (!m_strFileName.IsEmpty())
	{
		//�������,�����û�����,���û��Լ��棬����Լ�SAVE�������Ī���ĵȴ�
		if (nIsSave)
			ShowInExcel(TRUE);
		else
		{
			m_excelworkbook.Close(COleVariant(short(FALSE)), COleVariant(m_strFileName), m_covOptional);
			m_excelbooks.Close();
		}
		//���ļ����������
		m_strFileName.Empty();
	}
	m_excelsheets.ReleaseDispatch();
	m_excelworksheet.ReleaseDispatch();
	m_excelrange.ReleaseDispatch();
	m_excelworkbook.ReleaseDispatch();
	m_excelbooks.ReleaseDispatch();
}


void ZOperatorExcelFile::SaveasXSLFile(const CString & strFilePath)
{
	m_excelworkbook.SaveAs(COleVariant(strFilePath),
		m_covOptional,
		m_covOptional,
		m_covOptional,
		m_covOptional,
		m_covOptional,
		0,
		m_covOptional,
		m_covOptional,
		m_covOptional,
		m_covOptional,
		m_covOptional);
}


long ZOperatorExcelFile::GetSheetCount()
{
	return m_excelsheets.get_Count();
}


CString ZOperatorExcelFile::GetSheetName(long lSheetIndex)
{
	++lSheetIndex;
	CWorksheet sheet;
	sheet.AttachDispatch(m_excelsheets.get_Item(COleVariant(lSheetIndex)), TRUE);
	CString strName = sheet.get_Name();
	sheet.ReleaseDispatch();
	return strName;
}


BOOL ZOperatorExcelFile::LoadSheet(long lSheetIndex, BOOL nIsPreload)
{
	++lSheetIndex;
	LPDISPATCH lpDis = NULL;
	m_excelrange.ReleaseDispatch();
	m_excelworksheet.ReleaseDispatch();
	lpDis = m_excelsheets.get_Item(COleVariant(lSheetIndex));
	if (lpDis)
	{
		m_excelworksheet.AttachDispatch(lpDis, true);
		m_excelrange.AttachDispatch(m_excelworksheet.get_Cells(), true);
	}
	else
		return FALSE;
	if (nIsPreload)
		m_nIsPreloadSucceed=PreloadSheet();
	return TRUE;
}


BOOL ZOperatorExcelFile::LoadSheet(const CString & strSheet, BOOL nIsPreload)
{
	LPDISPATCH lpDis = NULL;
	m_excelrange.ReleaseDispatch();
	m_excelworksheet.ReleaseDispatch();
	lpDis = m_excelsheets.get_Item(COleVariant(strSheet));
	if (lpDis)
	{
		m_excelworksheet.AttachDispatch(lpDis, true);
		m_excelrange.AttachDispatch(m_excelworksheet.get_Cells(), true);

	}
	else
		return FALSE;
	if (nIsPreload)
		m_nIsPreloadSucceed=PreloadSheet();
	return TRUE;
}


long ZOperatorExcelFile::GetColumnCount()
{
	CRange range;
	CRange usedRange;
	usedRange.AttachDispatch(m_excelworksheet.get_UsedRange(), TRUE);
	range.AttachDispatch(usedRange.get_Columns(), TRUE);
	long l = range.get_Count();
	usedRange.ReleaseDispatch();
	range.ReleaseDispatch();
	return l;
}


long ZOperatorExcelFile::GetRowCount()
{
	CRange range;
	CRange usedRange;
	usedRange.AttachDispatch(m_excelworksheet.get_UsedRange(), TRUE);
	range.AttachDispatch(usedRange.get_Rows(), TRUE);
	long l = range.get_Count();
	usedRange.ReleaseDispatch();
	range.ReleaseDispatch();
	return l;
}


BOOL ZOperatorExcelFile::IsCellString(long lRow, long lColumn)
{
	++lRow;
	++lColumn;
	CRange range;
	range.AttachDispatch(m_excelrange.get_Item(COleVariant(lRow), COleVariant(lColumn)).pdispVal, TRUE);
	COleVariant vResult = range.get_Value2();
	if (vResult.vt == VT_BSTR)
		return TRUE;
	return FALSE;
}


BOOL ZOperatorExcelFile::IsCellInt(long lRow, long lColumn)
{
	++lRow;
	++lColumn;
	CRange range;
	range.AttachDispatch(m_excelrange.get_Item(COleVariant(lRow), COleVariant(lColumn)).pdispVal, TRUE);
	COleVariant vResult = range.get_Value2();
	if (vResult.vt == VT_INT || vResult.vt == VT_R8)
		return TRUE;
	return FALSE;
}


CString ZOperatorExcelFile::GetCellString(long lRow, long lColumn)
{
	++lRow;
	++lColumn;
	COleVariant vResult;
	CString str;
	if (!m_nIsPreloadSucceed)
	{
		CRange range;
		range.AttachDispatch(m_excelrange.get_Item(COleVariant((long)lRow), COleVariant((long)lColumn)).pdispVal, TRUE);
		vResult = range.get_Value2();
		range.ReleaseDispatch();
	}
	else
	{
		long sz_lReadAddr[2];
		VARIANT val;
		sz_lReadAddr[0] = lRow;
		sz_lReadAddr[1] = lColumn;
		m_olesafearray.GetElement(sz_lReadAddr, &val);
		vResult = val;
	}
	switch (vResult.vt)
	{
	case VT_BSTR:str = vResult.bstrVal; break;
	case VT_INT:str.Format(_T("%d"), vResult.pintVal); break;//����
	case VT_R8:str.Format(_T("%0.0f"), vResult.dblVal); break;//8�ֽڵ����� 
	case VT_DATE://ʱ���ʽ
	{
		SYSTEMTIME st;
		VariantTimeToSystemTime(vResult.date, &st);
		CTime tm(st);
		str = tm.Format("%Y-%m-%d");
	}
	break;
	case VT_EMPTY:str.Empty(); break;
	}
	return str;
}


double ZOperatorExcelFile::GetCellDouble(long lRow, long lColumn)
{
	++lRow;
	++lColumn;
	double d = 0;
	COleVariant vresult;
	if (!m_nIsPreloadSucceed)
	{
		CRange range;
		range.AttachDispatch(m_excelrange.get_Item(COleVariant((long)lRow), COleVariant((long)lColumn)).pdispVal, TRUE);
		vresult = range.get_Value2();
		range.ReleaseDispatch();
	}
	else
	{
		long sz_lReadAddr[2];
		VARIANT val;
		sz_lReadAddr[0] = lRow;
		sz_lReadAddr[1] = lColumn;
		m_olesafearray.GetElement(sz_lReadAddr, &val);
		vresult = val;
	}
	if (vresult.vt == VT_R8)
	{
		d = vresult.dblVal;
	}
	return d;
}


int ZOperatorExcelFile::GetCellInt(long lRow, long lColumn)
{
	++lRow;
	++lColumn;
	int n;
	COleVariant vresult;
	if (!m_nIsPreloadSucceed)
	{
		CRange range;
		range.AttachDispatch(m_excelrange.get_Item(COleVariant((long)lRow), COleVariant((long)lColumn)).pdispVal, TRUE);
		vresult = range.get_Value2();
		range.ReleaseDispatch();
	}
	else
	{
		long sz_lReadAddr[2];
		VARIANT val;
		sz_lReadAddr[0] = lRow;
		sz_lReadAddr[1] = lColumn;
		m_olesafearray.GetElement(sz_lReadAddr, &val);
		vresult = val;
	}
	n = static_cast<int>(vresult.dblVal);
	return n;
}


void ZOperatorExcelFile::SetCellString(long lRow, long lColumn, const CString & strData)
{
	COleVariant new_value(strData);
	CRange start_range = m_excelworksheet.get_Range(COleVariant(_T("A1")), m_covOptional);
	CRange write_range = start_range.get_Offset(COleVariant(lRow), COleVariant(lColumn));
	write_range.put_Value2(new_value);
	start_range.ReleaseDispatch();
	write_range.ReleaseDispatch();
}


void ZOperatorExcelFile::SetCellInt(long lRow, long lColumn, int nData)
{
	COleVariant new_value((long)nData);
	CRange start_range = m_excelworksheet.get_Range(COleVariant(_T("A1")), m_covOptional);
	CRange write_range = start_range.get_Offset(COleVariant(lRow), COleVariant(lColumn));
	write_range.put_Value2(new_value);
	start_range.ReleaseDispatch();
	write_range.ReleaseDispatch();
}


void ZOperatorExcelFile::ShowInExcel(BOOL nIsShow)
{
	m_excelapplication.put_Visible(nIsShow);
	m_excelapplication.put_UserControl(nIsShow);
}


CString ZOperatorExcelFile::GetFileName()
{
	return m_strFileName;
}


CString ZOperatorExcelFile::GetLoadSheetName()
{
	return m_excelworksheet.get_Name();
}


char * ZOperatorExcelFile::GetColumnName(long lColumn)
{
	static char sz_ColumnName[64];
	size_t nLen = 0;
	++lColumn;
	while (lColumn > 0)
	{
		int nNumData = lColumn % 26;
		lColumn /= 26;
		if (nNumData == 0)
		{
			nNumData = 26;
			lColumn--;
		}
		sz_ColumnName[nLen] = (char)((nNumData - 1) + 'A');
		nLen++;
	}
	sz_ColumnName[nLen] = '\0';
	_strrev(sz_ColumnName);//��ת
	return sz_ColumnName;
}


BOOL ZOperatorExcelFile::PreloadSheet()
{
	CRange used_range;
	used_range = m_excelworksheet.get_UsedRange();
	VARIANT ret_ary = used_range.get_Value2();
	if (!(ret_ary.vt & VT_ARRAY))
	{
		return FALSE;
	}
	m_olesafearray.Clear();
	m_olesafearray.Attach(ret_ary);
	return TRUE;
}