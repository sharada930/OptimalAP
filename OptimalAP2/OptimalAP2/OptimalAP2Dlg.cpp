
// OptimalAP2Dlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "OptimalAP2.h"
#include "OptimalAP2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT DrawThread( LPVOID lpParam );

// COptimalAP2Dlg ダイアログ

COptimalAP2Dlg::COptimalAP2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COptimalAP2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COptimalAP2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PICT, m_pict);
}

BEGIN_MESSAGE_MAP(COptimalAP2Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_EXIT, &COptimalAP2Dlg::OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_EXECUTE, &COptimalAP2Dlg::OnBnClickedBtnExecute)
	ON_BN_CLICKED(IDC_BTN_AP_10, &COptimalAP2Dlg::OnBnClickedBtnAp10)
	ON_BN_CLICKED(IDC_BTN_AP_20, &COptimalAP2Dlg::OnBnClickedBtnAp20)
	ON_BN_CLICKED(IDC_BTN_AP_30, &COptimalAP2Dlg::OnBnClickedBtnAp30)
	ON_BN_CLICKED(IDC_BTN_HUM_300, &COptimalAP2Dlg::OnBnClickedBtnHum300)
	ON_BN_CLICKED(IDC_BTN_HUM_500, &COptimalAP2Dlg::OnBnClickedBtnHum500)
	ON_BN_CLICKED(IDC_BTN_HUM_1000, &COptimalAP2Dlg::OnBnClickedBtnHum1000)
	ON_BN_CLICKED(IDC_BTN_SAVE, &COptimalAP2Dlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_CHK_GA, &COptimalAP2Dlg::OnBnClickedChkGA)
	ON_BN_CLICKED(IDC_CHK_OBSTACLE, &COptimalAP2Dlg::OnBnClickedChkObstacle)
	ON_BN_CLICKED(IDC_BTN_DRAW, &COptimalAP2Dlg::OnBnClickedBtnDraw)
END_MESSAGE_MAP()


// COptimalAP2Dlg メッセージ ハンドラー

BOOL COptimalAP2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。

	m_bDraw = FALSE;

	// iniファイルから設定を読み込む
	LoadIniFile();

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void COptimalAP2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// ここから
		//if( m_data.bChange ){
		OnGridDraw();
		//}
		// ここまで
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR COptimalAP2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



// 終了ボタン
void COptimalAP2Dlg::OnBnClickedBtnExit()
{
	OnCancel();
}

// 実行ボタン
void COptimalAP2Dlg::OnBnClickedBtnExecute()
{
	CButton *chkMode;


	// コントロールから内容を取り出す
	GetDlgItemText( IDC_EDIT_INPUT, m_data.cInputPath, sizeof( m_data.cInputPath ) );
	GetDlgItemText( IDC_EDIT_OUTPUT, m_data.cOutputPath, sizeof( m_data.cOutputPath ) );
	if( m_data.cInputPath[0] == 0x00 || m_data.cOutputPath[0] == 0x00 ){
		ErrorMessageBox( "WorkPath Error", MB_OK );
		goto EXIT_EXE;
	}
	m_data.iHuman = GetDlgItemInt( IDC_EDIT_HUMAN, 0, TRUE );
	m_data.iAP = GetDlgItemInt( IDC_EDIT_AP, 0, TRUE );
	m_data.iObstacle = GetDlgItemInt( IDC_EDIT_OBSTACLE, 0, TRUE );
	m_data.iGridX = GetDlgItemInt( IDC_EDIT_GRIDX, 0, TRUE );
	m_data.iGridY = GetDlgItemInt( IDC_EDIT_GRIDY, 0, TRUE );
	m_data.iGene = GetDlgItemInt( IDC_EDIT_GENE, 0, TRUE );
	m_data.iSample = GetDlgItemInt( IDC_EDIT_SAMPLE, 0, TRUE );

	// Obstacle Mode
	chkMode = ( CButton* )GetDlgItem( IDC_CHK_OBSTACLE );
	if( chkMode->GetCheck() == 1 ){
		m_data.bObs  = TRUE;
	}
	else{
		m_data.bObs = FALSE;
	}
	// GUI Mode
	chkMode = ( CButton* )GetDlgItem( IDC_CHK_GUI);
	if( chkMode->GetCheck() == 1 ){
		m_data.bGUI  = TRUE;
	}
	else{
		m_data.bGUI = FALSE;
	}

	// エラーチェック...

	// 描画データ処理スレッド起動
	if( m_data.bGUI ){
		m_bStop = FALSE;
		AfxBeginThread( DrawThread, this );
	}

	// 遺伝的アルゴリズム
	GeneticAlgorithm( &m_data ); 

	// 描画スレッド終了
	if( m_data.bGUI ){
		m_bStop = TRUE;
	}

	ErrorMessageBox( "GA Succeed", MB_OK );

EXIT_EXE:
	;
}

int COptimalAP2Dlg::ErrorMessageBox( LPCTSTR lpText, UINT uType ){
	return MessageBox( lpText, "OptimalAP2", uType );
}

void COptimalAP2Dlg::AddTailYen( PCHAR lpStr ){
	 int iSize = lstrlen( lpStr );
	if( lpStr[ iSize ] != '\\' ){
		lpStr[ iSize ] = '\\';
		lpStr[ iSize+1 ] = 0x00;
	}
}

BOOL COptimalAP2Dlg::LoadIniFile(){
	char		cIniPath[MAX_PATH];
	char		cInputPath[MAX_PATH];
	char		cOutputPath[MAX_PATH];
	int			iHuman;
	int			iAP;
	int			iObstacle;
	int			iGridX;
	int			iGridY;
	int			iGene;
	int			iSample;
	char		cGUI[16];
	char		cGA[16];
	char		cObs[16];
	CButton *chkGUI;
	CButton *chkGA;
	CButton *chkObs;

	GetCurrentDirectory( sizeof( cIniPath ), cIniPath );
	AddTailYen( cIniPath );
	lstrcat( cIniPath, "Settings.ini" );
	// 存在確認
	if( GetFileAttributes( cIniPath ) != INVALID_FILE_ATTRIBUTES ){
		// 入出力パス取得
		memset( cInputPath, 0x00, sizeof( cInputPath ) );
		memset( cOutputPath, 0x00, sizeof( cOutputPath ) );
		GetPrivateProfileString( "WORKPATH", "INPUT", "", cInputPath, sizeof( cInputPath ), cIniPath );
		GetPrivateProfileString( "WORKPATH", "OUTPUT", "", cOutputPath, sizeof( cOutputPath ), cIniPath );
		// 人の期待値取得
		iHuman = GetPrivateProfileInt( "SETTINGS", "HUMAN", 0, cIniPath );
		// APの数取得
		iAP = GetPrivateProfileInt( "SETTINGS", "AP", 0, cIniPath );
		// Obstacleの数
		iObstacle = GetPrivateProfileInt( "SETTINGS", "OBSTACLE", 0, cIniPath );
		// GridX取得
		iGridX = GetPrivateProfileInt( "SETTINGS", "GRIDX", 0, cIniPath );
		// GridY取得
		iGridY = GetPrivateProfileInt( "SETTINGS", "GRIDY", 0, cIniPath );
		// GENE
		iGene = GetPrivateProfileInt( "SETTINGS", "GENE", 0, cIniPath );
		// SAMPLE
		iSample = GetPrivateProfileInt( "SETTINGS", "SAMPLE", 0, cIniPath );
		// GUI
		GetPrivateProfileString( "MODE", "GUI", "OFF", cGUI, sizeof( cGUI ), cIniPath );
		// GA
		GetPrivateProfileString( "MODE", "GA", "OFF", cGA, sizeof( cGA ), cIniPath );
		// 障害物
		GetPrivateProfileString( "MODE", "OBSTACLE", "OFF", cObs, sizeof( cObs ), cIniPath );

		if( cInputPath[0] != 0x00 ){
			SetDlgItemText( IDC_EDIT_INPUT, cInputPath );
		}
		if( cOutputPath[0] != 0x00 ){
			SetDlgItemText( IDC_EDIT_OUTPUT, cOutputPath );
		}
		SetDlgItemInt( IDC_EDIT_HUMAN, iHuman, TRUE );
		SetDlgItemInt( IDC_EDIT_AP, iAP, TRUE );
		SetDlgItemInt( IDC_EDIT_OBSTACLE, iObstacle, TRUE );
		SetDlgItemInt( IDC_EDIT_GRIDX, iGridX, TRUE );
		SetDlgItemInt( IDC_EDIT_GRIDY, iGridY, TRUE );
		SetDlgItemInt( IDC_EDIT_GENE, iGene, TRUE );
		SetDlgItemInt( IDC_EDIT_SAMPLE, iSample, TRUE );
		if( lstrcmp( cGUI, "ON" ) == 0 ){
			chkGUI = ( CButton* )GetDlgItem( IDC_CHK_GUI );
			chkGUI->SetCheck( 1 );
		}	
		if( lstrcmp( cGA, "ON" ) == 0 ){
			chkGA = ( CButton* )GetDlgItem( IDC_CHK_GA );
			chkGA->SetCheck( 1 );
		}
		if( lstrcmp( cObs, "ON" ) == 0 ){
			chkObs = ( CButton* )GetDlgItem( IDC_CHK_OBSTACLE );
			chkObs->SetCheck( 1 );
		}
	}
	else{
		ErrorMessageBox( "Settings File Not Found", MB_OK );
		return FALSE;
	}
	return TRUE;
}

void COptimalAP2Dlg::OnBnClickedBtnAp10()
{
	SetDlgItemInt( IDC_EDIT_AP, 10, TRUE );
}

void COptimalAP2Dlg::OnBnClickedBtnAp20()
{
	SetDlgItemInt( IDC_EDIT_AP, 20, TRUE );
}

void COptimalAP2Dlg::OnBnClickedBtnAp30()
{
	SetDlgItemInt( IDC_EDIT_AP, 30, TRUE );
}

void COptimalAP2Dlg::OnBnClickedBtnHum300()
{
	SetDlgItemInt( IDC_EDIT_HUMAN, 300, TRUE );
}

void COptimalAP2Dlg::OnBnClickedBtnHum500()
{
	SetDlgItemInt( IDC_EDIT_HUMAN, 500, TRUE );
}

void COptimalAP2Dlg::OnBnClickedBtnHum1000()
{
	SetDlgItemInt( IDC_EDIT_HUMAN, 1000, TRUE );
}

// 現設定をINIファイルにSAVEする
void COptimalAP2Dlg::OnBnClickedBtnSave()
{
}

// 遺伝的アルゴリズムのディセーブル
void COptimalAP2Dlg::OnBnClickedChkGA()
{
	CButton *chkGA = (CButton*)GetDlgItem( IDC_CHK_GA );
	CEdit *editGene = (CEdit*)GetDlgItem( IDC_EDIT_GENE );
	CEdit *editSample = (CEdit*)GetDlgItem( IDC_EDIT_SAMPLE );
		
	if( chkGA->GetCheck() == 1 ){
		editGene->EnableWindow(1);
		editSample->EnableWindow(1);
	}
	else{
		editGene->EnableWindow(0);
		editSample->EnableWindow(0);
	}
}

// 障害物のディセーブル
void COptimalAP2Dlg::OnBnClickedChkObstacle()
{
	CButton *chkObs = (CButton*)GetDlgItem( IDC_CHK_OBSTACLE );
	CEdit *editObs = (CEdit*)GetDlgItem( IDC_EDIT_OBSTACLE );

	if( chkObs->GetCheck() == 1 ){
		editObs->EnableWindow(1);
	}
	else{
		editObs->EnableWindow(0);
	}
}

UINT DrawThread( LPVOID lpParam )
{
	COptimalAP2Dlg* pDlg = ( COptimalAP2Dlg* )lpParam;
	HANDLE hSendEvent = NULL;
	HANDLE hRevEvent = NULL;
	HANDLE hMap = NULL;

	// GAからの通知用
	hSendEvent = CreateEvent( NULL, TRUE, FALSE, "OPTIMALAP_DRAW_EVENT" );
	hMap = CreateFileMapping( NULL, NULL, PAGE_READWRITE, 0, sizeof( CGrid ), "OPTIMALAP_DRAW_MAP" );
	if( hMap != NULL && MapViewOfFile( hMap, FILE_MAP_WRITE || FILE_MAP_READ, 0, 0, 0 ) != NULL ){
		CloseHandle( hMap );
		hMap = NULL;
	}

	while( 1 ){
		// 待機処理
		WaitForSingleObject( hSendEvent, INFINITE );
		ResetEvent( hSendEvent );

		hMap = OpenFileMapping( FILE_MAP_WRITE || FILE_MAP_READ, FALSE, "OPTIMALAP_DRAW_MAP" );
		if( hMap != NULL ){
			// データもらう
			CGrid *tmp_grid;
			tmp_grid = (CGrid*)MapViewOfFile( hMap, FILE_MAP_READ, 0, 0, 0 );
			pDlg->m_drawgrid.Init( tmp_grid->m_iGridX, tmp_grid->m_iGridY );
			pDlg->m_drawgrid.CopyGridData( tmp_grid[0] );
			pDlg->m_drawgrid.m_iGridX = tmp_grid->m_iGridX;
			pDlg->m_drawgrid.m_iGridY = tmp_grid->m_iGridY;
			pDlg->m_drawgrid.m_Speed = tmp_grid->m_Speed;
			UnmapViewOfFile( tmp_grid );
			tmp_grid = NULL;

			// データ受取完了通知
			hRevEvent = OpenEvent( EVENT_MODIFY_STATE, FALSE, "OPTIMALAP_END_DRAW_EVENT" );
			SetEvent( hRevEvent );

			// 終了処理
			CloseHandle( hMap );
			hMap = NULL;
		}
		// 描画
		pDlg->m_bDraw = TRUE;
		pDlg->UpdateWindow();
		pDlg->Invalidate( FALSE );
	}

	if( hSendEvent != NULL ){
		CloseHandle( hSendEvent );
	}
	return TRUE;
}
// 描画ボタン　後々消す
void COptimalAP2Dlg::OnBnClickedBtnDraw()
{
	AfxBeginThread( DrawThread, this );
}

void COptimalAP2Dlg::OnGridDraw(){
	CDC*		pDC;
	CRect		rect;
	CPen		pen_red( PS_SOLID,1,RGB(255,0,0) );
	CPen		pen_black( PS_SOLID, 1, RGB(0,0,0) );
	CBrush		brush_white = (RGB(255,255,255));
	CBrush		brush_gray = (RGB(50,50,50));
	CBrush		brush_strong[] = {RGB(255,255,255),
												  /*赤*/(RGB(255,0,0)), /*緑*/(RGB(0,255,0)), /*青*/(RGB(0,0,255)),
												  /*？*/(RGB(255,255,0)), /*？*/(RGB(0,255,255)), /*？*/(RGB(255,0,255)) };
	CBrush		brush_weak[] = { RGB(255,255,255),
												  /*赤*/(RGB(255,200,200)), /*緑*/(RGB(200,255,200)), /*青*/(RGB(200,200,255)),
												  /*？*/(RGB(255,255,200)), /*？*/(RGB(200,255,255)), /*？*/(RGB(255,200,255)) };
	CPen*		oldpen;
	CBrush*	oldbr;
	CString		cstr;
	
	if( m_bDraw != FALSE ){
		// デバイスコンテキスト取得
		pDC=m_pict.GetDC();

		// デバイスコンテキストの大きさ取得
		m_pict.GetClientRect(&rect);

		// 背景白(初期ペン・初期ブラシ)
		pDC->Rectangle(&rect);

		// ペン・ブラシ変更
		oldpen = pDC->SelectObject(&pen_black);
		oldbr = pDC->SelectObject(&brush_white);
		
		// 背景透過処理
		pDC->SetBkMode( TRANSPARENT );

		// Grid描画
		int iX = 0;
		int iY = 0;
		int iDiff = 15;
		for( int j=1; j<=m_drawgrid.m_iGridY; j++ ){
			for( int i=1; i<=m_drawgrid.m_iGridX; i++ ){
				// 電波範囲
				if( m_drawgrid.m_grid[i][j].iAP >= 0 ){
					int iColor = m_drawgrid.m_grid[i][j].iAP;
					// 本来は３チャンネルのため３色だけでいいがいまのところ6色用意しておく
					pDC->SelectObject( &brush_weak[iColor] );
				}
				// APの場所
				if( m_drawgrid.m_grid[i][j].bAP != FALSE ){
					int iColor = m_drawgrid.m_grid[i][j].iAP;
					// 塗りつぶす色を変える
					pDC->SelectObject( &brush_strong[iColor] );
				}
				// 障害物
				if( m_drawgrid.m_grid[i][j].bNotValid != FALSE ){
					// 塗りつぶす色を変える
					pDC->SelectObject( &brush_gray );
				}
				// 四角形描写
				pDC->Rectangle( iX, iY, iX+iDiff, iY+iDiff );

				// 人の期待値
				if( m_drawgrid.m_grid[i][j].iExpVal > 0 ){
					CFont *miniFont;
					int iDiffFontX = 4;
					int iDiffFontY = 1;
					miniFont = new CFont;
					miniFont->CreatePointFont( 80, _T("ＭＳ Ｐゴシック") );
					pDC->SelectObject(miniFont);
					cstr.Format( "%i", m_drawgrid.m_grid[i][j].iExpVal );
					
					pDC->TextOutA( iX+iDiffFontX, iY+iDiffFontY, cstr );
				}

				iX += iDiff;
			}
			iX = 0;
			iY += iDiff;
		}

		// 円描画
		//pDC->Ellipse(10,10,100,100);

		// 正方形描写
		//pDC->MoveTo( 10, 10 );
		//pDC->Rectangle( 0,0,20,20 );

		// 初期化
		pDC->SelectObject(oldpen);
		pDC->SelectObject(oldbr);

		// 解放処理
		m_pict.ReleaseDC( pDC );
	}
}