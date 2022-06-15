#pragma once
#include "Scene.h"
class CMiniGame :public CScene
{
public:
	CMiniGame();
	~CMiniGame();
public:
	virtual void Initialize(void) override;
	virtual void Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;

private:
	ItemResult  Result;
	DWORD		m_TimeLimit;
	bool		m_Over;
};

