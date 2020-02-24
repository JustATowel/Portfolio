#pragma once
#include <fbxsdk.h>
#include <fstream>
#include <vector>
#include <string>
#include <DirectXMath.h>

#define MAX_INFLUENCES 4

namespace geometry
{
	using file_path_t = std::array<char, 260>;

	struct Vertex
	{
		Vertex() {}
		Vertex(float x, float y, float z, float u, float v, float nX, float nY, float nZ, float tx, float ty, float tz, float tw)
			: pos(x, y, z), texCoord(u, v), norm(nX, nY, nZ), tangent(tx, ty, tz, tw) {}

		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 texCoord;
		DirectX::XMFLOAT3 norm;
		DirectX::XMFLOAT4 tangent;
		int joints[4] = { -1,-1,-1,-1 };
		DirectX::XMFLOAT4 weights = { 0,0,0,0 };

	};
	struct Material_T
	{
		enum e_component { EMISSIVE = 0, DIFFUSE, SPECULAR, SHININESS, COUNT };

		struct component_t
		{
			float value[3] = { 0.0f,0.0f,0.0f };
			float factor = 0.0f;
			int64_t input = -1;
		};

		component_t& operator[](int i) { return components[i]; }
		const component_t& operator[](int i)const { return components[i]; }

	private:
		component_t components[COUNT];
	};
	struct path
	{
		char* path_add;
	};

	struct FBX_Joint
	{
		FbxNode* node;
		const char* name;
		int parent_index;
		int num_children;
		FBX_Joint() {}
		FBX_Joint(FbxNode* node, int parent_index)
			: node(node), parent_index(parent_index) {}
	};

	struct joint
	{
		DirectX::XMMATRIX joint_transform;
		int parent_index;
		
		joint() {}
		joint(DirectX::XMMATRIX joint_transform, int parent_index)
			: joint_transform(joint_transform), parent_index(parent_index) {}
	};

	struct matrix
	{
		DirectX::XMMATRIX xm_mat;
	};

	struct keyframe
	{
		double time;
		std::vector<DirectX::XMMATRIX> joints;
	};
	struct anim_clip
	{
		double duration;
		std::vector<geometry::keyframe> frames;
	};

	struct influence
	{
		int joint;
		float weight;
	};
}

class FbxLoader
{	
private:	
	
	std::vector<std::vector<geometry::influence>> control_point_influences;
	std::vector<geometry::FBX_Joint> FBX_joints;
	std::vector<geometry::joint> joints;
	std::vector<int> joint_skin_indices;
	std::vector<DirectX::XMMATRIX> inverse_bind;
	void GetAnimationData(FbxScene* _scene, const char* anim_filename, float scale = 1.0f);
	FbxPose* GetBindPoseSkeleton(FbxScene* _scene, int poseCount);
	void GetSkinningData(FbxPose* _bindPose);
	void GetMaterialData(FbxNode* childNode, const char* filename);
public:

	FbxScene* LoadFileIntoScene(const char* filename);	
	void ProcessFbxMesh(FbxScene* _scene, std::vector<geometry::Vertex>& vertexList, std::vector<unsigned int>& indiciesList, const char* filename, const char* anim_filename, float scale = 1.0f);
};