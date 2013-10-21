#include "stdafx.h"
#include "Obstacle.h"


CObstacle::CObstacle(void)
{
}


CObstacle::~CObstacle(void)
{
}

// AP�̏���������
void CObstacle::Init( int iObs )
{
	m_iTotalObs = iObs;

	// �z�񐶐�
	m_pObs = new OBS_DATA[m_iTotalObs+1];

	// �z��̏�����
	for( int i=1; i<=m_iTotalObs; i++ ){
		m_pObs[i].iPosX = 0;
		m_pObs[i].iPosY = 0;
	}
}

void CObstacle::Close()
{
	delete[] m_pObs;
}
