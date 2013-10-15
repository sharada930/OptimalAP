
// OptimalAP2.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル

// COptimalAP2App:
// このクラスの実装については、OptimalAP2.cpp を参照してください。
//

class COptimalAP2App : public CWinApp
{
public:
	COptimalAP2App();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern COptimalAP2App theApp;