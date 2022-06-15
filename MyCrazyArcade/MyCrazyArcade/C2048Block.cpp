#include "stdafx.h"
#include "C2048Block.h"
#include <iostream>
#include "BmpMgr.h"

C2048Block::C2048Block()
{
}


C2048Block::~C2048Block()
{
	Release();
}

void C2048Block::Initialize(void)
{
	m_tInfo.fCX = BLOCK_CX;
	m_tInfo.fCY = BLOCK_CX;

	m_iBlockNum = 0;
	m_fSpeed = 50.f;
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/MiniGame/Blank.bmp", L"Blank");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/MiniGame/2.bmp", L"2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/MiniGame/4.bmp", L"4");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/MiniGame/8.bmp", L"8");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/MiniGame/16.bmp", L"16");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/MiniGame/32.bmp", L"32");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/MiniGame/64.bmp", L"64");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/MiniGame/128.bmp", L"128");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/MiniGame/256.bmp", L"256");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/MiniGame/512.bmp", L"512");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/MiniGame/1024.bmp", L"1024");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/MiniGame/2048.bmp", L"2048");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/MiniGame/4096.bmp", L"4096");

	m_pFrameKey = L"Blank";
}

int C2048Block::Update(void)
{
	
	if (m_bDead)
		return OBJ_DEAD;

	Update_Rect();
	
	return OBJ_NOEVENT;
}

void C2048Block::Late_Update(void)
{
	AnimationMoving();
	Update_Num();
}

void C2048Block::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC, 				// 복사 받을, 최종적으로 그림을 그릴 DC
		int(m_tRect.left),				// 2,3 인자 :  복사받을 위치 X, Y
		int(m_tRect.top),
		int(m_tInfo.fCX),				// 4,5 인자 : 복사받을 가로, 세로 길이
		int(m_tInfo.fCY),
		hMemDC,							// 비트맵을 가지고 있는 DC
		0,								// 비트맵 출력 시작 좌표, X,Y
		0,
		(int)m_tInfo.fCX,				// 복사할 비트맵의 가로, 세로 길이
		(int)m_tInfo.fCY,
		RGB(255, 255, 255));			// 제거하
	//BitBlt(hDC, int(m_tRect.left), int(m_tRect.top), BLOCK_CX, BLOCK_CX, hMemDC, 0, 0, SRCCOPY);

}

void C2048Block::Release(void)
{
}

void C2048Block::SetBlock(int iNewNum)
{
	m_iBlockNum = iNewNum;
}

void C2048Block::Update_Num()
{
	if (m_iBlockNum == 0)
		m_pFrameKey = L"Blank";
	else if (m_iBlockNum == 2)
		m_pFrameKey = L"2";
	else if (m_iBlockNum == 4)
		m_pFrameKey = L"4";
	else if (m_iBlockNum == 8)
		m_pFrameKey = L"8";
	else if (m_iBlockNum == 16)
		m_pFrameKey = L"16";
	else if (m_iBlockNum == 32)
		m_pFrameKey = L"32";
	else if (m_iBlockNum == 64)
		m_pFrameKey = L"64";
	else if (m_iBlockNum == 128)
		m_pFrameKey = L"128";
	else if (m_iBlockNum == 256)
		m_pFrameKey = L"256";
	else if (m_iBlockNum == 512)
		m_pFrameKey = L"512";
	else if (m_iBlockNum == 1024)
		m_pFrameKey = L"1024";
	else if (m_iBlockNum == 2048)
		m_pFrameKey = L"2048";
	else if (m_iBlockNum == 4096)
		m_pFrameKey = L"4096";
}


void C2048Block::AnimationMoving()
{
	 if (m_eAnimationDir == DIR_LEFT) {
		m_tInfo.fX -= m_fSpeed;
	}
	else if (m_eAnimationDir == DIR_RIGHT) {
		m_tInfo.fX += m_fSpeed;
	}
	else if (m_eAnimationDir == DIR_UP) {
		m_tInfo.fY -= m_fSpeed;
	}
	else if (m_eAnimationDir == DIR_DOWN) {
		m_tInfo.fY += m_fSpeed;
	}
	
}