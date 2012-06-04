//
// flycam.cpp for  in /home/jochau_g//dev/opengl/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Fri Mar  2 17:27:21 2012 gael jochaud-du-plessix
// Last update Thu May 31 16:00:34 2012 gael jochaud-du-plessix
//

#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>

#include <assimp/assimp.hpp>
#include <assimp/aiPostProcess.h>
#include <assimp/aiScene.h>

#include "Example.hpp"
#include "flycam.hpp"

#define W_WIDTH 1280
#define W_HEIGHT 720

#define W_FRAMERATE 60

class App : public Example {
public:
  App(int ac, char** av) :
    Example(ac, av, W_WIDTH, W_HEIGHT, W_FRAMERATE, "glEngine : Geometries"),
    _materials()
  { 
    flycam::camSpeed = 1000;
    //_cameraType = Trackball;
    //printGPUMemInfo();

    Assimp::Importer importer;

    std::cout << "Loading scene...\n";
    const aiScene* scene = importer.ReadFile( _argv[1],
					      aiProcess_CalcTangentSpace       | 
					      aiProcess_Triangulate            |
					      aiProcess_JoinIdenticalVertices  |
					      aiProcess_SortByPType// 	       | 
					      // aiProcess_GenSmoothNormals
					      );
    
    if (!scene)
      throw new gle::Exception::Exception("Cannot load the scene");

    std::cout << "Importing scene...\n";

    std::cout << "Embeded textures: " << scene->mNumTextures << std::endl;

    _materials.resize(scene->mNumMaterials);
    for (GLuint i = 0; i < scene->mNumMaterials; ++i)
      _materials[i] = loadAssimpMaterial(_argv[1], scene, scene->mMaterials[i]);

    _scene = new gle::Scene();
    _sceneNode = new gle::Scene::Node(gle::Scene::Node::Empty);
    loadAssimpNode(scene, scene->mRootNode);
    _sceneNode->setRotation(gle::Vector3f(1, 0, 0), -90);
    *_scene << _sceneNode;
  }

  gle::Material* loadAssimpMaterial(const std::string& file, const aiScene* scene, aiMaterial* material)
  {
    gle::Material* gleMaterial = new gle::Material();
    
    aiString name;
    material->Get<aiString>(AI_MATKEY_NAME, name);
    std::cout << "loading material: '" << name.data << "'" << std::endl;

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
	    size_t slashPos = file.find_last_of('/');
	    if (slashPos != std::string::npos)
	      texture = file.substr(0, slashPos + 1) + texture;
	    try 
	      {
		gleMaterial->setColorMap(new gle::Texture(texture));
	      }
	    catch (std::exception* e)
	      {
		std::cout << "Cannot load texture '" << texture << "'" << std::endl;
	      }
	  }
      }
    
    return (gleMaterial);
  }

  void loadAssimpNode(const aiScene* scene, aiNode* node)
  {
    //std::cout << "loading node: '" << node->mName.data << "'" << std::endl;

    gle::Scene::Node* gleNode = new gle::Scene::Node(gle::Scene::Node::Empty);
    gleNode->setName(node->mName.data);

    for (GLuint i = 0; i < node->mNumMeshes; ++i)
      {
	aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

	if (!(mesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE))
	  continue ;

	// std::cout << "loading mesh: '" << mesh->mName.data
	// 	  << "', " << mesh->mNumFaces << " faces"
	// 	  << std::endl;

	//std::cout << "used material " << _materials[mesh->mMaterialIndex]->getName() << std::endl;

	gle::Mesh* gleMesh = new gle::Mesh(_materials[mesh->mMaterialIndex]);
	gleMesh->setName(mesh->mName.data);

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
		    vertexAttributes[v * gle::Mesh::VertexAttributesSize + 3] = mesh->mNormals[v].x;
		    vertexAttributes[v * gle::Mesh::VertexAttributesSize + 4] = mesh->mNormals[v].y;
		    vertexAttributes[v * gle::Mesh::VertexAttributesSize + 5] = mesh->mNormals[v].z;
		  }
		if (mesh->mTextureCoords[0])
		  {
		    vertexAttributes[v * gle::Mesh::VertexAttributesSize + 6] = mesh->mTextureCoords[0][v].x;
		    vertexAttributes[v * gle::Mesh::VertexAttributesSize + 7] = mesh->mTextureCoords[0][v].y;
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

    _sceneNode->addChild(gleNode);
    
    for (GLuint i = 0; i < node->mNumChildren; ++i)
      loadAssimpNode(scene, node->mChildren[i]);
  }

  void initScene()
  {
    _camera = new gle::PerspectiveCamera(gle::Vector3f(-20, 10, 0),
					 gle::Vector3f(0, 0, 0),
					 45, (GLfloat)_winWidth/_winHeight, 1, 10000);
    _renderer = new gle::Renderer();

    gle::PointLight *light = new gle::PointLight(gle::Vector3<GLfloat>(0, 0, 0),
                                                 gle::Color<GLfloat>(1.0, 1.0, 1.0));

    _scene->setFogDensity(0.0001);

    _camera->addChild(light);


    *_scene << _camera;

    _scene->updateScene();
  }

  void animate()
  {
    _scene->updateLights();
  }

private:
  std::vector<gle::Material*> _materials;
  gle::Scene::Node* _sceneNode;
};

int main(int ac, char **av)
{
  try {
    App app(ac, av);
    return (app.run());
  }
  catch (gle::Exception::CompilationError* e)
    {
      std::cout << "Compilation error in " << e->getFilename() << ":\n"
		<< e->what() << std::endl;
    }
  catch (std::exception *e)
    {
      std::cout << "Error: " << e->what() << std::endl;
    }
  return (EXIT_FAILURE);
}
