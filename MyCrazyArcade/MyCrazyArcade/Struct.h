#pragma once

typedef	struct tagInfo
{
	float	fX;
	float	fY;
	float	fCX;
	float	fCY;
	OBJID	eObjType;

	int		iIndexX;	//�迭�ε���
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
	int		iFrameStart;		// ���ϴ� �������� ��������Ʈ �̹��� ���� ����
	int		iFrameEnd;			// ��������Ʈ�� ������ ������ ����
	int		iMotion;			// ��� �ε���

	DWORD	dwSpeed;			// ��������Ʈ�� ���ư��� �ӵ�
	DWORD	dwTime;				// ��������Ʈ ��ġ�� ����Ǵ� �ð� 

}FRAME;


struct ItemResult {
	//bool _bGameOver;
	int _iRoller;
	int _iWaterBomb;
	int _iMedicine;
};
