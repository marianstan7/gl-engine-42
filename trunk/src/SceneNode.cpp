//
// SceneNode.cpp for glengine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Tue May 15 19:32:41 2012 loick michard
// Last update Thu Jul  5 22:43:44 2012 loick michard
//

#include <algorithm>
#include <Scene.hpp>

gle::Scene::Node::Node(gle::Scene::Node::Type type) :
  _type(type), _parent(NULL), _isDynamic(false), _projectShadow(true),
  _hasTarget(false), _addedNodes(0), _needUpdateMatrix(true)
{

}

gle::Scene::Node::Node(const gle::Scene::Node& other) :
  _type(other._type), _name(other._name), 
  _children(), _parent(NULL), _position(other._position),
  _isDynamic(other._isDynamic), _projectShadow(other._projectShadow),
  _target(other._target), _hasTarget(other._hasTarget),
  _scaleMatrix(other._scaleMatrix), _rotationMatrix(other._rotationMatrix),
  _customTransformationMatrix(other._customTransformationMatrix),
  _addedNodes(other._addedNodes), _needUpdateMatrix(other._needUpdateMatrix)
{
  for (gle::Scene::Node* const &child : other._children)
    {
      Node* newChild = child->duplicate();
      _children.push_back(newChild);
      newChild->setParent(this);
      _addedNodes |= newChild->getRecursiveType();
    }
  this->setAddedNodes(_addedNodes);
}

gle::Scene::Node::~Node()
{
}

gle::Scene::Node::Type gle::Scene::Node::getType() const
{
  return (_type);
}

const std::string& gle::Scene::Node::getName() const
{
  return (_name);
}

void gle::Scene::Node::setName(const std::string& name)
{
  _name = name;
}

void gle::Scene::Node::addChild(gle::Scene::Node* child)
{
  if (find(_children.begin(), _children.end(), child) == _children.end())
    {
      _children.push_back(child);
      this->setAddedNodes(_addedNodes | child->getRecursiveType());
    }
  child->setParent(this);
}

void gle::Scene::Node::removeChild(gle::Scene::Node* child)
{
  for (auto _child = _children.begin(); _child != _children.end(); ++_child)
    if (*_child == child)
      {
	this->setAddedNodes(_addedNodes | child->getRecursiveType());
	_children.erase(_child);
	return;
      }
  for (gle::Scene::Node* &_child : _children)
    _child->removeChild(child);
}

gle::Scene::Node*	gle::Scene::Node::getParent() const
{
  return (_parent);
}

const std::vector<gle::Scene::Node*>& gle::Scene::Node::getChildren() const
{
  return (_children);
}

int	gle::Scene::Node::getChildrenByName(std::string const & name,
					    std::vector<Node*> & vector)
{
  int     nb = 0;

  if (_name.find(name) != std::string::npos)
    {
      vector.push_back(this);
      ++nb;
    }
  for (gle::Scene::Node* &child : _children)
    nb += child->getChildrenByName(name, vector);
  return (nb);
}

gle::Scene::Node* gle::Scene::Node::getChildByName(std::string const& name)
{
  for (gle::Scene::Node* &child : _children)
    if (child->_name.find(name) != std::string::npos)
      return (child);
  for (gle::Scene::Node* &child : _children)
    {
      gle::Scene::Node* found = child->getChildByName(name);
      if (found)
	return (found);
    }
  return (NULL);
}

void gle::Scene::Node::setParent(gle::Scene::Node* parent)
{
  _parent = parent;
  setRecursiveNeedMatrixUpdate();
}

void gle::Scene::Node::updateMatrix()
{
  if (_parent)
    {
      if (_parent->_needUpdateMatrix)
	_parent->updateMatrix();
      _transformationMatrix = _parent->_transformationMatrix;
    }
  else
    _transformationMatrix.identity();
  
  _cameraTransformationMatrix = _transformationMatrix;
  _transformationMatrix.translate(_position);
  if (_hasTarget)
    _transformationMatrix.lookAt(_position,
				 _target,
				 Vector3<GLfloat>(0, 1, 0));
  _transformationMatrix *= _rotationMatrix;
  _transformationMatrix *= _scaleMatrix;
  _transformationMatrix *= _customTransformationMatrix;
  
  if (_type == Camera)
    {
      if (_hasTarget)
	{
	  _cameraTransformationMatrix *= gle::Matrix4<GLfloat>::cameraLookAt(_position,
									     _target,
									     Vector3<GLfloat>(0, 1, 0));
	  _cameraTransformationMatrix.translate(-_position.x, -_position.y, -_position.z);
	}
      _cameraTransformationMatrix *= _rotationMatrix;
      _cameraTransformationMatrix *= _scaleMatrix;
    }
  
  _absolutePosition.x = _absolutePosition.y = _absolutePosition.z = 0;
  _absolutePosition *= _transformationMatrix;
  
  Matrix4<GLfloat> inverse(_transformationMatrix);
  inverse.inverse();
  _normalMatrix = inverse;
  _normalMatrix.transpose();
  
  setRecursiveNeedMatrixUpdate();
  _needUpdateMatrix = false;
  this->update();
}

void gle::Scene::Node::setRecursiveNeedMatrixUpdate()
{
  _needUpdateMatrix = true;
  for (gle::Scene::Node* &child : _children)
    child->setRecursiveNeedMatrixUpdate();
}

void gle::Scene::Node::setMatrices(gle::Matrix4<GLfloat> &transformationMatrix,
				   gle::Matrix3<GLfloat> &normalMatrix)
{
  _transformationMatrix = transformationMatrix;
  _normalMatrix = normalMatrix;
  _needUpdateMatrix = false;
}

const gle::Matrix4<GLfloat>& gle::Scene::Node::getTransformationMatrix()
{
  if (_needUpdateMatrix)
    {
      this->updateMatrix();
      _needUpdateMatrix = false;
    }
  if (_type == Camera)
    return (_cameraTransformationMatrix);
  return (_transformationMatrix);
}

const gle::Matrix3<GLfloat>& gle::Scene::Node::getNormalMatrix()
{
  if (_needUpdateMatrix)
    {
      this->updateMatrix();
      _needUpdateMatrix = false;
    }
  return (_normalMatrix);
}

void gle::Scene::Node::setPosition(const gle::Vector3<GLfloat>& pos)
{
  _position = pos;
  _needUpdateMatrix = true;
}

void gle::Scene::Node::setRotation(const gle::Quaternion<GLfloat>& rotation)
{
  _rotationMatrix = rotation.getMatrix();
  _needUpdateMatrix = true;
}

void gle::Scene::Node::setTarget(const gle::Vector3<GLfloat>& target)
{
  _target = target;
  _hasTarget = true;
  _needUpdateMatrix = true;
}

void gle::Scene::Node::setScale(GLfloat scale)
{
  this->setScale(scale, scale, scale);
  _needUpdateMatrix = true;
}

void gle::Scene::Node::setScale(GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ)
{
  _scaleMatrix = gle::Matrix4<GLfloat>::scale(scaleX, scaleY, scaleZ);
  _needUpdateMatrix = true;
}

const gle::Vector3<GLfloat>& gle::Scene::Node::getPosition() const
{
  return (_position);
}

const gle::Vector3<GLfloat>& gle::Scene::Node::getAbsolutePosition()
{
  if (_needUpdateMatrix)
    {
      this->updateMatrix();
      _needUpdateMatrix = false;
    }  
  return (_absolutePosition);
}

bool      gle::Scene::Node::isDynamic() const
{
  return (!(!_isDynamic && (!_parent || !_parent->isDynamic())));
}

void      gle::Scene::Node::setDynamic(bool dynamic, bool deep)
{
  _isDynamic = dynamic;
  if (deep)
    for (gle::Scene::Node* child : _children)
      child->setDynamic(dynamic, true);
}

bool		gle::Scene::Node::projectShadow() const
{
  return (_projectShadow);
}

void		gle::Scene::Node::setProjectShadow(bool projectShadow)
{
  _projectShadow = projectShadow;
}

const gle::Vector3<GLfloat>& gle::Scene::Node::getTarget() const
{
  return (_target);
}

void gle::Scene::Node::translate(const gle::Vector3<GLfloat>& vec)
{
  _position += vec;
  _target += vec;
  _needUpdateMatrix = true;
}

void gle::Scene::Node::rotate(const gle::Quaternion<GLfloat>& rotation)
{
  _rotationMatrix *= rotation.getMatrix();
  _needUpdateMatrix = true;
}

void	gle::Scene::Node::setCustomTransformationMatrix(const Matrix4f& matrix)
{
  _customTransformationMatrix = matrix;
  _needUpdateMatrix = true;
}

gle::Scene::Node* gle::Scene::Node::duplicate() const
{
  return (new Node(*this));
}

void gle::Scene::Node::update()
{

}

std::vector<gle::Scene::Node*>& gle::Scene::Node::getDebugNodes(int mode)
{
  (void)mode;
  return (_debugNodes);
}

int gle::Scene::Node::getAddedNodes() const
{
  return (_addedNodes);
}

void gle::Scene::Node::setAddedNodes(int addedNodes)
{
  _addedNodes = addedNodes;
  if (_parent)
    _parent->setAddedNodes(_parent->_addedNodes | _addedNodes);
}

void gle::Scene::Node::setAddedNodesRecursive(int addedNodes)
{
  _addedNodes = addedNodes;
  for (Node* const& child : _children)
    child->setAddedNodesRecursive(_addedNodes);
}

int gle::Scene::Node::getRecursiveType() const
{
  int type = _type;

  for (Node* const &child : _children)
    type |= child->getRecursiveType();
  return (type);
}
