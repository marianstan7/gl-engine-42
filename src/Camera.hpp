//
// Camera.hpp for gle in /home/michar_l//glEngine
// 
// Made by loick michard
// Login   <michar_l@epitech.net>
// 
// Started on  Sat Feb 18 10:43:04 2012 loick michard
// Last update Tue Jun 26 09:47:27 2012 gael jochaud-du-plessix
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

    //! Returns the projection matrix associated with the camera

    const gle::Matrix4<GLfloat>&	getProjectionMatrix() const;

    //! Update the camera

    virtual void update();
    
    //! Update the projection matrix associated with the camera

    virtual void updateProjectionMatrix() = 0;

  protected:
    
    //! Projection matrix.

    gle::Matrix4<GLfloat> _projectionMatrix;
  };
}

#endif // _GLE_CAMERA_HPP_
