#pragma once

#include "Base.h"

/* 뷰, 투영행렬을 저장한다, */

NS_BEGIN(Engine)

class CPipeLine final : public CBase
{
private:
	CPipeLine();
	virtual ~CPipeLine() = default;

public:
	// 로직을 수행 할 원근투영을 위한 행렬을 파이프라인에 저장을 한다.
	// 그래서 매 프레임마다 한번씩 구해서 저장을 해준다.
	// 그리고 오브젝트들이 카메라의 위치를 다 들고 있어야 하는데 한번 계산을 해서 받아둔다.
	// 그렇게 하면 매 프레임마다 찾을 필요가 없이 그냥 사용만 하면 된다.
	// 그리고 뷰행렬 투영행렬도 저장을 해준다.
	void Set_Transform(D3DTS eState, _fmatrix TransformStateMatrix);

public:
	const _float4x4* Get_Transform_Float4x4(D3DTS eState);
	_matrix Get_Transform_Matrix(D3DTS eState);
	const _float4x4* Get_Transform_Float4x4_Inverse(D3DTS eState);
	_matrix Get_Transform_Matrix_Inverse(D3DTS eState);
	const _float4* Get_CamPosition();

public:
	void Update();

private:
	_float4x4				m_TransformStateMatrices[ENUM_CLASS(D3DTS::END)] = {};
	_float4x4				m_TransformStateMatrixInverse[ENUM_CLASS(D3DTS::END)] = {};
	_float4					m_vCamPosition = {};

public:
	static CPipeLine* Create();
	virtual void Free() override;
};

NS_END