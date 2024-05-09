# Mini bench setup for HIP APIs

This is a collection of scripts to benchmark CPU time witnessed by the user for HIP APIs (basically latency). Although this tells us nothing of whats actually happening, these metrics can tell us the perceived API response witnessed by the user.

## How to use

- make sure rocm is installed and hipcc is available in `$PATH`
- clone the repo
- `make gbench` to build google benchmark lib
- `make` to build and run the exec
