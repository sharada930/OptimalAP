#include <windows.h>
#pragma once
class CAPoint
{
	// Functions
public:
	CAPoint();
	~CAPoint(void);

	void Init( int iAP );
	void Close();
	void CopyAPData( CAPoint srcap );
	
	// Variables
typedef struct _AP_DATA{
	INT			iPosX;		// X座標
	INT			iPosY;		// Y座標
	INT			iTotalExpVal; // 人の期待値の合計値
	INT			iChannel;	// APのチャネル
	BOOL		bNear;		// 電波範囲外ではあるがカバーされていないグリッドが近くにあるAP
}AP_DATA,*PAP_DATA;

	int m_iTotalAP;
	AP_DATA *m_ap;

};

