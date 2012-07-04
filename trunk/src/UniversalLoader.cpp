//
// UniversalLoader.cpp for glEngine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Tue Jun 12 16:10:58 2012 loick michard
// Last update Wed Jul  4 14:18:09 2012 gael jochaud-du-plessix
//

#include <UniversalLoader.hpp>

gle::UniversalLoader::UniversalLoader() : _rootNode(NULL), _texturesPath("")
{

}

gle::UniversalLoader::~UniversalLoader()
{
  
}

gle::Scene::Node* gle::UniversalLoader::load(std::string const & file,
					     gle::Material* defaultMaterial)
{
  (void)defaultMaterial;
  const aiScene* scene = _importer.ReadFile(file,
					    aiProcess_CalcTangentSpace       |
					    aiProcess_Triangulate            |
					    aiProcess_JoinIdenticalVertices  |
					    aiProcess_SortByPType            |                                                                             
					    aiProcess_GenSmoothNormals                                                                                  
					    );
  if (!scene)
    throw new gle::Exception::Exception("Cannot load the scene");

  // for (GLuint i = 0; i < scene->mNumAnimations; ++i)
  //   {
  //     aiAnimation* anim = scene->mAnimations[i];
  //     std::cout << anim->mName.data << ":\n";
  //     for (GLuint j = 0; j < anim->mNumChannels; ++j)
  // 	{
  // 	  aiNodeAnim* channel = anim->mChannels[j];
  // 	  std::cout << channel->mNodeName.data << ": "
  // 		    << channel->mNumPositionKeys << ", "
  // 		    << channel->mNumRotationKeys << ", "
  // 		    << channel->mNumScalingKeys
  // 		    << "\n";
  // 	  for (GLuint k = 0; k < channel->mNumPositionKeys; ++k)
  // 	    {
  // 	      aiVectorKey &key = channel->mPositionKeys[k];
  // 	      //std::cout << key.mTime << ": " << gle::Vector3f(key.mValue.x, key.mValue.y, key.mValue.z) << "\n";
  // 	    }
  // 	}
  //   }

  _materials.resize(scene->mNumMaterials);
  for (GLuint i = 0; i < scene->mNumMaterials; ++i)
    _materials[i] = _loadAssimpMaterial(file, scene, scene->mMaterials[i]);
  _rootNode = new gle::Scene::Node(gle::Scene::Node::Empty);
  _loadAssimpNode(scene, scene->mRootNode);
  return (_rootNode);
}

gle::Material* gle::UniversalLoader::_loadAssimpMaterial(const std::string& file,
							 const aiScene* scene,
							 aiMaterial* material)
{
  gle::Material* gleMaterial = new gle::Material();
  
  aiString name;
  material->Get<aiString>(AI_MATKEY_NAME, name);
  //std::cout << "loading material: '" << name.data << "'" << std::endl;
  
  gleMaterial->setName(name.data);
  
  aiColor4D color;
  if (material->Get<aiColor4D>(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS)
    gleMaterial->setAmbientColor(gle::Colorf(color.r, color.g, color.b, color.a));
  if (material->Get<aiColor4D>(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
    gleMaterial->setDiffuseColor(gle::Colorf(color.r, color.g, color.b, color.a));
  if (material->Get<aiColor4D>(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
    gleMaterial->setSpecularColor(gle::Colorf(color.r, color.g, color.b, color.a));

  float shininess, strength;
  if (material->Get<float>(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS
      && material->Get<float>(AI_MATKEY_SHININESS_STRENGTH, strength) == AI_SUCCESS)
    gleMaterial->setShininess(shininess * strength);
  aiString textureFilePath;
  if (material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath) == AI_SUCCESS)
    {
      std::string texture = textureFilePath.data;
      if (texture[0] == '*')
	{
	  // Load embedded texture                                                                                                                         
	  GLuint textureId = atoi(texture.substr(1, texture.size()).c_str());
	  if (textureId < scene->mNumTextures)
	    {
	      aiTexture* tex = scene->mTextures[textureId];
	      if (tex->mHeight == 0)
		{
		  sf::Image texImage;
		  bool res = texImage.loadFromMemory((char*)tex->pcData, tex->mWidth);
		  if (res)
		    {
		      gle::Texture* gleTexture = new gle::Texture();
		      gleTexture->setData(texImage);
		      gleMaterial->setColorMap(gleTexture);
		    }
		  else
		    std::cout << "Cannot load texture" << std::endl;
		  // std::fstream datasFile("test", std::fstream::out);                                                                                    
		  // datasFile.write((char*)tex->pcData, tex->mWidth);                                                                                     
		}
	    }
	}
      else
	{
	  replace(texture.begin(), texture.end(), '\\', '/');
	  if (_texturesPath != "")
	    {
	      size_t slashPos = texture.find_last_of('/');
	      if (slashPos != std::string::npos)
		texture = _texturesPath + texture.substr(slashPos + 1, texture.size() - slashPos - 1);
	      else
		texture = _texturesPath + "/" + texture;
	    }
	  else
	    {
	      size_t slashPos = file.find_last_of('/');
	      if (slashPos != std::string::npos)
		texture = file.substr(0, slashPos + 1) + texture;
	    }
	  try
	    {
	      std::cout << texture << std::endl;
	      gle::Texture* tex = new gle::Texture(texture);
	      gleMaterial->setColorMap(tex);
	    }
	  catch (std::exception* e)
	    {
		std::cout << "Cannot load texture '" << texture << "'" << std::endl;
	    }
	}
    }  
  return (gleMaterial);
}

void gle::UniversalLoader::_loadAssimpNode(const aiScene* scene, aiNode* node)
{
  gle::Scene::Node* gleNode = new gle::Scene::Node(gle::Scene::Node::Empty);
  gleNode->setName(node->mName.data);

  for (GLuint i = 0; i < node->mNumMeshes; ++i)
    {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

      // First import the Skeleton assigned to this mesh
      gle::Skeleton* skeleton = _loadAssimpSkeleton(scene, mesh);

      if (!(mesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE))
	{
	  if (skeleton)
	    gleNode->addChild(skeleton);
	  continue ;
	}

      gle::Mesh* gleMesh = new gle::Mesh(_materials[mesh->mMaterialIndex]);
      gleMesh->setName(mesh->mName.data);

      if (skeleton)
      	gleMesh->addChild(skeleton);

      if (mesh->mNumVertices > 0)
	{
	  GLfloat* vertexAttributes = new GLfloat[mesh->mNumVertices * gle::Mesh::VertexAttributesSize];
	  for (GLuint v = 0; v < mesh->mNumVertices; ++v)
	    {
	      vertexAttributes[v * gle::Mesh::VertexAttributesSize + 0] = mesh->mVertices[v].x;
	      vertexAttributes[v * gle::Mesh::VertexAttributesSize + 1] = mesh->mVertices[v].y;
	      vertexAttributes[v * gle::Mesh::VertexAttributesSize + 2] = mesh->mVertices[v].z;
 	      if (mesh->mNormals)
		{
		  vertexAttributes[v * gle::Mesh::VertexAttributesSize + gle::Mesh::VertexAttributeSizeCoords + 0] = mesh->mNormals[v].x;
		  vertexAttributes[v * gle::Mesh::VertexAttributesSize + gle::Mesh::VertexAttributeSizeCoords + 1] = mesh->mNormals[v].y;
		  vertexAttributes[v * gle::Mesh::VertexAttributesSize + gle::Mesh::VertexAttributeSizeCoords + 2] = mesh->mNormals[v].z;
		}
	      if (mesh->mTextureCoords[0])
		{
		  vertexAttributes[v * gle::Mesh::VertexAttributesSize + gle::Mesh::VertexAttributeSizeCoords +
				   gle::Mesh::VertexAttributeSizeNormal + gle::Mesh::VertexAttributeSizeTangent + 0] = mesh->mTextureCoords[0][v].x;
		  vertexAttributes[v * gle::Mesh::VertexAttributesSize + gle::Mesh::VertexAttributeSizeCoords +
				   gle::Mesh::VertexAttributeSizeNormal + gle::Mesh::VertexAttributeSizeTangent + 1] = mesh->mTextureCoords[0][v].y;
		}
	    }
	  gleMesh->setVertexAttributes(vertexAttributes, mesh->mNumVertices);

	  gle::Array<GLuint> indexes;
	  for (GLuint f = 0; f < mesh->mNumFaces; ++f)
	    {
	      if (mesh->mFaces[f].mNumIndices == 3)
		indexes.push(mesh->mFaces[f].mIndices[0],
			     mesh->mFaces[f].mIndices[1],
			     mesh->mFaces[f].mIndices[2]);
	    }
	  gleMesh->setIndexes(indexes);
	}
      gleNode->addChild(gleMesh);
    }

  _rootNode->addChild(gleNode);

  for (GLuint i = 0; i < node->mNumChildren; ++i)
    _loadAssimpNode(scene, node->mChildren[i]);
}

gle::Skeleton* gle::UniversalLoader::_loadAssimpSkeleton(const aiScene* scene, aiMesh* mesh)
{
  if (mesh->mNumBones > 0)
    {
      gle::Skeleton* skeleton = new gle::Skeleton();
      skeleton->setName(std::string(mesh->mName.data) + "_skeleton");

      std::map<std::string, gle::Bone*> bones;

      for (GLuint i = 0; i < mesh->mNumBones; ++i)
	{
	  aiBone* bone = mesh->mBones[i];
	  gle::Bone* gleBone = new gle::Bone(1);
	  gleBone->setName(bone->mName.data);

	  // aiMatrix4x4& m = bone->mOffsetMatrix;
	  // gleBone
	  //   ->setCustomTransformationMatrix(
	  // 				    gle::Matrix4f(m.a1, m.a2, m.a3, m.a4,
	  // 						  m.b1, m.b2, m.b3, m.b4,
	  // 						  m.c1, m.c2, m.c3, m.c4,
	  // 						  m.d1, m.d2, m.d3, m.d4)
	  // 				    );

	  bones[gleBone->getName()] = gleBone;
	}
      
      for (std::pair<std::string, gle::Bone*> bone : bones)
	{
	  if (bone.second)
	    {
	      aiNode* node = scene->mRootNode->FindNode(bone.first.c_str());
	      if (node)
		{
		  aiMatrix4x4& m = node->mTransformation;
		  bone.second
		    ->setCustomTransformationMatrix(
		  				    gle::Matrix4f(m.a1, m.a2, m.a3, m.a4,
		  						  m.b1, m.b2, m.b3, m.b4,
		  						  m.c1, m.c2, m.c3, m.c4,
		  						  m.d1, m.d2, m.d3, m.d4)
		  				    );


		  for (GLuint i = 0; i < node->mNumChildren; ++i)
		    {
		      aiNode* child = node->mChildren[i];
		      gle::Bone* childBone = bones[child->mName.data];
		      if (childBone)
		        bone.second->addChild(childBone);
		    }
		  aiNode* parent = node->mParent;
		  if (parent)
		    {
		      gle::Bone* parentBone = bones[parent->mName.data];
		      if (parentBone)
			parentBone->addChild(bone.second);
		    }
		}
	    }
	}

      for (std::pair<std::string, gle::Bone*> bone : bones)
	{
	  if (bone.second)
	    skeleton->addChild(bone.second);
	}

      return (skeleton);
    }
  else
    return (NULL);
}

void gle::UniversalLoader::setTexturesPath(const std::string& texturesPath)
{
  _texturesPath = texturesPath;
}
