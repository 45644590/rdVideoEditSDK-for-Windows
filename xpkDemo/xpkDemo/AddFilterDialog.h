#pragma once
#include "afxcmn.h"
#include "XPKSlideCommon.h"
#include "XPKCommon.h"


// CAddFilterDialog �Ի���

class CAddFilterDialog : public CDialog
{
	DECLARE_DYNAMIC(CAddFilterDialog)

public:
	CAddFilterDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddFilterDialog();

// �Ի�������
	enum { IDD = IDD_FILTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CListCtrl listCtrl;

	char* GetResourcePath(char* name);
	afx_msg void OnBnClickedLomo();
	afx_msg void OnBnClickedFinish();
	afx_msg void OnBnClickedSunsetGlow();
	afx_msg void OnBnClickedHawaiianLslands();
	afx_msg void OnBnClickedOvercast();
	afx_msg void OnBnClickedDusk();
	afx_msg void OnBnClickedFleetingTime();
	afx_msg void OnBnClickedMigratoryBird();
	afx_msg void OnBnClickedCloud();
	char* findFile(int id) ;
	int refreshUI(int ctrl_id,int filter_id);
	int reSetFilter(int ctrl_id);
	afx_msg void OnBnClickedDelete();
	int OnClearAllFilter();

	int filter_count;
};
