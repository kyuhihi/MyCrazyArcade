#pragma once
#include "C2048Block.h"
#include "AbstractFactory.h"

class C2048BlockMgr
{
private:
	C2048BlockMgr();
	~C2048BlockMgr();

public:
	void		Initialize();
	const ItemResult&		Update();
	void		Late_Update();
	void		Render(HDC hDC);
	void		Release();

public:
	
	
	static C2048BlockMgr*		Get_Instance(void)
	{
		if (!m_pInstance)
		{
			m_pInstance = new C2048BlockMgr;
		}

		return m_pInstance;
	}

	static	void	Destroy_Instance(void)
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
private:
	void		Key_Input(void);
	void		CheckDirection(DIRECTION);
	void		EmptyBlockCheck();
	void		GetNewRandomNum();

	void		MergeGaro(DIRECTION);
	void		MergeSero(DIRECTION);

	void		FillBlank(DIRECTION);
	void		RemForReturn();
public:
	void		Move_Frame(void);
	//void		MoveAnimation(DIRECTION eDir);
	void		ItemPopUp();
	
	void		RenderNumberSelect();
	void		ItemDrop();
private:
	typedef struct m_tEmptyIndex
	{
		int iEmptyX;
		int iEmptyY;
		m_tEmptyIndex(int X, int Y) { iEmptyX = X; iEmptyY = Y; }
	}ForXY;
	
	struct m_AnimationItem
	{
		int _iX;
		int _iY;
		int _iSpeed;
		int _iItemNum;
	};
private:
	static C2048BlockMgr*		m_pInstance;
	vector <m_tEmptyIndex>	m_Empty;
	CObj*					m_ArrayBlock[4][4];
	CObj*					m_ReturnBlock[4][4];

public:
	ForXY					m_NewBlock;

	int	m_iMoveCount;
	FRAME		m_tFrame;

	RECT	m_DrawRect;
	float	m_fDrawCX;
	float	m_fDrawCY; 

	RECT	m_GameStartRect;
	float	m_fStartCX;
	float	m_fStartCY;

public:
	INFO	m_tInfo;	//마우스
	WCHAR	m_MousePoint[35];

	DWORD	m_StartTime;

	int	m_iForRenderTime;

	bool	m_bGameOver;
	bool	m_bGameStart;

public:
	RECT	m_NumRect1;
	float	m_NumRect1CX;
	float	m_NumRect1CY;

	RECT	m_NumRect2;
	float	m_NumRect2CX;
	float	m_NumRect2CY;

	int m_Num1;
	int m_Num2;

	RECT	m_ResultRect[4];
	float	m_ResultRectCX;
	float	m_ResultRectCY;

	RECT	m_ItemRect[4];
	float	m_ItemRectCX;
	float	m_ItemRectCY;
public:
	RECT	m_tGameOverRect;
	float	m_tGameOverRectCX;
	float	m_tGameOverRectCY;
public:
	int m_Item100;					//realnumber
	int m_Item10;
	int m_Item1;

	int m_Item100CX;
	int m_Item10CX;
	int m_Item1CX;

	int m_ItemCY;
	

	RECT	m_tResultNotice; //글자

	RECT	m_tItem100Rect;
	RECT	m_tItem10Rect;
	RECT	m_tItem1Rect;

	ItemResult Result;

	m_AnimationItem	m_AppendItemArray[15];
	int ApeendAmountCXY;

	int m_iUpCount;
	DWORD m_PopUpAnimationTime;
};

