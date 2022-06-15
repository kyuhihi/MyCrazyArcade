#pragma once
#include "Scene.h"
#include "BmpMgr.h"

class CShop : public CScene
{
public:
	CShop();
	~CShop();

public:
	virtual void Initialize(void) override;
	virtual void Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
public:
	void Mouse_Update_Rect();
private:
	RECT	m_tBox[4][2];
	float	m_fBoxCX;
	float	m_fBoxCY;

	bool	m_bNeedle;
	bool	m_bCANBUY;
	bool	m_bCAN_NOT_BUY;


	RECT	m_tExit;
	RECT	m_tNotice;

	RECT	m_tRect;
	INFO	m_tInfo;
	TCHAR	m_Gold[30];
};