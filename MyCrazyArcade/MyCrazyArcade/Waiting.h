#pragma once
#include "Scene.h"
#include "BmpMgr.h"

class CWaiting : public CScene
{
public:
	CWaiting();
	~CWaiting();

public:
	virtual void Initialize(void) override;
	virtual void Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
public:
	void Mouse_Update_Rect();
private:
	RECT	m_tBox[2];
	float	m_fBoxCX;
	float	m_fBoxCY;

	RECT	m_tShop;


	RECT	m_tRect;
	INFO	m_tInfo;


	RECT	m_tMiniGame;
};

