./bin/mavlink-ivy-interface 127.0.0.1 &
stty -F /dev/ttyUSB0 57600
nc -l -u 127.0.0.1 14550 > /dev/ttyUSB0
