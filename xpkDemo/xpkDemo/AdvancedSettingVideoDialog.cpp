// AdvancedSettingVideoDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "xpkDemo.h"
#include "AdvancedSettingVideoDialog.h"


// CAdvancedSettingVideoDialog �Ի���

IMPLEMENT_DYNAMIC(CAdvancedSettingVideoDialog, CDialog)

CAdvancedSettingVideoDialog::CAdvancedSettingVideoDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAdvancedSettingVideoDialog::IDD, pParent)
{
	
}

CAdvancedSettingVideoDialog::~CAdvancedSettingVideoDialog()
{
}

void CAdvancedSettingVideoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAdvancedSettingVideoDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CAdvancedSettingVideoDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CAdvancedSettingVideoDialog ��Ϣ�������

void CAdvancedSettingVideoDialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}
