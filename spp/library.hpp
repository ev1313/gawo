#pragma once

#include <istream>
#include <memory>
#include <string>
#include <unordered_map>
#include <map>
#include "spp/library.hpp"

namespace spp {

class Loader {
public:
  virtual ~Loader();

public:
  virtual std::unique_ptr <std::istream> open(const std::string &path) = 0;
  
};


class DefaultLoader : public Loader {
private:
  std::unordered_map <std::string, std::unique_ptr<std::istream>> m_files;

public:
  void add_data(const std::string& path, const std::string& data);
  void add_source(const std::string &path, const std::string &source);
  std::unique_ptr <std::istream> open(const std::string &path) override;
};

class Library {
public:
  Library();
  
  explicit Library(std::unique_ptr <spp::Loader> &&loader);
  
  virtual ~Library();

protected:
  unsigned int m_max_include_depth;
  std::unique_ptr <spp::Loader> m_loader;
  std::unordered_map <std::string, std::unique_ptr <spp::Program> > m_cache;

protected:
  void resolve_includes(spp::Program *in_program, unsigned int depth);
  
  virtual const spp::Program *_load(const std::string &path, unsigned int depth);

public:
  const spp::Program *load(const std::string &path);

public:
  inline void set_loader(std::unique_ptr <spp::Loader> &&loader) {
    m_loader = move(loader);
  }
  
  inline void set_max_include_depth(unsigned int depth) {
    m_max_include_depth = depth;
  }
  
};

}
