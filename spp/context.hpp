#pragma once

#include <unordered_map>
#include <set>
#include <string>
#include <map>

#include "spp/lexer.hpp"
#include "spp/ast.hpp"

/**
 * This namespace holds the Shader Preprocessor interface and implementation.
 */
namespace spp {

class location;

/**
 * Context for a Shader Preprocessor parser.
 */
class ParserContext {
public:
  ParserContext(std::istream &in, const std::string &source_path = "<memory>");
  
  virtual ~ParserContext();

private:
  std::istream &m_in;
  std::string m_source_path;
  
  Scanner m_scanner;

protected:
  std::vector <std::tuple <location, std::string> > m_errors;

public:
  inline Scanner &lexer() {
    return m_scanner;
  }
  
  inline const Scanner &lexer() const {
    return m_scanner;
  }
  
  std::unique_ptr <Program> parse();
  
};


class EvaluationContext {
private:
  std::map<std::string, std::string> m_defines;

public:
  void define(const std::string &name, const std::string &rhs);
  
  void define1ull(const std::string &name, const unsigned long long value);
  
  void define1ll(const std::string &name, const signed long long value);
  
  void define1f(const std::string &name, const float value);
  
  void define1d(const std::string &name, const double value);
  
  void undefine(const std::string& name);
  
  inline const std::map <std::string, std::string> &defines() const {
    return m_defines;
  }
};


}
