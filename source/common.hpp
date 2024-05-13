#pragma once
#include <iostream>

#define check(hip_call)                                                        \
  {                                                                            \
    auto res = hip_call;                                                       \
    if (res != hipSuccess) {                                                   \
      std::cout << "failed in call: " << #hip_call << std::endl;               \
      std::abort();                                                            \
    }                                                                          \
  }