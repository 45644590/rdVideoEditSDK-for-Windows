#pragma once


// CLoadMedialog �Ի���

class CLoadMedialog : public CDialog
{
	DECLARE_DYNAMIC(CLoadMedialog)

public:
	CLoadMedialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLoadMedialog();

	int loadMediaFinish();
	int loadMediaing();
// �Ի�������
	enum { IDD = IDD_LOADIMAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
//	afx_msg void OnTimer(UINT_PTR nIDEvent);

	

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();

	int loaded;
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
};
