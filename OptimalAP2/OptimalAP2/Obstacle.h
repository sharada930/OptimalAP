#include <windows.h>
#pragma once
class CObstacle
{
public:
	CObstacle(void);
	~CObstacle(void);

	// Functions
	void Init( int iObs );
	void Close();

	// Variables
typedef struct _OBS_DATA{
	INT			iPosX;		// Xç¿ïW
	INT			iPosY;		// Yç¿ïW
}OBS_DATA,*POBS_DATA;

	int m_iTotalObs;
	POBS_DATA m_pObs;

};