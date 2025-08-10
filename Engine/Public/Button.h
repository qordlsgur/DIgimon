#pragma once

#include "GameObject.h"

NS_BEGIN(Engine)

class ENGINE_DLL CButton abstract : public CGameObject
{
	// 이 구조체는 이제 UI가 처음생성이 될 때 초기의 위치를 잡아주는 구조체이다.
	// fX,fY는 UI가 화면상 어디에 있을 지 정해준다.
	// fSizeX, fSizeY는 이제 UI의 크기를 지정 해준다.
	public:
		typedef struct tagButtonDesc : public CGameObject::GAMEOBJECT_DESC
		{
			_float		fX, fY, fSizeX, fSizeY;
		}BUTTON_DESC;

protected:
	CButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CButton(const CButton& Prototype);
	virtual ~CButton() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimedelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	HRESULT Begin();
	HRESULT End();

	virtual void Set_Active() { m_bActive = !m_bActive; }

	virtual void OnClick() override;
	virtual void IsHover() { m_bHover = !m_bHover; }

protected:

	_float4x4				m_ViewMatrix{};
	_float4x4				m_ProjMatrix{};

	_bool					m_bActive = { false };
	_bool					m_bHover = { false };

	_float					m_fX{}, m_fY{}, m_fSizeX{}, m_fSizeY{};

	ID3D11DepthStencilState* m_pDepthDisable = nullptr;  // 깊이 테스트 OFF
	ID3D11DepthStencilState* m_pDepthEnable = nullptr;   // 깊이 테스트 ON

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END