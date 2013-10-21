
// OptimalAP2Dlg.h : ヘッダー ファイル
//

#pragma once
#include "afxwin.h"
#include "OptimalAP.h"

// COptimalAP2Dlg ダイアログ
class COptimalAP2Dlg : public CDialogEx
{
// コンストラクション
public:
	COptimalAP2Dlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
	enum { IDD = IDD_OPTIMALAP2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 自作
	int ErrorMessageBox( LPCTSTR lpText, UINT uType );
	BOOL LoadIniFile();
	void AddTailYen( PCHAR lpStr );
	SETTING_DATA m_data;
	BOOL m_bStop;
	BOOL m_bDraw;
	void OnGridDraw();
	CGrid m_drawgrid;

	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnBnClickedBtnExecute();
	afx_msg void OnBnClickedBtnAp10();
	afx_msg void OnBnClickedBtnAp20();
	afx_msg void OnBnClickedBtnAp30();
	afx_msg void OnBnClickedBtnHum300();
	afx_msg void OnBnClickedBtnHum500();
	afx_msg void OnBnClickedBtnHum1000();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedChkGA();
	afx_msg void OnBnClickedChkObstacle();
	afx_msg void OnBnClickedBtnDraw();
	CStatic m_pict;
};
