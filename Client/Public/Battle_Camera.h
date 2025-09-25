#pragma once

#include "Client_Defines.h"
#include "Camera.h"

NS_BEGIN(Engine)

NS_END

NS_BEGIN(Client)

class CBattle_Camera final : public CCamera
{
public:
	typedef struct tagCamera_Free : public CCamera::CAMERA_DESC
	{
		_float		fMouseSensor;
	}CAMERA_FREE_DESC;
private:
	CBattle_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBattle_Camera(const CBattle_Camera& Prototype);
	virtual ~CBattle_Camera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	void Save_MousePos();

private:
	_float						m_fMouseSensor = {};
	class CCamera_Manager*		m_pCamera_Manager = { nullptr };

	_float						m_fLerp{};

	_float						m_fDefaultDistanceY{};
	_float						m_fDefaultDistanceZ{};

	_float						m_fMaxDistanceY{};
	_float						m_fMaxDistanceZ{};

	_float						m_fMinDistanceY{};
	_float						m_fMinDistanceZ{};

	_vector						m_fMaxCameraDistance{};
	_vector						m_fMinCameraDistance{};

	_vector						m_fCameraDistanceOffset{};	// 카메라 줌 거리
	_vector						m_fCameraPosOffset{};		// 카메라 위치 오프셋
	_vector						m_fCameraHeightOffset{};	// 카메라 상하 높이

	_vector						m_fRotation{};

	_float						m_fTime{};

	POINT						m_pMousePos{};

	_float						m_fX{};
	_float						m_fY{};

public:
	static CBattle_Camera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END