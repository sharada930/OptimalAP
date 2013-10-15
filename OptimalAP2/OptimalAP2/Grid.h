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
	BOOL		bNotValid;		// AP配置禁止区域かどうか
	INT			iPower;		// 電波強度
	FLOAT		fSpeed;		// 通信速度
	INT			iExpVal;		// 人のいる期待値	整数値で良い？
	BOOL		bAP;			// APを配置したかどうか
	INT			iDistance; // APとの距離
	INT			iAP;			// APDATAとのリンク
}GRID_DATA,*PGRID_DATA;

	int m_iGridX;
	int m_iGridY;
	int m_iUnCoverArea;
	double m_Speed;
	GRID_DATA **m_grid;

};

