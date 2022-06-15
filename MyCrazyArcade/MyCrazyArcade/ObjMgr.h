#pragma once

#include "Obj.h"

class CObjMgr
{
private:
	CObjMgr();
	~CObjMgr();

public:
	CObj*		Get_Player() { return m_ObjList[OBJ_PLAYER].front(); }
	CObj*		Get_Monster() { return m_ObjList[OBJ_MONSTER].front(); }
	CObj*		Get_Pet() { return m_ObjList[OBJ_PET].front(); }
	CObj*		Get_Zelda() { return m_ObjList[OBJ_ZELDA].front(); }
	CObj*		Get_Boss() { return m_ObjList[OBJ_BOSS].front(); }

	bool		IsPlayer(){
		if (!m_ObjList[OBJ_PLAYER].empty())
		{
			return true;
		}
		return false;
		
	}
	CObj*		Get_Target(OBJID eID, CObj* pObj);
	
	void		Set_Bullet_Update_Rect();

public:
	void		Add_Object(OBJID eID, CObj* pObj);
	int			Update(void);
	void		Late_Update(void);
	void		Render(HDC hDC);
	void		Release(void);

	void		Delete_ID(OBJID eID);
	bool		Find_MulJulGi(float, float);
	bool		If_Block(int, int);

public:
	void		SetWater_Spot(int, int, bool);
	bool		GetWater_Spot(int, int);
	void		VanishBlock();
private:
	list<CObj*>	m_ObjList[OBJ_END];
	

public:
	static		CObjMgr*		Get_Instance(void)
	{
		if (!m_pInstance)
		{
			m_pInstance = new CObjMgr;
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
	static CObjMgr*			m_pInstance;
	list<CObj*>				m_RenderSort[RENDER_END];

public:
	WaterTile		m_bWATERSPOT[13][15];
	PlayerTile		m_RemPlayerBulletSpot;

};

