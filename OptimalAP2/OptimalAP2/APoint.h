#include <windows.h>
#pragma once
class CAPoint
{
	// Functions
public:
	CAPoint();
	~CAPoint(void);

	void Init( int iAP );
	void Close();
	void CopyAPData( CAPoint srcap );
	
	// Variables
typedef struct _AP_DATA{
	INT			iPosX;		// X���W
	INT			iPosY;		// Y���W
	INT			iTotalExpVal; // �l�̊��Ғl�̍��v�l
	INT			iChannel;	// AP�̃`���l��
	BOOL		bNear;		// �d�g�͈͊O�ł͂��邪�J�o�[����Ă��Ȃ��O���b�h���߂��ɂ���AP
}AP_DATA,*PAP_DATA;

	int m_iTotalAP;
	AP_DATA *m_ap;

};

