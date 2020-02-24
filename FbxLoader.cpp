#include "FbxLoader.h"
#include <queue>

using namespace DirectX;
FbxScene* FbxLoader::LoadFileIntoScene(const char* filename)
{
	// Initialize the SDK manager. This object handles memory management.
	FbxManager* lSdkManager = FbxManager::Create();
	// Create the IO settings object.
	FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);

	// Create an importer using the SDK manager.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

	// Use the first argument as the filename for the importer.
	if (!lImporter->Initialize(filename, -1, lSdkManager->GetIOSettings())) {
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
		exit(-1);
	}
	// Create a new scene so that it can be populated by the imported file.
	FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

	// Import the contents of the file into the scene.
	lImporter->Import(lScene);

	// The file is imported, so get rid of the importer.
	lImporter->Destroy();

	return lScene;
}

void FbxLoader::ProcessFbxMesh(FbxScene* _scene, std::vector<geometry::Vertex>& vertexList, std::vector<unsigned int>& indiciesList, const char* filename, const char* anim_filename, float scale)
{	
	FbxNode* node = _scene->GetRootNode();
	int childrenCount = node->GetChildCount();
	int poseCount = _scene->GetPoseCount();
	
	FbxPose* bind_pose = GetBindPoseSkeleton(_scene, poseCount);
	GetAnimationData(_scene, anim_filename);
	GetSkinningData(bind_pose);
	for (int pose_index = 0; pose_index < poseCount; ++pose_index)
	{
		
		////////////////////////////////////////////////////////////////////////////////
		// Mesh/Vertices Stuff
		////////////////////////////////////////////////////////////////////////////////

		for (int child_index = 0; child_index < childrenCount; ++child_index)
		{
			FbxPose* child = _scene->GetPose(pose_index);
			FbxNode* childNode = child->GetNode(child_index);
			FbxMesh* fbxMesh = childNode->GetMesh();

			
			
			if (fbxMesh != NULL)
			{
				// Get index count from mesh
				int numVertices = fbxMesh->GetControlPointsCount();


				// Resize the vertex vector to size of this mesh
				vertexList.resize(numVertices);

				//================= Process Vertices ===============
				for (int j = 0; j < numVertices; j++)
				{

					

					FbxVector4 vert = fbxMesh->GetControlPointAt(j);
					

					vertexList[j].pos.x = (float)vert.mData[0] / scale;
					vertexList[j].pos.y = (float)vert.mData[1] / scale;
					vertexList[j].pos.z = (float)vert.mData[2] / scale;
					for (int inf_index = 0; inf_index < control_point_influences[j].size(); ++inf_index)
					{
						vertexList[j].joints[inf_index] = control_point_influences[j][inf_index].joint;

						if (inf_index == 0)
							vertexList[j].weights.x = control_point_influences[j][inf_index].weight;
						else if (inf_index == 1)
							vertexList[j].weights.y = control_point_influences[j][inf_index].weight;
						else if (inf_index == 2)
							vertexList[j].weights.z = control_point_influences[j][inf_index].weight;
						else if (inf_index == 3)
							vertexList[j].weights.w = control_point_influences[j][inf_index].weight;
					}
				}
				
			
				
				int numIndices = fbxMesh->GetPolygonVertexCount();

				// No need to allocate int array, FBX does for us
				int* indices = fbxMesh->GetPolygonVertices();

				// Fill indiceList
				indiciesList.resize(numIndices);
				memcpy(indiciesList.data(), indices, numIndices * sizeof(int));

				FbxArray<FbxVector4> normalsVec;
				fbxMesh->GetPolygonVertexNormals(normalsVec);

				// Declare a new array for the second vertex array
				// Note the size is numIndices not numVertices
				std::vector<geometry::Vertex> vertexListExpanded;
				vertexListExpanded.resize(numIndices);

				FbxStringList luvSetNameList;
				fbxMesh->GetUVSetNames(luvSetNameList);
				const char* luvSetName = luvSetNameList.GetStringAt(0);
				const FbxGeometryElementUV* luvElement = fbxMesh->GetElementUV(luvSetName);
				const FbxGeometryElementNormal* lNormElement = fbxMesh->GetElementNormal();

				auto normalindexarray = lNormElement->GetIndexArray();

				int uvSetNamesCount = luvSetNameList.GetCount();

				if (uvSetNamesCount > 0)
				{
					bool t = fbxMesh->GenerateTangentsData(luvSetNameList[0]);
					int debug = 0;
				}

				const FbxGeometryElementTangent* lTangElement = fbxMesh->GetElementTangent();
				const FbxGeometryElementBinormal* lBinormElement = fbxMesh->GetElementBinormal();

				// align (expand) vertex array and set the normals
				for (int j = 0; j < numIndices; j++)
				{/**/
					vertexListExpanded[j].pos.x = vertexList[indices[j]].pos.x;
					vertexListExpanded[j].pos.y = vertexList[indices[j]].pos.y;
					vertexListExpanded[j].pos.z = vertexList[indices[j]].pos.z;
					vertexListExpanded[j].norm.x = normalsVec.GetAt(j)[0];
					vertexListExpanded[j].norm.y = normalsVec.GetAt(j)[1];
					vertexListExpanded[j].norm.z = normalsVec.GetAt(j)[2];
					vertexListExpanded[j].joints[0] = vertexList[indices[j]].joints[0];
					vertexListExpanded[j].joints[1] = vertexList[indices[j]].joints[1];
					vertexListExpanded[j].joints[2] = vertexList[indices[j]].joints[2];
					vertexListExpanded[j].joints[3] = vertexList[indices[j]].joints[3];
					vertexListExpanded[j].weights.x = vertexList[indices[j]].weights.x;
					vertexListExpanded[j].weights.y = vertexList[indices[j]].weights.y;
					vertexListExpanded[j].weights.z = vertexList[indices[j]].weights.z;
					vertexListExpanded[j].weights.w = vertexList[indices[j]].weights.w;
					

					////////////////////////////////////////////////
					// Texcoord
					if (luvElement->GetReferenceMode() == FbxLayerElement::eDirect)
					{
						FbxVector2 luvValue = luvElement->GetDirectArray().GetAt(indices[j]);

						vertexListExpanded[j].texCoord.x = luvValue[0];
						vertexListExpanded[j].texCoord.y = 1.0f - luvValue[1];
					}
					else if (luvElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
					{
						auto& index_array = luvElement->GetIndexArray();

						FbxVector2 lUVValue = luvElement->GetDirectArray().GetAt(index_array[j]);

						vertexListExpanded[j].texCoord.x = lUVValue[0];
						vertexListExpanded[j].texCoord.y = 1.0f - lUVValue[1];
					}
					////////////////////////////////////////////////

					////////////////////////////////////////////////
					// Tangent  ////////////////////////////////////
					////////////////////////////////////////////////
					if (lTangElement->GetReferenceMode() == FbxLayerElement::eDirect)
					{
						FbxVector4 lValue = lTangElement->GetDirectArray().GetAt(indices[j]);

						vertexListExpanded[j].tangent.x = lValue[0];
						vertexListExpanded[j].tangent.y = lValue[1];
						vertexListExpanded[j].tangent.z = lValue[2];
						vertexListExpanded[j].tangent.w = lValue[3];
					}
					else if (lTangElement->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
					{
						auto& index_array = lTangElement->GetIndexArray();

						FbxVector4 lValue = lTangElement->GetDirectArray().GetAt(index_array[j]);

						vertexListExpanded[j].tangent.x = lValue[0];
						vertexListExpanded[j].tangent.y = lValue[1];
						vertexListExpanded[j].tangent.z = lValue[2];
						vertexListExpanded[j].tangent.w = lValue[3];
					}
					////////////////////////////////////////////////
				}


				// make new indices to match the new vertex(2) array
				std::vector<unsigned int> indicesList;
				indicesList.resize(numIndices);

				for (int j = 0; j < numIndices; j++)
				{
					indicesList[j] = j;
				}

				std::vector<geometry::Vertex> compactedList;

				if (!true)
				{
					//compactify
					indicesList.clear();
					int foundIndex = 0;
					for (int i = 0; i < numIndices; ++i)
					{
						bool isFound = false;

						for (int j = 0; j < compactedList.size(); ++j)
						{
							if (abs(vertexListExpanded[i].pos.x - compactedList[j].pos.x) <= 0.001f &&
								abs(vertexListExpanded[i].pos.y - compactedList[j].pos.y) <= 0.001f &&
								abs(vertexListExpanded[i].pos.z - compactedList[j].pos.z) <= 0.001f &&
								abs(vertexListExpanded[i].norm.x - compactedList[j].norm.x) <= 0.001f &&
								abs(vertexListExpanded[i].norm.y - compactedList[j].norm.y) <= 0.001f &&
								abs(vertexListExpanded[i].norm.z - compactedList[j].norm.z) <= 0.001f &&
								abs(vertexListExpanded[i].texCoord.x - compactedList[j].texCoord.x) <= 0.001f &&
								abs(vertexListExpanded[i].texCoord.y - compactedList[j].texCoord.y) <= 0.001f &&
								abs(vertexListExpanded[i].tangent.x - compactedList[j].tangent.x) <= 0.001f &&
								abs(vertexListExpanded[i].tangent.y - compactedList[j].tangent.y) <= 0.001f &&
								abs(vertexListExpanded[i].tangent.z - compactedList[j].tangent.z) <= 0.001f &&
								vertexListExpanded[i].joints[0] == compactedList[j].joints[0] &&
								vertexListExpanded[i].joints[1] == compactedList[j].joints[1] &&
								vertexListExpanded[i].joints[2] == compactedList[j].joints[2] &&
								vertexListExpanded[i].joints[3] == compactedList[j].joints[3] &&
								abs(vertexListExpanded[i].weights.x - compactedList[j].weights.x) <= 0.001f &&
								abs(vertexListExpanded[i].weights.y - compactedList[j].weights.y) <= 0.001f &&
								abs(vertexListExpanded[i].weights.z - compactedList[j].weights.z) <= 0.001f &&
								abs(vertexListExpanded[i].weights.w - compactedList[j].weights.w) <= 0.001f)
							{
								isFound = true;
								indicesList.push_back(j);
								break;
							}
						}
						if (!isFound)
						{
							compactedList.push_back(vertexListExpanded[i]);
							indicesList.push_back(compactedList.size() - 1);
						}
					}
					vertexList.assign(compactedList.begin(), compactedList.end());
					indiciesList.assign(indicesList.begin(), indicesList.end());
				}
				else
				{
					// copy working data to the global SimpleMesh
					indiciesList.assign(indicesList.begin(), indicesList.end());
					vertexList.assign(vertexListExpanded.begin(), vertexListExpanded.end());
				}

				GetMaterialData(childNode, filename);
			
			}
		}
	}
}

void FbxLoader::GetAnimationData(FbxScene* _scene, const char* anim_filename, float scale)
{
	FbxAnimStack* anim_stack = _scene->GetCurrentAnimationStack();
	FbxTimeSpan time_span = anim_stack->GetLocalTimeSpan();
	FbxTime duration = time_span.GetDuration();
	FbxLongLong frame_count = duration.GetFrameCount(FbxTime::EMode::eFrames24);
	geometry::anim_clip clip;
	//TODO: set clip duration
	clip.duration = duration.GetSecondDouble();
	int poseCount = _scene->GetPoseCount();
	

	for (int FBX_joint_index = 0; FBX_joint_index < FBX_joints.size(); ++FBX_joint_index)
	{
		FbxAMatrix mat = FBX_joints[FBX_joint_index].node->EvaluateGlobalTransform();

		//mat = mat.Inverse();
		XMMATRIX M;

		for (int k = 0; k < 4; ++k)
		{
			FbxVector4 v = mat.GetRow(k);
			double x = v[0];
			double y = v[1];
			double z = v[2];
			if (k == 3)
			{
				x /= scale;
				y /= scale;
				z /= scale;
			}
			XMVECTOR vec = { v[0],v[1],v[2], v[3] };
			M.r[k] = vec;
		}

		joints.push_back(geometry::joint(M, FBX_joints[FBX_joint_index].parent_index));
	}
	for (FbxLongLong i = 1; i <= frame_count; ++i)
	{

		geometry::keyframe kf;
		FbxTime key_time;
		key_time.SetFrame(i, FbxTime::EMode::eFrames24);
		//TODO: set the keyframes time
		kf.time = key_time.GetSecondDouble();
		for (int j = 0; j < FBX_joints.size(); ++j)
		{
			FbxAMatrix mat = FBX_joints[j].node->EvaluateGlobalTransform(key_time);
			XMMATRIX M;
			for (int k = 0; k < 4; ++k)
			{
				FbxVector4 v = mat.GetRow(k);
				double x = v[0];
				double y = v[1];
				double z = v[2];
				if (k == 3)
				{
					x /= scale;
					y /= scale;
					z /= scale;
				}
				XMVECTOR vec = { x,y,z, v[3] };
				M.r[k] = vec;

			}
			
			kf.joints.push_back(M);
		}
		clip.frames.push_back(kf);
		//}
	}
	//save material count, materials, pathcount, and paths to file
	std::ofstream a_file(anim_filename, std::ios::trunc | std::ios::binary | std::ios::out);
	assert(a_file.is_open());

	
	uint16_t j_count = (uint16_t)joints.size();
	a_file.write((const char*)&j_count, sizeof(uint16_t));
	a_file.write((const char*)joints.data(), sizeof(geometry::joint) * joints.size());

	uint16_t frames_count = (uint16_t)clip.frames.size();
	uint16_t kf_joint_count = j_count;
	a_file.write((const char*)&frames_count, sizeof(uint16_t));
	a_file.write((const char*)&kf_joint_count, sizeof(uint16_t));
	a_file.write((const char*)&clip.duration, sizeof(double));


	for (int i = 0; i < clip.frames.size(); ++i)
	{
		a_file.write((const char*)&clip.frames[i].time, sizeof(double));
		a_file.write((const char*)clip.frames[i].joints.data(), sizeof(DirectX::XMMATRIX) * clip.frames[i].joints.size());
	}


	a_file.close();
}

FbxPose* FbxLoader::GetBindPoseSkeleton(FbxScene* _scene, int poseCount)
{
	for (int pose_index = 0; pose_index < poseCount; ++pose_index)
	{
		FbxPose* pose = _scene->GetPose(pose_index);

		if (pose->IsBindPose())
		{
			int item_count = pose->GetCount();
			FbxSkeleton* skeleton = nullptr;
			FbxNode* root = nullptr;
			for (int item_index = 0; item_index < item_count; ++item_index)
			{
				FbxNode* skel_node = pose->GetNode(item_index);


				if (skel_node->GetSkeleton())
				{
					skeleton = skel_node->GetSkeleton();
					root = skel_node;
					break;
				}

				else if (!skel_node->GetSkeleton())
					continue;
			}

			if (skeleton->IsSkeletonRoot())
			{
				geometry::FBX_Joint j = geometry::FBX_Joint(root, -1);
				j.name = root->GetName();
				FBX_joints.push_back(j);



				int size = FBX_joints.size();
				for (int i = 0; i < size; ++i)
				{
					FBX_joints[i].num_children = FBX_joints[i].node->GetChildCount();
					for (int j = 0; j < FBX_joints[i].num_children; ++j)
					{
						FbxNode* child_node = FBX_joints[i].node->GetChild(j);
						if (child_node != nullptr)
						{
							geometry::FBX_Joint child = geometry::FBX_Joint(child_node, i);
							child.name = FBX_joints[i].node->GetChild(j)->GetName();
							FBX_joints.push_back(child);
							size++;
						}
						
					}
				}

			}
			return pose;
		}
	}
	return nullptr;
}

void FbxLoader::GetSkinningData(FbxPose* _bindPose)
{	
	FbxNode* pose_node = _bindPose->GetNode(0);
	FbxMesh* currMesh = pose_node->GetMesh();
	int deformers_count = currMesh->GetDeformerCount();
	control_point_influences.resize(currMesh->GetControlPointsCount());
	if (currMesh != NULL)
	{
		FbxSkin* currSkin = nullptr;
		for (int deformer_index = 0; deformer_index < deformers_count; ++deformer_index)
		{
			currSkin = reinterpret_cast<FbxSkin*>(currMesh->GetDeformer(deformer_index, FbxDeformer::eSkin));
			if (currSkin)
				break;
		}

		int num_clusters = currSkin->GetClusterCount();

		for (int cluster_group_index = 0; cluster_group_index < num_clusters; ++cluster_group_index)
		{
			FbxCluster* currCluster = currSkin->GetCluster(cluster_group_index);
			FbxNode* link_Node = currCluster->GetLink();

			for (int fbx_joint_index = 0; fbx_joint_index < FBX_joints.size(); ++fbx_joint_index)
			{
				const char* name = link_Node->GetName();
				if (link_Node == FBX_joints[fbx_joint_index].node && name == FBX_joints[fbx_joint_index].name)
				{
					double* weights = currCluster->GetControlPointWeights();
					int* cp_indices = currCluster->GetControlPointIndices();
					int cluster_size = currCluster->GetControlPointIndicesCount();
					for (int cluster_index = 0; cluster_index < cluster_size; ++cluster_index)
					{
						geometry::influence inf;
						inf.joint = fbx_joint_index;
						inf.weight = (float)weights[cluster_index];
						int control_point_index = cp_indices[cluster_index];

						control_point_influences[control_point_index].push_back(inf);


						if (control_point_influences[control_point_index].size() > MAX_INFLUENCES)
						{
							float weakest_weight = 1;
							int weakest_index = 0;
							for (int weight_indexer = 0; weight_indexer < control_point_influences[control_point_index].size(); ++weight_indexer)
							{
								if (control_point_influences[control_point_index][weight_indexer].weight < weakest_weight)
								{
									weakest_index = weight_indexer;
									weakest_index = control_point_influences[control_point_index][weight_indexer].weight;
								}
							}

							control_point_influences[control_point_index].erase(control_point_influences[control_point_index].begin() + weakest_index);
						}

					}



					for (int cp_index = 0; cp_index < FBX_joints.size(); ++cp_index)
					{
						float weight_sum = 0;
						float num_weights = control_point_influences[cp_index].size();
						for (int norm_indexer = 0; norm_indexer < num_weights; ++norm_indexer)
						{
							if (control_point_influences[cp_index][norm_indexer].weight != -1)
								weight_sum += control_point_influences[cp_index][norm_indexer].weight;
						}

						if (weight_sum != 1.0f)
						{
							int weight_count = 0;
							for (int norm_indexer = 0; norm_indexer < num_weights; ++norm_indexer)
							{
								if (control_point_influences[cp_index][norm_indexer].weight != -1)
									++weight_count;
								control_point_influences[cp_index][norm_indexer].weight /= weight_sum;
							}
							for (int norm_indexer = 0; norm_indexer < weight_count; ++norm_indexer)
							{
								control_point_influences[cp_index][norm_indexer].weight /= weight_sum;
							}
						}
					}


				}
			}
		}
	}
		
}


void FbxLoader::GetMaterialData(FbxNode* childNode, const char* filename)
{
	std::vector<geometry::Material_T> mats;
	std::vector<std::string> path;
	int materialCount = childNode->GetSrcObjectCount<FbxSurfaceMaterial>();

	for (int index = 0; index < materialCount; index++)
	{
		FbxSurfaceMaterial* material = (FbxSurfaceMaterial*)childNode->GetSrcObject<FbxSurfaceMaterial>(index);

		if (material != NULL)
		{
			geometry::Material_T mat;

			std::string finalFileName;

			if (!material->Is<FbxSurfaceLambert>())
				continue;

			FbxSurfaceLambert* lamb = (FbxSurfaceLambert*)material;
			FbxDouble3 diffuse_color = lamb->Diffuse.Get();
			FbxDouble diffuse_factor = lamb->DiffuseFactor.Get();

			mat[geometry::Material_T::DIFFUSE].value[0] = diffuse_color[0];
			mat[geometry::Material_T::DIFFUSE].value[1] = diffuse_color[1];
			mat[geometry::Material_T::DIFFUSE].value[2] = diffuse_color[2];
			mat[geometry::Material_T::DIFFUSE].factor = diffuse_factor;
			FbxFileTexture* file_texture = lamb->Diffuse.GetSrcObject<FbxFileTexture>();
			if (file_texture)
			{
				//geometry::file_path_t f_path;
				const char* textureName = file_texture->GetFileName();


				std::string a(textureName);
				mat[geometry::Material_T::DIFFUSE].input = path.size();
				path.push_back(a);
			}

			FbxDouble3 emissive_color = lamb->Emissive.Get();
			FbxDouble emissive_factor = lamb->EmissiveFactor.Get();
			mat[geometry::Material_T::EMISSIVE].value[0] = emissive_color[0];
			mat[geometry::Material_T::EMISSIVE].value[1] = emissive_color[1];
			mat[geometry::Material_T::EMISSIVE].value[2] = emissive_color[2];
			mat[geometry::Material_T::EMISSIVE].factor = emissive_factor;
			file_texture = lamb->Emissive.GetSrcObject<FbxFileTexture>();

			if (file_texture)
			{
				const char* textureName = file_texture->GetFileName();
				std::string a(textureName);
				mat[geometry::Material_T::EMISSIVE].input = path.size();
				path.push_back(a);
			}

			if (material->Is<FbxSurfacePhong>())
			{
				FbxSurfacePhong* fong = (FbxSurfacePhong*)material;
				FbxDouble3 spec_color = fong->Specular.Get();
				FbxDouble spec_factor = fong->SpecularFactor.Get();
				mat[geometry::Material_T::SPECULAR].value[0] = spec_color[0];
				mat[geometry::Material_T::SPECULAR].value[1] = spec_color[1];
				mat[geometry::Material_T::SPECULAR].value[2] = spec_color[2];
				mat[geometry::Material_T::SPECULAR].factor = spec_factor;

				if (FbxFileTexture* file_texture = fong->Specular.GetSrcObject<FbxFileTexture>())
				{
					const char* textureName = file_texture->GetFileName();
					std::string a(textureName);
					mat[geometry::Material_T::SPECULAR].input = path.size();
					path.push_back(a);
				}

				FbxDouble shininess_factor = fong->Shininess.Get();
				mat[geometry::Material_T::SHININESS].factor = shininess_factor;

				if (FbxFileTexture* file_texture = fong->Shininess.GetSrcObject<FbxFileTexture>())
				{
					const char* textureName = file_texture->GetFileName();
					std::string a(textureName);
					mat[geometry::Material_T::SHININESS].input = path.size();
					path.push_back(a);
				}
			}
			mats.push_back(mat);
		}
	}

	//save material count, materials, pathcount, and paths to file
	std::ofstream file(filename, std::ios::trunc | std::ios::binary | std::ios::out);
	assert(file.is_open());
	uint16_t mat_count = (uint16_t)mats.size();

	uint16_t path_count = (uint16_t)path.size();
	uint16_t str_size;
	file.write((const char*)&mat_count, sizeof(uint16_t));
	file.write((const char*)mats.data(), sizeof(geometry::Material_T) * mats.size());

	file.write((const char*)&path_count, sizeof(uint16_t));
	for (int i = 0; i < path_count; ++i)
	{
		str_size = path[i].length();
		file.write((const char*)&str_size, sizeof(uint16_t));

		file.write((const char*)path[i].c_str(), str_size);
	}

	file.close();
}