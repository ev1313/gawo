#include <iomanip>
#include "spp/context.hpp"
#include "spp/library.hpp"

#include <fstream>

namespace spp {

Library::Library()
  :
  Library(std::make_unique <spp::DefaultLoader>()) {
  
}

Library::Library(std::unique_ptr <spp::Loader>&& loader)
  :
  m_max_include_depth(100), m_loader(move(loader)) {
  
}

Library::~Library() {

}

void Library::resolve_includes(spp::Program* in_program, unsigned int depth) {
  for (auto iter = in_program->begin();
       iter != in_program->end();
       ++iter) {
    spp::IncludeDirective* include = dynamic_cast<spp::IncludeDirective*>(&(*iter));
    if (!include) {
      continue;
    }
    
    const spp::Program* included = nullptr;
    try {
      included = _load(include->path(), depth);
    } catch (const std::runtime_error& err) {
      // include failed, this can be e.g. due to too deep recursion
      in_program->add_local_error(
        include->loc(),
        std::string("failed to load included file: ") + err.what());
      iter = in_program->erase(iter);
      if (iter == in_program->end()) {
        break;
      }
      continue;
    }
    
    if (!included) {
      in_program->add_local_error(include->loc(),
                                  "failed to load included file");
      iter = in_program->erase(iter);
      if (iter == in_program->end()) {
        break;
      }
      continue;
    }
    
    if (!included->errors().empty()) {
      for (auto& error: included->errors()) {
        in_program->add_error(error);
      }
      // include failed
      iter = in_program->erase(iter);
      if (iter == in_program->end()) {
        break;
      }
      continue;
    }
    
    iter = in_program->erase(iter);
    
    // we can safely +1 here, because a valid program always has a version
    // declaration and invalid programs have at least one error.
    for (auto included_iter = ++included->cbegin();
         included_iter != included->cend();
         ++included_iter) {
      iter = ++in_program->insert(iter, (*included_iter).copy());
    }
    
    if (iter == in_program->end()) {
      break;
    }
  }
}

const spp::Program* Library::_load(const std::string& path, unsigned int depth) {
  if (depth > m_max_include_depth) {
    throw std::runtime_error("maximum include depth exceeded");
  }
  
  {
    auto iter = m_cache.find(path);
    if (iter != m_cache.end()) {
      if (!iter->second) {
        throw std::runtime_error("recursive inclusion detected");
      }
      return iter->second.get();
    }
  }
  
  std::unique_ptr <std::istream> input(m_loader->open(path));
  if (!input) {
    return nullptr;
  }
  
  spp::ParserContext parser(*input, path);
  
  // mark the file as being loaded in the cache
  m_cache[path] = nullptr;
  
  auto program = parser.parse();
  if (!program) {
    return nullptr;
  }
  
  
  spp::Program* result = program.get();
  resolve_includes(result, depth + 1);
  m_cache[path] = move(program);
  
  return result;
}

const spp::Program* Library::load(const std::string& path) {
  return _load(path, 0);
}

/* spp::Loader */

Loader::~Loader() {

}

void DefaultLoader::add_data(const std::string& path, const std::string& data) {
  m_files[path] = std::make_unique<std::istringstream>(data);
}

void DefaultLoader::add_source(const std::string &path, const std::string &source) {
  m_files[path] = std::make_unique<std::ifstream>(source);
}

std::unique_ptr <std::istream> DefaultLoader::open(const std::string& path) {
  auto iter = m_files.find(path);
  if (iter == m_files.end()) {
    return std::make_unique<std::ifstream>(path);
  }
  
  return iter->second;
}


}
