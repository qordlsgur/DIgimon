#pragma once

#include "GameObject.h"

NS_BEGIN(Engine)

class ENGINE_DLL CUIObject abstract : public CGameObject
{
public:
	// 이 구조체는 이제 UI가 처음생성이 될 때 초기의 위치를 잡아주는 구조체이다.
	// fX,fY는 UI가 화면상 어디에 있을 지 정해준다.
	// fSizeX, fSizeY는 이제 UI의 크기를 지정 해준다.
	public:
		typedef struct tagUIObjectDesc : public CGameObject::GAMEOBJECT_DESC
		{
			_float		fX, fY, fSizeX, fSizeY;
		}UIOBJECT_DESC;

protected:
	CUIObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIObject(const CUIObject& Prototype);
	virtual ~CUIObject() = default;

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

	HRESULT Blend_Begin();
	HRESULT Blend_End();

	virtual RECT* Get_Pos() { return &m_pRect; }

	virtual void Set_Active() { m_bActive = !m_bActive; }

protected:
	// 기본적인 UI들은 이제 회면 가장 앞에 위치해 있다.
    // 몬스터나 플레이어가 UI쪽으로 와도 UI는 그 앞에 그려진다.
    // 그렇게 하기 위해서는 이제 멀리있으면 작게 가까이 있으면 크게 그려지는 원근 투영이 아닌
    // 어디에 있든 같은 크기를 가지고 있는 직교 투영으로 만들어 준다.
    // 직교투영을 해주는 방법은 일단 뷰 행렬을 항등 행렬로 만들어 준다.
    // 여기서 뷰 행렬잉 항등이라는건 카메라가 원점(0,0,0)에 위치하고, 방향 변환 없이 정면을 
    // 바라보고 있는 형태이다.
    // 그 후에 직교 투영 행렬을 만들어서 주는데 이제 직교 투영 행렬에 들어가는 값이
    // 화면의 가로값, 세로값, 카메라의 near, far 4개를 지정 해준다.
    // 여기서 near랑 far는 카메라가 바라보는 가장 가까운 위치부터 가장 먼 위치를 지정 해주는데
    // 0은 최소거리 1은 최대거리이다.
	_float4x4				m_ViewMatrix{};
	_float4x4				m_ProjMatrix{};

	_bool					m_bActive = { false };

	_float					m_fX{}, m_fY{}, m_fSizeX{}, m_fSizeY{};

	RECT					m_pRect{};

	ID3D11BlendState*		m_pBS = { nullptr };


	ID3D11DepthStencilState* m_pDepthStencilState_Disable = nullptr;
	ID3D11DepthStencilState* m_pDepthStencilState_Enable = nullptr;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END