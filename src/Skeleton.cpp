//
// Skeleton.cpp for glEngine in /home/michar_l//gl-engine-42/src
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Tue Jun 26 11:44:35 2012 loick michard
// Last update Tue Jun 26 11:52:35 2012 loick michard
//

#include <Skeleton.hpp>

gle::Skeleton::Skeleton() :
  Scene::Node(Scene::Node::Skeleton),
  _id(-1)
{

}

gle::Skeleton::~Skeleton()
{

}

void        gle::Skeleton::setId(GLint id)
{
  _id = id;
}

GLint       gle::Skeleton::getId() const
{
  return (_id);
}

