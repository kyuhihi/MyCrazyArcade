#include "stdafx.h"
#include "TileMgr.h"
#include "AbstractFactory.h"
#include "ObjMgr.h"
CTileMgr*	CTileMgr::m_pInstance = nullptr;

CTileMgr::CTileMgr()
{
}


CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize()
{
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			float		fX = (TILECX >> 1) + float(TILECX * j) + 20;// 20 40���ϸ��  �ٴ� �������40 40
			float		fY = (TILECY >> 1) + float(TILECY * i) + 40;

			CObj*	pTile = CAbstractFactory<CTile>::Create(fX, fY);
			//CObjMgr::Get_Instance()->Add_Object(OBJ_TILE, CAbstractFactory<CTile>::Create(fX, fY, OBJ_TILE));

			m_vecTile.push_back(pTile);
		}
	}

}

void CTileMgr::Update()
{
	for (auto& iter : m_vecTile)
		iter->Update();
}

void CTileMgr::Late_Update()
{
	for (auto& iter : m_vecTile)
		iter->Late_Update();
}

void CTileMgr::Render(HDC hDC)
{
	for (auto& iter : m_vecTile)
		iter->Render(hDC);
}

void CTileMgr::Release()
{
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
}

//void CTileMgr::Picking_Tile(POINT _pt, const int& _iDrawID, const int& _iOption)
//{
//	int		x = _pt.x / TILECX;
//	int		y = _pt.y / TILECY;
//
//	int	iIndex = y * TILEX + x;
//
//	if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
//		return;
//
//	dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_DrawID(_iDrawID);
//	dynamic_cast<CTile*>(m_vecTile[iIndex])->Set_Option(_iOption);
//}
//

//void CTileMgr::Save_Tile(void)
//{
//	HANDLE		hFile = CreateFile(L"../Data/Tile.dat", GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//
//	int		iDrawID = 0, iOption = 0;
//	DWORD	dwByte = 0;
//
//	for (auto& iter : m_vecTile)
//	{
//		iDrawID = dynamic_cast<CTile*>(iter)->Get_DrawID();
//		iOption = dynamic_cast<CTile*>(iter)->Get_Option();
//
//		WriteFile(hFile, &iter->Get_Info(), sizeof(INFO), &dwByte, NULL);
//		WriteFile(hFile, &iDrawID, sizeof(int), &dwByte, NULL);
//		WriteFile(hFile, &iOption, sizeof(int), &dwByte, NULL);
//	}
//
//	CloseHandle(hFile);
//}
//
//void CTileMgr::Load_Tile(void)
//{
//	HANDLE		hFile = CreateFile(L"../Data/Tile.dat",			// ���� ��ο� �̸� ���
//		GENERIC_READ,				// ���� ���� ��� (GENERIC_WRITE ���� ����, GENERIC_READ �б� ����)
//		NULL,						// �������, ������ �����ִ� ���¿��� �ٸ� ���μ����� ������ �� ����� ���ΰ�, NULL�� ��� �������� �ʴ´�
//		NULL,						// ���� �Ӽ�, �⺻��	
//		OPEN_EXISTING,				// ���� ���, CREATE_ALWAYS�� ������ ���ٸ� ����, �ִٸ� ���� ����, OPEN_EXISTING ������ ���� ��쿡�� ����
//		FILE_ATTRIBUTE_NORMAL,		// ���� �Ӽ�(�б� ����, ���� ��), FILE_ATTRIBUTE_NORMAL �ƹ��� �Ӽ��� ���� �Ϲ� ���� ����
//		NULL);						// �������� ������ �Ӽ��� ������ ���ø� ����, �츮�� ������� �ʾƼ� NULL
//
//	if (INVALID_HANDLE_VALUE == hFile)
//	{
//		// �˾� â�� ������ִ� ����� �Լ�
//		// 1. �ڵ� 2. �˾� â�� �������ϴ� �޽��� 3. �˾� â �̸� 4. ��ư �Ӽ�
//		MessageBox(g_hWnd, _T("Load File"), _T("Fail"), MB_OK);
//		return;
//	}
//
//	// 2. ���� ����
//
//	DWORD		dwByte = 0;
//	CTile		tInfo{};
//
//	while (true)
//	{
//		ReadFile(hFile, &tInfo, sizeof(CTile), &dwByte, nullptr);
//
//		if (0 == dwByte)	// ���̻� ���� �����Ͱ� ���� ���
//			break;
//
//		m_vecTile.push_back(new CTile(tInfo));
//	}
//
//
//	// 3. ���� �Ҹ�
//	CloseHandle(hFile);
//
//	MessageBox(g_hWnd, _T("Load �Ϸ�"), _T("����"), MB_OK);
//}
