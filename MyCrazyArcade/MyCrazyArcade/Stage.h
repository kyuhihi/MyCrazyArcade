#pragma once
#include "Scene.h"
class CStage : public CScene
{
public:
	CStage();
	virtual ~CStage();

public:
	virtual void Initialize(void) override;
	virtual void Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
private:
	void SetBlock();
	void UpdateUIRect();
private:
	RECT	m_Main_Needle;
	float	m_fNeedleCX;
	float	m_fNeedleCY;

	bool	m_bGameLose;
	bool	m_bGameWin;


	INFO	m_tInfo;	//¸¶¿ì½º
	WCHAR	m_MousePoint[35];

	DWORD	m_MusicStartTime;
	bool	m_bMusicStart;

	bool	m_bResult;

	int		m_Lose_CX;
	int		m_Lose_CY;
	int		m_Lose_Left;
	int		m_Lose_top;
	int		m_Lose_right;
	int		m_Lose_bottom;


	int		m_Win_CX;
	int		m_Win_CY;
	int		m_Win_Left;
	int		m_Win_top;
	int		m_Win_right;
	int		m_Win_bottom;

	int		m_MiddleSpotX;
	int		m_MiddleSpotY;

};

