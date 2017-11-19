#include <serialize.h>

void serialize_int(unsigned char *const buffer, int value) {

}

void serialize_bool(unsigned char *const buffer, bool value) {

}

void serialize_limit(unsigned char *const buffer, limit value) {

}

void serialize_level(unsigned char *const buffer, level value) {

}

int deserialize_int(unsigned char *const buffer) {
  return 0;
}

bool deserialize_bool(unsigned char *const buffer) {
  return false;
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
  l.limits = 0;
  return l;
}

