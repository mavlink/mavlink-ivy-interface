all:	
	mkdir build -p; 
	cmake -H. -B./build
	make -C build
	make -C ./paparazzi/ivy/


clean:
	rm -rf ./build
