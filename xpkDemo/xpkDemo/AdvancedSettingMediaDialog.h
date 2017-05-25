#pragma once



#include "afxwin.h"
#include "XPKCombobox.h"
#include "XPKCommon.h"
#include "XPKAddMedia.h"

// CAdvancedSettingMediaDialog �Ի���

class CAdvancedSettingMediaDialog : public CDialog
{
	DECLARE_DYNAMIC(CAdvancedSettingMediaDialog)

public:
	CAdvancedSettingMediaDialog(int id ,int imageWidth,int imageHeight,enum MEDIA_TYPES type,int64_t totalTime,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAdvancedSettingMediaDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_MEDIA_PROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	double getMediaSpeed();
	int64_t getMediaCutStartTime();
	int64_t getMediaCutEndTime();
	int64_t getMediaInalienableTime();

	afx_msg void OnBnClickedCheck1();

	int getDynamicClipRect(CString str,RECTF* rect);
	XPK_DYNAMAIC_CLIP_LIST* getDynamicClipList();
	int UpdateClipList();
	afx_msg void OnEnChangeMediaClipEndTime1();
	afx_msg void OnEnChangeMediaClipEndTime2();
	int addClipList(RECTF rect,int64_t time_start,int64_t time_end);

	int refreshUI();
	int picturePropertyChange( int change);
	int		getMediaRotateAngle();
	int		getMediaFileType();
	RECTF   getMediaClip();

	int m_resouceId;
	RECTF rtClip;				//�ü�����
	int propertyChange ;		//ý�������Ƿ����ı�
	int64_t m_media_total_time;	//ý�����ʱ��
	int angle;					//��ת�Ƕ�
	int m_media_height;			//ý��Ŀ��
	int m_media_width;			//ý��Ŀ��
	int64_t m_cut_start_time;	//ý��Ľ�ȡ��ʼʱ��
	int64_t m_cut_end_time;		//ý��Ľ�ȡ����ʱ��

	CXPKCombobox *m_pRorateBox;
	CXPKCombobox *m_pFlipBox;
	CXPKCombobox *m_pClipBox;
	CXPKCombobox *m_pSpeedBox;
	CButton m_useDynamicClip;

	//��̬�ü�
	XPK_DYNAMAIC_CLIP_LIST* pClipList;
	int m_init;
	CXPKAddMedia *m_pParent;	//������
	
};
