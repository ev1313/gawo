#pragma once

#include <epoxy/gl.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <spp/spp.hpp>
#include "spp/library.hpp"

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
  
  //checks the link status and prints an error, if sth. went wrong
  bool checkLinkStatus();
  //checks the compile status for the shader and prints an error, if sth. went wrong
  bool checkShaderCompileStatus(unsigned int shader);

protected:
public:
  Shader();
  
  ~Shader();
 
  //creates the program object
  void init();
  //binds the shader
  void bind();
  //unbinds the shader
  void unbind();

  // adds a define to the shader
  void define(std::string& name, std::string& data);
  // removes a define from the shader
  // TODO: extend the parser so the eval ctx gets the defines from a loaded shader.
  // currently the undefine just removes defines previously defined with the define method
  void undefine(std::string& name);
 
  // just loads the shader data to a spp lib
  bool load(const std::string& data, unsigned int shadertype);
 
  // compiles and links the shaders in the spp lib
  bool compileLink();
 
  // loads the shaders and directly compiles and links them
  bool loadCompileLink(const std::string& data, unsigned int shadertype);
 
  // loads the shader data from file and calls load(data, shadertype)
  bool loadFile(const std::string& path, unsigned int shadertype);
  
  // loads the shader data from file and compiles and links it
  bool loadFileCompileLink(const std::string& path, unsigned int shadertype);
 
  // glUniformLocation wrapper
  int location(const std::string& name);
  //returns the program id
  unsigned int program();
};
