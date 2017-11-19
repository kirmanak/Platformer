#ifndef _SERIALIZE_H_
#define _SERIALIZE_H_

#include <structs.h>

void serialize_int(unsigned char *const, const int);
void serialize_bool(unsigned char *const, const bool);
void serialize_limit(unsigned char *const, const limit);
void serialize_level(unsigned char *const, const level);
void serialize_client_condition(unsigned char *const, const client_condition);

int deserialize_int(unsigned char *const buffer);
bool deserialize_bool(unsigned char *const buffer);
limit deserialize_limit(unsigned char *const buffer);
level deserialize_level(unsigned char *const buffer);
client_condition deserialize_client_condition (unsigned char *const buffer);

#endif
