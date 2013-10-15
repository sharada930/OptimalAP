// OptimalAP.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B

#include "stdafx.h"
#include "OptimalAP.h"

// �O���[�o���ϐ�
TABU_LIST g_TabuList[10];
INT			g_iTabuNum = 1;
INT			g_iTotalAP;
INT			g_iUnCoverArea = 0;
INT			g_iRand = 1;
INT			iXList[5] = {-1,0,0,0,1};
INT			iYList[5] = {0,-1,0,1,0};


// Input Data	��X���͏o����悤�ɂ���
INT		g_iGridSize	=	3;		// ��̃O���b�h�̈�ӂ̒���(m)
INT		g_APPower = 30;	  //	�d�g���͂��̓͂�����
float		g_fMinSpeed = 10000;
INT		g_iGen = 0;
#define	AP_POWER	54

BOOL CheckCoverGrid( GRID_DATA grid[50][50] );

#define DEBUG_PRINT 0
//#define TABU_LIST

// �Ȉ�DebugPrint
void DebugGridPrint( LPCTSTR path, CGrid grid )
{
	CCSVFile csvfile;
	csvfile.Init( path );
	grid.WriteCSVFile( csvfile.m_hFile );
	csvfile.Close();
}
// ��main�֐�
INT OptimalAPMain()
{
#if 0
	CGrid grid;
	CAPoint ap;

	// �O���b�h�T�C�Y
	grid.Init( 30, 30 );
	// �l�̊��Ғl
	grid.RandomSetExpVal( 300 );
	// AP�̐�
	ap.Init( 10 );
	RandomSetAPToGrid( grid, ap );

	// �Ǐ��T���A���S���Y��
	LocalSearchAlgorithm( grid, ap );
	/*
	CCSVFile csvfile;
	csvfile.Init();
	grid.WriteCSVFile( csvfile.m_hFile );
	csvfile.Close();
	*/
	

#else
	//GenericAlgorithm();
#endif
	return 0;
}

void GeneticAlgorithm( PSETTING_DATA data ){
	int  child_num = data->iSample;
	CGrid* parent_grid = new CGrid[child_num];
	CGrid* child_grid = new CGrid[child_num];
	int iX = data->iGridX;
	int iY = data->iGridY;
	int iExpval = data->iHuman;
	int iMaxGene = data->iGene;

	g_iTotalAP = data->iAP; //�b��
	{
		//---------------------1.�����W�c�̌`��---------------------//
		CGrid temp_grid;
		CAPoint* ap = new CAPoint[child_num];
		int i = 0;
		int j = 0;
		int iLoop = 1;

		// �l�̊��Ғl�A��Q���͋���
		for( i=0; i<child_num; i++ ){
			// ��������������
			srand( 1 );
			// �O���b�h�T�C�Y
			child_grid[i].Init( iX, iY );
			// ��Q��
			if( data->bObs ){
				child_grid[i].RandomSetObstacle( data->iObstacle );
			}
			// �l�̊��Ғl
			child_grid[i].RandomSetExpVal( iExpval );
		}
		// AP�̔z�u�̓����_��
		srand( 1 );
		for( i=0; i<child_num; i++ ){
			// AP�̐�
			ap[i].Init( g_iTotalAP );
			RandomSetAPToGrid( child_grid[i], ap[i] );
		}
		//---------------------1.�����W�c�̌`�� �����܂�---------------------//

		while( iLoop <= iMaxGene){
			//---------------------2.�K���x---------------------//
			for( i=0; i<child_num; i++ ){
				// �Ǐ��T���@
				parent_grid[i] = LocalSearchAlgorithm( child_grid[i], ap[i] );
			}
			// �ʐM���x���ł��������̏��ɕ��ёւ� �o�u���\�[�g
			for( i=0; i<child_num; i++ ){
				for( j=child_num - 1; j > i; j-- ){
					if( parent_grid[j-1].m_Speed < parent_grid[j].m_Speed ){
						temp_grid = parent_grid[j];
						parent_grid[j] = parent_grid[j-1];
						parent_grid[j-1] = temp_grid;
					}
				}
			}
			//---------------------2.�K���x �����܂�---------------------//
			
			//Debug
			DebugGridPrint( data->cOutputPath, parent_grid[0] );
			//printf( "��%d����@�ʐM���x[%f]\r\n", iLoop, parent_grid[0].m_Speed );

			// �I������
			if( iLoop == iMaxGene ){
				break;
			}

			//---------------------3.�����@4.�ˑR�ψ�---------------------//
			// 2�������_���Ȑ��ŕ������A�g�ݍ��킹��
			child_grid[0] = RandomUnitGrid(parent_grid[0], parent_grid[1], &ap[0]);
			child_grid[1] = RandomUnitGrid(parent_grid[1], parent_grid[2], &ap[1]);
			child_grid[2] = RandomUnitGrid(parent_grid[2], parent_grid[3], &ap[2]);
			child_grid[3] = RandomUnitGrid(parent_grid[3], parent_grid[4], &ap[3]);
			child_grid[4] = RandomUnitGrid(parent_grid[4], parent_grid[5], &ap[4]);
			child_grid[5] = RandomUnitGrid(parent_grid[0], parent_grid[2], &ap[5]);
			child_grid[6] = RandomUnitGrid(parent_grid[0], parent_grid[3], &ap[6]);
			child_grid[7] = RandomUnitGrid(parent_grid[0], parent_grid[4], &ap[7]);
			child_grid[8] = RandomUnitGrid(parent_grid[0], parent_grid[5], &ap[8]);
			child_grid[9] = RandomUnitGrid(parent_grid[1], parent_grid[0], &ap[9]);

			// �e�����̒��g���J��
			for( i=0; i<child_num; i++ ){
				parent_grid[i].Close();
			}
			iLoop++;
		}
	}
}

CGrid RandomUnitGrid( CGrid grid1, CGrid grid2, CAPoint* newap )
{
	CGrid		newgrid;
	int				iDivX = 0;
	int				iAPCnt = 0;

	
	// ����������̉ӏ������߂�@�c�ɕ���
	if( grid1.m_iGridX != grid2.m_iGridX ){
		return newgrid;
	}
	iDivX = GetRandomInt( grid1.m_iGridX );
	
	// �R�s�[���Ă���
	newgrid.Init( grid1.m_iGridX, grid1.m_iGridY );
	for( int j=1; j<=grid1.m_iGridY; j++ ){
		for( int i=1; i<=grid1.m_iGridX; i++ ){
			if( i <= iDivX ){
				newgrid.m_grid[i][j].bAP = grid1.m_grid[i][j].bAP;
				newgrid.m_grid[i][j].bNotValid = grid1.m_grid[i][j].bNotValid;
				newgrid.m_grid[i][j].iExpVal = grid1.m_grid[i][j].iExpVal;
				//newgrid.m_grid[i][j].iPower = grid1.m_grid[i][j].iPower;
				//newgrid.m_grid[i][j].fSpeed = grid1.m_grid[i][j].fSpeed;
				//newgrid.m_grid[i][j].iAP = grid1.m_grid[i][j].iAP;
				//newgrid.m_grid[i][j].iDistance = grid1.m_grid[i][j].iDistance;
			}
			else{
				newgrid.m_grid[i][j].bAP = grid2.m_grid[i][j].bAP;
				newgrid.m_grid[i][j].bNotValid = grid2.m_grid[i][j].bNotValid;
				newgrid.m_grid[i][j].iExpVal = grid2.m_grid[i][j].iExpVal;
				//newgrid.m_grid[i][j].iPower = grid2.m_grid[i][j].iPower;
				//newgrid.m_grid[i][j].fSpeed = grid2.m_grid[i][j].fSpeed;
				//newgrid.m_grid[i][j].iAP = grid2.m_grid[i][j].iAP;
				//newgrid.m_grid[i][j].iDistance = grid2.m_grid[i][j].iDistance;
			}
		}
	}
	// AP�ԍ��������Ⴎ����ɂȂ�������v�Z������
	for( int j=1; j<=newgrid.m_iGridY; j++ ){
		for( int i=1; i<=newgrid.m_iGridX; i++ ){
			if( newgrid.m_grid[i][j].bAP ){
				iAPCnt++;
				newgrid.m_grid[i][j].iAP = iAPCnt;
			}
		}
	}

	// �ˑR�ψفI�I
	// ���ۂ�AP�̐���菭�Ȃ�������K���ɑ��₷
	while( iAPCnt < g_iTotalAP ){
		BOOL bFinish = FALSE;
		while( bFinish != TRUE ){
			INT iX = 0;
			INT iY = 0;

			iX = GetRandomInt(  newgrid.m_iGridX );
			iY = GetRandomInt(  newgrid.m_iGridY );
			if( newgrid.m_grid[iX][iY].bAP != TRUE ){
				// �O���b�h�f�[�^���Z�b�g
				newgrid.m_grid[iX][iY].bAP = TRUE;
				iAPCnt++;
				bFinish = TRUE;
				break;
			}
		}
	}
	// ���ۂ�AP�̐���葽��������K���Ɍ��炷
	while( iAPCnt > g_iTotalAP ){
		BOOL bFinish = FALSE;
		while( bFinish != TRUE ){
			int iDel = 0;
			iDel = GetRandomInt( iAPCnt );
			for( int j=1; j<=newgrid.m_iGridY; j++ ){
				for( int i=1; i<=newgrid.m_iGridX; i++ ){
					if( newgrid.m_grid[i][j].iAP == iDel ){
						newgrid.m_grid[i][j].bAP = FALSE;
						newgrid.m_grid[i][j].iAP = 0;
						iAPCnt--;
						bFinish = TRUE;
						break;
					}
				}
				if( bFinish != FALSE ){
					break;
				}
			}
		}
	}
	// AP�ԍ��������Ⴎ����ɂȂ�������܂��v�Z������...
	iAPCnt =0;
	for( int j=1; j<=newgrid.m_iGridY; j++ ){
		for( int i=1; i<=newgrid.m_iGridX; i++ ){
			if( newgrid.m_grid[i][j].bAP ){
				iAPCnt++;
				newgrid.m_grid[i][j].iAP = iAPCnt;
			}
		}
	}

	// ���x��AP�f�[�^���쐬�����
	newap->Init( g_iTotalAP );
	for( int j=1; j<=newgrid.m_iGridY; j++ ){
		for( int i=1; i<=newgrid.m_iGridX; i++ ){
			if( newgrid.m_grid[i][j].bAP ){
				// AP�̃f�[�^���Z�b�g
				newap->m_ap[newgrid.m_grid[i][j].iAP].iPosX = i;
				newap->m_ap[newgrid.m_grid[i][j].iAP].iPosY = j;
				newap->m_ap[newgrid.m_grid[i][j].iAP].iTotalExpVal = 0;
				newap->m_ap[newgrid.m_grid[i][j].iAP].iChannel = 0;
				newap->m_ap[newgrid.m_grid[i][j].iAP].bNear = FALSE;
			}
		}
	}

#if DEBUG_PRINT
	CCSVFile csvfile;
	csvfile.Init();
	grid1.WriteCSVFile( csvfile.m_hFile );
	csvfile.Init();
	grid2.WriteCSVFile( csvfile.m_hFile );
	csvfile.Init();
	newgrid.WriteCSVFile( csvfile.m_hFile );
#endif
	return newgrid;
}
// �Ǐ��T���@
// �ړI�F�Œ�̒ʐM���x�̍ő剻
CGrid LocalSearchAlgorithm( CGrid org_grid, CAPoint org_ap )
{
	INT iAP = 0;
	INT iCnt = 0;
	INT iMinUnCoverArea = 10000;
	BOOL bChange = FALSE;
	BOOL bRet = FALSE;
	BOOL bTabuList = FALSE;
	BOOL bCover = FALSE;
	double dFinalMinSpeed = 0;

	// Class
	CGrid		new_grid;
	CAPoint new_ap;
	CGrid		temp_grid;
	CAPoint temp_ap;

	new_grid.Init( org_grid.m_iGridX, org_grid.m_iGridY );
	temp_grid.Init( org_grid.m_iGridX, org_grid.m_iGridY );

	// AP���J�o�[���Ă���̈���v�Z
	CalcCoverArea( org_grid, org_ap );

#if DEBUG_PRINT
	org_grid.WriteCSVFile( hFile );
#endif
	// ��1�i�K�@�J�o�[��ڎw��
	while(  iCnt < 100 ){
		for( int iLoop=1; iLoop<=org_ap.m_iTotalAP; iLoop++ ){
			// �K�v��AP����������
			if( org_ap.m_ap[iLoop].bNear != TRUE ){
				continue;
			}
			for( int iPos=0; iPos<5; iPos++ ){
				// �ʒu��ς���
				if( org_ap.m_ap[iLoop].iPosX + iXList[iPos]  <= 0  ||
					org_ap.m_ap[iLoop].iPosX + iXList[iPos]  > org_grid.m_iGridX  ||
					org_ap.m_ap[iLoop].iPosY + iYList[iPos]  <= 0  ||
					org_ap.m_ap[iLoop].iPosY + iYList[iPos]  > org_grid.m_iGridY ){
					// �t�B�[���h�O
					continue;
				}
				// ��Q�����z�u����Ă���ꍇ
				if( org_grid.m_grid[org_ap.m_ap[iLoop].iPosX+iXList[iPos]][org_ap.m_ap[iLoop].iPosY+iYList[iPos]].bNotValid == TRUE ){
					continue;
				}
				// ���ł�AP���z�u����Ă���ꍇ
				if( org_grid.m_grid[org_ap.m_ap[iLoop].iPosX+iXList[iPos]][org_ap.m_ap[iLoop].iPosY+iYList[iPos]].bAP == TRUE ){
					continue;
				}
#ifdef TABU_LIST
				// �^�u�[�Ȃ�̓_����
				if( CheckTabuList( org_ap.m_ap[iLoop].iPosX + iXList[iPos], org_ap.m_ap[iLoop].iPosY + iYList[iPos] ) != TRUE ){
					continue;
				}
#endif
				// �z�u���Ă݂悤
				new_grid.CopyGridData( org_grid );
				new_ap.CopyAPData( org_ap );

				// �ʒu��ς��Ă݂܂�
				new_grid.m_grid[new_ap.m_ap[iLoop].iPosX][new_ap.m_ap[iLoop].iPosY].bAP = FALSE; // �O�̂�����
				new_grid.m_grid[new_ap.m_ap[iLoop].iPosX + iXList[iPos]][new_ap.m_ap[iLoop].iPosY + iYList[iPos]].bAP = TRUE; // ���炷

				new_ap.m_ap[iLoop].iPosX += iXList[iPos];
				new_ap.m_ap[iLoop].iPosY += iYList[iPos];
				
				// AP���J�o�[���Ă���̈���v�Z
				//�@�{���m�C�}�[�S�T���@���g�p
				CalcCoverArea( new_grid, new_ap );

				// �J�o�[����
				if( ( bCover = new_grid.CheckCover() ) == TRUE ){
					org_grid.CopyGridData( new_grid );
					org_ap.CopyAPData( new_ap );
					goto SPEEDUP_FLOW;
				}
				// �J�o�[���Ă�Ƃ����Ȃ�����o���Ƃ�
				if( new_grid.m_iUnCoverArea < iMinUnCoverArea ){
					iMinUnCoverArea = new_grid.m_iUnCoverArea;
					temp_grid.CopyGridData( new_grid );
					temp_ap.CopyAPData( new_ap );
					bChange = TRUE;
				}
				else{
#ifdef TABU_LIST
					// �^�u�[���X�g�ɒǉ�
					AddTabuList( apdata[iLoop].iPosX, apdata[iLoop].iPosY );
#endif
				}
			}
			// ����AP�ɍs���Ă݂悤
		}
		// ���낢�����ă_������������ꉞ�Ƃ��Ƃ����z�ɂ��悤
		if( bChange ){
			org_grid.CopyGridData( temp_grid );
			org_ap.CopyAPData( temp_ap );
			CalcCoverArea( org_grid, org_ap );
		}
		iMinUnCoverArea = 10000;
		iCnt++;
	}
	if( bCover != TRUE ){
		//goto END_ALGORITHM;
	}


// ��Q�i�K�@�@�ʐM���x�����ڎw��
SPEEDUP_FLOW:
#if DEBUG_PRINT
	org_grid.WriteCSVFile( hFile );
#endif

	bChange = FALSE;
	iCnt = 0;
	while(  iCnt < 100 ){
		for( int iLoop=1; iLoop<=org_ap.m_iTotalAP; iLoop++ ){
			for( int iPos=0; iPos<5; iPos++ ){
				// �ʒu��ς���
				if( org_ap.m_ap[iLoop].iPosX + iXList[iPos]  <= 0  ||
					org_ap.m_ap[iLoop].iPosX + iXList[iPos]  > org_grid.m_iGridX  ||
					org_ap.m_ap[iLoop].iPosY + iYList[iPos]  <= 0  ||
					org_ap.m_ap[iLoop].iPosY + iYList[iPos]  > org_grid.m_iGridY ){
						// �t�B�[���h�O
						continue;
				}
				// ���ł�AP���z�u����Ă���ꍇ
				if( org_grid.m_grid[org_ap.m_ap[iLoop].iPosX+iXList[iPos]][org_ap.m_ap[iLoop].iPosY+iYList[iPos]].bAP == TRUE ){
					continue;
				}
#ifdef TABU_LIST
				// �^�u�[�Ȃ�̓_����
				if( CheckTabuList( g_org_apdata[iLoop].iPosX + iXList[iPos], g_org_apdata[iLoop].iPosY + iYList[iPos] ) != TRUE ){
					continue;
				}
#endif
				// �z�u���Ă݂悤
				new_grid.CopyGridData( org_grid );
				new_ap.CopyAPData( org_ap );

				// �ʒu��ς��Ă݂܂�
				new_grid.m_grid[new_ap.m_ap[iLoop].iPosX][new_ap.m_ap[iLoop].iPosY].bAP = FALSE; // �O�̂�����
				new_grid.m_grid[new_ap.m_ap[iLoop].iPosX + iXList[iPos]][new_ap.m_ap[iLoop].iPosY + iYList[iPos]].bAP = TRUE; // ���炷

				new_ap.m_ap[iLoop].iPosX += iXList[iPos];
				new_ap.m_ap[iLoop].iPosY += iYList[iPos];
				
				// AP���J�o�[���Ă���̈���v�Z
				//�@�{���m�C�}�[�S�T���@���g�p
				CalcCoverArea( new_grid, new_ap );
				
				// �J�o�[����
				if( ( bCover = new_grid.CheckCover() ) == TRUE ){
					// �Œ�ʐM���x�̌v�Z
					double dSpeed = CalcSpeed( new_grid, new_ap );
					if( dSpeed > dFinalMinSpeed ){
						dFinalMinSpeed = dSpeed;
						temp_grid.CopyGridData( new_grid );
						temp_ap.CopyAPData( new_ap );
						bChange = TRUE;
					}
					else{
#ifdef TABU_LIST
						// �^�u�[���X�g�ɒǉ�
						AddTabuList( apdata[iLoop].iPosX, apdata[iLoop].iPosY );
#endif
					}
				}
				else{
#ifdef TABU_LIST
					// �^�u�[���X�g�ɒǉ�
					AddTabuList( apdata[iLoop].iPosX, apdata[iLoop].iPosY );
#endif
				}
			}
			// ����AP�ɍs���Ă݂悤
		}
		// ���낢�����ă_������������ꉞ�Ƃ��Ƃ����z�ɂ��悤
		if( bChange ){
			org_grid.CopyGridData( temp_grid );
			org_ap.CopyAPData( temp_ap );
			CalcCoverArea( org_grid, org_ap );
		}
		iMinUnCoverArea = 10000;
		iCnt++;
	}
	org_grid.m_Speed = dFinalMinSpeed;
#if DEBUG_PRINT
	org_grid.WriteCSVFile( hFile );
#endif
	//printf( "%d,%f\r\n", bCover, dFinalMinSpeed );
	return org_grid;
}

double CalcSpeed( CGrid grid, CAPoint ap )
{
	double dMinSpeed = 10000;
	for( int j=1; j<=grid.m_iGridX; j++ ){
		for( int i=1; i<=grid.m_iGridY; i++ ){
			// ������
			ap.m_ap[grid.m_grid[i][j].iAP].iTotalExpVal = 0;
		}
	}
	for( int j=1; j<=grid.m_iGridY; j++ ){
		for( int i=1; i<=grid.m_iGridX; i++ ){
			// �l�̊��Ғl�𑫂����킹��
			ap.m_ap[grid.m_grid[i][j].iAP].iTotalExpVal += grid.m_grid[i][j].iExpVal;
		}
	}
	for( int i=1; i<=ap.m_iTotalAP; i++ ){
		// �ʐM���x/�l�̊��Ғl
		if( ap.m_ap[i].iTotalExpVal != 0 && ( AP_POWER / ap.m_ap[i].iTotalExpVal ) < dMinSpeed ){
			dMinSpeed = (double)AP_POWER / (double)ap.m_ap[i].iTotalExpVal;
		}
	}
	return dMinSpeed;
}

VOID AddTabuList( INT iX, INT iY )
{
	if( g_iTabuNum > 10 ){
		g_iTabuNum = 1;
	}
	g_TabuList[g_iTabuNum].iPosX = iX;
	g_TabuList[g_iTabuNum].iPosY = iY;
	g_iTabuNum++;
}

BOOL CheckTabuList( INT iX, INT iY ){
	for( int i=1; i<=10; i++ ){
		if( g_TabuList[i].iPosX == iX && g_TabuList[i].iPosY == iY ){
			return FALSE;
		}
	}
	return TRUE;
}

VOID CalcCoverArea( CGrid grid, CAPoint ap )
{
	double dMinDis =10000;
	double dX;
	double dY;
	double dDis;

	INT iMinAP = -1;
	for( int j=1; j<=grid.m_iGridY; j++ ){
		for( int i=1; i<=grid.m_iGridX; i++ ){
			dMinDis =10000;
			iMinAP = 0;

			for( int iap=1; iap<=ap.m_iTotalAP; iap++ ){
				dX = ap.m_ap[iap].iPosX - i;
				dY = ap.m_ap[iap].iPosY - j;
				dDis = sqrt( dX*dX+dY*dY );
				if( dDis < dMinDis ){
					dMinDis = dDis;
					iMinAP = iap;
				}
			}
			// �d�g�͈̔͊O�ł������ꍇ�J�o�[����Ă��Ȃ�
			if( dMinDis < (g_APPower/g_iGridSize) ){
				// �O���b�h�Ɉ�ԋ����̋߂�AP�̏���^����
				grid.m_grid[i][j].iAP = iMinAP;
				//ap[iMinAP].bNear = FALSE;
			}
			else{
				grid.m_grid[i][j].iAP = 0;
				ap.m_ap[iMinAP].bNear = TRUE;
			}
		}
	}
}

VOID RandomSetAPToGrid( CGrid grid, CAPoint ap )
{
	INT iX = 0;
	INT iY = 0;

	for( int i=1; i<=ap.m_iTotalAP; i++ ){
		iX = GetRandomInt(  grid.m_iGridX );
		iY = GetRandomInt(  grid.m_iGridY );
		if( grid.m_grid[iX][iY].bAP != TRUE && grid.m_grid[iX][iY].bNotValid != TRUE ){
				
			// �O���b�h�f�[�^���Z�b�g
			grid.m_grid[iX][iY].bAP = TRUE;

			// AP�̃f�[�^���Z�b�g
			ap.m_ap[i].iPosX = iX;
			ap.m_ap[i].iPosY = iY;
			ap.m_ap[i].iTotalExpVal = 0;
			ap.m_ap[i].iChannel = 0;
			ap.m_ap[i].bNear = FALSE;
		}
		else{
			// �����Ȃ��������蒼��
			i--;
		}
	}
}

// 1~���͂������l�̗�����Ԃ�
INT GetRandomInt( INT iNum )
{
	INT		iRand = 0;

	iRand = rand()%iNum + 1;

	return iRand;
}

#if 0
// CSV�t�@�C����ǂݍ���
BOOL ReadCSVFile( PCHAR pCSVPath )
{
	HANDLE	hFile = NULL;
	DWORD		dwSize = 0;
	DWORD		dwRead = 0;
	PCHAR		pBuf = NULL;
	PCHAR		pCur;
	BOOL		bRet = FALSE;
	INT			i = 1;
	INT			j = 1;

	// �t�@�C�����݊m�F
	if( GetFileAttributes( pCSVPath ) != (DWORD)-1 ){
		// �t�@�C�����J��
		hFile = CreateFile( pCSVPath,  GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
		if( hFile != INVALID_HANDLE_VALUE ){
			dwSize = GetFileSize( hFile, NULL );
			pBuf = (PCHAR)LocalAlloc( LMEM_ZEROINIT, dwSize );
			if( pBuf != NULL ){
				ReadFile( hFile, pBuf, dwSize, &dwRead, NULL );
				if( pBuf[0] != 0x00  && dwRead != 0 ){
					pCur = pBuf;
					while( *pCur != '\0' ){
						if( *pCur == ',' ){
							pCur++;
						}
						else if( *pCur == '\r' ){
							pCur++;
						}
						else if( *pCur == '\n' ){
							pCur++;
							j++;
							g_iFieldSizeX = i - 1;
							i = 1;
							if( strstr( pCur, "," ) == NULL ){
								g_iFieldSizeY = j - 1;
								break;
							}
						}
						else{
							g_org_griddata[i][j].iExpVal = atoi( pCur );
							pCur++;
							i++;
						}
					}
					bRet = TRUE;
				}
				LocalFree( pBuf );
			}
		}
	}
	return bRet;
}
#endif