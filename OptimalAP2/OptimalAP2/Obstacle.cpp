#include "stdafx.h"
#include "Obstacle.h"


CObstacle::CObstacle(void)
{
}


CObstacle::~CObstacle(void)
{
}

// APの初期化処理
void CObstacle::Init( int iObs )
{
	m_iTotalObs = iObs;

	// 配列生成
	m_pObs = new OBS_DATA[m_iTotalObs+1];

	// 配列の初期化
	for( int i=1; i<=m_iTotalObs; i++ ){
		m_pObs[i].iPosX = 0;
		m_pObs[i].iPosY = 0;
	}
}

void CObstacle::Close()
{
	delete[] m_pObs;
}
