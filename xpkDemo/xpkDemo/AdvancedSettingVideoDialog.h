#pragma once


// CAdvancedSettingVideoDialog �Ի���

class CAdvancedSettingVideoDialog : public CDialog
{
	DECLARE_DYNAMIC(CAdvancedSettingVideoDialog)

public:
	CAdvancedSettingVideoDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAdvancedSettingVideoDialog();

// �Ի�������
	enum { IDD = IDD_ADVANCEDSETTINGVIDEODIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
