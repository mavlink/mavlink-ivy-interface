all:	
	mkdir -p build; 
	cmake -H. -B./build
	make -C build

clean:
	rm -rf ./build
	rm -rf ./bin

run:
	./build/interface/mavlink-ivy-interface

