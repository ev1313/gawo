#include <gawo/glclasses.hpp>

VAO::VAO() {}
VAO::~VAO() { glDeleteVertexArrays(1, &m_name); }

void VAO::init() { glGenVertexArrays(1, &m_name); }
void VAO::bind() { glBindVertexArray(m_name); }
void VAO::unbind() { glBindVertexArray(0); }

void VAO::fill(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer) {
  glEnableVertexAttribArray(index);
  glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

Shader::Shader()
    : m_program(0) {}

Shader::~Shader() {}

void Shader::init() { m_program = glCreateProgram(); }

bool Shader::check() {
  GLint len = 0;
  GLint result = 0;

  glGetProgramiv(m_program, GL_LINK_STATUS, &result);
  glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &len);

  if (result == GL_FALSE) {
    std::cout << "getting error log:" << std::endl;
    char* error = (char*)malloc(len + 1);
    glGetProgramInfoLog(m_program, len, NULL, error);
    std::string str(error);
    std::cout << str << std::endl;
  }

  return (bool)result;
}

bool Shader::checkShader(GLuint shader) {
  GLint len = 0;
  GLint result = 0;

  glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

  if (result == GL_FALSE) {
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    char* error = (char*)malloc(len + 1);
    glGetShaderInfoLog(shader, len, NULL, error);
    std::string str(error, error + len);
    std::cout << str << std::endl;
  }

  return result != GL_FALSE;
}

void Shader::bind() {
  if (m_program == 0) {
    std::cerr << "error: invalid to bind invalid program (0)! " << std::endl;
    return;
  }

  glUseProgram(m_program);
}

GLuint Shader::program() { return m_program; }
void Shader::unbind() { glUseProgram(0); }

bool Shader::load(const std::string& data, GLenum shadertype) {
  if (m_program == 0) {
    std::cout << "[shader] error: shader program is invalid (0)!" << std::endl;
    return false;
  }

  GLuint shader = glCreateShader(shadertype);

  const char* shaderdata = data.c_str();
  glShaderSource(shader, 1, &shaderdata, NULL);
  glCompileShader(shader);
  checkShader(shader);

  glAttachShader(m_program, shader);
  glLinkProgram(m_program);
  check();

  glDeleteShader(shader);

  return true;
}

bool Shader::loadFile(const std::string& path, GLenum shadertype) {
  std::string content;
  std::ifstream fileStream(path, std::ios::in);

  if (!fileStream.is_open()) {
    std::cerr << "Could not read file " << path << ". File does not exist." << std::endl;
    return false;
  }

  std::string line = "";
  while (!fileStream.eof()) {
    std::getline(fileStream, line);
    content.append(line + "\n");
  }

  fileStream.close();
  load(content, shadertype);
  std::cout << "successfully loaded shader " << path << std::endl;
  return true;
}

GLuint Shader::location(const std::string& name) { return glGetUniformLocation(m_program, name.c_str()); }

Texture::Texture() {}

Texture::~Texture() { glDeleteTextures(1, &m_name); }

void Texture::init() { glGenTextures(1, &m_name); }

void Texture::bind() { glBindTexture(GL_TEXTURE_2D, m_name); }

void Texture::activate(GLenum slot) {
  glActiveTexture(slot);
  bind();
}

void Texture::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

void Texture::fill(GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* data) {
  bind();
  glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, 1024, 1024, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
  //glTexImage2D(GL_TEXTURE_2D, level, internalFormat, width, height, border, format, type, data);
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

bool Framebuffer::check() {
  return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

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
  m_attachment = attachment; }

void RenderToTexture::init(GLint w, GLint h) {
  m_tex.init();
  m_tex.fill(0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
  m_rbo.init();
  m_rbo.bind();
  m_rbo.create(GL_DEPTH_COMPONENT, w, h);
  m_fbo.init();
  m_fbo.bind();
  m_fbo.attachTexture(m_attachment, m_tex.getName());
  m_fbo.attachRenderbuffer(GL_DEPTH_ATTACHMENT, m_rbo.getName());
}

void RenderToTexture::bind() {
  m_fbo.bind();
}

void RenderToTexture::unbind() {
  m_fbo.unbind();
}

GLuint RenderToTexture::getTexture() {
  return m_tex.getName();
}

void printGlError(GLenum err) {
  if (err != GL_NO_ERROR) {
    std::cout << "opengl error is: " << stringFromGlError(err) << std::endl;
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

const char* stringFromGlError(GLenum err) {
  switch (err) {
  case GL_INVALID_ENUM:
    return "GL_INVALID_ENUM";
    break;
  case GL_INVALID_VALUE:
    return "GL_INVALID_VALUE";
    break;
  case GL_INVALID_OPERATION:
    return "GL_INVALID_OPERATION";
    break;
  case GL_OUT_OF_MEMORY:
    return "GL_OUT_OF_MEMORY";
    break;
  case GL_INVALID_FRAMEBUFFER_OPERATION:
    return "GL_INVALID_FRAMEBUFFER_OPERATION";
    break;
  default:
    return "<unknown>";
    break;
  }
}
