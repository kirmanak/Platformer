// for 4 bytes int and 1 byte char

#include <serialize.h>

#include <stdlib.h>

void serialize_client_condition (unsigned char *const buffer, const client_condition value) {
  serialize_int(buffer, value.x);
  serialize_int(buffer + sizeof(int), value.y);
  serialize_bool(buffer + sizeof(int) * 2, value.close);
}

void serialize_bool(unsigned char *const buffer, const bool value) {
  buffer[0] = (unsigned char) value;
}

void serialize_int(unsigned char *const buffer, const int value) {
  buffer[0] = (unsigned char) value >> 24;
  buffer[1] = (unsigned char) value >> 16;
  buffer[2] = (unsigned char) value >> 8;
  buffer[3] = (unsigned char) value;
}

void serialize_limit(unsigned char *const buffer, const limit value) {
  serialize_int(buffer, value.up);
  serialize_int(buffer + sizeof(int) * 1, value.down);
  serialize_int(buffer + sizeof(int) * 2, value.right);
  serialize_int(buffer + sizeof(int) * 3, value.left);
}

void serialize_level(unsigned char *const buffer, const level value) {
  serialize_int(buffer, value.start_x);
  serialize_int(buffer + sizeof(int) * 1, value.start_y);
  serialize_int(buffer + sizeof(int) * 2, value.limits_size);
}

int deserialize_int(const unsigned char *const buffer) {
  int i = buffer[0];
  i = i << 8;
  i += buffer[1];
  i = i << 8;
  i += buffer[2];
  i = i << 8;
  i += buffer[3];
  return i;
}

limit deserialize_limit(unsigned char *const buffer) {
  limit l;
  l.up = deserialize_int(buffer);
  l.down = deserialize_int(buffer + sizeof(int) * 1);
  l.right = deserialize_int(buffer + sizeof(int) * 2);
  l.left = deserialize_int(buffer + sizeof(int) * 3);
  return l;
}

level deserialize_level(unsigned char *const buffer) {
  level l;
  l.start_x = deserialize_int(buffer);
  l.start_y = deserialize_int(buffer + sizeof(int) * 1);
  l.limits_size = deserialize_int(buffer + sizeof(int) * 2);
  l.limits = NULL;
  return l;
}

bool deserialize_bool(unsigned char *const buffer) {
  return buffer[0];
}

const client_condition deserialize_client_condition (unsigned char *const buffer) {
  client_condition cond;
  cond.x = deserialize_int(buffer);
  cond.y = deserialize_int(buffer + sizeof(int));
  cond.close = deserialize_bool(buffer + sizeof(int) * 2);
  return cond;
}


