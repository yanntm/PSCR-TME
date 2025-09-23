// strutil.h
#ifndef STRUTIL_H
#define STRUTIL_H

#include <cstddef> // for size_t

namespace pr
{

size_t length (const char *s);

char* newcopy (const char *s);

int compare (const char *a, const char *b);

}

#endif // STRUTIL_H
