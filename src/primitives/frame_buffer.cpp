#include "iso-tasty/primitives/frame_buffer.h"

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#ifndef NO_GL
  #ifdef _WIN32
  #include <windows.h>
  #endif

  #include <GL/gl.h>
  #include <GL/glu.h>
#endif

Apsis::Primitives::FrameBuffer::FrameBuffer(unsigned int width, unsigned int height) :
  _width(width),
  _height(height) {

  glEnable(GL_TEXTURE_2D);

  // RGBA8 RenderBuffer
  glGenFramebuffers(1, &_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER_EXT, _fbo);

  // Create and attach a color buffer
  glGenTextures(1, &_color);
  glBindTexture(GL_TEXTURE_2D, _color);

  glTexImage2D(GL_TEXTURE_2D,   0, GL_RGBA,
               _width, _height, 0, GL_BGRA_EXT,
               GL_UNSIGNED_BYTE, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                         GL_TEXTURE_2D,      _color, 0);

  /*{
    glGenRenderbuffersEXTPtr(1, &_color_rb);

    // We must bind color_rb before we call glRenderbufferStorageEXT
    glBindRenderbufferEXTPtr(GL_RENDERBUFFER_EXT, _color_rb);

    // The storage format is RGBA8
    glRenderbufferStorageEXTPtr(GL_RENDERBUFFER_EXT, GL_RGBA8, cast(int)width, cast(int)height);

    // The following is for a multisample FBO.
    // This is not strictly necessary ever. But illustrated here for posterity:
    // glRenderbufferStorageMultisampleEXTPtr(GL_RENDERBUFFER_EXT, 4, GL_RGBA8, width, height);

    // Attach color buffer to FBO
    glFramebufferRenderbufferEXTPtr(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                                    GL_RENDERBUFFER_EXT,      _color_rb);
  }*/

  // Some video cards need a depth buffer with a stencil buffer.
  // Otherwise, a stencil buffer can just be produced with a simple
  //   GL_STENCIL_INDEX_8 renderbuffer.
  // XXX: should probably check for this extension before I use it.
  glGenRenderbuffers(1, &_packed);
  glBindRenderbuffer(GL_RENDERBUFFER_EXT, _packed);

  glRenderbufferStorage(GL_RENDERBUFFER_EXT, GL_DEPTH_STENCIL_EXT, _width, _height);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,   GL_RENDERBUFFER_EXT, _packed);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, _packed);

  // We should check to see if the FBO is supported... now, for some reason.
  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER_EXT);
  switch(status) {
    case GL_FRAMEBUFFER_COMPLETE_EXT:
    break;
  case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
    //OutputDebugWriteA("Framebuffer object format is unsupported by the video hardware. (GL_FRAMEBUFFER_UNSUPPORTED_EXT)(FBO - 820)");

    // Damn.
    // XXX: Use some hideous older technology
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
    //putln("Incomplete attachment. (GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT)(FBO - 820)");
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
    //putln("Incomplete missing attachment. (GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT)(FBO - 820)");
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
   // putln("Incomplete dimensions. (GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT)(FBO - 820)");
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
   // putln("Incomplete formats. (GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT)(FBO - 820)");
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
   // putln("Incomplete draw buffer. (GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT)(FBO - 820)");
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
    //putln("Incomplete read buffer. (GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT)(FBO - 820)");
    break;
  case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT:
    //putln("Incomplete multisample buffer. (GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT)(FBO - 820)");
    break;
  default:
    //Programming error; will fail on all hardware
   // putln("Some video driver error or programming error occured. Framebuffer object status is invalid. (FBO - 823)");
    break;
  }
}

Apsis::Primitives::FrameBuffer::~FrameBuffer() {
  if (_counter.isAlone()) {
  }
}

unsigned int Apsis::Primitives::FrameBuffer::identifier() const {
  return this->_fbo;
}