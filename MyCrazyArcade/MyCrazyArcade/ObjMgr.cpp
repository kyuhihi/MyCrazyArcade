#include "stdafx.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "Bullet.h"
#include <iostream>
#include "Block.h"
#include "Player.h"
#include "Monster.h"

CObjMgr* CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr()
{
	m_RemPlayerBulletSpot.fX = 0;
	m_RemPlayerBulletSpot.fY = 0;

	for (int i = 0; i < 13; ++i)
	{
		for (int j = 0; j < 15; ++j)
		{
			m_bWATERSPOT[i][j].bBulletTrue = false;
			m_bWATERSPOT[i][j].iCount = 0;
		}
	}

}


CObjMgr::~CObjMgr()
{
	Release();
}

CObj* CObjMgr::Get_Target(OBJID eID, CObj* pObj)
{
	if (m_ObjList[eID].empty())
		return nullptr;

	CObj*		pTarget = nullptr;
	float		fDistance = 0.f;

	for (auto& iter : m_ObjList[eID])
	{
		if (iter->Get_Dead())
			continue;

		float	fWidth = abs(pObj->Get_Info().fX - iter->Get_Info().fX);
		float	fHeight = abs(pObj->Get_Info().fY - iter->Get_Info().fY);

		float	fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

		if ((!pTarget) || (fDistance > fDiagonal))
		{
			if (eID == OBJ_BLOCK) {
				if (dynamic_cast<CBlock*>(iter)->Get_Block_Type() == BLOCK_CAN_BREAK) {
					pTarget = iter;
					fDistance = fDiagonal;
				}
			}
			
		}
	}

	return pTarget;
}

void CObjMgr::Add_Object(OBJID eID, CObj * pObj)
{
	if ((eID >= OBJ_END) || (nullptr == pObj))
		return;
	if (eID == OBJ_BOSS)
		int i=1;
	m_ObjList[eID].push_back(pObj);
}

void CObjMgr::Set_Bullet_Update_Rect()
{
	PlayerTile Temp1 = dynamic_cast<CPlayer*>(m_ObjList[OBJ_PLAYER].front())->GetBulletSpot();
	if ((m_RemPlayerBulletSpot.fX == 0) && (m_RemPlayerBulletSpot.fY == 0))
	{
		m_RemPlayerBulletSpot.fX = Temp1.fX;
		m_RemPlayerBulletSpot.fY = Temp1.fY;
		return;
	}
	else if ((Temp1.fX != m_RemPlayerBulletSpot.fX) || (Temp1.fY != m_RemPlayerBulletSpot.fY))
	{
		for (auto& iter : m_ObjList[OBJ_BULLET])
		{
			INFO Temp ((*iter).Get_Info());
			if((Temp.fX == m_RemPlayerBulletSpot.fX)&&(Temp.fY == m_RemPlayerBulletSpot.fY))
				dynamic_cast<CBullet*>((iter))->SetBulletRect();
		}
		m_RemPlayerBulletSpot.fX = Temp1.fX;
		m_RemPlayerBulletSpot.fY = Temp1.fY;
	}
}

void CObjMgr::VanishBlock()
{
	for (auto&iter : m_ObjList[OBJ_BLOCK])
		(*iter).Set_Dead();
}

int CObjMgr::Update(void)
{
	Set_Bullet_Update_Rect();
	for (int i = 0; i < OBJ_END; ++i)
	{
		for (auto& iter = m_ObjList[i].begin();
			iter != m_ObjList[i].end(); )
		{
			int iResult = (*iter)->Update();

			if (OBJ_DEAD == iResult)
			{
				if ((*iter)->Get_Info().eObjType == OBJ_PLAYER) {
					return	GAME_LOSE;
				}
				else if (((*iter)->Get_Info().eObjType == OBJ_MONSTER)&&(!g_bStage2))
				{
					return GAME_WIN;
				}
				else if (((*iter)->Get_Info().eObjType == OBJ_BOSS) && (g_bStage2))
				{
					return GAME_WIN;
				}
				else {
					Safe_Delete<CObj*>(*iter);
					iter = m_ObjList[i].erase(iter);
				}
			}
			else
				++iter;
		}
	}

	return 0;
}

void CObjMgr::Late_Update(void)
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		for (auto& iter : m_ObjList[i])
		{
			iter->Late_Update();

			if (m_ObjList[i].empty())
				break;
			RENDERID eRender = iter->Get_RenderID();
			m_RenderSort[eRender].push_back(iter);

		}
	}
	CCollisionMgr::Collision_Rect(m_ObjList[OBJ_MULJULGY], m_ObjList[OBJ_BLOCK]);
	CCollisionMgr::Collision_Rect(m_ObjList[OBJ_MULJULGY], m_ObjList[OBJ_ITEM]);
	CCollisionMgr::Collision_Rect(m_ObjList[OBJ_MULJULGY], m_ObjList[OBJ_PLAYER]);
	CCollisionMgr::Collision_Rect(m_ObjList[OBJ_MULJULGY], m_ObjList[OBJ_MONSTER]);

	CCollisionMgr::Collision_Rect(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_PLAYER]);

	CCollisionMgr::Collision_Rect(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_ITEM]);
	CCollisionMgr::Collision_Rect(m_ObjList[OBJ_ZELDA], m_ObjList[OBJ_ITEM]);
	//=======================================================================================
	CCollisionMgr::Collision_Rect(m_ObjList[OBJ_BOSS], m_ObjList[OBJ_BLOCK]);
	CCollisionMgr::Collision_Rect(m_ObjList[OBJ_MULJULGY], m_ObjList[OBJ_BOSS]);
	CCollisionMgr::Collision_Rect(m_ObjList[OBJ_BOSS], m_ObjList[OBJ_PLAYER]);
	//=======================================================================================


	CCollisionMgr::Collision_Rect(m_ObjList[OBJ_BLOCK], m_ObjList[OBJ_BUTTON]); //퍼즐

	CCollisionMgr::Collision_Rect(m_ObjList[OBJ_PLAYER], m_ObjList[OBJ_ZELDA]);
	CCollisionMgr::Collision_Rect(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_ZELDA]);

	CCollisionMgr::Collision_Rect(m_ObjList[OBJ_MONSTER], m_ObjList[OBJ_ITEM]);

	CCollisionMgr::Collision_Rect(m_ObjList[OBJ_TILE], m_ObjList[OBJ_MULJULGY]);



	CCollisionMgr::Collision_RectEx(m_ObjList[OBJ_BLOCK], m_ObjList[OBJ_PLAYER]);

	CCollisionMgr::Collision_RectEx(m_ObjList[OBJ_BLOCK], m_ObjList[OBJ_ZELDA]);

	CCollisionMgr::Collision_RectEx(m_ObjList[OBJ_BULLET], m_ObjList[OBJ_PLAYER]);
	CCollisionMgr::Collision_RectEx(m_ObjList[OBJ_BLOCK], m_ObjList[OBJ_MONSTER]);
	


}

template<typename T>
bool		CompareY(T Dest, T Sour)
{
	return Dest->Get_Info().fY < Sour->Get_Info().fY;
}

void CObjMgr::Render(HDC hDC)
{
	//for (int i = 0; i < OBJ_END; ++i)
	//{
	//	for (auto& iter : m_ObjList[i])
	//		iter->Render(hDC);
	//}

	//for (int i = 0; i < 13; ++i)
	//{
	//	for (int j = 0; j < 15; ++j)
	//	{
	//		if (m_bWATERSPOT[i][j].bBulletTrue)
	//		{
	//			float		fX = (TILECX >> 1) + float(TILECX * j) + 20;// 20 40더하면됨  바닥 사이즈는40 40
	//			float		fY = (TILECY >> 1) + float(TILECY * i) + 40;
	//			Rectangle(hDC, fX - 20, fY - 20, fX + 20, fY + 20);
	//		}
	//	}
	//}

	for (int i = 0; i < RENDER_END; ++i)
	{
		m_RenderSort[i].sort(CompareY<CObj*>);

		for (auto& iter : m_RenderSort[i])
			iter->Render(hDC);

		m_RenderSort[i].clear();
	}
	
}

void CObjMgr::Release(void)
{
	for (int i = 0; i < OBJ_END; ++i)
	{
		for (auto& iter : m_ObjList[i])
			Safe_Delete<CObj*>(iter);

		m_ObjList[i].clear();
	}
}

void CObjMgr::Delete_ID(OBJID eID)
{
	for (auto& iter : m_ObjList[eID])
		Safe_Delete(iter);

	m_ObjList[eID].clear();
}

bool CObjMgr::Find_MulJulGi(float fX, float fY)
{
	INFO Temp;
	for (auto& iter : m_ObjList[OBJ_BLOCK])
	{
		Temp = iter->Get_Info();
		if ((Temp.fX == fX) && (Temp.fY == fY))
			return true;
	}
	return false;
}

bool CObjMgr::If_Block(int X, int Y)
{
	RECT Temp;
	if (X > 14 || X < 0 || Y < 0 || Y >12) {
		return true;
	}
	if (!m_ObjList[OBJ_BLOCK].empty()) {
		for (auto& iter : m_ObjList[OBJ_BLOCK])
		{
			Temp = iter->Get_Rect();
			int TempX = 40 + (X  * TILECX);
			int TempY = 60 + (Y  * TILECX);
			if (((int)Temp.bottom > TempY) && ((int)Temp.top < TempY) && ((int)Temp.left < TempX) && ((int)Temp.right > TempX)) {
				return true;
			}
		}
	}
	return false;
}

void CObjMgr::SetWater_Spot(int _X, int _Y, bool T_F)
{//left up down right순으로 -> true이면 물줄기자리
	if (T_F) {
		++m_bWATERSPOT[_Y][_X].iCount;
		m_bWATERSPOT[_Y][_X].bBulletTrue = T_F;
	}
	else {
		--m_bWATERSPOT[_Y][_X].iCount;
		if(m_bWATERSPOT[_Y][_X].iCount <= 0)
			m_bWATERSPOT[_Y][_X].bBulletTrue = T_F;
	}
}

bool CObjMgr::GetWater_Spot(int X, int Y)
{//이름이 워터스팟이니까 false이어야 피하는 방향임.
	if (X > 14 || X < 0 || Y < 0 || Y >12) {
		return true;
	}
	else if (m_bWATERSPOT[Y][X].bBulletTrue) {
		return true;//트루이면 피해야되잖아
	}
	return false;
}


