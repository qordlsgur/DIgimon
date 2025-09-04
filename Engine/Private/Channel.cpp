#include "Channel.h"
#include "Model.h"

#include "Bone.h"

CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(const CModel* pModel, CHANNEL& mChannel)
{
	strcpy_s(m_szName, mChannel.mName);

	m_iBoneIndex = pModel->Get_BoneIndex(m_szName);
	if (-1 == m_iBoneIndex)
		return E_FAIL;

	m_iNumKeyFrames = max(mChannel.mNumScalingKeys, mChannel.mNumRotationKeys);
	m_iNumKeyFrames = max(m_iNumKeyFrames, mChannel.mNumPositionKeys);

	_float3				vScale{};
	_float4				vRotation{};
	_float3				vTranslation{};

	for (size_t i = 0; i < m_iNumKeyFrames; i++)
	{

		m_KeyFrames.push_back(mChannel.mKeyFrame[i]);
	}

	return S_OK;
}

void CChannel::Update_TransformationMatrix(const vector<class CBone*>& Bones, _float fCurrentTrackPosition, _uint* pCurrentKeyFrameIndex)
{
	// 포지션이 0이 되면 뼈들의 순서도 다시 0으로
	if (0.f == fCurrentTrackPosition)
		*pCurrentKeyFrameIndex = 0;

	KEYFRAME		LastKeyFrame = m_KeyFrames.back();

	_vector			vScale{};
	_vector			vRotation{};
	_vector			vTranslation{};

	if (fCurrentTrackPosition >= LastKeyFrame.fTrackPosition)
	{
		vScale = XMLoadFloat3(&LastKeyFrame.vScale);
		vRotation = XMLoadFloat4(&LastKeyFrame.vRotation);
		vTranslation = XMVectorSetW(XMLoadFloat3(&LastKeyFrame.vTranslation), 1.f);
	}

	else /* 선형보간을 해야겠다. */
	{
		while (fCurrentTrackPosition >= m_KeyFrames[*pCurrentKeyFrameIndex + 1].fTrackPosition)
			++*pCurrentKeyFrameIndex;

		_float3		vSourScale{}, vDestScale{};
		_float4		vSourRotation{}, vDestRotation{};
		_float3		vSourTranslation{}, vDestTranslation{};

		vSourScale = m_KeyFrames[*pCurrentKeyFrameIndex].vScale;
		vDestScale = m_KeyFrames[*pCurrentKeyFrameIndex + 1].vScale;

		vSourRotation = m_KeyFrames[*pCurrentKeyFrameIndex].vRotation;
		vDestRotation = m_KeyFrames[*pCurrentKeyFrameIndex + 1].vRotation;

		vSourTranslation = m_KeyFrames[*pCurrentKeyFrameIndex].vTranslation;
		vDestTranslation = m_KeyFrames[*pCurrentKeyFrameIndex + 1].vTranslation;

		_float		fRatio = (fCurrentTrackPosition - m_KeyFrames[*pCurrentKeyFrameIndex].fTrackPosition) /
			(m_KeyFrames[*pCurrentKeyFrameIndex + 1].fTrackPosition - m_KeyFrames[*pCurrentKeyFrameIndex].fTrackPosition);

		vScale = XMVectorLerp(XMLoadFloat3(&vSourScale), XMLoadFloat3(&vDestScale), fRatio);
		vRotation = XMQuaternionSlerp(XMLoadFloat4(&vSourRotation), XMLoadFloat4(&vDestRotation), fRatio);
		vTranslation = XMVectorSetW(XMVectorLerp(XMLoadFloat3(&vSourTranslation), XMLoadFloat3(&vDestTranslation), fRatio), 1.f);

	}

	// _matrix		BoneTransformationMatrix = XMMatrixScaling() * 자전행렬 * ;
	_matrix		BoneTransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);

	Bones[m_iBoneIndex]->Set_TransformationMatrix(BoneTransformationMatrix);


}

void CChannel::Save_TransformationMatrix(const vector<class CBone*>& Bones, _float fCurrentTrackPosition, _uint* pCurrentKeyFrameIndex, vector<LERP>& mLerp)
{
	LERP Lerp;
	strcpy_s(Lerp.mName, sizeof(Lerp.mName), Bones[m_iBoneIndex]->Get_Name());

	KEYFRAME		LastKeyFrame = m_KeyFrames.back();


	_vector			vScale{};
	_vector			vRotation{};
	_vector			vTranslation{};

	_float3		vSourScale{}, vDestScale{};
	_float4		vSourRotation{}, vDestRotation{};
	_float3		vSourTranslation{}, vDestTranslation{};

	if (fCurrentTrackPosition >= LastKeyFrame.fTrackPosition)
	{
		vScale = XMLoadFloat3(&LastKeyFrame.vScale);
		vRotation = XMLoadFloat4(&LastKeyFrame.vRotation);
		vTranslation = XMVectorSetW(XMLoadFloat3(&LastKeyFrame.vTranslation), 1.f);
	}


	else /* 선형보간을 해야겠다. */
	{
		while (fCurrentTrackPosition >= m_KeyFrames[*pCurrentKeyFrameIndex + 1].fTrackPosition)
			++*pCurrentKeyFrameIndex;
		vSourScale = m_KeyFrames[*pCurrentKeyFrameIndex].vScale;
		vDestScale = m_KeyFrames[*pCurrentKeyFrameIndex + 1].vScale;

		vSourRotation = m_KeyFrames[*pCurrentKeyFrameIndex].vRotation;
		vDestRotation = m_KeyFrames[*pCurrentKeyFrameIndex + 1].vRotation;

		vSourTranslation = m_KeyFrames[*pCurrentKeyFrameIndex].vTranslation;
		vDestTranslation = m_KeyFrames[*pCurrentKeyFrameIndex + 1].vTranslation;

		_float		fRatio = (fCurrentTrackPosition - m_KeyFrames[*pCurrentKeyFrameIndex].fTrackPosition) /
			(m_KeyFrames[*pCurrentKeyFrameIndex + 1].fTrackPosition - m_KeyFrames[*pCurrentKeyFrameIndex].fTrackPosition);

		vScale = XMVectorLerp(XMLoadFloat3(&vSourScale), XMLoadFloat3(&vDestScale), fRatio);
		vRotation = XMQuaternionSlerp(XMLoadFloat4(&vSourRotation), XMLoadFloat4(&vDestRotation), fRatio);
		vTranslation = XMVectorSetW(XMVectorLerp(XMLoadFloat3(&vSourTranslation), XMLoadFloat3(&vDestTranslation), fRatio), 1.f);

	}

	_matrix	BoneTransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);
	XMStoreFloat4x4(&Lerp.mMatrix, BoneTransformationMatrix);

	mLerp.push_back(Lerp);
}

void CChannel::Change_TransformationMatrix(const vector<class CBone*>& Bones, _float Ratio, _float4x4 Matrix)
{
	// 이전 매트릭스를 나누기 위한 준비
	_vector		vScale{};
	_vector		vRotation{};
	_vector		vTranslation{};

	_float3		vSourScale{}, vDestScale{};
	_float4		vSourRotation{}, vDestRotation{};
	_float3		vSourTranslation{}, vDestTranslation{};

	// 마지막 위치의 애니메이션을 분리해서 스케일, 로테이션, 트랜슬레이션을 뽑음
	XMMatrixDecompose(&vScale, &vRotation, &vTranslation, XMLoadFloat4x4(&Matrix));
	XMStoreFloat3(&vSourScale, vScale);
	XMStoreFloat4(&vSourRotation, vRotation);
	XMStoreFloat3(&vSourTranslation, vTranslation);

	// 시작 해야할 애니메이션의 가장 앞부분의 스케일, 로테이션, 트렌슬레이션
	vDestScale = m_KeyFrames[0].vScale;
	vDestRotation = m_KeyFrames[0].vRotation;
	vDestTranslation = m_KeyFrames[0].vTranslation;

	vScale = XMVectorLerp(XMLoadFloat3(&vSourScale), XMLoadFloat3(&vDestScale), Ratio);
	vRotation = XMQuaternionSlerp(XMLoadFloat4(&vSourRotation), XMLoadFloat4(&vDestRotation), Ratio);
	vTranslation = XMVectorSetW(XMVectorLerp(XMLoadFloat3(&vSourTranslation), XMLoadFloat3(&vDestTranslation), Ratio), 1.f);


	// _matrix		BoneTransformationMatrix = XMMatrixScaling() * 자전행렬 * ;
	_matrix		BoneTransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);

	Bones[m_iBoneIndex]->Set_TransformationMatrix(BoneTransformationMatrix);
}

CChannel* CChannel::Create(const CModel* pModel, CHANNEL& mChannel)
{
	CChannel* pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(pModel, mChannel)))
	{
		MSG_BOX("Failed to Created : CChannel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChannel::Free()
{
	__super::Free();

	m_KeyFrames.clear();

}
