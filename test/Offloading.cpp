#include <omp.h>

int main() {
  int isHost;
#pragma omp target map(from:isHost)
  { isHost = omp_is_initial_device(); }
  return isHost;
}
