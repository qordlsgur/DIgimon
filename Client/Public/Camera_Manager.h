#pragma once

#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
class CGameObject;
class CCamera;
NS_END;

NS_BEGIN(Client)

class CCamera_Manager final : public CBase
{
	DECLARE_SINGLETON(CCamera_Manager);

public:
	CCamera_Manager();
	virtual ~CCamera_Manager() = default;

public:
	HRESULT Initialize();

	void Add_Camera(CCamera* pCamera);
	void Add_Player(CGameObject* pPlayer);
	void PlayerPos(_vector& Pos);
	_bool HasPlayer();
	void Set_LockCamera(_bool bLockCamera) { m_bLockCamera = bLockCamera; }
	_bool Get_LcokCamera() { return m_bLockCamera; }
	void Camera_Angle(_float Angle) { m_fCamera_Angle = Angle; }
	_float Get_Angle() { return m_fCamera_Angle; }

public:
	void Set_Battle(_bool Battle);

private:


	_bool m_bBattle = { false };

	vector<CCamera*> m_Cameras;
	CGameObject* m_pPlayer = { nullptr };
	_bool m_bLockCamera = { false };
	_float m_fCamera_Angle{};
public:
	virtual void Free();

};

NS_END