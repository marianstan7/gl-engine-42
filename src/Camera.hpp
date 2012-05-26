//
// Camera.hpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Sat Feb 18 10:43:04 2012 loick michard
// Last update Fri May 25 19:55:00 2012 loick michard
//

#ifndef _GLE_CAMERA_HPP_
# define _GLE_CAMERA_HPP_

# include <gle/opengl.h>
# include <Vector3.hpp>
# include <Matrix4.hpp>
# include <Quaternion.hpp>
# include <Scene.hpp>

namespace gle {
  
  //! Abstract class for cameras

  class Camera : public Scene::Node {
  public:

    //! Constructor of Camera
    /*!
      Initializes a camera.
      \param position Position of camera
      \param target Target of camera
    */

    Camera(gle::Vector3<GLfloat> const& position,
	   gle::Vector3<GLfloat> const& target);

    //! Destructor

    virtual ~Camera();

    //! Sets position of camera.
    /*!
      Update position and call updateMatrix()
      \param position New position
    */

    //virtual void setPosition(gle::Vector3<GLfloat> const& position);

    //! Sets target of camera.
    /*!
      Update target and call updateMatrix()
      \param target New target
    */
    
    //virtual void setTarget(gle::Vector3<GLfloat> const& target);

    //! Set the camera rotation
    /*!
      \param axis Axis of rotation
      \param angle Angle of rotation
    */

    //virtual void setRotation(gle::Vector3<GLfloat> const& axis, GLfloat angle);

    //! Translates camera.
    /*!
      Translate position and target and call updateMatrix()
      \param vec Vector of translation
    */

    //virtual void translate(gle::Vector3<GLfloat> const& vec);

    //! Returns camera position

    //gle::Vector3<GLfloat> const&	getPosition() const;

    //! Returns camera targer

    //gle::Vector3<GLfloat> const&	getTarget() const;

    //! Returns projection matrix

    gle::Matrix4<GLfloat>&		getProjectionMatrix();

    //void setProjectionTarget(const gle::Vector3<GLfloat>& target);

    //! Returns model view matrix

    //gle::Matrix4<GLfloat>&		getModelViewMatrix();

    //! Update projection matrix

    virtual void update();

    virtual void updateProjectionMatrix() = 0;

  protected:
    
    //! Projection matrix.

    gle::Matrix4<GLfloat> _projectionMatrix;

    //! Projection matrix.

    //gle::Matrix4<GLfloat> _modelViewMatrix;

    //! Camera position.

    //gle::Vector3<GLfloat> _position;

    //! Camera rotation

    //gle::Quaternion<GLfloat> _rotation;

    //! Camera target.

    gle::Vector3<GLfloat> _projectionTarget;
  };
}

#endif // _GLE_CAMERA_HPP_
