//#pragma once

#include "Client_Defines.h"
#include "Camera.h"

NS_BEGIN(Engine)
class CNavigation;

NS_END

NS_BEGIN(Client)

class CCamera_Free final : public CCamera
{
public:
	typedef struct tagCamera_Free : public CCamera::CAMERA_DESC
	{
		_float		fMouseSensor;
	}CAMERA_FREE_DESC;
private:
	CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Free(const CCamera_Free& Prototype);
	virtual ~CCamera_Free() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void Elapsed_Time(_float fTimeDelta);

private:
	void Save_MousePos();
	void LookPlayer(_float fTimeDelta);
	void LookField(_float fTimeDelta);

private:
	_float			m_fMouseSensor = {};
	class CCamera_Manager* m_pCamera_Manager = { nullptr };
	class CBattle_Manager* m_pBattle_Manager = { nullptr };

	CNavigation* m_pNavigationCom = { nullptr };
	_float m_fLerp{};

	_float	m_fDefaultDistanceY{};
	_float	m_fDefaultDistanceZ{};

	_float	m_fMaxDistanceY{};
	_float	m_fMaxDistanceZ{};

	_float	m_fMinDistanceY{};
	_float	m_fMinDistanceZ{};

	_vector	m_fMaxCameraDistance{};
	_vector m_fMinCameraDistance{};

	_vector m_fCameraDistanceOffset{};	// 카메라 줌 거리
	_vector m_fCameraPosOffset{};		// 카메라 위치 오프셋
	_vector m_fCameraHeightOffset{};	// 카메라 상하 높이
	_vector m_fCameraPlayerLook{};		// 카메라 플레이어 바라보는 방향
	_vector m_fCameraPlayerChase{};		// 카메라 플레이어 따라다니는 위치

	_vector m_fBottomCamera{};
	_float m_fSaveCamera{};

	_bool m_bSaveCamera = false;

	_vector m_fRotation{};

	_vector m_fPlayerPos{};
	_float	m_fTime{};
	_bool	m_bRightClick = { false };
	_bool	m_bPlayer = { false };

	POINT	m_pMousePos{};

	_float		m_fX{};
	_float		m_fY{};
	_vector m_bLastOffset{};
	_vector m_fLastPos{};
	_vector m_fLastRight{};
	_vector m_fLastUp{};
	_vector m_fLastLook{};

public:
	static CCamera_Free* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END