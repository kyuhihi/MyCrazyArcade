#pragma once
#include "Obj.h"
class CMonster : public CObj
{
public:
	CMonster();
	~CMonster();
private:
	enum MODE { MODE_FIND, MODE_AVOID, MODE_FIGHT };
public:
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
public:
	void		DeadCheck();
	void		Draw_Update_Rect(void);
	void		Monster_Update_Rect();
public:
	void  SetBullet()
	{
		if (m_iBulletMax > m_iBulletCapacity) {
			++m_iBulletCapacity;
		}
		if (!m_bFightMode)
			m_bFindModeBullet = false;
	}
	void  Set_Append_Item(ITEMTYPE eType);
	void Motion_Change(void);
	void Direction_Change();
	const STATE Get_Boss_State() { return m_eCurState; }

public:
	void Avoid_Bullet();
	void Avoid_Direction();
	void MonsterMoveTile();
	void EnemySetBullet();
public:
	void DeletePath();
	void Find_Where();
public:
	void ModeSelect();
	void DirectionCheck();
	void Avoid_How(const int iNewPathX, const int iNewPathY, int& DistanceX, int& DistanceY);
	void Nail_Down();
	void Monster_Dont_Move();

public:
	void Set_FightModeBool() { m_bFightMode = true; }
public:
	void  Set_Player_State(STATE eState)
	{
		m_eCurState = eState;
		if (eState == IDLE) {
			m_PangTime = 50000;
			m_DeadTime = 50000;
		}
	}
	void  Set_Player_PangTimer() {
		m_PangTime = GetTickCount();
	}
	const STATE Get_Monster_State() { return m_eCurState; }
private:
	RECT	m_DrawRect;
	float	m_fDrawCX;
	float	m_fDrawCY;
	PlayerTile				m_StdTile;

	PlayerTile				m_BulletSpot;
	

	PlayerTile				m_tTargetTile;

	int						m_iWaterPower;
	bool					m_bNeedle;

	STATE					m_ePreState;
	STATE					m_eCurState;

	DIRECTION					m_eCurDirection;
	DIRECTION					m_ePreDirection;

	MODE					m_eMode;

	MonsterShadowIndex		m_BeforeMove;

	MonsterShadowIndex		m_tShadowMove;
	list<MonsterShadowIndex*>			m_MoveHow;

	DWORD					m_AvoidTime;
	bool					m_bTime;

	DWORD					m_FindTime;
	bool					m_bFind_Timer;
	MonsterShadowIndex		m_RemNowTile;


	DWORD					m_ModeDelay;
	bool					m_bMode;

	bool					m_bFightMode;

	DWORD					m_PangTime;
	DWORD					m_DeadTime;

	bool					m_bFindModeBullet;

	DWORD					m_NotDeadTime;

private:
	bool					m_bDebugMode;
	void					DebuggingKey();
};