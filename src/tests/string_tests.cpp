#include "tests/string_tests.hpp"

#include "misc/string.hpp"

void testString() {
  using mcpp::String;

  std::cout << "--- TESTING STRINGS ---\n";

  String s = "this is a";

  std::cout << s << '\n';

  s += " string with concatenation";

  std::cout << s << '\n';

  s = "and reassignment";

  std::cout << s << std::endl;
}