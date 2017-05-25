#pragma once

#include "XPKCommon.h"
#include "XPKCombobox.h"
#include "afxcmn.h"
// CAddMusicDialg �Ի���

class CAddMusicDialg : public CDialog
{
	DECLARE_DYNAMIC(CAddMusicDialg)

public:
	CAddMusicDialg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddMusicDialg();

	void set_DropDownHight(CComboBox*box,UINT LinesToDisplay);
	void set_DropDownWidth(CComboBox*box,UINT nWidth );
// �Ի�������
	enum { IDD = IDD_MUSIC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	CXPKCombobox* m_music_factor;
	CXPKCombobox* m_music_fade_in;
	CXPKCombobox* m_music_fade_out;
public:

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedChooseMusicFinish();
	afx_msg void OnBnClickedChooseMusic();
	afx_msg void OnBnClickedRadioMOON();
	afx_msg void OnBnClickedRadioFISH();
	afx_msg void OnBnClickedClearAllMusic();
	afx_msg void OnBnClickedDeleteMusic();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);

	char* GetResourcePath(char* name);
	int setTotalTime(int64_t totalTime);
	int refreshUI();
	

	XPK_MUSIC_LIST* pMusic;
	int music_id;
	CListCtrl listCtrl;
};
