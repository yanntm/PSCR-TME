#include "String.h"

namespace pr
{

// TODO: Implement constructor e.g. using initialization list
String::String (const char *s)
{
  std::cout << "String constructor called for: " << s << std::endl;
}

String::~String ()
{
  std::cout << "String destructor called for: " << (data ? data : "(null)")
      << std::endl;
  // TODO
}

// TODO : add other operators and functions

}// namespace pr

