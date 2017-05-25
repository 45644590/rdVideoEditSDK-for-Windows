// XPKAddMedia.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "xpkDemo.h"
#include "XPKAddMedia.h"
#include "XPKWizard.h"

// CXPKAddMedia �Ի���

IMPLEMENT_DYNAMIC(CXPKAddMedia, CPropertyPage)

CXPKAddMedia::CXPKAddMedia()
	: CPropertyPage(CXPKAddMedia::IDD)
{
	m_resouce_id = 0; 
	m_mediaChange = 0;
	m_pMediaList = NULL;
	m_display_zoom = 0.0;
	hDisplayThread = NULL;
	curSnapshot = NULL;
	pFileDlg = NULL;
	hLoadMediaThread = NULL;
	loadEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	displayEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	memset(&m_audio_info,0,sizeof(Audio_Stream_Info));
	memset(&m_video_info,0,sizeof(Video_Stream_Info));
	pLoadMediaDlg = new CLoadMedialog;
	
	
	
}

CXPKAddMedia::~CXPKAddMedia()
{
	clearAllMedia();

	//�ر��߳�
	if (hDisplayThread)
	{
		TerminateThread(hDisplayThread,0);
		WaitForSingleObject(hDisplayThread,INFINITE );
	}
	if (hLoadMediaThread)
	{
		TerminateThread(hLoadMediaThread,0);
		WaitForSingleObject(hLoadMediaThread,INFINITE );
	}
}

void CXPKAddMedia::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CXPKAddMedia, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, &CXPKAddMedia::OnBnClickedAddFile)
	ON_BN_CLICKED(IDC_CLEARALLMEDIA, &CXPKAddMedia::OnBnClickedClearallmedia)
	ON_BN_CLICKED(IDC_DELETEMEDIA, &CXPKAddMedia::OnBnClickedDeletemedia)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CXPKAddMedia ��Ϣ�������

BOOL CXPKAddMedia::OnSetActive()
{
	// TODO: �ڴ����ר�ô����/����û���
	RECT dlg_rect = {0};
	RECT page_rect = {0};

	CPropertySheet *pSheet=(CPropertySheet *)GetParent();
	ASSERT_KINDOF(CPropertySheet,pSheet);
	pSheet->SetWizardButtons(PSWIZB_NEXT);//ID_WIZNEXT
	//pSheet->GetDlgItem(ID_WIZNEXT)->SetWindowText("next");
	//pSheet->GetDlgItem(ID_WIZBACK)->SetWindowText("back");
	pSheet->GetDlgItem(ID_WIZBACK)->ShowWindow(SW_HIDE);

	pSheet->GetClientRect(&dlg_rect);
	page_rect = dlg_rect;
	page_rect.bottom = page_rect.bottom - 50;
	MoveWindow(&page_rect);

	return CPropertyPage::OnSetActive();
}

int __stdcall play_finish(void* pUser)
{
	CXPKAddMedia* pDlg = (CXPKAddMedia*)pUser;

	return 1;
}


DWORD WINAPI loadMediaThreadProc(LPVOID lparams)
{
	CXPKAddMedia* pDlg =(CXPKAddMedia * )lparams ;
	char path[1024] = {0};
	DWORD nTime = 0;//GetTickCount();

	
	while(WAIT_OBJECT_0 == WaitForSingleObject(pDlg->loadEvent,INFINITE))
	{
		POSITION pos = pDlg->pFileDlg->GetStartPosition();

		while (pos != NULL)
		{
			//ȡ���ļ�·��
			
			strcpy(path,pDlg->pFileDlg->GetNextPathName(pos));
			OutputDebugString("\n");
			OutputDebugString(path);
			if(pDlg->addMedia(path)<=0)
			{
				pDlg->pLoadMediaDlg->loadMediaFinish();
				return 1;
			}
			Sleep(2000);
		}
		nTime = GetTickCount() - nTime;
		pDlg->pLoadMediaDlg->loadMediaFinish();
	}

	return 1;
}
DWORD WINAPI displayThreadProc(LPVOID lparams)
{
	CXPKAddMedia* pDlg =(CXPKAddMedia * )lparams ;
	//����ý�������ҵ�ѡ���������
	int angle = 0;
	int flip = 0;
	double speed = 0;
	RECTF rtClip = {0};
	RECT showRect = {0};
	int ret = 0;
	char buf[1024];
	char buf_temp[1024];
	int64_t start_time = 0;
	int64_t end_time = 0;
	Handle hMedia = NULL;
	int64_t timeline_end = 0;
	XPK_DYNAMAIC_CLIP_LIST *clip_list  = NULL;
	CWnd *wnd=pDlg->GetDlgItem(IDC_MEDIA_DISPLAY);
	XPK_MEDIA_LIST* list = NULL ;
	DWORD nTime = 0;
	DWORD sumTime = 0;
	XPKWizard* pSheet = reinterpret_cast<XPKWizard*>(pDlg->GetParent());

	
	while(WAIT_OBJECT_0 == WaitForSingleObject(pDlg->displayEvent,INFINITE))
	{
		
		OutputDebugString("addmedia display IN !\n");
		if (!pDlg->m_pMediaList)
		{
			AfxMessageBox("û��ý��..");
			return -__LINE__;
		}
		
		sumTime = GetTickCount();
		nTime = GetTickCount();
		list = pDlg->m_pMediaList;
		//�ȱ�������ֹͣ���ڲ��ŵ�ý��
		while(list)
		{
			if (pDlg->curSnapshot &&pDlg->curSnapshot != list->snapshot && list->snapshot->isSelected())
				list->snapshot->cancelSelected();
			if (list->handle)
			{
				xpkPreviewStop(list->handle);
				xpkCloseMediaEditor(list->handle);
				list->handle = NULL;
			}
			list = list->next;
		}
		nTime = GetTickCount() - nTime;
		sprintf(buf,"XPK CLOSE Media TIME: %d ",nTime);

		
		//�ҵ�ѡ�е�ý��
		list = pDlg->m_pMediaList;
		while(list)
		{
			if (list->snapshot->isSelected())
				break;
			list = list->next;
		}
		if (!list)
		{
			pDlg->m_pMediaList->snapshot->setSelected();
			list = pDlg->m_pMediaList;
		}


		//��ȡý��������ԣ���ת�Ƕ�/�ü�����...��
		if (MEDIA_PICTURE == list->type)
		{
			//��ת�Ƕ�
			angle = list->pPictureDlg->getMediaRotateAngle();
			//��ת��ʽ
			flip = list->pPictureDlg->getMediaFileType();
			//��ȡʱ��
			start_time = 0;
			end_time = list->total_time;
			//����
			speed = list->pPictureDlg->getMediaSpeed();
			//�ü�����
			rtClip = list->pPictureDlg->getMediaClip();
		}
		else
		{
			//��ת�Ƕ�
			angle = list->pMediaDlg->getMediaRotateAngle();
			//��ת��ʽ
			flip = list->pMediaDlg->getMediaFileType();
			//��ȡʱ��
			start_time = list->pMediaDlg->getMediaCutStartTime();
			end_time = list->pMediaDlg->getMediaCutEndTime();
			//����
			speed = list->pMediaDlg->getMediaSpeed();
			//�ü�����
			rtClip = list->pMediaDlg->getMediaClip();
			//��̬�ü�����
			clip_list = list->pMediaDlg->getDynamicClipList();
		}

		nTime = GetTickCount();
		list->handle = xpkCreate();
		if(!list->handle)
		{
			AfxMessageBox("xpkCreate error ");
			return -__LINE__;
		}
		if (end_time - start_time == list->total_time)
			timeline_end = list->total_time;
		else
			timeline_end = end_time - start_time;

		nTime = GetTickCount() - nTime;
		sprintf(buf_temp,"XPK create Media TIME: %d ",nTime);
		strcat(buf,buf_temp);
		nTime = GetTickCount();
		//���ý��
		hMedia = xpkAddMedia(list->handle,list->path,0,timeline_end,start_time,end_time);
		if(!hMedia)
		{
			AfxMessageBox("xpkAddMedia error ");
			return -__LINE__;
		}

		nTime = GetTickCount() - nTime;
		sprintf(buf_temp,"XPK add Media TIME: %d ",nTime);
		strcat(buf,buf_temp);

		ret = xpkSetMediaRotateAngle(list->handle,hMedia,angle);
		if (ret <= 0)
		{
			AfxMessageBox("xpkSetMediaRotateAngle error ");
			return -__LINE__;
		}


		ret = xpkSetMediaFlip(list->handle,hMedia,flip);
		if (ret <= 0)
		{
			AfxMessageBox("xpkSetMediaFlip error ");
			return -__LINE__;
		}
		//	xpkSetMediaFilter(list->handle,hMedia,VFT_BEAUTY);

		ret = xpkSetMediaClip(list->handle,hMedia,rtClip);
		if (ret <= 0)
		{
			AfxMessageBox("xpkSetMediaClip error ");
			return -__LINE__;
		}
#if 0
		ret = xpkSetMediaTransition(list->handle,hMedia,TRANSITION_CLOCK,NULL,"c:/clock1.kxfx");
		if (ret <= 0)
			return ret;
#endif
		ret = xpkSetMediaPlaySpeed(list->handle,hMedia,speed);
		if (ret <= 0)
		{
			AfxMessageBox("xpkSetMediaPlaySpeed error ");
			return -__LINE__;
		}


		while (clip_list)
		{
			sprintf(buf,"crop list : left = %f  rifht = %f top = %f bottom = %f start_time:%d end_time:%d\r\n ",clip_list->rect.left,
				clip_list->rect.right,clip_list->rect.top,clip_list->rect.bottom,clip_list->time_start,clip_list->time_end);
			OutputDebugString(buf);

			ret = xpkSetMediaDynamicClip(list->handle,hMedia,clip_list->time_start,clip_list->time_end,clip_list->rect);
			if (ret <= 0)
			{
				AfxMessageBox("xpkSetMediaDynamicClip error ");
				return -__LINE__;
			}
			clip_list = clip_list->next;
		}	


		//��ȡ��ʾ����
		pDlg->GetDlgItem(IDC_MEDIA_DISPLAY)->GetClientRect(&showRect);
		//�����������Ĵ�С
		xpkSetFrameSize(list->handle,showRect.right - showRect.left,showRect.bottom - showRect.top);
		//���ûص�����
		xpkSetPreviewPlayFinishCallback(list->handle,play_finish,pDlg);


		nTime = GetTickCount();
		//Ԥ����ʼ��
		ret = xpkPreviewInit(list->handle,wnd->m_hWnd);
		if(ret <= 0)
		{
			AfxMessageBox("xpkPreviewInit error ");
			return -__LINE__;
		}
		nTime = GetTickCount() - nTime;
		sprintf(buf_temp," XPK xpkPreviewInit  Time:%d  ",nTime);
		strcat(buf,buf_temp);

		ret = xpkPreviewSeek(list->handle,0);
		if(ret <= 0)
		{
			AfxMessageBox("xpkPreview error ");
			return -__LINE__;
		}

		sumTime = GetTickCount() - sumTime;
		sprintf(buf_temp," XPK display media Time:%d  \n",sumTime);
		strcat(buf,buf_temp);
		OutputDebugString(buf);

		ret = xpkPreview(list->handle);
		if(ret <= 0)
		{
			AfxMessageBox("xpkPreview error ");
			return -__LINE__;
		}
		OutputDebugString("addmedia display OUT !\n");
	}

	OutputDebugString("addmedia displayThreadProc OUT ..........!\n");

	return 1;
}

int CXPKAddMedia::display(CShowSnapshot* snapshot)
{
	if(!snapshot)
		return 1;
	curSnapshot = snapshot;
	SetEvent(displayEvent);
	return 1;
}
int CXPKAddMedia::addSnapshot(char* path,int widthSrc,int heightSrc,int width,int height,
							  int64_t total_time,enum MEDIA_TYPES type)
{
	char buf[1024];
	Handle hMedia = NULL;
	Handle handle = xpkCreate();
	CRect rect;
	int ret = 0;
	DWORD addMediaTime = 0;
	DWORD getSnapshotTime = 0;
	if (!handle)
	{
		AfxMessageBox("xpkCreate error");
		return -__LINE__;
	}
	m_resouce_id++;
	if (!m_pMediaList)
	{
		m_pMediaList = (XPK_MEDIA_LIST*)malloc(sizeof(XPK_MEDIA_LIST));
		if(!m_pMediaList)
			return -__LINE__;

		memset(m_pMediaList,0,sizeof(XPK_MEDIA_LIST));
		m_pMediaList->id = m_resouce_id;
		m_pMediaList->widthSrc = widthSrc;
		m_pMediaList->heightSrc = heightSrc;
		m_pMediaList->height = height;
		m_pMediaList->width = width;
		m_pMediaList->total_time = total_time;
		m_pMediaList->type = type;
		strcpy(m_pMediaList->path,path);

		addMediaTime = GetTickCount();
		//���ý��
		hMedia = xpkAddMedia(handle,path,0,total_time,0,0);
		if (!hMedia)
		{
			sprintf(buf,"xpkAddMedia error:%d ",-__LINE__);
			AfxMessageBox(buf);
			return -__LINE__;
		}
		addMediaTime = GetTickCount() - addMediaTime;

		m_pMediaList->pImage = (uint8_t* )malloc(m_pMediaList->width*m_pMediaList->height*4);
		if (!m_pMediaList->pImage)
		{
			sprintf(buf,"malloc error:%d ",-__LINE__);
			AfxMessageBox(buf);
			return -__LINE__;
		}

		getSnapshotTime = GetTickCount();
		//��ȡ����ͼ(������Ƶ�п��ܵ�һ֡����)
		ret = xpkGetSnapShot(handle,100000,m_pMediaList->width,m_pMediaList->height,m_pMediaList->pImage);
		if(ret <= 0)
		{
			sprintf(buf,"xpkGetSnapShot error:%d ",ret);
			AfxMessageBox(buf);
			return ret;
		}
		getSnapshotTime = GetTickCount()-getSnapshotTime;
		sprintf(buf," TEST addMediaTime:%d  getSnapshotTime:%d \n",addMediaTime,getSnapshotTime);
		OutputDebugString(buf);


		//��������ͼ��ʾ����		
		m_pMediaList->snapshot_rect.left	= SNAPSHOT_INTERSPACE;
		m_pMediaList->snapshot_rect.top		= SNAPSHOT_INTERSPACE + SNAPSHOT_INTERSPACE;
		m_pMediaList->snapshot_rect.right	= SNAPSHOT_INTERSPACE + SNAPSHOT_WIDTH;
		m_pMediaList->snapshot_rect.bottom	= m_pMediaList->snapshot_rect.top + SNAPSHOT_HEIGHT;

		m_pMediaList->snapshot = new CShowSnapshot;
		m_pMediaList->snapshot->Create("",WS_CHILD|WS_VISIBLE|SS_GRAYFRAME|SS_NOTIFY,m_pMediaList->snapshot_rect,this);

		//��ʾ����ͼ
		ret = m_pMediaList->snapshot->SetSnapshot((int8_t* )m_pMediaList->pImage,m_pMediaList->width,m_pMediaList->height);
		if (ret <= 0)
		{
			sprintf(buf,"SetSnapshot error:%d ",ret);
			AfxMessageBox(buf);
			return ret;
		}

		//����button��ʾ����
		int w = CBUTTON_WIDTH;
		int h = CBUTTON_HEIGHT;
		m_pMediaList->button_rect.left		= m_pMediaList->snapshot_rect.left + (m_pMediaList->snapshot_rect.right - m_pMediaList->snapshot_rect.left - w)/2;
		m_pMediaList->button_rect.right		= m_pMediaList->snapshot_rect.right-(m_pMediaList->snapshot_rect.right - m_pMediaList->snapshot_rect.left - w)/2;

		m_pMediaList->button_rect.top		= m_pMediaList->snapshot_rect.bottom + 10;
		m_pMediaList->button_rect.bottom	= m_pMediaList->snapshot_rect.bottom + 30;

		m_pMediaList->button	 = new CSetButton;
		m_pMediaList->button->Create("....",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,m_pMediaList->button_rect,this,RESOURCE_ID*2+m_pMediaList->id);

		//�����Ի���
		if(MEDIA_PICTURE ==type)
		{
			m_pMediaList->pPictureDlg = new CAdvancedSettingPictureDialog(RESOURCE_ID*3+m_pMediaList->id,widthSrc,heightSrc,type,total_time,this);
			m_pMediaList->pPictureDlg->Create(IDD_DIALOG_PICTURE_PROPERTY,this);

		}
		else
		{
			m_pMediaList->pMediaDlg = new CAdvancedSettingMediaDialog(RESOURCE_ID*3+m_pMediaList->id,widthSrc,heightSrc,type,total_time,this);
			m_pMediaList->pMediaDlg->Create(IDD_DIALOG_MEDIA_PROPERTY,this);
		}

		//����ת��commbox
		m_pMediaList->transition_box = new CXPKCombobox(COMBOBOX_TRANSITION);
		m_pMediaList->transition_box_rect.top		= m_pMediaList->button_rect.top;
		m_pMediaList->transition_box_rect.left		= m_pMediaList->button_rect.right + 5 ;
		m_pMediaList->transition_box_rect.right	= m_pMediaList->button_rect.right + 40 ;
		m_pMediaList->transition_box_rect.bottom	= m_pMediaList->transition_box_rect.bottom;
		m_pMediaList->transition_box->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST ,m_pMediaList->transition_box_rect,this,0);
	}
	else
	{
		XPK_MEDIA_LIST* list = m_pMediaList;
		int index_w = 0;
		int index_h = 0;
		int nMaxIndex = 0;
		CRect rect;
		RECT showRect = {0};
		int number_snapshot = 1;
		GetClientRect(&rect);
		GetDlgItem(IDC_MEDIA_DISPLAY)->GetClientRect(&showRect);
		//����һ���������ͼ����
		nMaxIndex = (rect.right - (showRect.right - showRect.left) - SNAPSHOT_INTERSPACE) / (SNAPSHOT_INTERSPACE + SNAPSHOT_WIDTH);
		//��ȡ��ǰ����ͼ����
		while(list)
		{
			number_snapshot++;
			list = list->next;
		}
		//���������һ���ڵ�
		list = m_pMediaList;
		while(list&&list->next)
			list = list->next;

		list->next = (XPK_MEDIA_LIST*)malloc(sizeof(XPK_MEDIA_LIST));
		if(!list->next)
			return -__LINE__;

		memset(list->next,0,sizeof(XPK_MEDIA_LIST));
		list->next->id = m_resouce_id;
		list->next->height = height;
		list->next->width = width;
		list->next->widthSrc = widthSrc;
		list->next->heightSrc = heightSrc;
		list->next->total_time = total_time;
		list->next->type = type;
		strcpy(list->next->path,path);

		//���ý��
		hMedia = xpkAddMedia(handle,path,0,total_time,0,0);
		if (!hMedia)
		{
			sprintf(buf,"xpkAddMedia error:%d ",-__LINE__);
			AfxMessageBox(buf);
			return -__LINE__;
		}

		list->next->pImage = (uint8_t* )malloc(list->next->width*list->next->height*4);
		if (!list->next->pImage)
		{
			sprintf(buf,"malloc error:%d ",-__LINE__);
			AfxMessageBox(buf);
			return -__LINE__;
		}

		//��ȡ����ͼ(������Ƶ�п��ܵ�һ֡����)
		ret = xpkGetSnapShot(handle,100000,list->next->width,list->next->height,list->next->pImage);
		if(ret <= 0)
		{
			sprintf(buf,"xpkGetSnapShot error:%d ",ret);
			AfxMessageBox(buf);
			return ret;
		}

		if(0 == number_snapshot%nMaxIndex )
		{
			index_w = nMaxIndex;
			index_h = number_snapshot/nMaxIndex;
		}
		else
		{
			index_w = number_snapshot - number_snapshot/nMaxIndex*nMaxIndex;
			index_h = number_snapshot/nMaxIndex + 1 ;
		}
		//������ʾ����
		list->next->snapshot_rect.left		= SNAPSHOT_INTERSPACE*index_w + SNAPSHOT_WIDTH*(index_w - 1);
		list->next->snapshot_rect.top		= SNAPSHOT_INTERSPACE*index_h + SNAPSHOT_HEIGHT*(index_h -1) + SNAPSHOT_INTERSPACE;
		list->next->snapshot_rect.right		= (SNAPSHOT_INTERSPACE + SNAPSHOT_WIDTH)*index_w;
		list->next->snapshot_rect.bottom	= (SNAPSHOT_INTERSPACE + SNAPSHOT_HEIGHT)*index_h + SNAPSHOT_INTERSPACE;

		list->next->snapshot  = new CShowSnapshot;
		list->next->snapshot->Create("",WS_CHILD|WS_VISIBLE|SS_GRAYFRAME|SS_NOTIFY,list->next->snapshot_rect,this);

		//memcpy(list->next->pImage,pImage,width*height*4);
		ret = list->next->snapshot->SetSnapshot((int8_t*)list->next->pImage,list->next->width,list->next->height);
		if(ret <= 0)
		{
			sprintf(buf,"SetSnapshot error:%d ",ret);
			AfxMessageBox(buf);
			return ret;
		}

		//������Чѡ����Ͽ�
		int w = CBUTTON_WIDTH;
		int h = CBUTTON_HEIGHT;
		list->next->button_rect.left	= list->next->snapshot_rect.left + (list->next->snapshot_rect.right - list->next->snapshot_rect.left - w)/2;
		list->next->button_rect.right	= list->next->snapshot_rect.right-(list->next->snapshot_rect.right - list->next->snapshot_rect.left - w)/2;

		list->next->button_rect.top		= list->next->snapshot_rect.bottom + 10;
		list->next->button_rect.bottom	= list->next->snapshot_rect.bottom + 30;

		list->next->button	 = new CSetButton;
		list->next->button->Create("....",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,list->next->button_rect,this,RESOURCE_ID*2+list->next->id);

		
		if (MEDIA_PICTURE == type )
		{	
			//����ͼƬ��Դ�Ի���
			list->next->pPictureDlg = new CAdvancedSettingPictureDialog(RESOURCE_ID*3+list->next->id,widthSrc,heightSrc,type,total_time,this);
			list->next->pPictureDlg->Create(IDD_DIALOG_PICTURE_PROPERTY,this);
		}
		else
		{
			//������Ƶ��Դ�Ի���
			list->next->pMediaDlg = new CAdvancedSettingMediaDialog(RESOURCE_ID*3+list->next->id,widthSrc,heightSrc,type,total_time,this);
			list->next->pMediaDlg->Create(IDD_DIALOG_MEDIA_PROPERTY,this);
		}
		
		
		//����ת��commbox
		list->next->transition_box = new CXPKCombobox(COMBOBOX_TRANSITION);
		list->next->transition_box_rect.top	= list->next->button_rect.top;
		list->next->transition_box_rect.left	= list->next->button_rect.right + 5 ;
		list->next->transition_box_rect.right	= list->next->button_rect.right + 40 ;
		list->next->transition_box_rect.bottom	= list->next->transition_box_rect.bottom;
		list->next->transition_box->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST ,list->next->transition_box_rect,this,0);

	}
	m_mediaChange = 1;
	if(handle)
		xpkCloseMediaEditor(handle);
	return 1;
}
//��ʾ������ͼ��Ӧ�����öԻ���
int CXPKAddMedia::showAdvanceDlg(int id)
{
	XPK_MEDIA_LIST* pList = m_pMediaList;
	while (pList)
	{
		if (MEDIA_PICTURE ==  pList->type)
		{
			if (id == pList->pPictureDlg->m_resouceId)
			{
				pList->pPictureDlg->refreshUI();
				break;
			}
		}
		else
		{
			if (id == pList->pMediaDlg->m_resouceId)
			{
				pList->pMediaDlg->refreshUI();
				break;
			}
		}
		pList = pList->next;
	}
	return 1;
}

int  CXPKAddMedia::clearAllMedia()
{
	XPK_MEDIA_LIST* pList = NULL;
	

	while(m_pMediaList)
	{
		pList = m_pMediaList->next;
		if (m_pMediaList->handle)
		{
			xpkPreviewStop(m_pMediaList->handle);
			xpkCloseMediaEditor(m_pMediaList->handle);
			m_pMediaList->handle = NULL;
		}
		free(m_pMediaList->pImage);
		delete m_pMediaList->button;
		if(m_pMediaList->pMediaDlg)
			delete m_pMediaList->pMediaDlg;
		if(m_pMediaList->pPictureDlg)
			delete m_pMediaList->pPictureDlg;
		delete m_pMediaList->snapshot;
		delete m_pMediaList->transition_box;
		free(m_pMediaList);
		m_pMediaList = pList;
	}
	m_resouce_id = 0;
	return 1;
}

int CXPKAddMedia::addMedia(char* path)
{
	int width = 0;
	int height = 0;
	int ret = 0;
	char debug[MAX_PATH];
	enum MEDIA_TYPES type;
	int64_t duration = 0;
	XPK_MEDIA_LIST* pList = m_pMediaList;
	DWORD nTime = 0;

	OutputDebugString("addMedia IN !\n");
	while(pList)
	{
		if (!strcmp(path,pList->path))
		{
			AfxMessageBox("����Դ�Ѿ���ӹ���....");
			return -__LINE__;
		}
		pList = pList->next;
	}

	//��ȡý����Ϣ
#if 1
	ret = xpkGetMediaInfo(path,&duration,&m_audio_info,&m_video_info);
	if(ret <= 0)
	{
		AfxMessageBox("xpkGetMediaInfo error");
		return -__LINE__;
	}

	width = m_video_info.width;
	height = m_video_info.height;
#else
	duration = 180000000;
	width = 640;
	height = 480;
#endif
	if(width <= 0 ||height <= 0)
	{
		AfxMessageBox("width or height error");
		return -__LINE__;
	}

	//��ȡ��Դ����
	type = xpkDetectMediaType(path);
	if (type == MEDIA_PICTURE)
		duration = TIME_DURATION_PICTURE;

	nTime = GetTickCount();
	//�������ݵ�����
	ret = addSnapshot(path,width,height,SNAPSHOT_WIDTH,SNAPSHOT_HEIGHT,duration,type);
	if(ret <= 0)
		return -__LINE__;
	nTime = GetTickCount() - nTime;
	sprintf(debug,"TEST addSnapshot TIME:%d \n",nTime);
	OutputDebugString(debug);

	OutputDebugString("addMedia OUT !\n");
	return 1;
}
//��ʾ����ͼ
int CXPKAddMedia::refreshSnapshot()
{
	XPK_MEDIA_LIST *list = m_pMediaList;
	int index = 0;
	int index_w = 0;
	int index_h = 0;
	int nMaxIndex = 0;
	CRect rect;
	RECT showRect = {0};
	GetDlgItem(IDC_MEDIA_DISPLAY)->GetClientRect(&showRect);
	GetClientRect(&rect);
	nMaxIndex = (rect.right - (showRect.right - showRect.left) - SNAPSHOT_INTERSPACE) / (SNAPSHOT_INTERSPACE + SNAPSHOT_WIDTH);
	while(list)
	{
		index++;
		list->id = index;

		if(0 == list->id%nMaxIndex )
		{
			index_w = nMaxIndex;
			index_h = list->id/nMaxIndex;
		}
		else
		{
			index_w = list->id - list->id/nMaxIndex*nMaxIndex;
			index_h = list->id/nMaxIndex + 1 ;
		}
		//������ʾ����
		list->snapshot_rect.left		= SNAPSHOT_INTERSPACE*index_w + SNAPSHOT_WIDTH*(index_w - 1);
		list->snapshot_rect.top			= SNAPSHOT_INTERSPACE*index_h + SNAPSHOT_HEIGHT*(index_h -1) + SNAPSHOT_INTERSPACE;
		list->snapshot_rect.right		= (SNAPSHOT_INTERSPACE + SNAPSHOT_WIDTH)*index_w;
		list->snapshot_rect.bottom		= (SNAPSHOT_INTERSPACE + SNAPSHOT_HEIGHT)*index_h + SNAPSHOT_INTERSPACE;
		list->snapshot->MoveWindow(&list->snapshot_rect);

		int width  = CBUTTON_WIDTH;
		int height = CBUTTON_HEIGHT;
		list->button_rect.left		= list->snapshot_rect.left + (list->snapshot_rect.right - list->snapshot_rect.left - width)/2;
		list->button_rect.right		= list->snapshot_rect.right-(list->snapshot_rect.right - list->snapshot_rect.left - width)/2;

		list->button_rect.top		= list->snapshot_rect.bottom + 10;
		list->button_rect.bottom	= list->snapshot_rect.bottom + 30;
		list->button->MoveWindow(&list->button_rect);

		list->transition_box_rect.top		= list->button_rect.top;
		list->transition_box_rect.bottom	= list->button_rect.bottom;
		list->transition_box_rect.left		= list->button_rect.right + 5 ;
		list->transition_box_rect.right	= list->button_rect.right + 40 ;
		list->transition_box->MoveWindow(&list->transition_box_rect);

		list = list->next;
	}
	return 1;
}
void CXPKAddMedia::OnBnClickedAddFile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int ret = 0;
	char path[MAX_PATH];
	DWORD nTime = 0;
	INT_PTR res ;
	char buf[1024];
	XPKWizard* pSheet = reinterpret_cast<XPKWizard*>(GetParent());

	XPK_MEDIA_LIST *pList = NULL;
	//CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("λͼ�ļ�(*.BMP;*.jpg;*mp4;*.mov)|*.BMP;*.jpg;*mp4;*.mov|"));
	
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |OFN_ALLOWMULTISELECT,_T("λͼ�ļ�(*.BMP;*.jpg;*.mpg;*mp4;*.mov;*.png)|*.BMP;*.jpg;*.mpg;*mp4;*.mov;*.png|"));
	TCHAR *pBuffer = new TCHAR[MAX_PATH    * 20];//�������ͬʱ��20���ļ�
	dlg.m_ofn.lpstrFile = pBuffer;
	dlg.m_ofn.nMaxFile = MAX_PATH * 20;
	dlg.m_ofn.lpstrFile[0] = '\0';

	if (dlg.DoModal() == IDOK)
	{

		nTime = GetTickCount();
		POSITION pos = dlg.GetStartPosition();
		while (pos != NULL)
		{
			//ȡ���ļ�·��
			strcpy(path,dlg.GetNextPathName(pos));
			OutputDebugString("\n");
			OutputDebugString(path);
			if(addMedia(path)<=0)
			{
				delete pBuffer;
				return ;
			}
		}
		nTime = GetTickCount() - nTime;
	}
	sprintf(buf,"XPK addmedia time:%d \n",nTime);
	OutputDebugString(buf);

	//ת��combobox
	pList = m_pMediaList;
	while(pList && pList->next)
	{
		pList->transition_box->ShowWindow(SW_SHOW);
		pList = pList->next;
	}
	if(pList)
		pList->transition_box->ShowWindow(SW_HIDE);
	delete pBuffer;

	//Ĭ�ϲ��ŵ�һ��
	if(m_pMediaList)
	{
		m_pMediaList->snapshot->setSelected();
		display(m_pMediaList->snapshot);
	}

	//����˾�
	pSheet->clearAllFilter();
}

BOOL CXPKAddMedia::OnInitDialog()
{
	int ret = 0;
	CPropertyPage::OnInitDialog();
	RECT rect = {0};
	RECT showRect = {0};


#if 0
	FONT_PEP font = {0};
	int bmp_width = 640;
	int bmp_height = 480;
	Handle hTextBmp = xpkCreateTextBmp();
	RGBQUAD clr = {0,0,0};

	//	ret = xpkSetTextBmpSize(hTextBmp,bmp_width,bmp_height);
	ret = xpkSetTextBmpText(hTextBmp,"��Բ \n\r 123 \n 456 \n789 \n");
	ret = xpkSetTextBmpOutPutPath(hTextBmp,"c:/111.bmp");

	strcpy(font.name,"��Բ");
	font.width = 60;
	font.height = 30;
	font.fontClr.rgbRed = 255;
	font.fontClr.rgbGreen = 0;
	font.fontClr.rgbBlue = 0;
	ret = xpkSetTextBmpTextFont(hTextBmp,font);
	ret = xpkSetTextBmpBackGroundColor(hTextBmp,clr);
	ret = xpkEditTextBmp(hTextBmp);
	xpkDestroyTextBmp(hTextBmp);
	
	
#endif

	//������ʾ����λ��
	GetClientRect(&rect);
	showRect.left = rect.right - DISPLAY_WIDTH - SNAPSHOT_INTERSPACE;
	showRect.right = rect.right - SNAPSHOT_INTERSPACE;
	showRect.top = SNAPSHOT_INTERSPACE ;
	showRect.bottom =  DISPLAY_HEIGHT + SNAPSHOT_INTERSPACE;
	if (showRect.right%2 != 0)
		showRect.right -= 1;
	if(showRect.bottom%2 != 0)
		showRect.bottom -= 1;
	GetDlgItem(IDC_MEDIA_DISPLAY)->MoveWindow(&showRect);

	m_display_zoom = (float)DISPLAY_WIDTH/(float)rect.right;

	int count = xpkGetTotalFilter();
	for (int i = 1;i<count;i++)
	{
		char *buf = xpkGetFilterName(i);
		OutputDebugString(buf);
		OutputDebugString("/n");
	}
	hDisplayThread = CreateThread(NULL,0,displayThreadProc,this,0,NULL);
	hLoadMediaThread = CreateThread(NULL,0,loadMediaThreadProc,this,0,NULL);

	

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CXPKAddMedia::OnBnClickedClearallmedia()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	XPKWizard* pSheet = reinterpret_cast<XPKWizard*>(GetParent());
	clearAllMedia();
	Invalidate();
	m_mediaChange = 1;
	pSheet->clearAllFilter();
}

void CXPKAddMedia::OnBnClickedDeletemedia()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	XPK_MEDIA_LIST* list = m_pMediaList;
	XPKWizard* pSheet = reinterpret_cast<XPKWizard*>(GetParent());
	XPK_MEDIA_LIST* temp = NULL;
	int number = 0;
	if(!list)
		return;

	pSheet->clearAllFilter();

	list = m_pMediaList;
	while(list )
	{
		number++;
		list = list->next;
	}
	
	//������ǰѡ�еĽڵ㣬����ɾ��
	for (int i = 0;i<number;i++)
	{
		if (!m_pMediaList->next && m_pMediaList->snapshot->isSelected())
		{
			if (m_pMediaList->handle)
			{
				xpkPreviewStop(m_pMediaList->handle);
				xpkCloseMediaEditor(m_pMediaList->handle);
				m_pMediaList->handle = NULL;
			}
			free(m_pMediaList->pImage);
			delete m_pMediaList->snapshot;
			delete m_pMediaList->button;
			if(m_pMediaList->pPictureDlg)
				delete m_pMediaList->pPictureDlg;
			if (m_pMediaList->pMediaDlg)
				delete m_pMediaList->pMediaDlg;
			delete m_pMediaList->transition_box;
			free(m_pMediaList);
			m_pMediaList = NULL;
			continue;
		}

		if(m_pMediaList->snapshot->isSelected())
		{
			XPK_MEDIA_LIST *temp = m_pMediaList->next;
			if (m_pMediaList->handle)
			{
				xpkPreviewStop(m_pMediaList->handle);
				xpkCloseMediaEditor(m_pMediaList->handle);
				m_pMediaList->handle = NULL;
			}
			free(m_pMediaList->pImage);
			delete m_pMediaList->snapshot;
			delete m_pMediaList->button;
			if(m_pMediaList->pPictureDlg)
				delete m_pMediaList->pPictureDlg;
			if (m_pMediaList->pMediaDlg)
				delete m_pMediaList->pMediaDlg;
			delete m_pMediaList->transition_box;
			free(m_pMediaList);
			m_pMediaList = temp;
			continue;
		}

		list = m_pMediaList;
		while (list)
		{
			if (list->next&&list->next->snapshot->isSelected())
			{
				if (list->next->next)
				{
					XPK_MEDIA_LIST *temp = list->next->next;
					if (list->next->handle)
					{
						xpkPreviewStop(list->next->handle);
						xpkCloseMediaEditor(list->next->handle);
						list->next->handle = NULL;
					}
					free(list->next->pImage);
					delete list->next->snapshot;
					delete list->next->button;
					if(list->next->pPictureDlg)
						delete list->next->pPictureDlg;
					if(list->next->pMediaDlg)
						delete list->next->pMediaDlg;
					delete list->next->transition_box;
					free(list->next);
					list->next = temp;
				}
				else
				{
					if (list->next->handle)
					{
						xpkPreviewStop(list->next->handle);
						xpkCloseMediaEditor(list->next->handle);
						list->next->handle = NULL;
					}
					free(list->next->pImage);
					delete list->next->snapshot;
					delete list->next->button;
					if (list->next->pPictureDlg)
						delete list->next->pPictureDlg;
					if(list->next->pMediaDlg)
						delete list->next->pMediaDlg;
					delete list->next->transition_box;
					free(list->next);
					list->next = NULL;
				}
				break;
			}
			list = list->next;
		}	
	}
	
UI_REFRESH:
	//ˢ����ʾ����
	refreshSnapshot();

	list = m_pMediaList;
	while(list && list->next)
	{
		list->transition_box->ShowWindow(SW_SHOW);
		list = list->next;
	}
	if(list)
		list->transition_box->ShowWindow(SW_HIDE);
	Invalidate();
	m_mediaChange = 1;

	//Ĭ�ϲ��ŵ�һ��
	if (m_pMediaList)
	{
		m_pMediaList->snapshot->setSelected();
		display(m_pMediaList->snapshot);
	}
	
}
int CXPKAddMedia::transitionChange(int change)
{
	m_mediaChange = change;
	return 1;
}
LRESULT CXPKAddMedia::OnWizardNext()
{
	// TODO: �ڴ����ר�ô����/����û���
	XPK_MEDIA_LIST* plist = m_pMediaList;
	XPKWizard *pSheet = (XPKWizard*)GetParent();
	if(!plist)
	{
		AfxMessageBox("�������ý����Դ....");
		return -1;
	}
	//ֹͣ��ǰ���ڲ��ŵ�ý��

	while (plist)
	{
		if (plist->handle)
		{
			xpkPreviewStop(plist->handle);
			xpkCloseMediaEditor(plist->handle);
			plist->handle = NULL;
		}
		plist = plist->next;
	}
	plist = m_pMediaList;
	while(plist)
	{	
		if (plist->type == MEDIA_PICTURE && plist->pPictureDlg->propertyChange)
		{
			m_mediaChange = 1;
			break;
		}
		else if(plist->type >= MEDIA_SLIDE && plist->pMediaDlg->propertyChange)
		{
			m_mediaChange = 1;
			break;
		}
		plist = plist->next;
	}
	
	if(pSheet->init()<=0)
	{
		AfxMessageBox("init error ");
		return -__LINE__;
	}
	pSheet->statu = XKP_NONE;
	//�ָ�Ĭ������
	resetMediaProperty();
	return CPropertyPage::OnWizardNext();
}
int CXPKAddMedia::resetMediaProperty()
{
	XPKWizard *pSheet = (XPKWizard*)GetParent();
	XPK_MEDIA_LIST* plist = m_pMediaList;
	while(plist)
	{
		if (plist->type == MEDIA_PICTURE && plist->pPictureDlg->propertyChange)
			plist->pPictureDlg->propertyChange = 0;
		if(plist->type >= MEDIA_SLIDE && plist->pMediaDlg->propertyChange)
			plist->pMediaDlg->propertyChange = 0;
		plist = plist->next;
	}
	m_mediaChange = 0;
	return 1;
}
void CXPKAddMedia::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������

#if DIALOG_SIZE_CHANGE

	if (m_display_zoom )
	{
		//ˢ����ʾ����
		RECT rect = {0};
		RECT showRect = {0};
		GetClientRect(&rect);
		showRect.left = rect.right - rect.right*m_display_zoom - SNAPSHOT_INTERSPACE;
		showRect.right = rect.right - SNAPSHOT_INTERSPACE;
		showRect.top = SNAPSHOT_INTERSPACE ;
		showRect.bottom =  (showRect.right - showRect.left)*(float)DISPLAY_HEIGHT/(float)DISPLAY_WIDTH + SNAPSHOT_INTERSPACE;

		if ((showRect.right - showRect.left)%2 != 0)
			showRect.right -= 1;
		if((showRect.bottom - showRect.top)%2 != 0)
			showRect.bottom -= 1;

		GetDlgItem(IDC_MEDIA_DISPLAY)->MoveWindow(&showRect);

		refreshSnapshot();
	}

#endif
}

HBRUSH CXPKAddMedia::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if( pWnd->GetDlgCtrlID() == IDC_MEDIA_DISPLAY )
	{
		pDC->SetTextColor(RGB(255,0,0));  //����������ɫ
		pDC->SetBkMode(TRANSPARENT); //�������屳��Ϊ͸��
		return (HBRUSH)::GetStockObject(BLACK_BRUSH);  // ���ñ���ɫ
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void CXPKAddMedia::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	CPropertyPage::OnClose();
}
