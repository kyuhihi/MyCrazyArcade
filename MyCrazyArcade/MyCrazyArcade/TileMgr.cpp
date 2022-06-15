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
			float		fX = (TILECX >> 1) + float(TILECX * j) + 20;// 20 40더하면됨  바닥 사이즈는40 40
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
//	HANDLE		hFile = CreateFile(L"../Data/Tile.dat",			// 파일 경로와 이름 명시
//		GENERIC_READ,				// 파일 접근 모드 (GENERIC_WRITE 쓰기 전용, GENERIC_READ 읽기 전용)
//		NULL,						// 공유방식, 파일이 열려있는 상태에서 다른 프로세스가 오픈할 때 허용할 것인가, NULL인 경우 공유하지 않는다
//		NULL,						// 보안 속성, 기본값	
//		OPEN_EXISTING,				// 생성 방식, CREATE_ALWAYS는 파일이 없다면 생성, 있다면 덮어 쓰기, OPEN_EXISTING 파일이 있을 경우에면 열기
//		FILE_ATTRIBUTE_NORMAL,		// 파일 속성(읽기 전용, 숨기 등), FILE_ATTRIBUTE_NORMAL 아무런 속성이 없는 일반 파일 생성
//		NULL);						// 생성도리 파일의 속성을 제공할 템플릿 파일, 우리는 사용하지 않아서 NULL
//
//	if (INVALID_HANDLE_VALUE == hFile)
//	{
//		// 팝업 창을 출력해주는 기능의 함수
//		// 1. 핸들 2. 팝업 창에 띄우고자하는 메시지 3. 팝업 창 이름 4. 버튼 속성
//		MessageBox(g_hWnd, _T("Load File"), _T("Fail"), MB_OK);
//		return;
//	}
//
//	// 2. 파일 쓰기
//
//	DWORD		dwByte = 0;
//	CTile		tInfo{};
//
//	while (true)
//	{
//		ReadFile(hFile, &tInfo, sizeof(CTile), &dwByte, nullptr);
//
//		if (0 == dwByte)	// 더이상 읽을 데이터가 없을 경우
//			break;
//
//		m_vecTile.push_back(new CTile(tInfo));
//	}
//
//
//	// 3. 파일 소멸
//	CloseHandle(hFile);
//
//	MessageBox(g_hWnd, _T("Load 완료"), _T("성공"), MB_OK);
//}
