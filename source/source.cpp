#include <benchmark/benchmark.h>
#include <hip/hip_runtime.h>
#include <iostream>
#include <vector>

#define check(hip_call)                                                        \
  {                                                                            \
    auto res = hip_call;                                                       \
    if (res != hipSuccess) {                                                   \
      std::cout << "failed in call: " << #hip_call << std::endl;               \
      std::abort();                                                            \
    }                                                                          \
  }

static void BM_StreamCreateDestroy(benchmark::State &state) {
  for (auto _ : state) {
    hipStream_t stream;
    check(hipStreamCreate(&stream));
    check(hipStreamDestroy(stream));
  }
}

BENCHMARK(BM_StreamCreateDestroy);

static void BM_MemcpyAsyncHostToDevice(benchmark::State &state) {
  size_t size = state.range(0);
  hipStream_t stream;
  check(hipStreamCreate(&stream));
  float *dptr;
  check(hipMalloc(&dptr, sizeof(float) * size));
  std::vector<float> in(size, 10.0f);
  for (auto _ : state) {
    check(hipMemcpyAsync(dptr, in.data(), sizeof(float) * size,
                         hipMemcpyHostToDevice, stream));
    check(hipStreamSynchronize(stream));
  }
  check(hipFree(dptr));
  check(hipStreamDestroy(stream));
}

BENCHMARK(BM_MemcpyAsyncHostToDevice)
    ->RangeMultiplier(2)
    ->Range(1024, 33554432);

static void BM_MemcpyAsyncDeviceToHost(benchmark::State &state) {
  size_t size = state.range(0);
  hipStream_t stream;
  check(hipStreamCreate(&stream));
  float *dptr;
  check(hipMalloc(&dptr, sizeof(float) * size));
  std::vector<float> in(size, 1.0f);
  check(hipMemset(dptr, 0, sizeof(float) * size));
  for (auto _ : state) {
    check(hipMemcpyAsync(in.data(), dptr, sizeof(float) * size,
                         hipMemcpyDeviceToHost, stream));
    check(hipStreamSynchronize(stream));
  }
  check(hipFree(dptr));
  check(hipStreamDestroy(stream));
}

BENCHMARK(BM_MemcpyAsyncDeviceToHost)
    ->RangeMultiplier(2)
    ->Range(1024, 33554432);
