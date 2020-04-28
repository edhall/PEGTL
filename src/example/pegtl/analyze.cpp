// Copyright (c) 2017-2020 Dr. Colin Hirsch and Daniel Frey
// Please see LICENSE for license or visit https://github.com/taocpp/PEGTL/

#include <tao/pegtl.hpp>

#include <tao/pegtl/contrib/analyze.hpp>

using namespace TAO_PEGTL_NAMESPACE;

struct bar;

struct foo
   : sor< digit, bar >
{};

struct bar
   : plus< foo >
{};

int main()  // NOLINT(bugprone-exception-escape)
{
   if( analyze< foo >() != 0 ) {
      std::cout << "there are problems" << std::endl;
      return 1;
   }
   return 0;
}
