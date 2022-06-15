#pragma once
#include "Obj.h"
#include "KeyMgr.h"
class C2048Block :public CObj
{
public:
	C2048Block();
	~C2048Block();

	// CObj을(를) 통해 상속됨
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;

public:
	void AnimationMoving ( );
	void SetAnimationDirecion(int eDir) {
		if (eDir == 1)
			m_eAnimationDir = DIR_LEFT;
		else if (eDir == 3)
			m_eAnimationDir = DIR_RIGHT;
		else if (eDir == 2)
			m_eAnimationDir = DIR_UP;
		else if (eDir == 4)
			m_eAnimationDir = DIR_DOWN;
	}
private:
	void SetBlock(int iNewNum);
	void Update_Num();
public:
	int& GetBlockNum() { return m_iBlockNum; }
	int m_iBlockNum;
	DIRECTION m_eAnimationDir;
};

