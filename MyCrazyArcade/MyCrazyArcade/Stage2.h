#pragma once
#include "Scene.h"

class CStage2 : public CScene
{
public:
	CStage2();
	~CStage2();

	// CScene을(를) 통해 상속됨
	virtual void Initialize(void) override;
	virtual void Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;

private:
	void SetBlock();
	void UpdateUIRect();
	void BombEffect();
private:
	RECT	m_Main_Needle;
	float	m_fNeedleCX;
	float	m_fNeedleCY;

	RECT	m_Main_Zelda_Needle_Rect;
	float	m_fZelda_NeedleCX;
	float	m_fZelda_NeedleCY;
	RECT	m_ZeldaNeedleDrawRect;

	bool	m_bFallingComplete;
	POINT	m_BossFallingPoint;
	RECT	m_tBossFallingEffect;
	float	m_fSpearDistance;
	bool	m_bBossInitialComplete;

	int		m_Main_ShieldCX;
	int		m_Main_ShieldiX;
	int		m_Main_ShieldiY;

	bool	m_bGameLose;
	bool	m_bGameWin;
	bool	m_bNotFinish;


	INFO	m_tInfo;	//마우스
	WCHAR	m_MousePoint[35];

	DWORD	m_MusicStartTime;
	bool	m_bMusicStart;

	int		m_Lose_CX;
	int		m_Lose_CY;
	int		m_Lose_Left;
	int		m_Lose_top;
	int		m_Lose_right;
	int		m_Lose_bottom;

	bool	m_bFinalSound;

	int		m_Win_CX;
	int		m_Win_CY;
	int		m_Win_Left;
	int		m_Win_top;
	int		m_Win_right;
	int		m_Win_bottom;

	int		m_MiddleSpotX;
	int		m_MiddleSpotY;

	

	RECT	m_tAppearRect;
	DWORD	m_AppearTime;
	bool	m_bBossAppear;

	TCHAR*		m_pFrameKey;
};

