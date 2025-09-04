#pragma once

#include "Base.h"

/* 이 뼈는 시간대별 취해야할 뼈의 상태들 .*/
/* Update함수에서 내가 취해야할 시간을 넣어주면 시간에 맞느 ㅅ아태행렬을 보간해서 만들꺼라고. */
/* 만든행렬을 뼈(CBone)에게 전달해줘야지. */
NS_BEGIN(Engine)

class CChannel final : public CBase
{
private:
	CChannel();
	virtual ~CChannel() = default;

public:

	HRESULT Initialize(const class CModel* pModel, CHANNEL& mChannel);
	void Update_TransformationMatrix(const vector<class CBone*>& Bones, _float fCurrentTrackPosition, _uint* pCurrentKeyFrameIndex);
	void Save_TransformationMatrix(const vector<class CBone*>& Bones, _float fCurrentTrackPosition, _uint* pCurrentKeyFrameIndex, vector<LERP>& mLerp);
	void Change_TransformationMatrix(const vector<class CBone*>& Bones, _float Ratio, _float4x4 Matrix);

	const _char* Get_Name() { return m_szName; }

	const vector<KEYFRAME> Get_Frame() { return m_KeyFrames; }

private:
	_char					m_szName[MAX_PATH] = {};
	_int					m_iBoneIndex = { -1 };

	_uint					m_iNumKeyFrames = {};
	vector<KEYFRAME>		m_KeyFrames;

public:
	static CChannel* Create(const class CModel* pModel, CHANNEL& mChannel);
	virtual void Free() override;
};

NS_END