#include <stdio.h>
#include <windows.h>
#include <Math.h>
#include <time.h>
#include "CSVFile.h"
#include "Grid.h"
#include "APoint.h"
#include "Obstacle.h"

// �_�C�A���O������炤�f�[�^
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

// �O���b�h���̎��f�[�^
typedef struct _GRID_DATA{
	BOOL		bNotValid;		// AP�z�u�֎~��悩�ǂ���
	INT			iPower;		// �d�g���x
	FLOAT		fSpeed;		// �ʐM���x
	INT			iExpVal;		// �l�̂�����Ғl	�����l�ŗǂ��H
	BOOL		bAP;			// AP��z�u�������ǂ���
	INT			iDistance; // AP�Ƃ̋���
	INT			iAP;			// APDATA�Ƃ̃����N
}GRID_DATA,*PGRID_DATA;

typedef struct _AP_DATA{
	INT			iPosX;		// X���W
	INT			iPosY;		// Y���W
	INT			iTotalExpVal; // �l�̊��Ғl�̍��v�l
	INT			iChannel;	// AP�̃`���l��
	BOOL		bNear;		// �d�g�͈͊O�ł͂��邪�J�o�[����Ă��Ȃ��O���b�h���߂��ɂ���AP
}AP_DATA,*PAP_DATA;

typedef struct _TABU_LIST{
	INT			iPosX;		// X���W
	INT			iPosY;		// Y���W
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