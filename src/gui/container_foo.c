#include "gui/container_foo.h"

struct Container_Foo *create_container_foo() {
  struct Container_Foo *result = calloc(1, sizeof(struct Container_Foo));
  return result;
}
