// LoadMedialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "xpkDemo.h"
#include "LoadMedialog.h"


// CLoadMedialog �Ի���

IMPLEMENT_DYNAMIC(CLoadMedialog, CDialog)

CLoadMedialog::CLoadMedialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLoadMedialog::IDD, pParent)
{

	loaded = 0;
	 
}

CLoadMedialog::~CLoadMedialog()
{
	loaded = 0;
}

void CLoadMedialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLoadMedialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CLoadMedialog::OnBnClickedButton1)
//	ON_WM_TIMER()
ON_WM_TIMER()
END_MESSAGE_MAP()


// CLoadMedialog ��Ϣ�������



int CLoadMedialog::loadMediaing()
{
	DoModal();
	return 0;
}
int CLoadMedialog::loadMediaFinish()
{
	loaded = 1;
	return 0;
}
void CLoadMedialog::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	loaded = 1;
	
}


void CLoadMedialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (loaded)
	{
		EndDialog(IDCANCEL);
		loaded = 0;
	
		return ;
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CLoadMedialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetTimer(1,50,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
