#pragma once
#include "Include.h"

class CObj
{
public:
	CObj();
	virtual ~CObj();

public:
	void		Set_Pos(float _fX, float _fY)
	{
		m_tInfo.fX = _fX;
		m_tInfo.fY = _fY;
	}

	
	void		Set_Dir(DIRECTION eDir) { m_eDir = eDir; }
	void		Set_Dead() { m_bDead = true; }
	void		Set_Angle(float _fAngle) { m_fAngle = _fAngle; }
	void		Set_Target(CObj* _pTarget) { m_pTarget = _pTarget; }

	void		Set_PosX(float _fX) { m_tInfo.fX += _fX; }

	void		Set_Index(int iX, int iY) { m_tInfo.iIndexX = iX;  m_tInfo.iIndexY = iY; }

	void		Set_PosY(float _fY) { m_tInfo.fY += _fY; }
	void		Set_Type(OBJID eID) { m_tInfo.eObjType = eID; }
	void		Set_FrameKey(TCHAR* pFrameKey) { m_pFrameKey = pFrameKey; }

	bool		Get_Dead() { return m_bDead; }
	float		Get_Speed() { return m_fSpeed; }


	 INFO&		Get_Info(void)  { return m_tInfo; }
	const RECT&		Get_Rect(void) const { return m_tRect; }
	const RENDERID	Get_RenderID(void) const { return m_eRender; }


public:
	virtual		void	Initialize(void)	PURE;
	virtual		int		Update(void)		PURE;
	virtual		void	Late_Update(void)	PURE;
	virtual		void	Render(HDC hDC)		PURE;
	virtual		void	Release(void)		PURE;

protected:
	void		Update_Rect(void);
	void		Move_Frame(void);

protected:
	INFO		m_tInfo;
	RECT		m_tRect;
	FRAME		m_tFrame;

	float		m_fSpeed;
	float		m_fAngle;

	DIRECTION	m_eDir;
	bool		m_bDead;

	CObj*		m_pTarget;
	TCHAR*		m_pFrameKey;
	
protected:
	int						m_iBulletMax;			//¹°Ç³¼± °¹¼ö
	int						m_iBulletCapacity;		//Çö ¹°Ç³¼± ³²Àº ¼ö·®
	RENDERID				m_eRender;

};

