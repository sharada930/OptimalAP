#include <windows.h>
#pragma once
class CGrid
{
	// Functions
public:
	CGrid();
	~CGrid();

	void		Init( int iX, int iY );
	void		Close();
	BOOL	ReadGridFromCSVFile();
	void		WriteCSVFile( HANDLE hFile );
	void		CopyGridData( CGrid srcgrid );
	BOOL	CheckCover();
	void		RandomSetExpVal( INT iNum );
	void		RandomSetObstacle( INT iNum );
	INT		GetRandomInt( INT iNum );
	
	// Variables
typedef struct _GRID_DATA{
	BOOL		bNotValid;		// AP�z�u�֎~��悩�ǂ���
	INT			iPower;		// �d�g���x
	FLOAT		fSpeed;		// �ʐM���x
	INT			iExpVal;		// �l�̂�����Ғl	�����l�ŗǂ��H
	BOOL		bAP;			// AP��z�u�������ǂ���
	INT			iDistance; // AP�Ƃ̋���
	INT			iAP;			// APDATA�Ƃ̃����N
}GRID_DATA,*PGRID_DATA;

	int m_iGridX;
	int m_iGridY;
	int m_iUnCoverArea;
	double m_Speed;
	GRID_DATA **m_grid;

};

