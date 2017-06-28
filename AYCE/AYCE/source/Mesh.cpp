/*****************************************************************************/
/*!
\file   Mesh.cpp
\author Yeongki Baek
\par    email: yeongki.baek\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for Mesh features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
//#include "Mesh.h"
//namespace VK
//{
//	Mesh::Mesh(const std::string& filename) {
//		Assimp::Importer importer;
//		const aiScene* scene = importer.ReadFile(filename,
//		/*	aiProcess_GenNormals |
//			aiProcess_Triangulate |
//			aiProcess_SortByPType |
//			aiProcess_JoinIdenticalVertices
//			*/
//			aiProcessPreset_TargetRealtime_MaxQuality);
//		
//		/*aiProcess_CalcTangentSpace      |
//        aiProcess_GenNormals            |
//        aiProcess_JoinIdenticalVertices | 
//        aiProcess_Triangulate      |     
//        aiProcess_GenUVCoords     |      
//			aiProcess_SortByPType);*/
//
//		if (!scene) {
//			std::cerr << "Scene is null";
//		}
//
//		if (scene->HasMeshes()) {
//			aiMesh* m = scene->mMeshes[0];
//			for (unsigned int i = 0; i < m->mNumVertices; i++) {
//				//aiVector3D* tangent = &(m->mTangents[i]);
//				aiVector3D* vertex = &(m->mVertices[i]);
//				aiVector3D* normal = &(m->mNormals[i]);
//				
//				vertices.push_back(vertex->x);
//				vertices.push_back(vertex->y);
//				vertices.push_back(vertex->z);
//				normals.push_back(normal->x);
//				normals.push_back(normal->y);
//				normals.push_back(normal->z);
//				/*
//				if (tangent != nullptr)
//				{
//					if (tangent->x &&tangent->y && tangent->z)
//					{
//						tangents.push_back(tangent->x);
//						tangents.push_back(tangent->y);
//						tangents.push_back(tangent->z);
//					}
//				}
//				*/
//				/*
//				if (mesh->HasNormals()) {
//					for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
//						mesh->mNormals[i] = (m * mesh->mNormals[i]).Normalize();
//					}
//				}
//				if (mesh->HasTangentsAndBitangents()) {
//					for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
//						mesh->mTangents[i] = (m * mesh->mTangents[i]).Normalize();
//						mesh->mBitangents[i] = (m * mesh->mBitangents[i]).Normalize();
//					}
//				}
//				*/
//			}
//			for (unsigned int i = 0; i < m->mNumFaces; i++) {
//				aiFace* face = &(m->mFaces[i]);
//				if (face->mNumIndices != 3) {
//					std::cerr << "numIndices is not 3";
//				}
//				for (unsigned int j = 0; j < face->mNumIndices; j++) {
//					indices.push_back(face->mIndices[j]);
//				}
//			}
//		}
//		else {
//			std::cerr << "Scene has no meshes";
//		}
//
//		glGenBuffers(1, &positionBufferObject);
//		glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &(vertices[0]), GL_STATIC_DRAW);
//
//		glGenBuffers(1, &normalBufferObject);
//		glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * normals.size(), &(normals[0]), GL_STATIC_DRAW);
//
//		glGenBuffers(1, &indexBufferObject);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)* indices.size(), &(indices[0]), GL_STATIC_DRAW);
//
//
//		/*
//		#define aisgl_min(x,y) (x<y?x:y)
//#define aisgl_max(x,y) (y>x?y:x)
//
//void get_bounding_box_for_node (const aiNode* nd,
//	aiVector3D* min,
//	aiVector3D* max)
//
//{
//	aiMatrix4x4 prev;
//	unsigned int n = 0, t;
//
//	for (; n < nd->mNumMeshes; ++n) {
//		const aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
//		for (t = 0; t < mesh->mNumVertices; ++t) {
//
//			aiVector3D tmp = mesh->mVertices[t];
//
//			min->x = aisgl_min(min->x,tmp.x);
//			min->y = aisgl_min(min->y,tmp.y);
//			min->z = aisgl_min(min->z,tmp.z);
//
//			max->x = aisgl_max(max->x,tmp.x);
//			max->y = aisgl_max(max->y,tmp.y);
//			max->z = aisgl_max(max->z,tmp.z);
//		}
//	}
//
//	for (n = 0; n < nd->mNumChildren; ++n) {
//		get_bounding_box_for_node(nd->mChildren[n],min,max);
//	}
//}
//
//
//void get_bounding_box (aiVector3D* min, aiVector3D* max)
//{
//
//	min->x = min->y = min->z =  1e10f;
//	max->x = max->y = max->z = -1e10f;
//	get_bounding_box_for_node(scene->mRootNode,min,max);
//}
//
//bool Import3DFromFile( const std::string& pFile)
//{
//
//	//check if file exists
//	std::ifstream fin(pFile.c_str());
//	if(!fin.fail()) {
//		fin.close();
//	}
//	else{
//		printf("Couldn't open file: %s\n", pFile.c_str());
//		printf("%s\n", importer.GetErrorString());
//		return false;
//	}
//
//	scene = importer.ReadFile( pFile, aiProcessPreset_TargetRealtime_Quality);
//
//	// If the import failed, report it
//	if( !scene)
//	{
//		printf("%s\n", importer.GetErrorString());
//		return false;
//	}
//
//	// Now we can access the file's contents.
//	printf("Import of scene %s succeeded.",pFile.c_str());
//
//	aiVector3D scene_min, scene_max, scene_center;
//	get_bounding_box(&scene_min, &scene_max);
//	float tmp;
//	tmp = scene_max.x-scene_min.x;
//	tmp = scene_max.y - scene_min.y > tmp?scene_max.y - scene_min.y:tmp;
//	tmp = scene_max.z - scene_min.z > tmp?scene_max.z - scene_min.z:tmp;
//	scaleFactor = 1.f / tmp;
//
//	// We're done. Everything will be cleaned up by the importer destructor
//	return true;
//}
//		*/
//	}
//
//
//	Mesh::~Mesh()
//	{
//		vertices.clear();
//		normals.clear();
//		indices.clear();
//	}
//
//	void Mesh::draw(ShaderProgram* shader, bool drawNormals) {
//		glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
//		shader->vertexAttribPointer("position", 3, GL_FLOAT, 0, nullptr, GL_FALSE);
//
//		if (drawNormals) {
//			glBindBuffer(GL_ARRAY_BUFFER, normalBufferObject);
//			shader->vertexAttribPointer("normal", 3, GL_FLOAT, 0, nullptr, GL_FALSE);
//		}
//
//		// Bind the index buffer
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
//
//		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
//	}
//}