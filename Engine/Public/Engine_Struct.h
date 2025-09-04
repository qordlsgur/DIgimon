#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef struct tagEngineDesc
	{
		HINSTANCE			hInstance;
		HWND				hWnd;
		unsigned int		iNumLevels;
		unsigned int		iWinSizeX, iWinSizeY;
		WINMODE				eWindowMode;
	}ENGINE_DESC;

	// 구조체
	// 이 구조체는 VertexPosition이랑 TexCoord를 정의 해주는 구조체이다.
	// 원래는 밑에 vPosition이랑 vTexCoord밖에 없었는데 셰이더를 추가 할 때마다 계속
	// 밑에 static으로 되어있는 2개를 불러 줘야 했다.
	// 하지만 그걸 계속하다가 까먹으면 또 셰이더에서 오류가 나서 아예 직접 추가를 해줬다.
	// 그냥 추가를 했을때는 VTXPOSTEX pvt~이렇게 선언을 해야 실행이 된다.
	// 그리고 구조체의 size마저 늘어나게 된다. 그거를 방지 하기 위해서 static const를 이용해
	// 선언을 안하고 컴파일러 단계에서 할당이 되어서 바로 사용이 가능하다.
	// int 나 그런건 되는데 배열은 또 초기화가 안된다.
	// 11버전에 추가가 된건데 constexpr을 이용하면 이제 배열도 가능해졌다.
	typedef struct tagVertexPositionTexcoord
	{
		XMFLOAT3			vPosition;
		XMFLOAT2			vTexcoord;

		static constexpr unsigned int					iNumElements = { 2 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

	}VTXPOSTEX;

	// 위에는 이제 위치랑 텍스쳐만 할 수 있다면 이번에는 위치랑 텍스쳐 그리고 법선까지 추가됨
	typedef struct tagVertexPositionNormalTexcoord
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vNormal;
		XMFLOAT2			vTexcoord;

		static constexpr unsigned int					iNumElements = { 3 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
	}VTXNORTEX;

	typedef struct tagVertexMesh
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vNormal;
		XMFLOAT3			vTangent;
		XMFLOAT2			vTexcoord;

		static constexpr unsigned int					iNumElements = { 4 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
	}VTXMESH;

	typedef struct tagVertexAnimationMesh
	{
		XMFLOAT3			vPosition;
		XMFLOAT3			vNormal;
		XMFLOAT3			vTangent;
		XMFLOAT2			vTexcoord;

		XMUINT4				vBlendIndex;
		XMFLOAT4			vBlendWeight;

		static constexpr unsigned int					iNumElements = { 6 };
		static constexpr D3D11_INPUT_ELEMENT_DESC		Elements[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "BLENDINDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
	}VTXANIMMESH;

	typedef struct tagKeyFrame
	{
		XMFLOAT3		vScale;
		XMFLOAT4		vRotation;
		XMFLOAT3		vTranslation;
		float			fTrackPosition;
	}KEYFRAME;

	typedef struct LerpAnim
	{
		char mName[MAX_PATH];
		XMFLOAT4X4 mMatrix{};
	}LERP;

	typedef struct Material_Type_Data
	{
		unsigned int textureCount{};
		vector<wstring> szPerfectPath;
	}MATERIAL_TYPE;

	typedef struct Material_Data
	{
		unsigned int Type_Count{};
		vector<MATERIAL_TYPE> Path;
	}MATERIAL_DATA;

	typedef struct Vertex_Data
	{
		XMFLOAT3 mVertices{};
		XMFLOAT3 mNormals{};
		XMFLOAT3 mTangents{};
		XMFLOAT2 mTextureCoords{};
	}VERTEX;

	typedef struct Index_Data
	{
		unsigned int mIndices[3];
	}INDEX;

	typedef struct Mesh_Data
	{
		char mName[MAX_PATH];
		unsigned int mMaterialIndex{};
		unsigned int mNumVertices{};
		unsigned int mNumIndices{};
		unsigned int mNumFaces{};
		vector<VERTEX> mVertex;
		vector<INDEX> mIndex;
	}MESH;

	typedef struct NonAnim_Model_Data
	{
		unsigned int mNumMeshes{};
		unsigned int mNumMaterials{};
		vector<MESH> m_Mesh;
		vector<MATERIAL_DATA> Material_Data;
	}NONANIM_DATA;

	typedef struct Weight
	{
		unsigned int mVertexId{};
		float x{};
		float y{};
		float z{};
		float w{};
	}WEIGHT;

	typedef struct Bone
	{
		XMFLOAT4X4 mOffsetMatrix{};
		int mBoneIndex{};
		vector<WEIGHT> mWeight;

	}BONE;

	typedef struct AnimMesh_Data
	{
		char mName[MAX_PATH];
		unsigned int mMaterialIndex{};
		unsigned int mNumVertices{};
		unsigned int mNumIndices{};
		unsigned int mNumFaces{};
		unsigned int mNumBones{};
		vector<VERTEX> mVertex;
		vector<INDEX> mIndex;
		vector<BONE> mBone;
	}ANIM_MESH;


	typedef struct Bone_Data
	{
		char Bone_Name[MAX_PATH];
		int iParentIndex{};
		XMFLOAT4X4 mTransformation{};
	}BONE_DATA;

	typedef struct Channel
	{
		char mName[MAX_PATH];
		unsigned int mNumScalingKeys{};
		unsigned int mNumRotationKeys{};
		unsigned int mNumPositionKeys{};
		vector<KEYFRAME> mKeyFrame;
	}CHANNEL;

	typedef struct Animation
	{
		char mName[MAX_PATH];
		float mfDuration{};
		float mfTicksPerSecond{};
		unsigned int miNumChannels{};
		vector<CHANNEL> mChannel;
	}ANIMATION;

	typedef struct Anim_Model_Data
	{
		unsigned int mNumMeshes{};
		unsigned int mNumMaterials{};
		unsigned int mNumAnimations{};
		vector<ANIM_MESH> m_Mesh;
		vector<MATERIAL_DATA> Material_Data;
		vector<BONE_DATA> Bone_Data;
		vector<ANIMATION> mAnimation;
	}ANIM_DATA;

}

#endif // Engine_Struct_h__
