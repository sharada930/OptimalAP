#include "stdafx.h"
#include "Grid.h"

#define CSVPATH "C:\\Temp\\Grid.csv"

CGrid::CGrid()
{

}
CGrid::~CGrid(void)
{
}

// Gridの初期化処理
void CGrid::Init( int iX, int iY )
{
	m_iGridX = iX;
	m_iGridY = iY;

	// 配列生成
	m_grid = new PGRID_DATA[m_iGridX+1];
	for( int i=0; i<m_iGridX+1; i++ ) {
		m_grid[i] = new GRID_DATA[m_iGridY+1];
	}

	// 配列の初期化
	for( int j=0; j<=m_iGridY; j++ ){
		for( int i=0; i<=m_iGridX; i++ ){
			m_grid[i][j].bAP = FALSE;
			m_grid[i][j].bNotValid = FALSE;
			m_grid[i][j].iExpVal = 0;
			m_grid[i][j].iPower = 0;
			m_grid[i][j].fSpeed = 0;
			m_grid[i][j].iAP = 0;
			m_grid[i][j].iDistance = -1;
		}
	}
}

void CGrid::Close()
{
	/*
	for( int i=0; i<m_iGridX+1; i++ ) {
		delete[] m_grid[m_iGridY+1];
	}
	*/
	delete[] m_grid;
}

// 1~入力した数値の乱数を返す
INT CGrid::GetRandomInt( INT iNum )
{
	INT		iRand = 0;

	iRand = rand()%iNum + 1;

	return iRand;
}
// CSVファイルからGridデータを読み込む
/*** 注意　クラス生成の際に指定したサイズで配列を作るのでそれを超えるGridを読みこんだら飛ぶと思う　***/
BOOL CGrid::ReadGridFromCSVFile()
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
	if( GetFileAttributes( CSVPATH ) != (DWORD)-1 ){
		// ファイルを開く
		hFile = CreateFile( CSVPATH,  GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
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
							m_iGridX = i - 1;
							i = 1;
							if( strstr( pCur, "," ) == NULL ){
								m_iGridY = j - 1;
								break;
							}
						}
						else{
							m_grid[i][j].iExpVal = atoi( pCur );
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

void CGrid::WriteCSVFile( HANDLE hFile )
{
	DWORD dwWrite = 0;
	CHAR	cTemp[256];

	// 期待値
	WriteFile( hFile, "ExpVal\r\n" , lstrlen( "ExpVal\r\n" ), &dwWrite, NULL ); 
	for( int j=1; j<=m_iGridY; j++ ){
		for( int i=1; i<=m_iGridX; i++ ){
			if( m_grid[i][j].bNotValid == TRUE ){
				WriteFile( hFile, "X," , lstrlen( "X," ), &dwWrite, NULL ); 
			}
			else{
				sprintf_s( cTemp, "%i,", m_grid[i][j].iExpVal );
				WriteFile( hFile, cTemp , lstrlen( cTemp ), &dwWrite, NULL ); 
			}
		}
		WriteFile( hFile, "\r\n" , lstrlen( "\r\n" ), &dwWrite, NULL ); 
	}
	WriteFile( hFile, "\r\n" , lstrlen( "\r\n" ), &dwWrite, NULL ); 

	// AP(GRID_DATA)
	WriteFile( hFile, "AP\r\n" , lstrlen( "AP\r\n" ), &dwWrite, NULL ); 
	for( int j=1; j<=m_iGridY; j++ ){
		for( int i=1; i<=m_iGridX; i++ ){
			if( m_grid[i][j].bNotValid == TRUE ){
				WriteFile( hFile, "X," , lstrlen( "X," ), &dwWrite, NULL ); 
			}
			else if( m_grid[i][j].bAP ){
				WriteFile( hFile, "A," , lstrlen( "A," ), &dwWrite, NULL ); 
			}
			else{
				WriteFile( hFile, "0," , lstrlen( "0," ), &dwWrite, NULL ); 
			}
		}
		WriteFile( hFile, "\r\n" , lstrlen( "\r\n" ), &dwWrite, NULL ); 
	}
	WriteFile( hFile, "\r\n" , lstrlen( "\r\n" ), &dwWrite, NULL ); 

	// iAP(GRID_DATA)
	WriteFile( hFile, "iAP\r\n" , lstrlen( "iAP\r\n" ), &dwWrite, NULL ); 
	for( int j=1; j<=m_iGridY; j++ ){
		for( int i=1; i<=m_iGridX; i++ ){
			if( m_grid[i][j].bNotValid == TRUE ){
				WriteFile( hFile, "X," , lstrlen( "X," ), &dwWrite, NULL ); 
			}
			else{
				sprintf_s( cTemp, "%i,", m_grid[i][j].iAP );
				WriteFile( hFile, cTemp , lstrlen( cTemp ), &dwWrite, NULL ); 
			}
		}
		WriteFile( hFile, "\r\n" , lstrlen( "\r\n" ), &dwWrite, NULL ); 
	}
	WriteFile( hFile, "\r\n" , lstrlen( "\r\n" ), &dwWrite, NULL ); 
}
// GridDataのコピー Initいらず
VOID CGrid::CopyGridData( CGrid srcgrid )
{
	//Init( srcgrid.m_iGridX, srcgrid.m_iGridY );

	for( int j=1; j<=m_iGridY; j++ ){
		for( int i=1; i<=m_iGridX; i++ ){
			m_grid[i][j].bAP = srcgrid.m_grid[i][j].bAP;
			m_grid[i][j].bNotValid = srcgrid.m_grid[i][j].bNotValid;
			m_grid[i][j].iExpVal = srcgrid.m_grid[i][j].iExpVal;
			m_grid[i][j].iPower = srcgrid.m_grid[i][j].iPower;
			m_grid[i][j].fSpeed = srcgrid.m_grid[i][j].fSpeed;
			m_grid[i][j].iAP = srcgrid.m_grid[i][j].iAP;
			m_grid[i][j].iDistance = srcgrid.m_grid[i][j].iDistance;
		}
	}
}

// グリッドがすべてカバーできてるかチェック,人がいないとこでもいいから空きマスの数
// カバーされている　TRUE
// カバーされていない　FALSE
BOOL CGrid::CheckCover()
{
	m_iUnCoverArea = 0;
	for( int j=1; j<=m_iGridY; j++ ){
		for( int i=1; i<=m_iGridX; i++ ){
			if( m_grid[i][j].iAP == 0 && m_grid[i][j].iExpVal > 0){
				m_iUnCoverArea++;
			}
		}
	}
	if( m_iUnCoverArea > 0 ){
		return FALSE; 
	}
	return TRUE;
}

// 人の期待値をランダムに割り振る
void CGrid::RandomSetExpVal( INT iNum )
{
	// 人のいる期待値を割り振る
	for( int i=1; i<=iNum; i++ ){
		if( m_grid[GetRandomInt( m_iGridX )][GetRandomInt( m_iGridY )].bNotValid != TRUE ){
			m_grid[GetRandomInt( m_iGridX )][GetRandomInt( m_iGridY )].iExpVal += 1;
		}
		else{
			i--;
		}
	}
}

// 障害物をランダムに割り振る
void CGrid::RandomSetObstacle( INT iNum )
{
	// 障害物を割り振る
	for( int i=1; i<=iNum; i++ ){
		if( m_grid[GetRandomInt( m_iGridX )][GetRandomInt( m_iGridY )].bNotValid != TRUE ){
			m_grid[GetRandomInt( m_iGridX )][GetRandomInt( m_iGridY )].bNotValid = TRUE;
		}
		else{
			i--;
		}
	}
}