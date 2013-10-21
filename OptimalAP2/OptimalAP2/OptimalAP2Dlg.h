
// OptimalAP2Dlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "afxwin.h"
#include "OptimalAP.h"

// COptimalAP2Dlg �_�C�A���O
class COptimalAP2Dlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	COptimalAP2Dlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
	enum { IDD = IDD_OPTIMALAP2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// ����
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
