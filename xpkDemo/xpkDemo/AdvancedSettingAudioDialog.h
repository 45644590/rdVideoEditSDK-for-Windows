#pragma once
#include "afxwin.h"
#include "XPKCombobox.h"
#include "XPKCommon.h"


// CAdvancedSettingAudioDialog �Ի���

class CAdvancedSettingAudioDialog : public CDialog
{
	DECLARE_DYNAMIC(CAdvancedSettingAudioDialog)

public:
	CAdvancedSettingAudioDialog(int id,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAdvancedSettingAudioDialog();

// �Ի�������


public:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void set_DropDownWidth(CXPKCombobox*box,UINT nWidth );
	void set_DropDownHight(CXPKCombobox*box,UINT LinesToDisplay);

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	int musicPropertyChange();
	int64_t getMusicCutStartTime();
	int64_t getMusicCutEndTime();

	DECLARE_MESSAGE_MAP()

public:
	int refreshUI();

	int m_resouceId;			//��Դid
	CXPKCombobox* m_music_factor;
	CXPKCombobox* m_music_fade_in;
	CXPKCombobox* m_music_fade_out;
	CXPKCombobox* m_music_speed;

	//
	int m_change;
	
};
