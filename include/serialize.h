#ifndef _SERIALIZE_H_
#define _SERIALIZE_H_

#include <structs.h>

void serialize_int(unsigned char *const, const int);
void serialize_limit(unsigned char *const, const limit);
void serialize_level(unsigned char *const, const level);

int deserialize_int(unsigned char *const buffer);
limit deserialize_limit(unsigned char *const buffer);
level deserialize_level(unsigned char *const buffer);

#endif
