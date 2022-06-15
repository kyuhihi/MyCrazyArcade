#pragma once
#include "Scene.h"
class CLogo :
	public CScene
{
public:
	CLogo();
	virtual ~CLogo();

public:
	virtual void Initialize(void) override;
	virtual void Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
private:
	void Mouse_Update_Rect();
private:
	RECT	m_tShop;

	RECT	m_tRect;
	INFO	m_tInfo;
};