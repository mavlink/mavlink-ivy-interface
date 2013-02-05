all:	
	mkdir -p build; 
	cmake -H. -B./build
	make -C build

clean:
	rm -rf ./build
	rm -rf ./bin

run:
	./build/interface/mavlink-ivy-interface

usb:
	stty -F /dev/ttyUSB0 57600
	nc -l -u 127.0.0.1 14550 > /dev/ttyUSB0


