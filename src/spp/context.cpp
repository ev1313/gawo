#include "spp/context.hpp"
#include "spp/library.hpp"

#include <iomanip>

namespace spp {

ParserContext::ParserContext(std::istream& in, const std::string& source_path)
  :
  m_in(in), m_source_path(source_path), m_scanner(*this, &m_in, nullptr), m_errors() {
  
}

ParserContext::~ParserContext() {

}

std::unique_ptr <Program> ParserContext::parse() {
  auto prog = std::make_unique <Program>(m_source_path);
  Parser parser(*this, *prog);
  if (parser.parse() != 0) {
    return nullptr;
  }
  return prog;
}

void EvaluationContext::define(const std::string& name, const std::string& rhs) {
  if(m_defines.count(name)) {
    throw std::invalid_argument("spp::EvaluationContext duplicate define " + name);
  }
  m_defines.insert(std::make_pair(name, rhs));
}

void EvaluationContext::define1ull(const std::string& name, const unsigned long long value) {
  define(name, std::to_string(value));
}

void EvaluationContext::define1ll(const std::string& name, const signed long long value) {
  define(name, std::to_string(value));
}

void EvaluationContext::define1f(const std::string& name, const float value) {
  std::ostringstream tmp;
  tmp << std::setprecision(10) << value;
  define(name, tmp.str());
}

void EvaluationContext::define1d(const std::string& name, const double value) {
  std::ostringstream tmp;
  tmp << std::setprecision(18) << value;
  define(name, tmp.str());
}

void EvaluationContext::undefine(const std::string& name) {
  m_defines.erase(name);
}

}
