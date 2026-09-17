#include "String.h"

namespace pr
{

// TODO: copier s dans data (liste d'initialisation)
String::String (const char *s) : data(nullptr)
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

