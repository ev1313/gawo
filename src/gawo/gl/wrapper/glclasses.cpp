#include <gawo/gl/wrapper/glclasses.hpp>
#include <gawo/io/log.hpp>

static io::Logger &logger = io::logging().get_logger("gawo.gl");

VAO::VAO() {}

VAO::~VAO() { glDeleteVertexArrays(1, &m_name); }

void VAO::init() { glGenVertexArrays(1, &m_name); }

void VAO::bind() { glBindVertexArray(m_name); }

void VAO::unbind() { glBindVertexArray(0); }

void VAO::fill(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) {
  glEnableVertexAttribArray(index);
  glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

Texture::Texture() {}

Texture::~Texture() { glDeleteTextures(1, &m_name); }

void Texture::init() { glGenTextures(1, &m_name); }

void Texture::bind() { glBindTexture(GL_TEXTURE_2D, m_name); }

void Texture::activate(GLenum slot) {
  glActiveTexture(slot);
  bind();
}

void Texture::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

void Texture::fill(GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format,
                   GLenum type, const GLvoid *data) {
  bind();
  glTexImage2D(GL_TEXTURE_2D, level, internalFormat, width, height, border, format, type, data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

GLuint Texture::getName() { return m_name; }

Framebuffer::Framebuffer() {}

Framebuffer::~Framebuffer() { glDeleteFramebuffers(1, &m_name); }

void Framebuffer::init() { glGenFramebuffers(1, &m_name); }

void Framebuffer::bind() { glBindFramebuffer(GL_FRAMEBUFFER, m_name); }

void Framebuffer::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void Framebuffer::attachRenderbuffer(GLenum attachment, GLuint rbo) {
  bind();
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rbo);
}

void Framebuffer::attachTexture(GLenum attachment, GLuint tex) {
  bind();
  glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, tex, 0);
}

bool Framebuffer::check() { return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE; }

Renderbuffer::Renderbuffer() {}

Renderbuffer::~Renderbuffer() { glDeleteRenderbuffers(1, &m_name); }

void Renderbuffer::init() { glGenRenderbuffers(1, &m_name); }

void Renderbuffer::bind() { glBindRenderbuffer(GL_RENDERBUFFER, m_name); }

void Renderbuffer::unbind() { glBindRenderbuffer(GL_RENDERBUFFER, 0); }

void Renderbuffer::create(GLenum internalformat, GLsizei width, GLsizei height) {
  bind();
  glRenderbufferStorage(GL_RENDERBUFFER, internalformat, width, height);
}

GLuint Renderbuffer::getName() { return m_name; }

RenderToTexture::RenderToTexture(GLenum attachment) {
  m_tex = Texture();
  m_attachment = attachment;
}

void RenderToTexture::init(GLint w, GLint h) {
  m_tex.init();
  m_tex.fill(0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
  m_rbo.init();
  m_rbo.bind();
  m_rbo.create(GL_DEPTH_COMPONENT, w, h);
  m_fbo.init();
  m_fbo.bind();
  m_fbo.attachRenderbuffer(GL_DEPTH_ATTACHMENT, m_rbo.getName());
  m_fbo.attachTexture(m_attachment, m_tex.getName());
}

void RenderToTexture::bind() { m_fbo.bind(); }

void RenderToTexture::unbind() { m_fbo.unbind(); }

GLuint RenderToTexture::getTexture() { return m_tex.getName(); }

void printGlError(GLenum err) {
  if (err != GL_NO_ERROR) {
    logger.log(io::LOG_ERROR) << "opengl error is: " << stringFromGlError(err) << std::endl;
  }
}

void discardLastGlError(bool print) {
  GLenum err = glGetError();
  if (print) {
    printGlError(err);
  }
}

// return false if there's an error
bool checkAndPrintGlError() {
  GLenum err = glGetError();
  
  printGlError(err);
  
  return true;
}

const char *stringFromGlError(GLenum err) {
  switch (err) {
    case GL_INVALID_ENUM:
      return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE:
      return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION:
      return "GL_INVALID_OPERATION";
    case GL_OUT_OF_MEMORY:
      return "GL_OUT_OF_MEMORY";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      return "GL_INVALID_FRAMEBUFFER_OPERATION";
    default:
      return "<unknown>";
  }
}
