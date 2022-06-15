#pragma once
#include "Obj.h"
class CBlock : public CObj
{
public:
	CBlock();
	~CBlock();
public:
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
public:
	void Set_Block_Type(BLOCKTYPE iType, int& iOption, bool bStage2) { m_eBlockType = iType; m_BlockCanBreakOption = iOption; m_bStage2 = bStage2; }
	void Set_Block_TouchCount(DIRECTION eDir) {
		++m_iMoveBlockTouchCount; 
		if(m_iMoveBlockTouchCount > 15)
			m_eDir = eDir;
	}

	void Block_Update_Rect();

	int	 Get_Block_Type() { return m_eBlockType; }
	int  Get_Block_Can_Break_Option() { return m_BlockCanBreakOption;}
public:
	RECT	m_DrawRect;
	float	m_fDrawCX;
	float	m_fDrawCY;

	int				m_BlockCanBreakOption;
	
	int				m_iMoveBlockTouchCount;
	bool			m_bMove;
	BLOCKTYPE		m_eBlockType;
	MonsterShadowIndex	m_tTargetTile;

	bool			m_bStage2;
	
};

