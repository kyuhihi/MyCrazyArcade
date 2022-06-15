#pragma once
#include "Obj.h"
class CPet :public CObj
{
public:
	CPet();
	~CPet();

public:	
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
public:
	void  Set_Pet_State(DIRECTION eDir, float _fX, float _fY);

	void Motion_Change();
private:
	DIRECTION					m_ePreDirection;
	DIRECTION					m_eCurDirection;
	int						m_iWaterPower;
	DWORD					m_BulletTime;
};

