#pragma once
#include "afxwin.h"
#include "XPKCombobox.h"
#include "XPKCommon.h"
#include "XPKEditCtrlEx.h"


// CAdvancedSettingPictureDialog �Ի���

class CXPKAddMedia;

class CAdvancedSettingPictureDialog : public CDialog
{
	DECLARE_DYNAMIC(CAdvancedSettingPictureDialog)

public:
	CAdvancedSettingPictureDialog(int id ,int imageWidth,int imageHeight,enum MEDIA_TYPES type,int64_t totalTime,CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAdvancedSettingPictureDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_PICTURE_PROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	void set_DropDownHight(CXPKCombobox*box,UINT LinesToDisplay);
	void set_DropDownWidth(CXPKCombobox*box,UINT nWidth );
	int refreshUI();	
	int picturePropertyChange(int change);
	double getMediaSpeed();
	int64_t getMediaCutStartTime();
	int64_t getMediaCutEndTime();
	int64_t getMediaInalienableTime();
	int		getMediaRotateAngle();
	int		getMediaFileType();
	RECTF   getMediaClip();

	int m_resouceId;				

	CXPKCombobox *m_pSpeedBox;
	CXPKCombobox *m_pRorateBox;
	CXPKCombobox *m_pFlipBox;
	CXPKCombobox *m_pClipBox;
	
	int m_media_width;						//ý����
	int m_media_height;						//ý����
	enum MEDIA_TYPES m_media_type;			//ý������

	RECTF rtClip;							//�ü�����
	int angle;								//��ת�Ƕ�
	int propertyChange;						//ý�������Ƿ����ı�
	int64_t m_media_total_time;				//ý����ʱ��

	CXPKAddMedia *m_pParent;
};
