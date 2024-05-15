#include "common.hpp"
#include <benchmark/benchmark.h>
#include <hip/hip_runtime.h>
#include <vector>

static void BM_MemcpyWithStreamHostToDevice(benchmark::State &state) {
  size_t size = state.range(0);
  hipStream_t stream;
  check(hipStreamCreate(&stream));
  float *dptr;
  check(hipMalloc(&dptr, sizeof(float) * size));
  std::vector<float> in(size, 10.0f);
  for (auto _ : state) {
    check(hipMemcpyWithStream(dptr, in.data(), sizeof(float) * size,
                              hipMemcpyHostToDevice, stream));
  }
  check(hipDeviceSynchronize());
  check(hipFree(dptr));
  check(hipStreamDestroy(stream));
}

BENCHMARK(BM_MemcpyWithStreamHostToDevice)
    ->RangeMultiplier(2)
    ->Range(1024, 33554432);

static void BM_MemcpyWithStreamDeviceToHost(benchmark::State &state) {
  size_t size = state.range(0);
  hipStream_t stream;
  check(hipStreamCreate(&stream));
  float *dptr;
  check(hipMalloc(&dptr, sizeof(float) * size));
  std::vector<float> in(size, 1.0f);
  check(hipMemset(dptr, 0, sizeof(float) * size));
  for (auto _ : state) {
    check(hipMemcpyWithStream(in.data(), dptr, sizeof(float) * size,
                              hipMemcpyDeviceToHost, stream));
  }
  check(hipDeviceSynchronize());
  check(hipFree(dptr));
  check(hipStreamDestroy(stream));
}

BENCHMARK(BM_MemcpyWithStreamDeviceToHost)
    ->RangeMultiplier(2)
    ->Range(1024, 33554432);
