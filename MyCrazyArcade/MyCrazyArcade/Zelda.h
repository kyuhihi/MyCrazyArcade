#pragma once
#include "Obj.h"
class CZelda : public CObj
{
public:
	CZelda();
	~CZelda();
public:
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;

public:
	void Motion_Change(void);
	void Set_Zelda_Dir(DIRECTION);
	void Set_Zelda_Bomb();
	void  Set_Append_Item(ITEMTYPE eType);
	void  SetBullet() { if (m_iBulletMax >m_iBulletCapacity) ++m_iBulletCapacity; }
	void NailDown();
	bool SOS() 
	{ 
		if (m_bNeedle) {
			m_bNeedle = false;
			return true;
		}
		else
			return false;
	}
public:
	bool Get_Zelda_Needle(){ return m_bNeedle; }
	void ShieldToPlayer();
public:
	PlayerTile				m_StdTile;
	PlayerTile				m_BulletSpot;
	int						m_iWaterPower;

	DIRECTION					m_ePreDirection;
	DIRECTION					m_eCurDirection;
	bool					m_bShoes;
	bool					m_bNeedle;

	DWORD					m_ShieldCreateTime;

	int						m_ArrowCX;
	int						m_ArrowCY;

};

