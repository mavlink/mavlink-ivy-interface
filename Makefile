all:	
	mkdir -p build; 
	cmake -H. -B./build
	make -C build

clean:
	rm -rf ./build

run:
	./build/interface/mavlink-ivy-interface

