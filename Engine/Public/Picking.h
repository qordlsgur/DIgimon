#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CPicking final :public CBase
{
private:
	CPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CPicking() = default;

public:
	HRESULT Initialize(HWND hWnd);
	void Update();
	void Transform_ToLocalSpace(const _matrix* pWorldMatrixInverse);
	_bool Picking_InWorldSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut);
	_bool Picking_InLocalSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut);


private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	HWND					m_hWnd = {};
	_float					m_fWinSizeX{}, m_fWinSizeY{};
	_float3					m_vRayDir[ENUM_CLASS(RAY::END)];
	_float3					m_vRayPos[ENUM_CLASS(RAY::END)];

	class CGameInstance* m_pGameInstance = { nullptr };
public:
	static CPicking* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd);
	virtual void Free() override;
};

NS_END