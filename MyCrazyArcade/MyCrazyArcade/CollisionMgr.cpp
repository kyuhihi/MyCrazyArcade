#include "stdafx.h"
#include "CollisionMgr.h"
#include "Player.h"
#include "Block.h"
#include "Item.h"
#include "Monster.h"
#include "Bullet.h"
#include "Zelda.h"
#include "SoundMgr.h"
#include "Tile.h"
#include "MulJulgy.h"
#include "Boss.h"

DWORD CCollisionMgr::m_NotDieTime = 50000;
bool CCollisionMgr::m_bBreakBubble = false;
bool CCollisionMgr ::m_bButtonPress = false;

CCollisionMgr::CCollisionMgr()
{
	
}


CCollisionMgr::~CCollisionMgr()
{
}


void CCollisionMgr::Collision_Rect(list<CObj*> _Dest, list<CObj*> _Sour)
{
	RECT		rc{};

	for (auto& Dest : _Dest)
	{
		for (auto& Sour : _Sour)
		{
			if (IntersectRect(&rc, &(Dest->Get_Rect()), &(Sour->Get_Rect())))
			{
				INFO Left = Dest->Get_Info();
				INFO Right = Sour->Get_Info();

				if ((Left.eObjType == OBJ_MULJULGY) && (Right.eObjType == OBJ_BLOCK))
				{
					int iBlockType = dynamic_cast<CBlock*>(Sour)->Get_Block_Type();
					if (iBlockType == BLOCK_CAN_BREAK) {
						Dest->Set_Dead();
						Sour->Set_Dead();
					}
					else if (iBlockType == BLOCK_CANNOT_BREAK)
					{
						Dest->Set_Dead();
					}
				}
				else if ((Left.eObjType == OBJ_MULJULGY) && (Right.eObjType == OBJ_ITEM))
				{
					Sour->Set_Dead();
				}
				else if ((Left.eObjType == OBJ_MULJULGY) && (Right.eObjType == OBJ_PLAYER))
				{
					if (dynamic_cast<CPlayer*>(Sour)->GetPlayerShield())
					{
						dynamic_cast<CPlayer*>(Sour)->SetShieldTime();
					}
					else if (0 == dynamic_cast<CPlayer*>(Sour)->GetShieldTime())
					{
						dynamic_cast<CPlayer*>(Sour)->Set_Player_State(WHILEBUBBLE);
						dynamic_cast<CPlayer*>(Sour)->Set_Player_PangTimer();
					}
				}
				else if ((Left.eObjType == OBJ_MULJULGY) && (Right.eObjType == OBJ_MONSTER))
				{
					if ((!m_bBreakBubble) && (m_NotDieTime == 50000)) {
						m_bBreakBubble = true;
						m_NotDieTime = GetTickCount();
						dynamic_cast<CMonster*>(Sour)->Set_Player_State(WHILEBUBBLE);
						dynamic_cast<CMonster*>(Sour)->Set_Player_PangTimer();
					}
				}
				else if ((Left.eObjType == OBJ_MULJULGY) && (Right.eObjType == OBJ_BOSS))
				{
					if (dynamic_cast<CMulJulgy*>(Dest)->Get_WhosBullet() != BOMB_BOSS)
					{
						if (dynamic_cast<CBoss*>(Sour)->GetNotDieTime() == 50000)
						{
							dynamic_cast<CBoss*>(Sour)->Set_Minus_Hp();
						}
					}
				}
				else if ((Left.eObjType == OBJ_PLAYER) && (Right.eObjType == OBJ_ITEM))
				{
					ITEMTYPE eType = dynamic_cast<CItem*>(Sour)->Get_Item_Type();
					dynamic_cast<CPlayer*>(Dest)->Set_Append_Item(eType);
					Sour->Set_Dead();
				}
				else if ((Left.eObjType == OBJ_ZELDA) && (Right.eObjType == OBJ_ITEM))
				{
					ITEMTYPE eType = dynamic_cast<CItem*>(Sour)->Get_Item_Type();
					if (eType != ITEM_SHIELD) {
						dynamic_cast<CZelda*>(Dest)->Set_Append_Item(eType);
						Sour->Set_Dead();
					}
				}
				else if ((Left.eObjType == OBJ_MONSTER) && (Right.eObjType == OBJ_ITEM))
				{
					ITEMTYPE eType = dynamic_cast<CItem*>(Sour)->Get_Item_Type();
					dynamic_cast<CMonster*>(Dest)->Set_Append_Item(eType);

					Sour->Set_Dead();
				}
				else if ((Left.eObjType == OBJ_MONSTER) && (Right.eObjType == OBJ_PLAYER))
				{
					if (!g_bStage2) {
						if ((WHILEBUBBLE == dynamic_cast<CMonster*>(Dest)->Get_Monster_State())) {
							dynamic_cast<CMonster*>(Dest)->Set_Player_State(PANGBUBBLE);
							Dest->Set_Dead();
						}
						else if (WHILEBUBBLE == dynamic_cast<CPlayer*>(Sour)->Get_Player_State())
						{
							dynamic_cast<CPlayer*>(Sour)->Set_Player_State(PANGBUBBLE);
							Sour->Set_Dead();
						}
					}
					else if (g_bStage2)
					{
						if ((WHILEBUBBLE == dynamic_cast<CMonster*>(Dest)->Get_Monster_State())) {
							dynamic_cast<CMonster*>(Dest)->Set_Player_State(IDLE);
						}
						else if (WHILEBUBBLE == dynamic_cast<CPlayer*>(Sour)->Get_Player_State())
						{
							dynamic_cast<CPlayer*>(Sour)->Set_Player_State(IDLE);
						}
					}
				}
				else if ((Left.eObjType == OBJ_BOSS) && (Right.eObjType == OBJ_PLAYER))
				{
					STATE PlayerState = dynamic_cast<CPlayer*>(Sour)->Get_Player_State();
					STATE BossState = dynamic_cast<CBoss*>(Dest)->Get_Boss_State();
					if (((PlayerState == RIGHT_LEFT)||(PlayerState == IDLE )||(PlayerState == UP_DOWN))&& (BossState != WHILEBUBBLE ))
					{
						dynamic_cast<CPlayer*>(Sour)->Set_Player_State(WHILEBUBBLE);
						dynamic_cast<CPlayer*>(Sour)->Set_Player_PangTimer();
					}
					else if (((PlayerState == RIGHT_LEFT) || (IDLE == PlayerState)||(PlayerState == UP_DOWN)) && ( BossState == WHILEBUBBLE))
					{
						Dest->Set_Dead();
					}
				}
				else if ((Left.eObjType == OBJ_PLAYER) && (Right.eObjType == OBJ_ZELDA))
				{
					if (WHILEBUBBLE == dynamic_cast<CPlayer*>(Dest)->Get_Player_State()) {
						dynamic_cast<CPlayer*>(Dest)->Set_Player_State(IDLE);
					}
				}
				else if ((Left.eObjType == OBJ_BOSS) && (Right.eObjType == OBJ_BLOCK))
				{
					Sour->Set_Dead();
				}
				else if ((Left.eObjType == OBJ_MONSTER) && (Right.eObjType == OBJ_ZELDA))
				{
					if (WHILEBUBBLE == dynamic_cast<CMonster*>(Dest)->Get_Monster_State()) {
						dynamic_cast<CMonster*>(Dest)->Set_Player_State(IDLE);
					}
				}
				else if ((Left.eObjType == OBJ_BLOCK) && (Right.eObjType == OBJ_BUTTON))
				{
					if ((Left.fX == Right.fX) && (Left.fY == Right.fY)) {
						Dest->Set_Dead();
						Sour->Set_Dead();
						m_bButtonPress = true;

					}
				}
				else if ((Left.eObjType == OBJ_TILE) && (Right.eObjType == OBJ_MULJULGY))
				{
					if (dynamic_cast<CMulJulgy*>(Sour)->Get_WhosBullet() != BOMB_BOSS) {
						dynamic_cast<CTile*>(Dest)->Set_Option();
					}
				}

			}
		}
	}
}

									// 고정되어 있는 물체  // 움직이는 물체		
void CCollisionMgr::Collision_RectEx(list<CObj*> _Dest, list<CObj*> _Sour)
{
	for (auto& Dest : _Dest)
	{
		for (auto& Sour : _Sour)
		{
			float	fX = 0.f, fY = 0.f;

			if (Check_Rect(Dest, Sour, &fX, &fY))
			{
				INFO Left = Dest->Get_Info();
				INFO Right = Sour->Get_Info();
				if ((Left.eObjType == OBJ_BLOCK) && (Right.eObjType == OBJ_PLAYER))
				{
					if (fX > fY)
					{
						// 상 충돌
						if (Dest->Get_Info().fY > Sour->Get_Info().fY) {
							Sour->Set_PosY(-fY);
							if (dynamic_cast<CBlock*>(Dest)->Get_Block_Can_Break_Option() == 1)
								dynamic_cast<CBlock*>(Dest)->Set_Block_TouchCount(DIR_DOWN);
						}

						else {// 하 충돌
							Sour->Set_PosY(fY);
							if (dynamic_cast<CBlock*>(Dest)->Get_Block_Can_Break_Option() == 1)
								dynamic_cast<CBlock*>(Dest)->Set_Block_TouchCount(DIR_UP);
						}
					}
					// 좌우 충돌
					else
					{
						// 좌 충돌
						if (Dest->Get_Info().fX > Sour->Get_Info().fX)
						{
							Sour->Set_PosX(-fX);
							if (dynamic_cast<CBlock*>(Dest)->Get_Block_Can_Break_Option() == 1)
								dynamic_cast<CBlock*>(Dest)->Set_Block_TouchCount(DIR_RIGHT);
						}
						// 우 충돌
						else {
							Sour->Set_PosX(fX);
							if (dynamic_cast<CBlock*>(Dest)->Get_Block_Can_Break_Option() == 1)
								dynamic_cast<CBlock*>(Dest)->Set_Block_TouchCount(DIR_LEFT);

						}

					}
				}
				else if ((Left.eObjType == OBJ_BULLET) && (Right.eObjType == OBJ_PLAYER))
				{
					if (Dest->Get_Rect().left != 0) {
						if (fX > fY)
						{
							// 상 충돌
							if (Dest->Get_Info().fY > Sour->Get_Info().fY) {
								Sour->Set_PosY(-fY);
								if (dynamic_cast<CPlayer*>(Sour)->GetPlayerShoes())
									dynamic_cast<CBullet*>(Dest)->Set_Bullet_TouchCount(DIR_DOWN);
							}

							else {// 하 충돌
								Sour->Set_PosY(fY);
								if (dynamic_cast<CPlayer*>(Sour)->GetPlayerShoes())
									dynamic_cast<CBullet*>(Dest)->Set_Bullet_TouchCount(DIR_UP);
							}
						}
						// 좌우 충돌
						else
						{
							// 좌 충돌
							if (Dest->Get_Info().fX > Sour->Get_Info().fX)
							{
								Sour->Set_PosX(-fX);
								if (dynamic_cast<CPlayer*>(Sour)->GetPlayerShoes())
									dynamic_cast<CBullet*>(Dest)->Set_Bullet_TouchCount(DIR_RIGHT);
							}
							// 우 충돌
							else {
								Sour->Set_PosX(fX);
								if (dynamic_cast<CPlayer*>(Sour)->GetPlayerShoes())
									dynamic_cast<CBullet*>(Dest)->Set_Bullet_TouchCount(DIR_LEFT);

							}

						}
					}
				}
				else if ((Left.eObjType == OBJ_BLOCK) && (Right.eObjType == OBJ_MONSTER))
				{

					if (fX > fY)
					{
						// 상 충돌
						if (Dest->Get_Info().fY > Sour->Get_Info().fY) {
							Sour->Set_PosY(-fY);
						}

						else {// 하 충돌
							Sour->Set_PosY(fY);
						}
					}
					// 좌우 충돌
					else
					{
						// 좌 충돌
						if (Dest->Get_Info().fX > Sour->Get_Info().fX)
						{
							Sour->Set_PosX(-fX);
							
						}
						// 우 충돌
						else {
							Sour->Set_PosX(fX);

						}

					}
				}
				else if ((Left.eObjType == OBJ_BLOCK) && (Right.eObjType == OBJ_ZELDA))
				{

					if (fX > fY)
					{
						// 상 충돌
						if (Dest->Get_Info().fY > Sour->Get_Info().fY) {
							Sour->Set_PosY(-fY);
							if (dynamic_cast<CBlock*>(Dest)->Get_Block_Can_Break_Option() == 1)
								dynamic_cast<CBlock*>(Dest)->Set_Block_TouchCount(DIR_DOWN);
						}

						else {// 하 충돌
							Sour->Set_PosY(fY);
							if (dynamic_cast<CBlock*>(Dest)->Get_Block_Can_Break_Option() == 1)
								dynamic_cast<CBlock*>(Dest)->Set_Block_TouchCount(DIR_UP);
						}
					}
					// 좌우 충돌
					else
					{
						// 좌 충돌
						if (Dest->Get_Info().fX > Sour->Get_Info().fX)
						{
							Sour->Set_PosX(-fX);
							if (dynamic_cast<CBlock*>(Dest)->Get_Block_Can_Break_Option() == 1)
								dynamic_cast<CBlock*>(Dest)->Set_Block_TouchCount(DIR_RIGHT);
						}
						// 우 충돌
						else {
							Sour->Set_PosX(fX);
							if (dynamic_cast<CBlock*>(Dest)->Get_Block_Can_Break_Option() == 1)
								dynamic_cast<CBlock*>(Dest)->Set_Block_TouchCount(DIR_LEFT);

						}

					}
				}
			
			}

			// 상하 충돌

		}
	}
}


bool CCollisionMgr::Check_Rect(CObj* pDest, CObj* pSour, float *pX, float* pY)
{
	float		fWidth = abs(pDest->Get_Info().fX - pSour->Get_Info().fX);
	float		fHeight = abs(pDest->Get_Info().fY - pSour->Get_Info().fY);

	float		fCX = (pDest->Get_Info().fCX + pSour->Get_Info().fCX) * 0.5f;
	float		fCY = (pDest->Get_Info().fCY + pSour->Get_Info().fCY) * 0.5f;

	if ((fCX > fWidth) && (fCY > fHeight))
	{
		*pX = fCX - fWidth;
		*pY = fCY - fHeight;

		return true;
	}

	return false;
}

bool CCollisionMgr::Check_Sphere(CObj* pDest, CObj* pSour)
{

	// abs : 절대값을 구해주는 함수
	float	fWidth = fabs(pDest->Get_Info().fX - pSour->Get_Info().fX);
	float	fHeight = fabs(pDest->Get_Info().fY - pSour->Get_Info().fY);

	// sqrt : 루트를 씌워주는 함수
	float	fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

	float	fRadius = (pDest->Get_Info().fCX + pSour->Get_Info().fCX) * 0.5f;

	return fRadius > fDiagonal;
}

void CCollisionMgr::Collision_Sphere(list<CObj*> _Dest, list<CObj*> _Sour)
{

	for (auto& Dest : _Dest)
	{
		for (auto& Sour : _Sour)
		{
			if (Check_Sphere(Dest, Sour))
			{
				Dest->Set_Dead();
				Sour->Set_Dead();
			}
		}
	}
}
