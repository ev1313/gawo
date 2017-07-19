#pragma once

#include <epoxy/gl.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <spp/spp.hpp>

/*
 *
 * shader wrapper with the ability of adding defines before compiling using the shader preprocessor.
 *
 * */
class Shader {
private:
  unsigned int m_program;
 
  spp::Library m_lib;
  spp::EvaluationContext m_evalctx;
  
  bool check();
  
  bool checkShader(unsigned int shader);

protected:
public:
  Shader();
  
  ~Shader();
  
  void init();
  
  void bind();
  
  void unbind();
 
  bool load(const std::string& data, unsigned int shadertype);
  
  bool compileLink();
  
  bool loadCompileLink(const std::string& data, unsigned int shadertype);
  
  bool loadFile(const std::string& path, unsigned int shadertype);
  
  bool loadFileCompileLink(const std::string& path, unsigned int shadertype);
  
  int location(const std::string& name);
  
  unsigned int program();
};
