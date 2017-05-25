#ifndef __XPK_DEMO_COMMON_H__
#define __XPK_DEMO_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "XPKSlideCommon.h"
#include "XPKSlide.h"

#pragma comment(lib,"SlideKernel.lib")

#ifdef __cplusplus
}
#endif

#include "PepAVToolStruct.h"


#ifdef __GNUC__
typedef long long  int64_t;
typedef unsigned long long   uint64_t;
#else
typedef __int64  int64_t;
typedef unsigned __int64   uint64_t;
#endif

#define SWS_BICUBIC           4

#define TIME_DURATION_PICTURE 4000000
enum COMBOBOX_TYPE{COMBOBOX_NONE = 0,COMBOBOX_TRANSITION,COMBOBOX_MEDIA_PROPERTY,COMBOBOX_MUSIC_PROPERTY,COMBOBOX_PICTURE_PROPERTY};


#define COMBOBOX_MUSIC_PROPERTY_WIDTH 117
#define COMBOBOX_MUSIC_PROPERTY_HEIGHT 18

#define PREVIEW_WIDTH			640
#define PREVIEW_HEIGHT			360

// #define PREVIEW_WIDTH			640
// #define PREVIEW_HEIGHT			480


#define SNAPSHOT_WIDTH          100
#define SNAPSHOT_HEIGHT         100
#define SNAPSHOT_INTERSPACE     40
#define  RESOURCE_ID 100000
//��Ͽ���Чѡ�����
#define  CBUTTON_WIDTH 100;
#define  CBUTTON_HEIGHT 10;

#define  DIALOG_SIZE_CHANGE 1


enum XPK_STATU
{
	XKP_NONE=0,
	XKP_PREVIEW,	//��ǰ��Ԥ��״̬
	XKP_EDIT		//��ǰ�����������ļ�״̬
};

//����ļ���
#define OUTPUT_FILE "xkp_slide.mp4"

//apng
#define XPK_APNG_BLACK_LINE "//resource//����.kxfx"
#define XPK_APNG_LOVE "//resource//����.kxfx"
#define XPK_APNG_SNOW "//resource//ѩ��_kxfx.kxfx"
#define XPK_APNG_FISH "//resource//��������.kxfx"

//music
#define XPK_MUSIC_FISH "//resource//��������.mp3"
#define XPK_MUSIC_MOON "//resource//���¼�ʱ��.mp3"


//��̬�ü�
typedef struct XPK_DYNAMAIC_CLIP_LIST
{
	RECTF rect;
	int64_t time_start;
	int64_t time_end;

	XPK_DYNAMAIC_CLIP_LIST* next;

}XPK_DYNAMAIC_CLIP_LIST;


//����
typedef struct XPK_MUSIC_LIST
{
	Handle hAudio;
	char path[MAX_PATH];
	int64_t start_time;
	int64_t end_time;
	int64_t timeline_start;
	int64_t timeline_end;
	float factor;
	double speed;					//�����ٶ�
	int64_t	fade_in;				//����
	int64_t fade_out;				//����
	int index;

	struct XPK_MUSIC_LIST* next;
};

//��Ļ
typedef struct XPK_SUBTITLE_LIST
{
	Handle hSubtitle;
	char srtPath[MAX_PATH];
	char name[256];
	int64_t timeline_start;
	int64_t timeline_end;
	char szText[MAX_PATH];
	RECTF rect;
	FONT_PEP font;
	RGBQUAD clr;
	int effect;
	int index;

	XPK_SUBTITLE_LIST* next;

}XPK_SUBTITLE_LIST;

//apng ��Ч
typedef struct XPK_APNG_RGB_OVERLAY_LIST
{

	Handle hApng;
	char szPath[MAX_PATH];
	int64_t timeline_start;
	int64_t timeline_end;
	int64_t time_start;
	int64_t time_end;
	int index;
	XPK_APNG_RGB_OVERLAY_LIST* next;

}XPK_APNG_RGB_OVERLAY_LIST;


//�˾�����ͬʱ���ö��
typedef struct XPK_FILTER_LIST
{
	int id;
	XPK_FILTER_LIST* next;

}XPK_FILTER_LIST;

#endif;