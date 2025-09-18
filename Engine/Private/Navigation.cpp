#include "Navigation.h"

#include "Cell.h"
#include "Shader.h"
#include "GameInstance.h"

_float4x4 CNavigation::m_WorldMatrix = { };

CNavigation::CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{ pDevice, pContext }
{

}

CNavigation::CNavigation(const CNavigation& Prototype)
	: CComponent{ Prototype }
	, m_Cells{ Prototype.m_Cells }
#ifdef _DEBUG
	, m_pShader{ Prototype.m_pShader }
#endif
{
	for (auto& pCell : m_Cells)
		Safe_AddRef(pCell);

#ifdef _DEBUG
	Safe_AddRef(m_pShader);
#endif
}

HRESULT CNavigation::Initialize_Prototype(const _tchar* pNavigationDataFiles)
{
	ifstream in(pNavigationDataFiles, ios::binary);
	if (!in)
		return E_FAIL;

	_uint Cell_Size = 0;
	in.read(reinterpret_cast<_char*>(&Cell_Size), sizeof(Cell_Size));

	m_Cells.reserve(Cell_Size);  // 미리 용량 확보

	for (_uint i = 0; i < Cell_Size; ++i)
	{
		_float3 vPoints[3];
		in.read(reinterpret_cast<_char*>(&vPoints[0]), sizeof(_float3));
		in.read(reinterpret_cast<_char*>(&vPoints[1]), sizeof(_float3));
		in.read(reinterpret_cast<_char*>(&vPoints[2]), sizeof(_float3));

		CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, i);
		if (!pCell)
			return E_FAIL;

		m_Cells.push_back(pCell); // 탐색용 벡터에 넣기
	}


#ifdef _DEBUG
	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;
#endif

	SetUp_Neighbors();

	return S_OK;
}

HRESULT CNavigation::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return S_OK;

	NAVIGATION_DESC* pDesc = static_cast<NAVIGATION_DESC*>(pArg);

	m_iCurrentCellIndex = pDesc->iCurrentCellIndex;

	return S_OK;
}

_bool CNavigation::isMove(_fvector vPosition)
{
	// 우선 받아온 포지션에 역행렬을 넣어서 월드에서 로컬로 내려준다.
	_vector		vLocalPos = XMVector3TransformCoord(vPosition, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));

	// 
	_int		iNeighborIndex = { -1 };

	// 지금 플레이어가 셀 안에 있나 없나 검사를 한다.
	_bool		isIn = m_Cells[m_iCurrentCellIndex]->isIn(vLocalPos, &iNeighborIndex);

	if (true == isIn)
	{
		return true;
	}
	else
	{
		// 지금 플레이어가 나간 방향에 이웃이 있나 없나 확인을 해 주는데 만약 나간 방향에 이웃이 있으면.
		/* 나간 방향에 이웃이 있냐? */
		if (-1 != iNeighborIndex)
		{
			while (true)
			{
				// 만약 없으면 false
				if (-1 == iNeighborIndex)
					return false;


				// 만약 
				if (true == m_Cells[iNeighborIndex]->isIn(vLocalPos, &iNeighborIndex))
					break;
			}


			m_iCurrentCellIndex = iNeighborIndex;
			return true;
		}
		else
			return false;
	}
}


void CNavigation::Compute_Height(CTransform* pTransform)
{
	// 오브젝트의 트렌스폼을 들고와서 플레이어의 위치를 로컬로 내려준다.
	_vector		vLocalPos = XMVector3TransformCoord(pTransform->Get_State(STATE::POSITION), XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));

	// 그리고 셀의 높이를 들고와서 평면의 방정식을 이용한다.
	_float		fHeight = m_Cells[m_iCurrentCellIndex]->Compute_Height(vLocalPos);

	// 그리고 그 y값을 들고와서 오브젝트의 y값에 넣어준 다음에
	vLocalPos = XMVectorSetY(vLocalPos, fHeight);

	// 마지막에 트렌스폼을 y축만큼 올려준다.
	pTransform->Set_State(STATE::POSITION, XMVector3TransformCoord(vLocalPos, XMLoadFloat4x4(&m_WorldMatrix)));
}

#ifdef _DEBUG

HRESULT CNavigation::Render()
{
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	_float4		vColor = { };
	_float4x4	WorldMatrix = m_WorldMatrix;

	if (-1 == m_iCurrentCellIndex)
		vColor = _float4(0.f, 1.f, 0.f, 1.f);
	else
	{
		WorldMatrix._42 += 0.05f;
		vColor = _float4(1.f, 0.f, 0.f, 1.f);
	}

	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &WorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_RawValue("g_vColor", &vColor, sizeof(_float4))))
		return E_FAIL;

	m_pShader->Begin(0);

	if (-1 != m_iCurrentCellIndex)
	{
		return m_Cells[m_iCurrentCellIndex]->Render();
	}

	for (auto& pCell : m_Cells)
		pCell->Render();

	return S_OK;
}
#endif


void CNavigation::SetUp_Neighbors()
{
	for (auto& pSourCell : m_Cells)
	{
		for (auto& pDestCell : m_Cells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->Compare(pSourCell->Get_Point(NAVI_POINT::A), pSourCell->Get_Point(NAVI_POINT::B)))
			{
				pSourCell->Set_Neighbor(NAVI_LINE::AB, pDestCell);
			}

			if (true == pDestCell->Compare(pSourCell->Get_Point(NAVI_POINT::B), pSourCell->Get_Point(NAVI_POINT::C)))
			{
				pSourCell->Set_Neighbor(NAVI_LINE::BC, pDestCell);
			}

			if (true == pDestCell->Compare(pSourCell->Get_Point(NAVI_POINT::C), pSourCell->Get_Point(NAVI_POINT::A)))
			{
				pSourCell->Set_Neighbor(NAVI_LINE::CA, pDestCell);
			}
		}
	}
}

CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pNavigationDataFiles)
{
	CNavigation* pInstance = new CNavigation(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pNavigationDataFiles)))
	{
		MSG_BOX("Failed to Created : CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CNavigation::Clone(void* pArg)
{
	CNavigation* pInstance = new CNavigation(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CNavigation::Free()
{
	__super::Free();

#ifdef _DEBUG
	Safe_Release(m_pShader);
#endif

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);

	m_Cells.clear();
}
