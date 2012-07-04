//
// FrameBuffer.hpp for  in /home/jochau_g//dev/gl-engine-42
// 
// Made by gael jochaud-du-plessix
// Login   <jochau_g@epitech.net>
// 
// Started on  Thu May 31 17:20:05 2012 gael jochaud-du-plessix
// Last update Wed Jul  4 16:24:42 2012 gael jochaud-du-plessix
//

#ifndef _GLE_FRAME_BUFFER_HPP_
# define _GLE_FRAME_BUFFER_HPP_

# include <gle/opengl.h>
# include <Texture.hpp>
# include <RenderBuffer.hpp>

namespace gle {

  //! Class to handle a buffer that can be used as a rendering destination
  /*!
    FrameBuffers are objects that can handle textures or render buffer as destination
    for rendering.
    A framebuffer is valid for rendering if its functions isComplete() returns true.
   */

  class FrameBuffer {
  public:

    //! Attachment types for rendering destinations
    /*!
      Specify the attachment point on wich the destination is attached.
     */

    enum Attachment {
      AttachmentColor = GL_COLOR_ATTACHMENT0,
      /*! Color destination */
      AttachmentDepth = GL_DEPTH_ATTACHMENT,
      /*! Depth destination */
      AttachmentStencil = GL_STENCIL_ATTACHMENT
      /*! Stencil destination */
    };

    //! Static function that return a reference to the default framebuffer
    /*!
      The default framebuffer comes from the window manager, so it's the default
      rendering destination.
     */

    static FrameBuffer& getDefaultFrameBuffer();

    //! Construct a framebuffer
    /*!
      \param id Allows to specify the id of an existing OpenGL FrameBuffer object
     */

    FrameBuffer(GLint id=-1);

    //! Destruct the framebuffer

    virtual ~FrameBuffer();

    //! Update the framebuffer
    /*!
      Framebuffer are updated after rendering operations.
      This function can be overwrited to perform operations
      on attached textures.
     */

    virtual void update();

    //! Bind the framebuffer
    /*!
      Bind the framebuffer as the current destination for rendering operations
     */

    void bind() const;

    //! Attach a texture to the framebuffer
    /*!
      \param texture The texture to be attached
      \param attachment The attachment point
     */

    void attach(gle::Texture const& texture, Attachment attachment);

    //! Detach a texture from the framebuffer
    /*!
      \param texture The texture to be detached
      \param attachment The attachment point
     */

    void detach(gle::Texture const& texture, Attachment attachment);

    //! Attach a render buffer to the framebuffer
    /*!
      \param renderBuffer The render buffer to be attached
      \param attachment The attachment point
     */

    void attach(gle::RenderBuffer const& renderBuffer, Attachment attachment);

    //! Detach a render buffer from the framebuffer
    /*!
      \param renderBuffer The render buffer to be detached
      \param attachment The attachment point
     */

    void detach(gle::RenderBuffer const& renderBuffer, Attachment attachment);

    //! Returns the current OpenGL status of the framebuffer

    GLenum	getStatus() const;

    //! Indicates whether or not the frame buffer is complete for rendering

    bool	isComplete() const;

    //! Returns the OpenGL ID of the frame buffer
    
    GLuint	getId() const;

  private:
    GLuint	_id;
  };

}

#endif
