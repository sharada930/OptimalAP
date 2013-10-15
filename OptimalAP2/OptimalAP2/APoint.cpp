#include "stdafx.h"
#include "APoint.h"


CAPoint::CAPoint(void)
{
}
CAPoint::~CAPoint(void)
{
}

// APの初期化処理
void CAPoint::Init( int iAP )
{
	m_iTotalAP = iAP;

	// 配列生成
	m_ap = new AP_DATA[m_iTotalAP+1];

	// 配列の初期化
	for( int i=1; i<=m_iTotalAP; i++ ){
		m_ap[i].iPosX = 0;
		m_ap[i].iPosY = 0;
		m_ap[i].iTotalExpVal = 0;
		m_ap[i].iChannel = 0;
		m_ap[i].bNear = FALSE;
	}
}

void CAPoint::Close()
{
	delete[] m_ap;
}
// APDataのコピー
void CAPoint::CopyAPData( CAPoint srcap )
{
	Init( srcap.m_iTotalAP );

	// 配列の初期化
	for( int i=1; i<=m_iTotalAP; i++ ){
		m_ap[i].iPosX = srcap.m_ap[i].iPosX;
		m_ap[i].iPosY = srcap.m_ap[i].iPosY;
		m_ap[i].iTotalExpVal = srcap.m_ap[i].iTotalExpVal;
		m_ap[i].iChannel = srcap.m_ap[i].iChannel;
		m_ap[i].bNear = srcap.m_ap[i].bNear;
	}
}