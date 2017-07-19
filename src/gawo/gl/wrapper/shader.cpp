#include <gawo/io/log.hpp>
#include <gawo/gl/wrapper/glclasses.hpp>
#include <spp/spp.hpp>
#include "gawo/gl/wrapper/shader.hpp"

static io::Logger& logger = io::logging().get_logger("gawo.gl");

Shader::Shader()
  : m_program(0) {
  
  
}

Shader::~Shader() {}

void Shader::init() { m_program = glCreateProgram(); }

bool Shader::check() {
  GLint len = 0;
  GLint result = 0;
  
  glGetProgramiv(m_program, GL_LINK_STATUS, &result);
  glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &len);
  
  if (result == GL_FALSE) {
    logger.log(io::LOG_ERROR) << "getting error log:" << std::endl;
    char* error = (char*) malloc((size_t) (len + 1));
    glGetProgramInfoLog(m_program, len, NULL, error);
    std::string str(error);
    logger.log(io::LOG_ERROR) << str << std::endl;
  }
  
  return (bool) result;
}

bool Shader::checkShader(GLuint shader) {
  GLint len = 0;
  GLint result = 0;
  
  glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
  
  if (result == GL_FALSE) {
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    std::string str;
    str.reserve(len);
    glGetShaderInfoLog(shader, len, NULL, &str[0]);
    logger.log(io::LOG_ERROR) << str << std::endl;
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

bool Shader::loadCompileLink(const std::string& data, GLenum shadertype) {
  if (m_program == 0) {
    logger.log(io::LOG_ERROR) << "shader program is invalid!" << std::endl;
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
  std::ifstream file(path, std::ios_base::in);
  
  if (file) {
    std::stringstream data;
    //may not be the fastest method, but i think this shouldn't be a big bottleneck
    data << file.rdbuf();
    
    spp::ParserContext ctx(data);
    spp::Library lib;
    spp::EvaluationContext ectx;
    std::unique_ptr <spp::Program> prog(ctx.parse());
    
    std::ostringstream out;
    prog->evaluate(out, ectx);
    
    logger.log(io::LOG_INFO) << "successfully loaded shader " << path << std::endl;
    
    file.close();
  } else {
    logger.log(io::LOG_ERROR) << "Could not read file " << path << ". File does not exist." << std::endl;
    return false;
  }
  return true;
}

bool Shader::loadFileCompileLink(const std::string& path, unsigned int shadertype) {
  bool result = loadFile(path, shadertype);
  if(result) {
    return compileLink();
  } else { return result; }
}

GLint Shader::location(const std::string& name) { return glGetUniformLocation(m_program, name.c_str()); }

bool Shader::compileLink() {
  return false;
}

bool Shader::load(const std::string& data, unsigned int shadertype) {
  return false;
}
