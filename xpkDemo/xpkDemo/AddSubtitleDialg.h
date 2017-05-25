#pragma once
#include "afxwin.h"
#include "XPKCommon.h"
#include "afxcmn.h"
// CAddSubtitleDialg �Ի���



class CAddSubtitleDialg : public CDialog
{
	DECLARE_DYNAMIC(CAddSubtitleDialg)

public:
	CAddSubtitleDialg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddSubtitleDialg();

// �Ի�������
	enum { IDD = IDD_SUBTITLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedSubtitleFinish();
	afx_msg void OnBnClickedBold();
	afx_msg void OnBnClickedItalics();
	afx_msg void OnBnClickedUnderline();
	afx_msg void OnBnClickedDeleteSubtitle();
	afx_msg void OnBnClickedClearAllSubtitle();

	afx_msg void OnBnClickedFontColor();
	void set_DropDownHight(CComboBox*box,UINT LinesToDisplay);
	void set_DropDownWidth(CComboBox*box,UINT nWidth );
	int setTotalTime(int64_t totalTime);
	
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	int refreshUI();

	//��ɫ
	int m_ColorRed;
	int m_ColorGreen;
	int m_ColorBlue;

	//����
	int m_bold;
	int m_underline;
	int m_italics;
	//��Ļ
	char szText[2048];

	//����
	RECTF fontRect;
	FONT_PEP font;
	RGBQUAD clr;

	CComboBox m_fontSizeCtrl;
	CComboBox m_fontNameCtrl;

	int m_fontNameNumber;
	
	CListCtrl listCtrl;
	int subtitle_id;
	
	XPK_SUBTITLE_LIST* pSubtitle;
	
};
