#include <gawo/io/log.hpp>
#include <gawo/gl/wrapper/glclasses.hpp>
#include "gawo/gl/wrapper/shader.hpp"

static io::Logger &logger = io::logging().get_logger("gawo.gl");

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
    logger.log(io::LOG_ERROR) << "getting error log:" << std::endl;
    char *error = (char *) malloc((size_t) (len + 1));
    glGetProgramInfoLog(m_program, len, NULL, error);
    std::string str(error);
    std::cout << str << std::endl;
  }
  
  return (bool) result;
}

bool Shader::checkShader(GLuint shader) {
  GLint len = 0;
  GLint result = 0;
  
  glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
  
  if (result == GL_FALSE) {
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    char *error = (char *) malloc((size_t) (len + 1));
    glGetShaderInfoLog(shader, len, NULL, error);
    std::string str(error, error + len);
    std::cout << str << std::endl;
  }
  
  return result != GL_FALSE;
}

void Shader::bind() {
  if (m_program == 0) {
    logger.log(io::LOG_ERROR) << "error: invalid to bind invalid program (0)! " << std::endl;
    return;
  }
  
  glUseProgram(m_program);
}

GLuint Shader::program() { return m_program; }

void Shader::unbind() { glUseProgram(0); }

bool Shader::load(const std::__cxx11::string &data, GLenum shadertype) {
  if (m_program == 0) {
    logger.log(io::LOG_ERROR) << "[shader] error: shader program is invalid (0)!" << std::endl;
    return false;
  }
  
  GLuint shader = glCreateShader(shadertype);
  
  const char *shaderdata = data.c_str();
  glShaderSource(shader, 1, &shaderdata, NULL);
  glCompileShader(shader);
  checkShader(shader);
  
  glAttachShader(m_program, shader);
  glLinkProgram(m_program);
  check();
  
  glDeleteShader(shader);
  
  return true;
}

bool Shader::loadFile(const std::__cxx11::string &path, GLenum shadertype) {
  std::string content;
  std::ifstream fileStream(path, std::ios_base::in);
  
  if (!fileStream.is_open()) {
    logger.log(io::LOG_ERROR) << "Could not read file " << path << ". File does not exist." << std::endl;
    return false;
  }
  
  std::string line = "";
  while (!fileStream.eof()) {
    getline(fileStream, line);
    content.append(line + "\n");
  }
  
  fileStream.close();
  load(content, shadertype);
  logger.log(io::LOG_INFO) << "successfully loaded shader " << path << std::endl;
  return true;
}

GLint Shader::location(const std::__cxx11::string &name) { return glGetUniformLocation(m_program, name.c_str()); }