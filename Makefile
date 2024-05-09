all:bench

bench:
	hipcc source/source.cpp source/main.cpp -I bench_dir/build/install/include -L bench_dir/build/install/lib -lbenchmark -lbenchmark_main -o out
	./out

json:
	hipcc source/source.cpp source/main.cpp -I bench_dir/build/install/include -L bench_dir/build/install/lib -lbenchmark -lbenchmark_main -o out
	./out --benchmark_out=out.json --benchmark_out_format=json

gbench:
	mkdir -p bench_dir
	git clone https://github.com/google/benchmark.git bench_dir/
	cmake -E make_directory "bench_dir/build"
	cmake -E chdir "bench_dir/build" cmake -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=install -DBENCHMARK_ENABLE_TESTING=OFF -DBENCHMARK_INSTALL_DOCS=OFF -DCMAKE_CXX_COMPILER=clang++ ../
	cmake --build "bench_dir/build" --config Release --parallel $(nproc)
	cmake --install "bench_dir/build" --config Release

clean:
	rm -rf bench_dir
	rm -rf *.o *.out *.json

help:
	echo "make gbench # build google benchmark"
	echo "make bench # build sources with gbench"
	echo "make clean"