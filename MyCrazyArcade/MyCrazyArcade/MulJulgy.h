#pragma once
#include "Obj.h"
class CMulJulgy : public CObj
{
public:
	CMulJulgy();
	~CMulJulgy();
public:
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
public:
	void	SetWaterPower_Direction(int _iPower, DIRECTION _eDir, int iWhos) { m_iWaterPower = _iPower; m_eDir = _eDir; m_WhosBullet = iWhos; }
	const int& Get_WhosBullet() { return m_WhosBullet; }
private:
	DIRECTION m_eDir;
	int		m_iWaterPower;
	DWORD	m_PangTime;
	int		m_WhosBullet;
};

