//
// Created by yuval on 6/15/2023.
//
#include "VirtualMemory.h"

#include <cstdio>
#include <cassert>


void SimpleTest(){
  VMinitialize();
  for (uint64_t i = 0; i < (2 * NUM_FRAMES); ++i) {
    printf("writing to %llu\n", (long long int) i);
    assert(VMwrite(5 * i * PAGE_SIZE, i));
  }

  for (uint64_t i = 0; i < (2 * NUM_FRAMES); ++i) {
    word_t value;
    assert(VMread(5 * i * PAGE_SIZE, &value));
    printf("reading from %llu %d\n", (long long int) i, value);
    assert(uint64_t(value) == i);
  }
  printf("Test 1 - Success\n");
}


void SimpleTest2(){
  VMinitialize();
  for (uint64_t i = 0; i < (2 * NUM_FRAMES); ++i) {
    printf("writing to %llu\n", (long long int) i);
    assert(VMwrite(5 * i * PAGE_SIZE, i));
  }
  for (uint64_t i = 0; i < (2 * NUM_FRAMES); i+=5) {
    printf("writing to %llu\n", (long long int) i);
    assert(VMwrite(5 * i * PAGE_SIZE, 0));
  }

  for (uint64_t i = (2 * NUM_FRAMES)-1; i < (2 * NUM_FRAMES); --i) {
    word_t value;
    assert(VMread(5 * i * PAGE_SIZE, &value));
    printf("reading from %llu %d\n", (long long int) i, value);
    if (i % 5 == 0){
      assert(uint64_t(value) == 0);
    }
    else{
      assert(uint64_t(value) == i);
    }
  }
  printf("Test 2 - Success\n");
}

void test_invalid_input(){
  VMinitialize();
  assert(VMwrite (VIRTUAL_MEMORY_SIZE, 1) == 0);
  printf("Test 3 - Success\n");
}


int main(int argc, char **argv) {
  SimpleTest();
  SimpleTest2();
  test_invalid_input();
  return 0;
}
