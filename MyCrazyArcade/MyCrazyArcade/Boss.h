#pragma once
#include "Obj.h"
class CBoss :public CObj
{
public:
	CBoss();
	~CBoss();

public:
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
public:
	void Draw_Update_Rect();
	void Motion_Change(void);
	void Direction_Change();
	void BossAppear();

	void SetBossFalling() { m_bInitialFalling = true; };
	bool GetAppear() { return m_bAppear; }
	DWORD	GetNotDieTime() { return m_BossNotDieTime; }
	void FollowDirection();
	void Set_Minus_Hp();

private:
	void Mode_Select();

	void RunSetting();
	
	int  m_iMulJulgySetting;
	void MulJulgySetting();

	void RainingBombSpotSet();
	void RaingBomb();

	void DeletePath();
	void NailDown();
public:
	STATE	Get_Boss_State() { return m_eCurState; }
	void	Set_Boss_State(STATE eState) { m_eCurState = eState; }
private:
	enum MODE {  MODE_RUN, MODE_WATERFLOW ,MODE_RAINBOMB};

public:
	RECT	m_DrawRect;
	float	m_fDrawCX;
	float	m_fDrawCY;
	bool	m_bEffectComplete;
	DWORD	m_PangTime;
public:
	int		m_iHp;
	bool	m_bAppear;
	bool	m_bInitialFalling;
	int		m_iWaterPower;

	MODE	m_eMode;

	
	STATE					m_ePreState;
	STATE					m_eCurState;

	DIRECTION					m_eCurDirection;
	DIRECTION					m_ePreDirection;
public:
	list<PlayerTile*>			m_tTarget;

	PlayerTile					m_RaingSpot[10];
	bool						m_bModeCasting;
	DWORD						m_ModeDelayTime;
	DWORD						m_ModeCastingTime;

	DWORD						m_SpawnBulletTime;
	DWORD						m_BossNotDieTime;
	int							m_iBulletSpawnCount;

	RECT						m_tAppearRect;
};

