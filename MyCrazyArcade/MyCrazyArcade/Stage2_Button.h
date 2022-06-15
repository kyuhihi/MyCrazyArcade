#pragma once
#include "Obj.h"

class CStage2_Button : public CObj
{
public:
	CStage2_Button();
	~CStage2_Button();

	// CObj을(를) 통해 상속됨
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;

public:
	void Button_Update_Rect();

public:
	RECT	m_DrawRect;
	float	m_fDrawCX;
	float	m_fDrawCY;
};

