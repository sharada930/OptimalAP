#include <stdio.h>
#include <windows.h>
#include <Math.h>
#include <time.h>
#include "CSVFile.h"
#include "Grid.h"
#include "APoint.h"
#include "Obstacle.h"

// ダイアログからもらうデータ
typedef struct _SETTING_DATA{
	char		cInputPath[MAX_PATH];
	char		cOutputPath[MAX_PATH];
	int			iHuman;
	int			iAP;
	int			iObstacle;
	int			iGridX;
	int			iGridY;
	int			iGene;
	int			iSample;
	BOOL	bGUI;
	BOOL	bGA;
	BOOL	bObs;
}SETTING_DATA, *PSETTING_DATA;

// グリッド一つ一つの持つデータ
typedef struct _GRID_DATA{
	BOOL		bNotValid;		// AP配置禁止区域かどうか
	INT			iPower;		// 電波強度
	FLOAT		fSpeed;		// 通信速度
	INT			iExpVal;		// 人のいる期待値	整数値で良い？
	BOOL		bAP;			// APを配置したかどうか
	INT			iDistance; // APとの距離
	INT			iAP;			// APDATAとのリンク
}GRID_DATA,*PGRID_DATA;

typedef struct _AP_DATA{
	INT			iPosX;		// X座標
	INT			iPosY;		// Y座標
	INT			iTotalExpVal; // 人の期待値の合計値
	INT			iChannel;	// APのチャネル
	BOOL		bNear;		// 電波範囲外ではあるがカバーされていないグリッドが近くにあるAP
}AP_DATA,*PAP_DATA;

typedef struct _TABU_LIST{
	INT			iPosX;		// X座標
	INT			iPosY;		// Y座標
}TABU_LIST,*PTABU_LIST;


INT GetRandomInt( INT iNum );
CGrid LocalSearchAlgorithm( CGrid org_grid, CAPoint org_ap );
VOID RandomSetExpVal( CGrid grid, INT iNum );
VOID RandomSetAPToGrid( CGrid grid, CAPoint ap );
VOID CalcCoverArea( CGrid grid, CAPoint ap );
VOID AddTabuList( INT iX, INT iY );
BOOL CheckTabuList( INT iX, INT iY );
double CalcSpeed( CGrid grid, CAPoint ap );
BOOL ReadCSVFile( PCHAR pCSVPath );
void GeneticAlgorithm( PSETTING_DATA data );
CGrid RandomUnitGrid( CGrid grid1, CGrid grid2, CAPoint* newap );
BOOL CheckCrossingObstacle( double X, double Y, double APX, double APY, double ObsX, double ObsY );