#pragma once
#include "Obj.h"
#include "MulJulgy.h"
#include "Block.h"
#include "Item.h"

template<typename T>
class CAbstractFactory
{
public:
	CAbstractFactory() {}
	~CAbstractFactory() {}

public:
	
	
	static CObj*	Create(void)
	{
		CObj*	pObj = new T;
		pObj->Initialize();

		return pObj;
	}

	static CObj*	Create(float _fX, float _fY, DIRECTION eDir = DIR_END)
	{
		CObj*	pObj = new T;

		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);

		if (DIR_END >= eDir)
			pObj->Set_Dir(eDir);

		return pObj;
	}

	static CObj*	Create(float _fX, float _fY,  OBJID eID)
	{
		CObj*	pObj = new T;

		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);
		pObj->Set_Type(eID);
		

		return pObj;
	}
	//아이템 생성용
	static CObj*	Create(float _fX, float _fY, OBJID eID, ITEMTYPE eItemNum)
	{
		
		CObj*	pObj = new T;

		pObj->Set_Pos(_fX, _fY);
		pObj->Set_Type(eID);
		dynamic_cast<CItem*>(pObj)->Set_Item_Type(eItemNum);
		pObj->Initialize();

		return pObj;
	}
	//블럭생성 함수
	static CObj*	Create(float _fX, float _fY, OBJID eID, BLOCKTYPE Type, int iOption,bool bStage2)
	{
		CObj*	pObj = new T;

		pObj->Set_Pos(_fX, _fY);
		pObj->Set_Type(eID);
		if (eID == OBJ_BLOCK)
		{
			dynamic_cast<CBlock*>(pObj)->Set_Block_Type(Type, iOption, bStage2);
		}
		pObj->Initialize();
		pObj->Set_Dir(DIR_END);

		return pObj;
	}
	//이거 물풍선 만드는 코드
	static CObj*	Create(float _fX, float _fY, OBJID eID, int iWaterPower, DIRECTION eDir, int iWhos)
	{
		CObj*	pObj = new T;
		pObj->Set_Pos(_fX, _fY);
		pObj->Set_Type(eID);
		if (eID == OBJ_BULLET) {
			dynamic_cast<CBullet*>(pObj)->SetWaterPower(iWaterPower, iWhos);
		}
		else if (eID == OBJ_MULJULGY)
		{
			dynamic_cast<CMulJulgy*>(pObj)->SetWaterPower_Direction(iWaterPower, eDir,iWhos);
		}
		pObj->Initialize();
		pObj->Set_Dir(eDir);

		return pObj;
	}

	//2048
	static CObj*	Create(float _fX, float _fY, int iIndexX, int iIndexY, DIRECTION eDir = DIR_END)
	{
		CObj*	pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);
		pObj->Set_Index(iIndexX, iIndexY);
		if (DIR_END >= eDir)
			pObj->Set_Dir(eDir);
		pObj->Set_Type(OBJ_2048);
		return pObj;
	}
	//애니메이션 생성용
	static CObj*	Create(float _fX, float _fY, int iIndexX, int iDummy, DIRECTION eDDir, int iDummy2,int iDummy3)
	{
		int iDirection = eDDir;
		CObj*	pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);
		pObj->Set_Type(OBJ_2048);
		dynamic_cast<C2048Block*>(pObj)->SetAnimationDirecion(iDirection);
		return pObj;
	}
};


