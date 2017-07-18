#define CATCH_CONFIG_RUNNER

#include <gawo/io/log.hpp>
#include "catch.hpp"

int main(int argc, char *argv[]) {
  io::logging().attach_sink <io::LogAsynchronousSink>(
    std::move(std::make_unique <io::LogTTYSink>())
  )->set_synchronous(true);
  
  int result = Catch::Session().run(argc, argv);
  
  return (result < 0xff ? result : 0xff);
}
