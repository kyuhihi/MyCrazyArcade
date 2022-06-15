#pragma once
#include "Obj.h"

class CItem: public CObj
{
public:
	CItem();
	~CItem();
public:
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
private:
	void Item_Update_Rect();
public:
	void Set_Item_Type(ITEMTYPE iType) { m_eItemType = iType; }

	ITEMTYPE	 Get_Item_Type() { return m_eItemType; }

public:
	RECT	m_DrawRect;
	float	m_fDrawCX;
	float	m_fDrawCY;
	
	PlayerTile  m_PlayerSpot;
	ITEMTYPE		m_eItemType;
};

