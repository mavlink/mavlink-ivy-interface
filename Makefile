all:	
	mkdir build -p; 
	cmake -H. -B./build
	make -C build

clean:
	rm -rf ./build

run:
	./build/interface/mavlink-ivy-interface

