#pragma once

#include "GameObject.h"

NS_BEGIN(Engine)

class ENGINE_DLL CPartObject abstract : public CGameObject
{
public:
	typedef struct tagPartObjectDesc
	{
		class CTransform* pParentTransform = { nullptr };
		_vector vPosition;
		_matrix vMatrix;
		_vector vPosition2;
		_float LR;
	}PARTOBJECT_DESC;

protected:
	CPartObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPartObject(const CPartObject& Prototype);
	virtual ~CPartObject() = default;


public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void Set_Animation(const _char* szName, _bool bisLoop = true) {};
	virtual _bool Get_AnimFinish() { return m_isAnimFinish; }
	virtual void Set_State(DIGIMONSTATE eState) { m_eState = eState; }

	virtual _float Get_TrackPosition() { return m_fTrackPosition; }
	virtual void Set_Dissolve(_bool Dissolve) { m_bDissolve = Dissolve; }
	virtual void Set_Hit(_bool Hit) { m_bHit = Hit; }
	virtual _bool Get_Hit() { return m_bHit; }
	void Compute_Depth();
protected:
	class CTransform*	m_pParentTransformCom = { nullptr };
	_vector				m_vPosition{};

	_float4x4			m_CombinedWorldMatrix = {};
	_bool				m_isAnimFinish = { false };
	DIGIMONSTATE		m_eState = { DIGIMONSTATE::END };
	_float m_fTrackPosition{};
	_bool m_bDissolve = { false };
	_float m_fTime{};
	_bool m_bHit = { false };

	_float				m_fDepth = {};

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END

