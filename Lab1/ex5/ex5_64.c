#include <stdio.h>
#include <stdint.h>
#include <time.h>
#define print_size(x) printf("Size of %s: %lu\n", #x, sizeof(x)); 

int main(void) {
  print_size(char);
  print_size(short);
  print_size(int);
  print_size(long int);
  print_size(unsigned int);
  print_size(void*);
  print_size(size_t);
  print_size(float);
  print_size(double);
  print_size(int8_t);
  print_size(int16_t);
  print_size(int32_t);
  print_size(int64_t);
  print_size(time_t);
  print_size(clock_t);
  print_size(struct tm);
  print_size(NULL);
  return 0;
}