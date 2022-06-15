#include "stdafx.h"
#include "Monster.h"
#include "ObjMgr.h"
#include "KeyMgr.h"
#include "BmpMgr.h"
#include "AbstractFactory.h"
#include "Bullet.h"
#include <iostream>


CMonster::CMonster()
	: m_eCurState(IDLE), m_ePreState(END) , m_eCurDirection(DIR_END), m_ePreDirection(DIR_END) ,m_eMode(MODE_FIND)
{
}


CMonster::~CMonster()
{
	Release();

}

void CMonster::Initialize(void)
{
	m_tInfo.fX = 600.f;
	m_tInfo.fY = 528.f;

	m_tTargetTile.fX = 600.f;
	m_tTargetTile.fY = 540.f;

	m_fDrawCX = 70.f;
	m_fDrawCY = 70.f;

	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 30.f;

	m_BeforeMove.iX = -1;
	m_BeforeMove.iY = -1;

	m_StdTile.fX = m_tInfo.fX;
	m_StdTile.fY = m_tInfo.fY;

	m_BulletSpot.fX = 600.f;
	m_BulletSpot.fY = 540.f;

	m_tShadowMove.iX = 14;
	m_tShadowMove.iY = 13;

	m_fSpeed = 3.f;

	m_iWaterPower = 1;
	m_AvoidTime = 0;
	m_eRender = RENDER_GAMEOBJECT;
	m_tInfo.eObjType = OBJ_MONSTER;
	m_iBulletCapacity = 1;
	m_iBulletMax = m_iBulletCapacity;

	m_bNeedle = false;
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/monsterAI.bmp", L"MonsterMove");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/monsterAIbubble.bmp", L"MonsterBubble");

	m_pFrameKey = L"MonsterMove";

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iMotion = 0;
	m_tFrame.dwSpeed = 100;
	m_tFrame.dwTime = GetTickCount();
	m_ModeDelay = 0;
	m_FindTime = 0;
	m_bMode = false;
	m_bFind_Timer = false;

	m_bFightMode = false;
	m_bNeedle = true;
	m_bFindModeBullet = false;

	m_bDebugMode = false;
	
	m_PangTime = 50000;
	m_DeadTime = 50000;
	m_NotDeadTime = 50000;

}
void CMonster::DeletePath()
{
	if (!m_MoveHow.empty()) {
		auto iter = m_MoveHow.begin();
		int TempX = 40 + ((*iter)->iX  * TILECX); //���������ϴ� �߽� ��ǥ.
		int TempY = 60 + ((*iter)->iY  * TILECX);
		RECT TargetTemp{ LONG(TempX - 20), LONG(TempY - 20), LONG(TempX + 20) ,LONG(TempY + 20) };
		if ((TargetTemp.bottom > m_tRect.bottom) && (TargetTemp.top < m_tRect.top) && (TargetTemp.left < m_tRect.left) && (TargetTemp.right > m_tRect.right))
		{//�̶� �ش��ϴ� ����Ʈ�� ��带 ��������.
			Safe_Delete<MonsterShadowIndex*>(*iter);
			m_MoveHow.pop_front();
		}
	}
}
void	CMonster::DebuggingKey()
{
	if (CKeyMgr::Get_Instance()->Key_Up(VK_F9)) {
		if(m_bDebugMode == false)
			m_bDebugMode = true;
		else if(m_bDebugMode)
			m_bDebugMode = false;
	}
}
void CMonster::DeadCheck()
{
	if (m_PangTime != 50000)
	{
		if (m_bNeedle)
		{
			Set_Player_State(BREAKBUBBLE);
			m_bNeedle = false;
			m_PangTime = 50000;
			m_eMode = MODE_AVOID;
			m_NotDeadTime = GetTickCount();
		}
		else if (m_PangTime + 3000 < GetTickCount())
		{
			Set_Player_State(PANGBUBBLE);
			m_DeadTime = GetTickCount();
			m_PangTime = 50000;
		}
	}
	else if (m_DeadTime != 50000)
	{
		if (m_DeadTime + 1000 < GetTickCount())
		{
			m_bDead = true;
		}
	}

	if ((m_NotDeadTime != 50000) && (m_NotDeadTime + 1000 < GetTickCount()))
	{
		m_NotDeadTime = 50000;
		Set_Player_State(IDLE);
	}
}
int CMonster::Update(void)
{
	DebuggingKey();
	DeadCheck();

	if (m_bDead)
		return OBJ_DEAD;
	DeletePath();

	ModeSelect();

	if (m_eMode == MODE_AVOID)
	{
		DirectionCheck();
		Nail_Down();
		Draw_Update_Rect();
		Monster_Update_Rect();
	}
	else if(m_eMode == MODE_FIND)
	{
		DirectionCheck();
		Nail_Down();
		Draw_Update_Rect();
		Monster_Update_Rect();
	}
	Direction_Change();//��ǹٲٴ°�.
	MonsterMoveTile();

	Monster_Dont_Move();

	/*	Draw_Update_Rect();
		Monster_Update_Rect();*/

	return OBJ_NOEVENT;
}
void CMonster::ModeSelect()
{
	Avoid_Bullet();
	if (m_eMode == MODE_FIND) {
		if ((m_tTargetTile.fX != -1) && (m_tTargetTile.fY != -1))
		{//������ε��϶��� �����̸� �߰��Ѵ�.
			if ((m_AvoidTime == 0) || m_AvoidTime + 800 < GetTickCount()) {
				m_bTime = false;
				m_AvoidTime = GetTickCount();
				EnemySetBullet();
			}
		}
	}
	
}

void CMonster::DirectionCheck()
{//Avoid Bullet ���� �̹� ƥ�ڸ��� ������ ��Ȳ.
	int DistanceX(int((m_BulletSpot.fX - m_tTargetTile.fX) / 40)); //�¿���Ϸ� ��ĭ�̵��ؾ��ϴ��� ���ð�.
	int DistanceY(int((m_BulletSpot.fY - m_tTargetTile.fY) / 40));
	//�ִܰŸ��� �����δ�.
	if (!m_MoveHow.empty()) {
		m_BeforeMove = *m_MoveHow.back();
	}
	else {
		m_BeforeMove.iX = -1;
		m_BeforeMove.iY = -1;
	}
	//<< m_tTargetTile.fX << "\t" << m_tTargetTile.fY << endl;
	if (DistanceX != 0 || DistanceY != 0)
	{
		if (DistanceX > 0 && (m_BeforeMove.iX != m_tShadowMove.iX - 1 && m_BeforeMove.iY != m_tShadowMove.iY))
		{//�̶� �¸� Ǫ�ù�
			if (!CObjMgr::Get_Instance()->If_Block(m_tShadowMove.iX - 1, m_tShadowMove.iY)) {
				MonsterShadowIndex* Temp = new MonsterShadowIndex(m_tShadowMove.iX - 1, m_tShadowMove.iY);
				--m_tShadowMove.iX;
				--DistanceX;
				m_MoveHow.push_back(Temp);
			}
		}
		else if (DistanceX < 0 && (m_BeforeMove.iX != m_tShadowMove.iX + 1 && m_BeforeMove.iY != m_tShadowMove.iY))
		{//�̶� �츦 Ǫ�ù�
			if (!CObjMgr::Get_Instance()->If_Block(m_tShadowMove.iX + 1, m_tShadowMove.iY)) {
				MonsterShadowIndex* Temp = new MonsterShadowIndex(m_tShadowMove.iX + 1, m_tShadowMove.iY);
				++m_tShadowMove.iX;
				++DistanceX;
				m_MoveHow.push_back(Temp);
			}
		}

		if (DistanceY > 0 && (m_BeforeMove.iX != m_tShadowMove.iX && m_BeforeMove.iY != m_tShadowMove.iY - 1))
		{//�̶� ���� Ǫ�ù�
			if (!CObjMgr::Get_Instance()->If_Block(m_tShadowMove.iX, m_tShadowMove.iY-1)) {
				MonsterShadowIndex* Temp = new MonsterShadowIndex(m_tShadowMove.iX, m_tShadowMove.iY - 1);
				--m_tShadowMove.iY;
				--DistanceY;
				m_MoveHow.push_back(Temp);
			}
		}
		else if (DistanceY < 0 && (m_BeforeMove.iX != m_tShadowMove.iX && m_BeforeMove.iY != m_tShadowMove.iY + 1))
		{//�̶� �Ʒ��� Ǫ�ù�
			if (!CObjMgr::Get_Instance()->If_Block(m_tShadowMove.iX, m_tShadowMove.iY + 1)) {
				MonsterShadowIndex* Temp = new MonsterShadowIndex(m_tShadowMove.iX, m_tShadowMove.iY + 1);
				++m_tShadowMove.iY;
				++DistanceY;
				m_MoveHow.push_back(Temp);
			}
		}
	}
}

void CMonster::Avoid_How(const int iNewPathX, const int iNewPathY,int& DistanceX,int& DistanceY)
{
	
		MonsterShadowIndex TempMove(iNewPathX, iNewPathY);
		if (!CObjMgr::Get_Instance()->If_Block(((TempMove).iX), (TempMove).iY))
		{//�ش�Ÿ���� ����̶��
			if ((!CObjMgr::Get_Instance()->If_Block(m_tShadowMove.iX - 1, m_tShadowMove.iY)))
			{//�� Ž��
				MonsterShadowIndex* Temp = new MonsterShadowIndex(m_tShadowMove.iX - 1, m_tShadowMove.iY);
				m_MoveHow.push_back(Temp);
				--m_tShadowMove.iX;
				--DistanceX;
				return;
			}
			else if ((!CObjMgr::Get_Instance()->If_Block(m_tShadowMove.iX + 1, m_tShadowMove.iY)))
			{//�� Ž��
				MonsterShadowIndex* Temp = new MonsterShadowIndex(m_tShadowMove.iX + 1, m_tShadowMove.iY);
				m_MoveHow.push_back(Temp);
				++m_tShadowMove.iX;
				++DistanceX;
				cout << "RIGHT" << endl;
				return;
			}
			else if ((!CObjMgr::Get_Instance()->If_Block(m_tShadowMove.iX, m_tShadowMove.iY - 1)))
			{//�� Ž��
				MonsterShadowIndex* Temp = new MonsterShadowIndex(m_tShadowMove.iX, m_tShadowMove.iY - 1);
				m_MoveHow.push_back(Temp);
				--m_tShadowMove.iY;
				--DistanceY;
				cout << "UP" << endl;
				return;
			}
			else if ((!CObjMgr::Get_Instance()->If_Block(m_tShadowMove.iX, m_tShadowMove.iY + 1)))
			{//�� Ž��
				MonsterShadowIndex* Temp = new MonsterShadowIndex(m_tShadowMove.iX, m_tShadowMove.iY + 1);
				m_MoveHow.push_back(Temp);
				++m_tShadowMove.iY;
				cout << "DOWN" << endl;
				++DistanceY;
				return;
			}

		}
}




void CMonster::Nail_Down()
{
	if (!m_MoveHow.empty())
	{
		auto iter = m_MoveHow.begin();
		int TempX = 40 + ((*iter)->iX  * TILECX); //���������ϴ� �߽� ��ǥ.
		int TempY = 60 + ((*iter)->iY  * TILECX);
		//RECT TargetTemp{ LONG(TempX - 20), LONG(TempY - 20), LONG(TempX + 20) ,LONG(TempY + 20) };
		//if ((TargetTemp.bottom > m_tRect.bottom) && (TargetTemp.top < m_tRect.top) && (TargetTemp.left < m_tRect.left) && (TargetTemp.right > m_tRect.right))
		//{//�̶� �ش��ϴ� ����Ʈ�� ��带 ��������.
		//	Safe_Delete<MonsterShadowIndex*>(*iter);
		//	m_MoveHow.pop_front();
		//	Nail_Down();
		//}
		float DistanceX = m_tInfo.fX - (float)TempX;
		float DistanceY = m_tInfo.fY - (float)TempY;
		if (abs(DistanceX) < abs(DistanceY))
		{//Y���� ���̰�ũ�ϱ� Y�ο�������.
			if (DistanceY > 0)
			{//��
					m_eCurDirection = DIR_UP;
				
			}
			else if (DistanceY < 0)
			{//�Ʒ�
					m_eCurDirection = DIR_DOWN;
			}
		}
		else
		{//X���� ���̰�ũ�ϱ� X�ο�������.
			if (DistanceX > 0)
			{//��
					m_eCurDirection = DIR_LEFT;
			}
			else if (DistanceX < 0)
			{//��
					m_eCurDirection = DIR_RIGHT;
			}
		}
	}
	else {
		if (m_eMode == MODE_FIND && !m_bTime) {
			m_AvoidTime = GetTickCount();
			m_bTime = true;
		}
		m_eCurDirection = DIR_END;
	}
}

void CMonster::Late_Update(void)
{
	Motion_Change();
	Move_Frame();

	float iDistanceX = m_StdTile.fX - m_tInfo.fX;
	float iDistanceY = m_StdTile.fY - m_tInfo.fY;
	if (iDistanceX < -20) { //�÷��̾� ���������� ����
		m_BulletSpot.fX += TILECX;
		m_StdTile.fX += TILECX;
	}
	else if (iDistanceX > 20) {//�÷��̾� �������ΰ���
		if (m_BulletSpot.fX > 40) {
			m_BulletSpot.fX -= TILECX;
			m_StdTile.fX -= TILECX;
		}
	}
	else if (iDistanceY > 20) {//�÷��̾� ���ΰ���
		if (m_BulletSpot.fY > 60) {
			m_BulletSpot.fY -= TILECX;
			m_StdTile.fY -= TILECX;
		}
	}
	else if (iDistanceY < -20) { //�÷��̾� �Ʒ��ΰ���
								 //if (m_BulletSpot.fY > 60) {
		m_BulletSpot.fY += TILECX;
		m_StdTile.fY += TILECX;

	}
	

}

void CMonster::Render(HDC hDC)
{
	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	GdiTransparentBlt(hDC, 								// ���� ����, ���������� �׸��� �׸� DC
		int(m_DrawRect.left),							// 2,3 ���� :  ������� ��ġ X, Y
		int(m_DrawRect.top),
		(int)m_fDrawCX,										// 4,5 ���� : ������� ����, ���� ����
		(int)m_fDrawCY,
		hMemDC,											// ��Ʈ���� ������ �ִ� DC
		m_tFrame.iFrameStart * (int)m_fDrawCX,			// ��Ʈ�� ��� ���� ��ǥ, X,Y
		m_tFrame.iMotion * (int)m_fDrawCY,
		(int)m_fDrawCX, (int)m_fDrawCY,
		RGB(0, 255, 0));								// ����
	//RECT TargetTemp{ m_tTargetTile.fX - 20, m_tTargetTile.fY - 20,m_tTargetTile.fX + 20,m_tTargetTile.fY + 20 };
	//SelectObject(hDC, GetStockObject(DC_BRUSH));

	/*if (m_eMode == MODE_AVOID) {
		SetDCBrushColor(hDC, RGB(255, 0, 0));
		Rectangle(hDC, TargetTemp.left, TargetTemp.top, TargetTemp.right, TargetTemp.bottom);
	}
	else if(m_eMode ==MODE_FIND) {
		SetDCBrushColor(hDC, RGB(0, 0, 255));
		Rectangle(hDC, int(m_BulletSpot.fX - 20.f), int(m_BulletSpot.fY - 20.f), int(m_BulletSpot.fX + 20.f), int(m_BulletSpot.fY + 20.f));
	}
	else if (m_eMode == MODE_FIGHT) {
		SetDCBrushColor(hDC, RGB(100, 100, 100));
		Rectangle(hDC, int(m_BulletSpot.fX - 20.f), int(m_BulletSpot.fY - 20.f), int(m_BulletSpot.fX + 20.f), int(m_BulletSpot.fY + 20.f));
	}
	SetDCBrushColor(hDC, RGB(0, 255, 0));
	Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	SetDCBrushColor(hDC, RGB(255, 255, 255));*/

}

void CMonster::Release(void)
{
	if (!m_MoveHow.empty()) {
		for (auto& iter : m_MoveHow) {
			Safe_Delete<MonsterShadowIndex*>(iter);
			iter++;
		}
		m_MoveHow.clear();
	}
}

void CMonster::Draw_Update_Rect(void)
{
	m_DrawRect.left = LONG(m_tInfo.fX - (m_fDrawCX * 0.5f));
	m_DrawRect.top = LONG(m_tInfo.fY - (m_fDrawCY * 0.5f));
	m_DrawRect.right = LONG(m_tInfo.fX + (m_fDrawCX* 0.5f));
	m_DrawRect.bottom = LONG(m_tInfo.fY + (m_fDrawCY * 0.5f));
}

void CMonster::Monster_Update_Rect()
{
	m_tRect.left = LONG(m_tInfo.fX - (m_tInfo.fCX * 0.5f));
	m_tRect.top = LONG(m_tInfo.fY - (m_tInfo.fCX * 0.5f));
	m_tRect.right = LONG(m_tInfo.fX + (m_tInfo.fCY* 0.5f));
	m_tRect.bottom = LONG(m_tInfo.fY + (m_tInfo.fCY * 0.5f));
}

void CMonster::Set_Append_Item(ITEMTYPE eType)
{
	if (eType == ITEM_WATERBOMB) {
		if (m_iBulletMax < 7) {
			++m_iBulletMax;
			++m_iBulletCapacity;
		}
	}
	else if (eType == ITEM_ROLLER) {
		m_fSpeed += 0.5f;
	}
	else if (eType == ITEM_MEDICINE) {
		if (m_iWaterPower < 6) {
			++m_iWaterPower;
		}
	}
	else if (eType == ITEM_NEEDLE)
		m_bNeedle = true;
}

//���� ��� ����
void CMonster::Motion_Change(void)
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case WHILEBUBBLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 1000;
			m_tFrame.dwTime = GetTickCount();
			m_pFrameKey = L"MonsterBubble";
			break;
		case BREAKBUBBLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iMotion = 1;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount();
			m_pFrameKey = L"MonsterBubble";
			break;
		case PANGBUBBLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 2;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			m_pFrameKey = L"MonsterBubble";
			break;
		case WIN:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iMotion = 3;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			m_pFrameKey = L"MonsterBubble";
			break;
		}
		m_ePreState = m_eCurState;
	}
}
//��ǹٲٴ°�.
void CMonster::Direction_Change()
{
	if (m_eCurState == IDLE) {

		if (m_ePreDirection != m_eCurDirection)
		{
			switch (m_eCurDirection)
			{
			case DIR_LEFT:
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 4;
				m_tFrame.iMotion = 3;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_pFrameKey = L"MonsterMove";
				break;
			case DIR_RIGHT:
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 4;
				m_tFrame.iMotion = 2;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_pFrameKey = L"MonsterMove";
				break;
			case DIR_DOWN:
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 4;
				m_tFrame.iMotion = 0;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_pFrameKey = L"MonsterMove";
				break;
			case DIR_UP:
				m_tFrame.iFrameStart = 0;
				m_tFrame.iFrameEnd = 4;
				m_tFrame.iMotion = 1;
				m_tFrame.dwSpeed = 100;
				m_tFrame.dwTime = GetTickCount();
				m_pFrameKey = L"MonsterMove";
				break;
			}

			m_ePreDirection = m_eCurDirection;
		}
	}


}

void CMonster::Avoid_Bullet()
{
	if (CObjMgr::Get_Instance()->GetWater_Spot(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
	{//now CautionBlock
		if (m_eMode == MODE_FIND)
		{
			if (!m_MoveHow.empty())
			{
				for (auto iter = m_MoveHow.begin(); iter != m_MoveHow.end(); ++iter)
				{
					Safe_Delete<MonsterShadowIndex*>(*iter);
				}
				m_MoveHow.clear();
				m_tTargetTile.fX = -1;
				m_tTargetTile.fY = -1;
			}
			m_tShadowMove.iX = int((m_BulletSpot.fX - 40) / TILECX);
			m_tShadowMove.iY = int((m_BulletSpot.fY - 60) / TILECX);
		}
		m_eMode = MODE_AVOID;
		m_bMode = false;
		m_ModeDelay = 0;
		Avoid_Direction();
	}
	else
	{
		if (!m_bFightMode) {
			if (!m_bMode) {
				m_bMode = true;
				m_ModeDelay = GetTickCount();
			}
			if (m_bMode && (m_ModeDelay + 500 < GetTickCount())) {
				m_eMode = MODE_FIND;
				m_bMode = false;
				m_ModeDelay = 0;
			}
		}
		else if (m_bFightMode)
		{//���⼭ �ο��� ��������.
			EnemySetBullet();
		}
	}
}

void CMonster::Avoid_Direction()
{
	m_tTargetTile.fX = m_BulletSpot.fX;
	m_tTargetTile.fY = m_BulletSpot.fY;
	if (!CObjMgr::Get_Instance()->GetWater_Spot(int((m_BulletSpot.fX - 40) / TILECX - 1), int((m_BulletSpot.fY - 60) / TILECX)))
	{//��
		if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40) / TILECX - 1), int((m_BulletSpot.fY - 60) / TILECX)))
		{
			m_tTargetTile.fX -= TILECX;

			return;
		}
	}
	if (!CObjMgr::Get_Instance()->GetWater_Spot(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 - TILECX) / TILECX)))
	{//��
		if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60) / TILECX - 1)))
		{
			m_tTargetTile.fY -= TILECX;
			return;
		}

	}
	if (!CObjMgr::Get_Instance()->GetWater_Spot(int((m_BulletSpot.fX - 40) / TILECX + 1), int((m_BulletSpot.fY - 60) / TILECX)))
	{//��
		if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40) / TILECX + 1), int((m_BulletSpot.fY - 60) / TILECX)))
		{
			m_tTargetTile.fX += TILECX;
			return;
		}

	}
	if (!CObjMgr::Get_Instance()->GetWater_Spot(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60) / TILECX + 1)))
	{//��
		if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60) / TILECX + 1)))
		{
			m_tTargetTile.fY += TILECX;

			return;
		}
	}
	if (!CObjMgr::Get_Instance()->GetWater_Spot(int((m_BulletSpot.fX - 40 - TILECX * 2) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
	{//����
		if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 - TILECX * 2) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
		{
			if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 - TILECX) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
			{
				m_tTargetTile.fX -= TILECX * 2;
				return;
			}
		}
	}
	if (!CObjMgr::Get_Instance()->GetWater_Spot(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 - TILECX * 2) / TILECX)))
	{//���
		if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 - TILECX * 2) / TILECX)))
		{
			/*if (!CObjMgr::Get_Instance()->GetWater_Spot(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 - TILECX ) / TILECX)))
			{*/
			if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 - TILECX) / TILECX)))
			{
				m_tTargetTile.fY -= TILECX * 2;
				//cout << 5 << endl;
				return;
			}
			//}
		}

	}
	if (!CObjMgr::Get_Instance()->GetWater_Spot(int((m_BulletSpot.fX - 40 + TILECX * 2) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
	{//���
		if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 + TILECX * 2) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
		{
			/*	if (!CObjMgr::Get_Instance()->GetWater_Spot(int((m_BulletSpot.fX - 40 + TILECX) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
			{*/
			if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 + TILECX) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
			{
				m_tTargetTile.fX += TILECX * 2;
				//cout << 6 << endl;
				return;
				//	}
			}
			//	}
		}

	}
	if (!CObjMgr::Get_Instance()->GetWater_Spot(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 + TILECX * 2) / TILECX)))
	{//����
		if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 + TILECX * 2) / TILECX)))
		{
			/*	if (!CObjMgr::Get_Instance()->GetWater_Spot(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 + TILECX) / TILECX)))
			{*/
			if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 + TILECX) / TILECX)))
			{
				m_tTargetTile.fY += TILECX * 2;
				//cout << 7 << endl;

				return;
			}
			//}
			//}
			/*m_tTargetTile.fY += TILECX * 2;
			return;*/
		}
	}
	if (!CObjMgr::Get_Instance()->GetWater_Spot(int((m_BulletSpot.fX - 40 - TILECX) / TILECX), int((m_BulletSpot.fY - 60 - TILECX) / TILECX)))
	{//�»�
		if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 - TILECX) / TILECX), int((m_BulletSpot.fY - 60 - TILECX) / TILECX)))
		{
			if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 - TILECX) / TILECX)))
			{//��
				m_tTargetTile.fY -= TILECX;
				m_tTargetTile.fX -= TILECX;
				//cout << "L1U1" << endl;
				return;
			}
			else if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 - TILECX) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
			{//��
				m_tTargetTile.fY -= TILECX;
				m_tTargetTile.fX -= TILECX;
				//cout << "L1U1" << endl;
				return;
			}
		}
	}
	if (!CObjMgr::Get_Instance()->GetWater_Spot(int((m_BulletSpot.fX - 40 + TILECX) / TILECX), int((m_BulletSpot.fY - 60 - TILECX) / TILECX)))
	{//���
		if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 + TILECX) / TILECX), int((m_BulletSpot.fY - 60 - TILECX) / TILECX)))
		{
			if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 + TILECX) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
			{//��
				m_tTargetTile.fY -= TILECX;
				m_tTargetTile.fX += TILECX;
				//cout << 1 << endl;
				return;
			}
			else if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 - TILECX) / TILECX)))
			{//��
				m_tTargetTile.fY -= TILECX;
				m_tTargetTile.fX += TILECX;
				//cout << 1 << endl;
				return;
			}
		}
	}

	if (!CObjMgr::Get_Instance()->GetWater_Spot(int((m_BulletSpot.fX - 40 - TILECX) / TILECX), int((m_BulletSpot.fY - 60 + TILECX) / TILECX)))
	{//����
		if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 - TILECX) / TILECX), int((m_BulletSpot.fY - 60 + TILECX) / TILECX)))
		{
			if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 - TILECX) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
			{//��
				m_tTargetTile.fY += TILECX;
				m_tTargetTile.fX -= TILECX;
				//cout << 2 << endl;
				return;
			}
			else if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 + TILECX) / TILECX)))
			{//��
				m_tTargetTile.fY += TILECX;
				m_tTargetTile.fX -= TILECX;
				//cout << 2 << endl;
				return;
			}

		}

	}
	if (!CObjMgr::Get_Instance()->GetWater_Spot(int((m_BulletSpot.fX - 40 - TILECX) / TILECX), int((m_BulletSpot.fY - 60 + TILECX) / TILECX)))
	{//����
		if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 - TILECX) / TILECX), int((m_BulletSpot.fY - 6060 + TILECX) / TILECX)))
		{

			if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 - TILECX) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
			{//��
				m_tTargetTile.fY += TILECX;
				m_tTargetTile.fX += TILECX;
				//cout << 3 << endl;
				return;
			}
			else if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 + TILECX) / TILECX)))
			{//��
				m_tTargetTile.fY += TILECX;
				m_tTargetTile.fX += TILECX;
				//cout << 3 << endl;
				return;
			}
		}
	}
	if (!CObjMgr::Get_Instance()->GetWater_Spot(int((m_BulletSpot.fX - 40 - TILECX * 3) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
	{//������
		if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 - TILECX * 3) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
		{
			if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 - TILECX * 2) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
			{
				if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 - TILECX) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
				{
					m_tTargetTile.fX -= TILECX * 3;
					return;

				}
			}
		}
	}
	if (!CObjMgr::Get_Instance()->GetWater_Spot(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 - TILECX * 3) / TILECX)))
	{//����
		if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 - TILECX * 3) / TILECX)))
		{
			if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 - TILECX * 2) / TILECX)))
			{
				if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 - TILECX) / TILECX)))
				{
					m_tTargetTile.fY -= TILECX * 3;
					return;
				}
			}
		}

	}
	if (!CObjMgr::Get_Instance()->GetWater_Spot(int((m_BulletSpot.fX - 40 + TILECX * 3) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
	{//����
		if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 + TILECX * 3) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
		{
			if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 + TILECX * 2) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
			{
				if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 + TILECX) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
				{
					m_tTargetTile.fX += TILECX * 3;
					return;
				}
			}
		}

	}
	if (!CObjMgr::Get_Instance()->GetWater_Spot(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 + TILECX * 3) / TILECX)))
	{//������
		if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 + TILECX * 3) / TILECX)))
		{
			if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 + TILECX * 2) / TILECX)))
			{
				if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 + TILECX) / TILECX)))
				{
					m_tTargetTile.fY += TILECX * 3;
					//cout << 7 << endl;

					return;
				}
			}
		
		}
	}
	if (!CObjMgr::Get_Instance()->GetWater_Spot(int((m_BulletSpot.fX - 40 - TILECX) / TILECX), int((m_BulletSpot.fY - 60 - TILECX * 2) / TILECX)))
	{//�����
		if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 - TILECX) / TILECX), int((m_BulletSpot.fY - 60 - TILECX * 2) / TILECX)))
		{//�����
			if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 - TILECX * 2) / TILECX)))
			{//���
				if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 - TILECX) / TILECX)))
				{//��
					m_tTargetTile.fY -= TILECX * 2;
					m_tTargetTile.fX -= TILECX;

					return;
				}
			}
			else if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 - TILECX) / TILECX), int((m_BulletSpot.fY - 60 - TILECX) / TILECX)))
			{//�»�
				if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 - TILECX) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
				{//��
					m_tTargetTile.fY -= TILECX * 2;
					m_tTargetTile.fX -= TILECX;
					return;
				}
			}
		}
		
	}
	if (!CObjMgr::Get_Instance()->GetWater_Spot(int((m_BulletSpot.fX - 40 + TILECX) / TILECX), int((m_BulletSpot.fY - 60 - TILECX * 2) / TILECX)))
	{//����
		if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 + TILECX) / TILECX), int((m_BulletSpot.fY - 60 - TILECX * 2) / TILECX)))
		{//����
			if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 - TILECX * 2) / TILECX)))
			{//���
				if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 - TILECX) / TILECX)))
				{//��
					m_tTargetTile.fY -= TILECX * 2;
					m_tTargetTile.fX += TILECX;
					//cout << 9 << endl;
					return;
				}
			}
			else if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 + TILECX) / TILECX), int((m_BulletSpot.fY - 60 - TILECX) / TILECX)))
			{//���
				if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 + TILECX) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
				{//��
					m_tTargetTile.fY -= TILECX * 2;
					m_tTargetTile.fX += TILECX;
					//cout << 9 << endl;
					return;
				}
			}
		
		}
		/*m_tTargetTile.fY -= TILECX * 2;
		m_tTargetTile.fX += TILECX;
		return;*/
	}
	if (!CObjMgr::Get_Instance()->GetWater_Spot(int((m_BulletSpot.fX - 40 + TILECX * 2) / TILECX), int((m_BulletSpot.fY - 60 - TILECX) / TILECX)))
	{//����
		if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 + TILECX * 2) / TILECX), int((m_BulletSpot.fY - 60 - TILECX) / TILECX)))
		{//����
			if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 + TILECX * 2) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
			{//���
				if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 + TILECX ) / TILECX), int((m_BulletSpot.fY - 60 ) / TILECX)))
				{//��
					m_tTargetTile.fY -= TILECX;
					m_tTargetTile.fX += TILECX * 2;
					return;
				}
			}
			else if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 + TILECX) / TILECX), int((m_BulletSpot.fY - 60 - TILECX) / TILECX)))
			{//���
				if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60- TILECX) / TILECX)))
				{//��
					m_tTargetTile.fY -= TILECX;
					m_tTargetTile.fX += TILECX * 2;
					return;
				}
			}
		}
	}
	if (!CObjMgr::Get_Instance()->GetWater_Spot(int((m_BulletSpot.fX - 40 + TILECX * 2) / TILECX), int((m_BulletSpot.fY - 60 + TILECX) / TILECX)))
	{//�����
		if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 + TILECX * 2) / TILECX), int((m_BulletSpot.fY - 60 + TILECX) / TILECX)))
		{//�����
			if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 + TILECX * 2) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
			{//���
				if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 + TILECX) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
				{//��
					m_tTargetTile.fY += TILECX;
					m_tTargetTile.fX += TILECX * 2;
					return;
				}
			}
			else if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 + TILECX) / TILECX), int((m_BulletSpot.fY - 60 +TILECX) / TILECX)))
			{//�Ͽ�
				if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 + TILECX) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
				{//��
					m_tTargetTile.fY += TILECX;
					m_tTargetTile.fX += TILECX * 2;
					return;
				}
			}
		}
	}
	if (!CObjMgr::Get_Instance()->GetWater_Spot(int((m_BulletSpot.fX - 40 - TILECX) / TILECX), int((m_BulletSpot.fY - 60 + TILECX * 2) / TILECX)))
	{//������
		if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 - TILECX) / TILECX), int((m_BulletSpot.fY - 60 + TILECX * 2) / TILECX)))
		{//������
			if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 + TILECX * 2) / TILECX)))
			{//����
				if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 ) / TILECX), int((m_BulletSpot.fY - 60 + TILECX) / TILECX)))
				{//��
					m_tTargetTile.fY += TILECX * 2;
					m_tTargetTile.fX -= TILECX;
					return;
				}
			}
			else if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 + TILECX ) / TILECX)))
			{//����
				if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 - TILECX) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
				{//��
					m_tTargetTile.fY += TILECX * 2;
					m_tTargetTile.fX -= TILECX;
					return;
				}
			}
		}
		
	}
	if (!CObjMgr::Get_Instance()->GetWater_Spot(int((m_BulletSpot.fX - 40 + TILECX) / TILECX), int((m_BulletSpot.fY - 60 + TILECX * 2) / TILECX)))
	{//���Ͽ�
		if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 + TILECX) / TILECX), int((m_BulletSpot.fY - 60 + TILECX * 2) / TILECX)))
		{//���Ͽ�
			if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 + TILECX * 2) / TILECX)))
			{//����
				if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 + TILECX) / TILECX)))
				{//��
					m_tTargetTile.fY += TILECX * 2;
					m_tTargetTile.fX += TILECX;
					return;
				}
			}
			else if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 + TILECX) / TILECX), int((m_BulletSpot.fY - 60 + TILECX) / TILECX)))
			{//����
				if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX + 40) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
				{//��
					m_tTargetTile.fY += TILECX * 2;
					m_tTargetTile.fX += TILECX;
					return;
				}
			}
		}
	}
	if (!CObjMgr::Get_Instance()->GetWater_Spot(int((m_BulletSpot.fX - 40 - TILECX * 2) / TILECX), int((m_BulletSpot.fY - 60 - TILECX) / TILECX)))
	{//���»�
		if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 - TILECX * 2) / TILECX), int((m_BulletSpot.fY - 60 - TILECX) / TILECX)))
		{//���»�
			if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 - TILECX * 2) / TILECX), int((m_BulletSpot.fY - 60 ) / TILECX)))
			{//����
				if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 - TILECX ) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
				{//��
					m_tTargetTile.fY -= TILECX;
					m_tTargetTile.fX -= TILECX * 2;
					return;
				}
			}
			else if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 - TILECX) / TILECX), int((m_BulletSpot.fY - 60 - TILECX) / TILECX)))
			{//�»�
				if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40) / TILECX), int((m_BulletSpot.fY - 60 - TILECX) / TILECX)))
				{//��
					m_tTargetTile.fY -= TILECX;
					m_tTargetTile.fX -= TILECX * 2;
					return;
				}
			}
		}
		/*m_tTargetTile.fY -= TILECX;
		m_tTargetTile.fX -= TILECX * 2;
		return;*/
	}
	if (!CObjMgr::Get_Instance()->GetWater_Spot(int((m_BulletSpot.fX - 40 - TILECX * 2) / TILECX), int((m_BulletSpot.fY - 60 + TILECX) / TILECX)))
	{//������
		if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 - TILECX * 2) / TILECX), int((m_BulletSpot.fY - 60 + TILECX) / TILECX)))
		{//������
			if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 - TILECX * 2) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
			{//����
				if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 - TILECX) / TILECX), int((m_BulletSpot.fY - 60) / TILECX)))
				{//��
					m_tTargetTile.fY += TILECX;
					m_tTargetTile.fX -= TILECX * 2;
					return;
				}
			}
			else if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 - TILECX ) / TILECX), int((m_BulletSpot.fY - 60 + TILECX) / TILECX)))
			{//����
				if (!CObjMgr::Get_Instance()->If_Block(int((m_BulletSpot.fX - 40 ) / TILECX), int((m_BulletSpot.fY - 60 + TILECX) / TILECX)))
				{//��
					m_tTargetTile.fY += TILECX;
					m_tTargetTile.fX -= TILECX * 2;
					return;
				}
			}
		}
	}
}

void CMonster::MonsterMoveTile()
{
	if (m_eCurState == WHILEBUBBLE)
	{
		return;
	}
	else if (m_eCurState == PANGBUBBLE)
		return;

	if (m_eCurDirection == DIR_END)
		return;
	else if (m_eCurDirection == DIR_UP)
	{
		m_tInfo.fY -= m_fSpeed;
	}
	else if (m_eCurDirection == DIR_DOWN)
	{
		m_tInfo.fY += m_fSpeed;
	}
	else if (m_eCurDirection == DIR_LEFT)
	{
		m_tInfo.fX -= m_fSpeed;
	}
	else if (m_eCurDirection == DIR_RIGHT)
	{
		m_tInfo.fX += m_fSpeed;
	}
}

void CMonster::EnemySetBullet()
{
	INFO PlayerTemp(CObjMgr::Get_Instance()->Get_Player()->Get_Info());
	if ((abs(PlayerTemp.fX - m_tInfo.fX) < 60.f) && (abs(PlayerTemp.fY - m_tInfo.fY) < 60.f))
	{//�̰� �ο���
		m_bFightMode = true;
		//m_eMode = MODE_FIGHT;
		if (m_eCurState != WHILEBUBBLE)
		{
			if (m_iBulletCapacity > 0) {
				CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, CAbstractFactory<CBullet>::Create(m_BulletSpot.fX, m_BulletSpot.fY, OBJ_BULLET, m_iWaterPower, DIR_END, BOMB_MONSTER));
				--m_iBulletCapacity;
			}
		}
	}
	else if(!m_bFightMode)
	{
		Find_Where();
	}
	
}

void CMonster::Find_Where()
{
		m_pTarget = CObjMgr::Get_Instance()->Get_Target(OBJ_BLOCK, this);
		if (m_pTarget != nullptr)
		{
			INFO TEMP((*m_pTarget).Get_Info());
			m_tTargetTile.fX = TEMP.fX;
			m_tTargetTile.fY = TEMP.fY;

			if (((m_tTargetTile.fX == m_BulletSpot.fX)&&(abs(m_tTargetTile.fY - m_tInfo.fY)<42.f))||((m_tTargetTile.fY == m_BulletSpot.fY)&&(abs(m_tTargetTile.fX - m_tInfo.fX)<42.f)))
			{
				if (m_iBulletCapacity > 0 &&(!m_bFindModeBullet)) {
					CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, CAbstractFactory<CBullet>::Create(m_BulletSpot.fX, m_BulletSpot.fY, OBJ_BULLET, m_iWaterPower, DIR_END, BOMB_MONSTER));
					--m_iBulletCapacity;
					m_eMode = MODE_AVOID;
					m_bFindModeBullet = true;
				}
			}
		}
}

void CMonster::Monster_Dont_Move()
{
	if (m_eCurDirection == DIR_END)
	{
		if (!m_bFind_Timer) {
			m_FindTime = GetTickCount();
			m_bFind_Timer = true;
		}
		if ((m_FindTime != 0) && (m_FindTime + 1000 < GetTickCount()))
		{
			m_tShadowMove.iX = int((m_BulletSpot.fX - 40) / TILECX);
			m_tShadowMove.iY = int((m_BulletSpot.fY - 60) / TILECX);
			MonsterShadowIndex* Temp;
			if ((!CObjMgr::Get_Instance()->If_Block(m_tShadowMove.iX - 1, m_tShadowMove.iY)))
			{//�� Ž��
				if (!CObjMgr::Get_Instance()->GetWater_Spot(int(m_tShadowMove.iX-1), int(m_tShadowMove.iY)))
				{
					MonsterShadowIndex* Temp = new MonsterShadowIndex(m_tShadowMove.iX - 1, m_tShadowMove.iY);
					m_MoveHow.push_back(Temp);
					--m_tShadowMove.iX;
					return;
				}
			}
			else if ((!CObjMgr::Get_Instance()->If_Block(m_tShadowMove.iX + 1, m_tShadowMove.iY)))
			{//�� Ž��
				if (!CObjMgr::Get_Instance()->GetWater_Spot(int(m_tShadowMove.iX + 1), int(m_tShadowMove.iY)))
				{
					MonsterShadowIndex* Temp = new MonsterShadowIndex(m_tShadowMove.iX + 1, m_tShadowMove.iY);
					m_MoveHow.push_back(Temp);
					++m_tShadowMove.iX;
					//cout << "RIGHT"  << endl;
					return;
				}
			}
			else if ((!CObjMgr::Get_Instance()->If_Block(m_tShadowMove.iX, m_tShadowMove.iY - 1)))
			{//�� Ž��
				if (!CObjMgr::Get_Instance()->GetWater_Spot(int(m_tShadowMove.iX), int(m_tShadowMove.iY - 1)))
				{
					MonsterShadowIndex* Temp = new MonsterShadowIndex(m_tShadowMove.iX, m_tShadowMove.iY - 1);
					m_MoveHow.push_back(Temp);
					--m_tShadowMove.iY;
					//cout << "UP" << endl;
					return;
				}
			}
			else if ((!CObjMgr::Get_Instance()->If_Block(m_tShadowMove.iX, m_tShadowMove.iY + 1)))
			{//�� Ž��
				if (!CObjMgr::Get_Instance()->GetWater_Spot(int(m_tShadowMove.iX), int(m_tShadowMove.iY + 1)))
				{
					MonsterShadowIndex* Temp = new MonsterShadowIndex(m_tShadowMove.iX, m_tShadowMove.iY + 1);
					m_MoveHow.push_back(Temp);
					++m_tShadowMove.iY;
					//cout << "DOWN" << endl;
					return;
				}
			}
			m_eMode = MODE_AVOID;
			m_FindTime = 0;
			m_bFind_Timer = false;
		}
	}
	else
	{
		m_FindTime = 0;
		m_bFind_Timer = false;
	}
	
}