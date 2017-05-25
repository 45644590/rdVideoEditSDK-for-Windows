#pragma once
#include "afxwin.h"

#include "XPKCommon.h"
#include "afxcmn.h"

// CAddApngDialg �Ի���

class CAddApngDialg : public CDialog
{
	DECLARE_DYNAMIC(CAddApngDialg)

public:
	CAddApngDialg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddApngDialg();

// �Ի�������
	enum { IDD = IDD_APNG_RGBA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedApngFinish();
	afx_msg void OnBnClickedRadioSnow();
	afx_msg void OnBnClickedRadioFish();

	afx_msg void OnBnClickedClearAllApng();
	afx_msg void OnBnClickedDeleteApng();
	afx_msg void OnBnClickedRadioBlackLine();
	afx_msg void OnBnClickedRadioLove();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);

	virtual BOOL OnInitDialog();

	int setTotalTime(int64_t totalTime);
	char* GetResourcePath(char* name);
	int refreshUI();
	

	char szPath[MAX_PATH];
	CListCtrl listCtrl;
	int apng_index;		//����id
	struct XPK_APNG_RGB_OVERLAY_LIST *pApng;
};
