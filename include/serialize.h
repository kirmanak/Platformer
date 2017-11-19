#ifndef _SERIALIZE_H_
#define _SERIALIZE_H_

#include <structs.h>

void serialize_int(unsigned char *const buffer, int value);
void serialize_bool(unsigned char *const buffer, bool value);
void serialize_limit(unsigned char *const buffer, limit value);
void serialize_level(unsigned char *const buffer, level value);

int deserialize_int(unsigned char *const buffer);
bool deserialize_bool(unsigned char *const buffer);
limit deserialize_limit(unsigned char *const buffer);
level deserialize_level(unsigned char *const buffer);

#endif
