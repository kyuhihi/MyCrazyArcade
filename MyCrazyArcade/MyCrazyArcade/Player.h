#pragma once
#include "Obj.h"

class CPlayer : public CObj
{


public:
	CPlayer();
	virtual ~CPlayer();

public:
	virtual void Initialize(void)	override;
	virtual int	 Update(void)		override;
	virtual	void Late_Update(void);
	virtual void Render(HDC hDC)	override;
	virtual void Release(void)		override;

private:
	void		Key_Input(void);
	void		Motion_Change(void);
	void		Draw_Update_Rect(void);
	void		Player_Update_Rect();

public:
	void  SetBullet(){ if (m_iBulletMax >m_iBulletCapacity) ++m_iBulletCapacity;}
	const STATE Get_Player_State() { return m_eCurState; }
	bool  GetPlayerShoes() { return m_bShoes; }
	bool  GetPlayerShield() { return m_bShield; }
	void  SetShieldTime();

	void  Set_Append_Item(ITEMTYPE eType); 
	void  Set_Player_State(STATE eState) { 
		m_eCurState = eState; 
		if (eState == IDLE) {
			m_PangTime = 50000;
			m_DeadTime = 50000;
		}
	}
	void  Set_Player_PangTimer() { m_PangTime = GetTickCount(); }
	void  Set_Player_DeadTimer() { m_DeadTime = GetTickCount(); }

	bool  Get_PlayerNeedle() { return m_bNeedle; }
	void  Set_PlayerNeedle() {  m_bNeedle = true; }
	const PlayerTile GetBulletSpot() { return m_BulletSpot; }
	void SetMiniGameResult(ItemResult NewItem);
	const DWORD		GetShieldTime() { return m_ShieldTime; }

private:
	STATE					m_ePreState;
	STATE					m_eCurState;

	RECT	m_DrawRect;
	float	m_fDrawCX;
	float	m_fDrawCY;

	float	m_ZeldaHelpCX;
	float	m_ZeldaHelpCY;


	bool					m_bNeedle;
	DWORD					m_SOSTime;
	
	PlayerTile				m_StdTile;

	PlayerTile				m_BulletSpot;
	int						m_iWaterPower;
	
	WCHAR	CashUp[20];

	DWORD					m_PangTime;
	DWORD					m_DeadTime;
	DWORD					m_CashUpTime;
	DWORD					m_ShieldTime;

	int						m_iAppendCash;

	bool					m_bShoes;
	bool					m_bShield;
	
};
