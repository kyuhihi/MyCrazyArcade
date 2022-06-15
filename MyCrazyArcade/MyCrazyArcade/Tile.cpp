#include "stdafx.h"
#include "Tile.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
//#include <iostream>

CTile::CTile()
{
}


CTile::~CTile()
{
	Release();

}

void CTile::Initialize(void)
{
	m_bInitial = false;
	m_eRender = RENDER_BACKGROUND;
	m_tInfo.eObjType = OBJ_TILE;
	m_tInfo.fCX = TILECX;
	m_tInfo.fCY = TILECY;
	m_iOption = 0;

	m_InitialTime = GetTickCount();

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/Tile/ZeldaTile.bmp", L"Tile");

}

int CTile::Update(void)
{
	if ((m_bInitial) && (m_iOption == 2))
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ_BLOCK, CAbstractFactory<CBlock>::Create(m_tInfo.fX, m_tInfo.fY, OBJ_BLOCK, BLOCK_CANNOT_BREAK, 0, true));
		m_bDead = true;
	}
	if (m_bDead)
		return OBJ_DEAD;
	if ((!m_bInitial) && (m_InitialTime + 1000 < GetTickCount()))
	{
		m_bInitial = true;
	}
	

	Update_Rect();
	return OBJ_NOEVENT;
}

void CTile::Late_Update(void)
{

}

void CTile::Render(HDC hDC)
{
	//cout << (int)m_tInfo.fX << "\t" << (int)m_tInfo.fY << endl;

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Tile");

	
	GdiTransparentBlt(hDC, 					// ���� ����, ���������� �׸��� �׸� DC
		int(m_tRect.left),	// 2,3 ���� :  ������� ��ġ X, Y
		int(m_tRect.top),
		TILECX,			// 4,5 ���� : ������� ����, ���� ����
		TILECY,
		hMemDC,							// ��Ʈ���� ������ �ִ� DC
		0,0,
		TILECX, TILECY,
		RGB(255, 0, 255));			// �����ϰ��� �ϴ� ����
}

void CTile::Release(void)
{

}