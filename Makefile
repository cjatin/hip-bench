all:bench

bench-build:
	hipcc source/source.cpp source/main.cpp source/memcpywithstream.cpp source/kernel.cpp -I bench_dir/build/install/include -L bench_dir/build/install/lib -lbenchmark -lbenchmark_main -o bench.out

bench:bench-build
	./bench.out

json:bench-build
	./bench.out --benchmark_out=out.json --benchmark_out_format=json

gbench:
	mkdir -p bench_dir
	git clone https://github.com/google/benchmark.git bench_dir/
	cmake -E make_directory "bench_dir/build"
	cmake -E chdir "bench_dir/build" cmake -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=install -DBENCHMARK_ENABLE_TESTING=OFF -DBENCHMARK_INSTALL_DOCS=OFF -DCMAKE_CXX_COMPILER=clang++ ../
	cmake --build "bench_dir/build" --config Release --parallel $(nproc)
	cmake --install "bench_dir/build" --config Release

clean-exe:
	rm -rf *.o *.out *.json

clean:clean-exe
	rm -rf bench_dir

kernel-build:
	hipcc source/main.cpp source/kernel.cpp -I bench_dir/build/install/include -L bench_dir/build/install/lib -lbenchmark -lbenchmark_main -o kernel.out

kernel:kernel-build
	./kernel.out

kernel-json:kernel-build
	./kernel.out --benchmark_out=out.json --benchmark_out_format=json

memcpywithstream-build:
	hipcc source/memcpywithstream.cpp source/main.cpp -I bench_dir/build/install/include -L bench_dir/build/install/lib -lbenchmark -lbenchmark_main -o mws.out

memcpywithstream:memcpywithstream-build
	./mws.out

memcpywithstream-json:memcpywithstream-build
	./mws.out --benchmark_out=out.json --benchmark_out_format=json

help:
	echo "make gbench # build google benchmark"
	echo "make bench # build sources with gbench"
	echo "make clean"