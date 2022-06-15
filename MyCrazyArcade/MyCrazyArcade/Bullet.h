#pragma once
#include "Obj.h"
class CBullet :
	public CObj
{
public:
	CBullet();
	virtual ~CBullet();

public:
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual		void	Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;

public:
	void	SetWaterPower(int _iPower, int _iType) { m_iWaterPower = _iPower; m_WhosBullet = _iType; }
	int GetWaterPower( ) { return m_iWaterPower; }
	void Bullet_Update_Rect();
	void SetBulletRect() { m_bBulletUpdateRect = true; }

	void Set_Bullet_TouchCount(DIRECTION eDir) {
		++m_iBulletTouchCount;
		if (m_iBulletTouchCount > 5)
			m_eDir = eDir;
	}
	void BlockMove();
	void ReSetBulletSpot(int);
private:
	int		m_iWaterPower;
	DWORD	m_PangTime;
	int		m_WhosBullet;
	RECT	m_Stage1CannotBreak;

public:
	RECT	m_DrawRect;
	float	m_fDrawCX;
	float	m_fDrawCY;

	bool	m_bBulletUpdateRect;

	int				m_iBulletTouchCount;
	bool	m_bMove;
	MonsterShadowIndex	m_tTargetTile;
};

