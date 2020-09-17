
stoplightd : stoplightd.cpp lights.h buzzer.h buttons.h
	g++ -Wall -Wextra -o stoplightd  stoplightd.cpp -I /usr/include/libdaemonize-1.0 -L /usr/lib -l daemonize-1.0 -l pthread -l gpiod

clean :
	rm -f stoplightd.o stoplightd
