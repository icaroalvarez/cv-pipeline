// tests-main.cpp
#define CATCH_CONFIG_RUNNER
#include "catch2/catch.hpp"
#include "trompeloeil.hpp"

int main( int argc, char* argv[] ) {

    // trompeloeil runtime adapter
    // https://github.com/rollbear/trompeloeil/blob/master/docs/CookBook.md#unit_test_frameworks
    trompeloeil::set_reporter([](
            trompeloeil::severity s,
            const char* file,
            unsigned long line,
            const std::string msg)
                              {
                                  std::ostringstream os;
                                  if (line) os << file << ':' << line << '\n';
                                  os << msg;
                                  auto failure = os.str();
                                  if (s == trompeloeil::severity::fatal)
                                  {
                                      FAIL(failure);
                                  }
                                  else
                                  {
                                      CAPTURE(failure);
                                      CHECK(failure.empty());
                                  }
                              });

    // global setup...
    int result = Catch::Session().run( argc, argv );

    // global clean-up...
    return result;
}