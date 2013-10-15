#include "stdafx.h"
#include "CSVFile.h"

CCSVFile::CCSVFile()
{
}
CCSVFile::~CCSVFile()
{
}

int g_iCnt = 0;
// �t�@�C���쐬
void CCSVFile::Init( LPCTSTR csvpath)
{	
	char	cPath[MAX_PATH];

	memset( cPath, 0x00, sizeof( cPath ) );
	if( csvpath[0] != 0x00 ){
		sprintf_s( cPath, "%s\\OptimalAP%i.csv", csvpath, g_iCnt );
	}
	else{
		sprintf_s( cPath, "C:\\Temp\\OptimalAP%i.csv", g_iCnt );
	}
	// ���������Ă�������Ƃ�
	if( m_hFile != NULL ){
		CloseHandle( m_hFile );
		m_hFile = NULL;
	}
	// �O�̃t�@�C������������ꉞ�����Ƃ�
	if( GetFileAttributes( cPath ) != INVALID_FILE_ATTRIBUTES ){
		DeleteFile( cPath );
	}
	// �t�@�C���쐬
	m_hFile = CreateFile( cPath,  GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if( m_hFile == INVALID_HANDLE_VALUE ){
		m_hFile = NULL;
	}
	g_iCnt++;
}

// �t�@�C���N���[�Y
void CCSVFile::Close()
{
	if( m_hFile != NULL ){
		CloseHandle( m_hFile );
		m_hFile = NULL;
	}
}

