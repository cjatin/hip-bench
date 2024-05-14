#include "common.hpp"
#include <benchmark/benchmark.h>
#include <hip/hip_runtime.h>
#include <vector>

__global__ void accumulate_values(float *a, float *b, size_t size) {
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if (i < size) {
    atomicAdd(a + i, b[i]);
  }
}

static void BM_KernelxN_Memcpy_Sync(benchmark::State &state) {
  float *din, *dvals;
  constexpr size_t size = 256;
  check(hipMalloc(&din, sizeof(float) * size));
  check(hipMalloc(&dvals, sizeof(float) * size));

  hipStream_t stream;
  check(hipStreamCreate(&stream));

  std::vector<float> vals(size, 1.0f);
  check(hipMemcpyWithStream(dvals, vals.data(), sizeof(float) * size,
                            hipMemcpyHostToDevice, stream));

  size_t launch_iter = state.range(0);
  std::vector<float> out(size, 0.0f);
  for (auto _ : state) {
    check(hipMemsetAsync(din, 0, sizeof(float) * size, stream));

    for (size_t i = 0; i < launch_iter; i++) {
      accumulate_values<<<1, size, 0, stream>>>(din, dvals, size);
    }

    check(hipMemcpyWithStream(out.data(), din, sizeof(float) * size,
                              hipMemcpyDeviceToHost, stream));
    check(hipDeviceSynchronize());
  }

  if (out[0] != launch_iter) {
    std::cout << out[0] << std::endl;
    std::abort();
  }

  check(hipFree(din));
  check(hipFree(dvals));
}

BENCHMARK(BM_KernelxN_Memcpy_Sync)->RangeMultiplier(2)->Range(100, 1000);