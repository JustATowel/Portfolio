#include "FbxLoader.h"
#include <iostream>

using namespace std;

int main(int ac, char** av)
{
	FbxLoader fbx;
	cout << "Type name of fbx file to be converted: ";
	string filename;

	getline(cin, filename);

	
	string passFileName(filename);
	
	passFileName.append(".fbx");
	passFileName.insert(0, "assets/fbx/");

	string finalFileName(filename);

	finalFileName.append(".bin");
	finalFileName.insert(0, "assets/meshes/");

	string matFileName(filename);

	matFileName.append(".bin");
	matFileName.insert(0, "assets/materials/");

	string animFileName(filename);

	animFileName.append(".bin");
	animFileName.insert(0, "assets/animations/");

	vector<geometry::Vertex> v;
	vector<unsigned int> i;

	fbx.ProcessFbxMesh(fbx.LoadFileIntoScene(passFileName.c_str()), v, i, matFileName.c_str(), animFileName.c_str());

	ofstream file(finalFileName.data(), ios::trunc | ios::binary | ios::out);
	assert(file.is_open());
	uint32_t index_count = (uint32_t)i.size();
	uint32_t vert_count = (uint32_t)v.size();
	file.write((const char*)&index_count, sizeof(uint32_t));
	file.write((const char*)i.data(), sizeof(uint32_t) * i.size());
	file.write((const char*)&vert_count, sizeof(uint32_t));
	for (int vert_indexer = 0; vert_indexer < v.size(); ++vert_indexer)
	{
		file.write((const char*)&v[vert_indexer].pos, sizeof(DirectX::XMFLOAT3));
		file.write((const char*)&v[vert_indexer].texCoord, sizeof(DirectX::XMFLOAT2));
		file.write((const char*)&v[vert_indexer].norm, sizeof(DirectX::XMFLOAT3));
		file.write((const char*)&v[vert_indexer].tangent, sizeof(DirectX::XMFLOAT4));
		file.write((const char*)&v[vert_indexer].joints, sizeof(int) * 4);
		file.write((const char*)&v[vert_indexer].weights, sizeof(DirectX::XMFLOAT4));
	}
	
	
	file.close();
}