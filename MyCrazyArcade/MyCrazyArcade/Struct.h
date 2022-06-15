#pragma once

typedef	struct tagInfo
{
	float	fX;
	float	fY;
	float	fCX;
	float	fCY;
	OBJID	eObjType;

	int		iIndexX;	//배열인덱스
	int		iIndexY;
}INFO;
 struct PlayerTile
{
	float fX;
	float fY;
	PlayerTile() { };
	PlayerTile(float _X, float _Y) { fX = _X; fY = _Y; }
};

 struct WaterTile
 {
	 bool bBulletTrue;
	 int  iCount;
 };

 struct MonsterShadowIndex
 {
	 int iX;
	 int iY;
	 MonsterShadowIndex() {};
	 MonsterShadowIndex(int _X, int _Y) { iX = _X; iY = _Y; }
 };


typedef struct tagFrame
{
	int		iFrameStart;		// 원하는 지점부터 스프라이트 이미지 구동 시작
	int		iFrameEnd;			// 스프라이트가 끝나는 지점을 지정
	int		iMotion;			// 모션 인덱스

	DWORD	dwSpeed;			// 스프라이트가 돌아가는 속도
	DWORD	dwTime;				// 스프라이트 위치가 변경되는 시간 

}FRAME;


struct ItemResult {
	//bool _bGameOver;
	int _iRoller;
	int _iWaterBomb;
	int _iMedicine;
};
