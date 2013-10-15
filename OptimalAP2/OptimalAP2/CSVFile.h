#include <windows.h>

#pragma once
class CCSVFile
{
	// Functions
public:
	CCSVFile();
	~CCSVFile();
	void Init( LPCTSTR path );
	void Close();

private:

	// Variables
public:
	HANDLE m_hFile;
private:
	

};