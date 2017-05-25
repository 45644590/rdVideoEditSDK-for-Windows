#pragma once

#include "Resource.h"
#include "windows.h"
#include "showSnapshot.h"
#include "AdvancedSettingPictureDialog.h"
#include "AdvancedSettingMediaDialog.h"
#include "setButton.h"
#include "XPKCommon.h"
#include "XPKCombobox.h"
#include "LoadMedialog.h"



#define  DISPLAY_WIDTH 400
#define  DISPLAY_HEIGHT 224

// #define  DISPLAY_WIDTH 400
// #define  DISPLAY_HEIGHT 100



extern struct XPK_MEDIA_LIST;
// CXPKAddMedia �Ի���

class CXPKAddMedia : public CPropertyPage
{
	DECLARE_DYNAMIC(CXPKAddMedia)

public:
	CXPKAddMedia();
	virtual ~CXPKAddMedia();

// �Ի�������
	enum { IDD = IDD_XPKADDMEDIA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
	afx_msg void OnBnClickedAddFile();
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedClearallmedia();	//ɾ�������ļ�
	afx_msg void OnBnClickedDeletemedia();		//ɾ��ָ���ļ�
	virtual LRESULT OnWizardNext();

	int addMedia(char* path);
	int showAdvanceDlg(int id);
	int addSnapshot(char* path,int widthSrc,int heightSrc,int width,int height,
		int64_t end_time,enum MEDIA_TYPES type);
	int clearAllMedia();
	int refreshSnapshot();
	int transitionChange(int change);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	int resetMediaProperty();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	int display(CShowSnapshot* snapshot);		//��ʾ

	
	struct Video_Stream_Info m_video_info;		//ý����Ƶ����
	struct Audio_Stream_Info m_audio_info;		//ý����Ƶ����

	int m_mediaChange;							//ý�������Ƿ����ı�
	XPK_MEDIA_LIST* m_pMediaList;
	int m_resouce_id;							//��Դid

	float m_display_zoom;

	CShowSnapshot* curSnapshot;
	Handle hDisplayThread;
	Handle displayEvent;
	Handle hLoadMediaThread;
	Handle loadEvent;
	CLoadMedialog *pLoadMediaDlg;
	CFileDialog* pFileDlg;
	afx_msg void OnClose();
};
