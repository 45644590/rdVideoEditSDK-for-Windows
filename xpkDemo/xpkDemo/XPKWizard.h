#pragma once


#include "XPKPreview.h"
#include "XPKEdit.h"
#include "XPKAddMedia.h"
#include "XPKCommon.h"
// XPKWizard


#define WM_RESIZEPAGE WM_USER + 111

typedef struct XPK_MEDIA_LIST
{
	char path[MAX_PATH];
	Handle handle;
	int widthSrc;				//ý�����ʵ��
	int heightSrc;				//ý�����ʵ��
	enum MEDIA_TYPES type;		//ý������
	int width;					//����ͼ��
	int height;					//����ͼ��
	unsigned char* pImage;		//����ͼ����
	int64_t total_time;			//ý�����ʱ��
	int id;						//��ԴID

	//����ͼ��ʾ
	CShowSnapshot* snapshot;	//��ʾ����ؼ�
	RECT snapshot_rect;			//��ʾ����
	
	CSetButton* button;			//�༭��ť
	RECT button_rect;
	
	CAdvancedSettingPictureDialog* pPictureDlg;		//ͼƬ�༭�Ի���
	CAdvancedSettingMediaDialog* pMediaDlg;			//��Ƶ�༭�Ի���
	
	CXPKCombobox* transition_box;			//ת��combobox
	RECT transition_box_rect;

	XPK_MEDIA_LIST* next;
};



class XPKWizard : public CPropertySheet
{
	DECLARE_DYNAMIC(XPKWizard)

public:
	XPKWizard(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	XPKWizard(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~XPKWizard();

protected:
	DECLARE_MESSAGE_MAP()

	CXPKEdit m_editDlg;
	CXPKPreview m_previewDlg;
	CXPKAddMedia m_addDlg;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnResizePage(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	int init();

	int clearAllFilter();
	char* GetResourcePath(char* name);
	int getClipRect(XPK_MEDIA_LIST* pList,RECTF* rtClip);
	int adddubmusic();
	int addSubtitle();
	int addApngList();

	int64_t total_time;				//��ʱ��
	Handle hSlide;
	XPK_MEDIA_LIST* m_pMediaList;	//ý������
	XPK_FILTER_LIST* m_pFilterList;	//�˾�����
	char szFilePath[MAX_PATH];
	XPK_STATU statu ;				//��ǰ״̬

	float m_button_next_zoom;
	float m_button_back_zoom;
	float m_button_finish_zoom;

	afx_msg void OnClose();
};


