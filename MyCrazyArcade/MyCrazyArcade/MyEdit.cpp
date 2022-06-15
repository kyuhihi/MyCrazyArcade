#include "stdafx.h"
#include "MyEdit.h"
#include "TileMgr.h"
#include "KeyMgr.h"
#include "BmpMgr.h"
#include "ObjMgr.h"


CMyEdit::CMyEdit()
{
}


CMyEdit::~CMyEdit()
{
	Release();
}

void CMyEdit::Initialize(void)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/BackBuffer.bmp", L"BackBuffer");

	CTileMgr::Get_Instance()->Initialize();
}

void CMyEdit::Update(void)
{
	CTileMgr::Get_Instance()->Update();
	Key_Input();

}

void CMyEdit::Late_Update(void)
{
	CTileMgr::Get_Instance()->Late_Update();
}

void CMyEdit::Render(HDC hDC)
{
	HDC		hGroundMemDC = CBmpMgr::Get_Instance()->Find_Image(L"BackBuffer");
	BitBlt(hDC, 0, 0, WINCX, WINCY, hGroundMemDC, 0, 0, SRCCOPY);
	CObjMgr::Get_Instance()->Render(hDC);
	CTileMgr::Get_Instance()->Render(hDC);

}

void CMyEdit::Release(void)
{
	CTileMgr::Get_Instance()->Destroy_Instance();
}

void CMyEdit::Key_Input(void)
{

	//if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
	//{
	//	POINT		pt;
	//	GetCursorPos(&pt);
	//	ScreenToClient(g_hWnd, &pt);
	//	CTileMgr::Get_Instance()->Picking_Tile(pt, 1, 0);
	//}

	//if (CKeyMgr::Get_Instance()->Key_Down('S'))
	//	CTileMgr::Get_Instance()->Save_Tile();
	//if (CKeyMgr::Get_Instance()->Key_Down('D'))
	//	CTileMgr::Get_Instance()->Load_Tile();
}


