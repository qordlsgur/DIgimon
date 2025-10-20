#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CBounding abstract : public CBase
{
public:
	typedef struct tagBoundingDesc
	{
		_float3			vCenter;
	}BOUNDING_DESC;
protected:
	CBounding(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding() = default;

public:
	HRESULT Initialize();
	virtual void Update(_fmatrix WorldMatrix) = 0;
	virtual _bool Intersect(class CBounding* pTarget) = 0;

	// 생성될 위치
	virtual void Set_Matrix(_matrix Matrix) = 0;

#ifdef _DEBUG
public:
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor) = 0;
#endif

protected:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	// 내 위치
	_matrix				m_pMatrix{};

	// 던질 위치
	_vector				m_vTarget{};
	_bool m_bSizeUp = { false };
	_bool m_bLaunch = { false };
public:
	virtual void Free();
};

NS_END