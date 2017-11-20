#ifndef _SERIALIZE_H_
#define _SERIALIZE_H_

#include <structs.h>

void serialize_int(unsigned char *, int);

void serialize_bool(unsigned char *, bool);

void serialize_limit(unsigned char *, limit);

void serialize_level(unsigned char *, level);

void serialize_client_condition(unsigned char *, client_condition);

int deserialize_int(const unsigned char *);

bool deserialize_bool(const unsigned char *);

limit deserialize_limit(const unsigned char *);

level deserialize_level(const unsigned char *);

client_condition deserialize_client_condition(const unsigned char *);

#endif
