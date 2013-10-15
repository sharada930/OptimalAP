// OptimalAP.cpp : コンソール アプリケーションのエントリ ポイントを定義します。

#include "stdafx.h"
#include "OptimalAP.h"

// グローバル変数
TABU_LIST g_TabuList[10];
INT			g_iTabuNum = 1;
INT			g_iTotalAP;
INT			g_iUnCoverArea = 0;
INT			g_iRand = 1;
INT			iXList[5] = {-1,0,0,0,1};
INT			iYList[5] = {0,-1,0,1,0};


// Input Data	後々入力出来るようにする
INT		g_iGridSize	=	3;		// 一つのグリッドの一辺の長さ(m)
INT		g_APPower = 30;	  //	電波が届くの届く距離
float		g_fMinSpeed = 10000;
INT		g_iGen = 0;
#define	AP_POWER	54

BOOL CheckCoverGrid( GRID_DATA grid[50][50] );

#define DEBUG_PRINT 0
//#define TABU_LIST

// 簡易DebugPrint
void DebugGridPrint( LPCTSTR path, CGrid grid )
{
	CCSVFile csvfile;
	csvfile.Init( path );
	grid.WriteCSVFile( csvfile.m_hFile );
	csvfile.Close();
}
// 旧main関数
INT OptimalAPMain()
{
#if 0
	CGrid grid;
	CAPoint ap;

	// グリッドサイズ
	grid.Init( 30, 30 );
	// 人の期待値
	grid.RandomSetExpVal( 300 );
	// APの数
	ap.Init( 10 );
	RandomSetAPToGrid( grid, ap );

	// 局所探索アルゴリズム
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

	g_iTotalAP = data->iAP; //暫定
	{
		//---------------------1.初期集団の形成---------------------//
		CGrid temp_grid;
		CAPoint* ap = new CAPoint[child_num];
		int i = 0;
		int j = 0;
		int iLoop = 1;

		// 人の期待値、障害物は共通
		for( i=0; i<child_num; i++ ){
			// 乱数初期化処理
			srand( 1 );
			// グリッドサイズ
			child_grid[i].Init( iX, iY );
			// 障害物
			if( data->bObs ){
				child_grid[i].RandomSetObstacle( data->iObstacle );
			}
			// 人の期待値
			child_grid[i].RandomSetExpVal( iExpval );
		}
		// APの配置はランダム
		srand( 1 );
		for( i=0; i<child_num; i++ ){
			// APの数
			ap[i].Init( g_iTotalAP );
			RandomSetAPToGrid( child_grid[i], ap[i] );
		}
		//---------------------1.初期集団の形成 ここまで---------------------//

		while( iLoop <= iMaxGene){
			//---------------------2.適応度---------------------//
			for( i=0; i<child_num; i++ ){
				// 局所探索法
				parent_grid[i] = LocalSearchAlgorithm( child_grid[i], ap[i] );
			}
			// 通信速度が最も早いもの順に並び替え バブルソート
			for( i=0; i<child_num; i++ ){
				for( j=child_num - 1; j > i; j-- ){
					if( parent_grid[j-1].m_Speed < parent_grid[j].m_Speed ){
						temp_grid = parent_grid[j];
						parent_grid[j] = parent_grid[j-1];
						parent_grid[j-1] = temp_grid;
					}
				}
			}
			//---------------------2.適応度 ここまで---------------------//
			
			//Debug
			DebugGridPrint( data->cOutputPath, parent_grid[0] );
			//printf( "第%d世代　通信速度[%f]\r\n", iLoop, parent_grid[0].m_Speed );

			// 終了処理
			if( iLoop == iMaxGene ){
				break;
			}

			//---------------------3.交叉　4.突然変異---------------------//
			// 2つをランダムな線で分離し、組み合わせる
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

			// 親たちの中身を開放
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

	
	// 分離する線の箇所を決める　縦に分割
	if( grid1.m_iGridX != grid2.m_iGridX ){
		return newgrid;
	}
	iDivX = GetRandomInt( grid1.m_iGridX );
	
	// コピーしていく
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
	// AP番号がぐちゃぐちゃになったから計算し直す
	for( int j=1; j<=newgrid.m_iGridY; j++ ){
		for( int i=1; i<=newgrid.m_iGridX; i++ ){
			if( newgrid.m_grid[i][j].bAP ){
				iAPCnt++;
				newgrid.m_grid[i][j].iAP = iAPCnt;
			}
		}
	}

	// 突然変異！！
	// 実際のAPの数より少なかったら適当に増やす
	while( iAPCnt < g_iTotalAP ){
		BOOL bFinish = FALSE;
		while( bFinish != TRUE ){
			INT iX = 0;
			INT iY = 0;

			iX = GetRandomInt(  newgrid.m_iGridX );
			iY = GetRandomInt(  newgrid.m_iGridY );
			if( newgrid.m_grid[iX][iY].bAP != TRUE ){
				// グリッドデータをセット
				newgrid.m_grid[iX][iY].bAP = TRUE;
				iAPCnt++;
				bFinish = TRUE;
				break;
			}
		}
	}
	// 実際のAPの数より多かったら適当に減らす
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
	// AP番号がぐちゃぐちゃになったからまた計算し直す...
	iAPCnt =0;
	for( int j=1; j<=newgrid.m_iGridY; j++ ){
		for( int i=1; i<=newgrid.m_iGridX; i++ ){
			if( newgrid.m_grid[i][j].bAP ){
				iAPCnt++;
				newgrid.m_grid[i][j].iAP = iAPCnt;
			}
		}
	}

	// 今度はAPデータを作成するよ
	newap->Init( g_iTotalAP );
	for( int j=1; j<=newgrid.m_iGridY; j++ ){
		for( int i=1; i<=newgrid.m_iGridX; i++ ){
			if( newgrid.m_grid[i][j].bAP ){
				// APのデータをセット
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
// 局所探索法
// 目的：最低の通信速度の最大化
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

	// APがカバーしている領域を計算
	CalcCoverArea( org_grid, org_ap );

#if DEBUG_PRINT
	org_grid.WriteCSVFile( hFile );
#endif
	// 第1段階　カバーを目指す
	while(  iCnt < 100 ){
		for( int iLoop=1; iLoop<=org_ap.m_iTotalAP; iLoop++ ){
			// 必要なAPだけ動かす
			if( org_ap.m_ap[iLoop].bNear != TRUE ){
				continue;
			}
			for( int iPos=0; iPos<5; iPos++ ){
				// 位置を変える
				if( org_ap.m_ap[iLoop].iPosX + iXList[iPos]  <= 0  ||
					org_ap.m_ap[iLoop].iPosX + iXList[iPos]  > org_grid.m_iGridX  ||
					org_ap.m_ap[iLoop].iPosY + iYList[iPos]  <= 0  ||
					org_ap.m_ap[iLoop].iPosY + iYList[iPos]  > org_grid.m_iGridY ){
					// フィールド外
					continue;
				}
				// 障害物が配置されている場合
				if( org_grid.m_grid[org_ap.m_ap[iLoop].iPosX+iXList[iPos]][org_ap.m_ap[iLoop].iPosY+iYList[iPos]].bNotValid == TRUE ){
					continue;
				}
				// すでにAPが配置されている場合
				if( org_grid.m_grid[org_ap.m_ap[iLoop].iPosX+iXList[iPos]][org_ap.m_ap[iLoop].iPosY+iYList[iPos]].bAP == TRUE ){
					continue;
				}
#ifdef TABU_LIST
				// タブーなやつはダメだ
				if( CheckTabuList( org_ap.m_ap[iLoop].iPosX + iXList[iPos], org_ap.m_ap[iLoop].iPosY + iYList[iPos] ) != TRUE ){
					continue;
				}
#endif
				// 配置してみよう
				new_grid.CopyGridData( org_grid );
				new_ap.CopyAPData( org_ap );

				// 位置を変えてみます
				new_grid.m_grid[new_ap.m_ap[iLoop].iPosX][new_ap.m_ap[iLoop].iPosY].bAP = FALSE; // 前のを消す
				new_grid.m_grid[new_ap.m_ap[iLoop].iPosX + iXList[iPos]][new_ap.m_ap[iLoop].iPosY + iYList[iPos]].bAP = TRUE; // ずらす

				new_ap.m_ap[iLoop].iPosX += iXList[iPos];
				new_ap.m_ap[iLoop].iPosY += iYList[iPos];
				
				// APがカバーしている領域を計算
				//　ボロノイ図ー全探索法を使用
				CalcCoverArea( new_grid, new_ap );

				// カバー判定
				if( ( bCover = new_grid.CheckCover() ) == TRUE ){
					org_grid.CopyGridData( new_grid );
					org_ap.CopyAPData( new_ap );
					goto SPEEDUP_FLOW;
				}
				// カバーしてるとこ少ないやつを覚えとく
				if( new_grid.m_iUnCoverArea < iMinUnCoverArea ){
					iMinUnCoverArea = new_grid.m_iUnCoverArea;
					temp_grid.CopyGridData( new_grid );
					temp_ap.CopyAPData( new_ap );
					bChange = TRUE;
				}
				else{
#ifdef TABU_LIST
					// タブーリストに追加
					AddTabuList( apdata[iLoop].iPosX, apdata[iLoop].iPosY );
#endif
				}
			}
			// 次のAPに行ってみよう
		}
		// いろいろやってダメだったから一応とっといた奴にしよう
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


// 第２段階　　通信速度向上を目指す
SPEEDUP_FLOW:
#if DEBUG_PRINT
	org_grid.WriteCSVFile( hFile );
#endif

	bChange = FALSE;
	iCnt = 0;
	while(  iCnt < 100 ){
		for( int iLoop=1; iLoop<=org_ap.m_iTotalAP; iLoop++ ){
			for( int iPos=0; iPos<5; iPos++ ){
				// 位置を変える
				if( org_ap.m_ap[iLoop].iPosX + iXList[iPos]  <= 0  ||
					org_ap.m_ap[iLoop].iPosX + iXList[iPos]  > org_grid.m_iGridX  ||
					org_ap.m_ap[iLoop].iPosY + iYList[iPos]  <= 0  ||
					org_ap.m_ap[iLoop].iPosY + iYList[iPos]  > org_grid.m_iGridY ){
						// フィールド外
						continue;
				}
				// すでにAPが配置されている場合
				if( org_grid.m_grid[org_ap.m_ap[iLoop].iPosX+iXList[iPos]][org_ap.m_ap[iLoop].iPosY+iYList[iPos]].bAP == TRUE ){
					continue;
				}
#ifdef TABU_LIST
				// タブーなやつはダメだ
				if( CheckTabuList( g_org_apdata[iLoop].iPosX + iXList[iPos], g_org_apdata[iLoop].iPosY + iYList[iPos] ) != TRUE ){
					continue;
				}
#endif
				// 配置してみよう
				new_grid.CopyGridData( org_grid );
				new_ap.CopyAPData( org_ap );

				// 位置を変えてみます
				new_grid.m_grid[new_ap.m_ap[iLoop].iPosX][new_ap.m_ap[iLoop].iPosY].bAP = FALSE; // 前のを消す
				new_grid.m_grid[new_ap.m_ap[iLoop].iPosX + iXList[iPos]][new_ap.m_ap[iLoop].iPosY + iYList[iPos]].bAP = TRUE; // ずらす

				new_ap.m_ap[iLoop].iPosX += iXList[iPos];
				new_ap.m_ap[iLoop].iPosY += iYList[iPos];
				
				// APがカバーしている領域を計算
				//　ボロノイ図ー全探索法を使用
				CalcCoverArea( new_grid, new_ap );
				
				// カバー判定
				if( ( bCover = new_grid.CheckCover() ) == TRUE ){
					// 最低通信速度の計算
					double dSpeed = CalcSpeed( new_grid, new_ap );
					if( dSpeed > dFinalMinSpeed ){
						dFinalMinSpeed = dSpeed;
						temp_grid.CopyGridData( new_grid );
						temp_ap.CopyAPData( new_ap );
						bChange = TRUE;
					}
					else{
#ifdef TABU_LIST
						// タブーリストに追加
						AddTabuList( apdata[iLoop].iPosX, apdata[iLoop].iPosY );
#endif
					}
				}
				else{
#ifdef TABU_LIST
					// タブーリストに追加
					AddTabuList( apdata[iLoop].iPosX, apdata[iLoop].iPosY );
#endif
				}
			}
			// 次のAPに行ってみよう
		}
		// いろいろやってダメだったから一応とっといた奴にしよう
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
			// 初期化
			ap.m_ap[grid.m_grid[i][j].iAP].iTotalExpVal = 0;
		}
	}
	for( int j=1; j<=grid.m_iGridY; j++ ){
		for( int i=1; i<=grid.m_iGridX; i++ ){
			// 人の期待値を足し合わせる
			ap.m_ap[grid.m_grid[i][j].iAP].iTotalExpVal += grid.m_grid[i][j].iExpVal;
		}
	}
	for( int i=1; i<=ap.m_iTotalAP; i++ ){
		// 通信速度/人の期待値
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
			// 電波の範囲外であった場合カバーされていない
			if( dMinDis < (g_APPower/g_iGridSize) ){
				// グリッドに一番距離の近いAPの情報を与える
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
				
			// グリッドデータをセット
			grid.m_grid[iX][iY].bAP = TRUE;

			// APのデータをセット
			ap.m_ap[i].iPosX = iX;
			ap.m_ap[i].iPosY = iY;
			ap.m_ap[i].iTotalExpVal = 0;
			ap.m_ap[i].iChannel = 0;
			ap.m_ap[i].bNear = FALSE;
		}
		else{
			// おけなかったらやり直し
			i--;
		}
	}
}

// 1~入力した数値の乱数を返す
INT GetRandomInt( INT iNum )
{
	INT		iRand = 0;

	iRand = rand()%iNum + 1;

	return iRand;
}

#if 0
// CSVファイルを読み込む
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

	// ファイル存在確認
	if( GetFileAttributes( pCSVPath ) != (DWORD)-1 ){
		// ファイルを開く
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