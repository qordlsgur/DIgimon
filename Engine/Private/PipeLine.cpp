#include "PipeLine.h"

CPipeLine::CPipeLine()
{
}

// enum을 설정 해서 Vieww 인지 Proj인지 구분을 해둔다.
void CPipeLine::Set_Transform(D3DTS eState, _fmatrix TransformStateMatrix)
{
	// 계산용을 매개변수에 선언을 해서 받을 때 저장용으로 바꿔서 값을 넘김 
	XMStoreFloat4x4(&m_TransformStateMatrices[ENUM_CLASS(eState)], TransformStateMatrix);
}

const _float4x4* CPipeLine::Get_Transform_Float4x4(D3DTS eState)
{
	// 저장용 Matrix를 넘겨준다.
	return &m_TransformStateMatrices[ENUM_CLASS(eState)];
}

_matrix CPipeLine::Get_Transform_Matrix(D3DTS eState)
{
	// 연산용 Matrix를 넘겨준다.
	return XMLoadFloat4x4(&m_TransformStateMatrices[ENUM_CLASS(eState)]);
}

const _float4x4* CPipeLine::Get_Transform_Float4x4_Inverse(D3DTS eState)
{
	return &m_TransformStateMatrixInverse[ENUM_CLASS(eState)];
}

_matrix CPipeLine::Get_Transform_Matrix_Inverse(D3DTS eState)
{
	return XMLoadFloat4x4(&m_TransformStateMatrixInverse[ENUM_CLASS(eState)]);
}

const _float4* CPipeLine::Get_CamPosition()
{
	// 카메라의 위치를 넘겨준다.
	return &m_vCamPosition;
}

void CPipeLine::Update()
{
	// 매 프레임마다 View랑 Proj를 역행렬로 바꾼다.
	for (_uint i = 0; i < ENUM_CLASS(D3DTS::END); ++i)
	{
		XMStoreFloat4x4(&m_TransformStateMatrixInverse[i],XMMatrixInverse(nullptr, Get_Transform_Matrix(static_cast<D3DTS>(i))));
	}

	// 카메라의 위치에 뷰의 역행렬을 던져준다.
	memcpy(&m_vCamPosition, &m_TransformStateMatrixInverse[ENUM_CLASS(D3DTS::VIEW)].m[3],	sizeof(_float4));
}

CPipeLine* CPipeLine::Create()
{
	return new CPipeLine();
}

void CPipeLine::Free()
{
	__super::Free();
}
