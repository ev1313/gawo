#pragma once

#include <epoxy/gl.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

class Shader {
private:
  unsigned int m_program;
  
  bool check();
  
  bool checkShader(unsigned int shader);

protected:
public:
  Shader();
  
  ~Shader();
  
  void init(); // call to init when opengl context exists
  
  void bind();
  
  void unbind();
  
  bool load(const std::__cxx11::string &data, unsigned int shadertype);
  
  bool loadFile(const std::__cxx11::string &path, unsigned int shadertype);
  
  int location(const std::__cxx11::string &name);
  
  unsigned int program();
};
