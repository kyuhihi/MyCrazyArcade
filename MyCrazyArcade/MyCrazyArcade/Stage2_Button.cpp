#include "stdafx.h"
#include "Stage2_Button.h"
#include "BmpMgr.h"
#include "ObjMgr.h"

CStage2_Button::CStage2_Button()
{
}


CStage2_Button::~CStage2_Button()
{
}

void CStage2_Button::Initialize(void)
{
	m_eRender = RENDER_GAMEOBJECT;
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Block/Stage2Button.bmp", L"Stage2Button"); //���
	m_fDrawCX = 40.f;
	m_fDrawCY = 47.f;
	m_tInfo.fCX = TILECX;
	m_tInfo.fCY = TILECY;
}

int CStage2_Button::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;
	Update_Rect();
	Button_Update_Rect();
	return OBJ_NOEVENT;
}

void CStage2_Button::Late_Update(void)
{
}

void CStage2_Button::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Stage2Button");
	GdiTransparentBlt(hDC, 					// ���� ����, ���������� �׸��� �׸� DC
		int(m_DrawRect.left),	// 2,3 ���� :  ������� ��ġ X, Y
		int(m_DrawRect.top),
		m_fDrawCX,			// 4,5 ���� : ������� ����, ���� ����
		m_fDrawCY,
		hMemDC,							// ��Ʈ���� ������ �ִ� DC
		0, 0,
		m_fDrawCX, m_fDrawCY,
		RGB(255, 255, 255));			// �����ϰ��� �ϴ� ����
}

void CStage2_Button::Release(void)
{
}

void CStage2_Button::Button_Update_Rect()
{
	m_DrawRect.left = LONG(m_tInfo.fX - (m_fDrawCX * 0.5f));
	m_DrawRect.top = LONG(m_tInfo.fY - (m_fDrawCY * 0.5f));
	m_DrawRect.right = LONG(m_tInfo.fX + (m_fDrawCX* 0.5f));
	m_DrawRect.bottom = LONG(m_tInfo.fY + (m_fDrawCY * 0.5f));
}