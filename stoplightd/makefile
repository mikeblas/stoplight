

stoplightd: stoplightd.o regularlightmode.o
	g++ -Wall -Wextra -o stoplightd  stoplightd.o regularlightmode.o -L /usr/lib -l daemonize-1.0 -l pthread -l gpiod

stoplightd.o: stoplightd.cpp lights.h buzzer.h buttons.h
	g++ -c -Wall -Wextra -o stoplightd.o  stoplightd.cpp -I /usr/include/libdaemonize-1.0

regularlightmode.o: regularlightmode.cpp regularlightmode.h
	g++ -c -Wall -Wextra -o regularlightmode.o regularlightmode.cpp -I /usr/include/libdaemonize-1.0

clean:
	rm -f stoplightd.o
	rm -f stoplightd
	rm -f regularlightmode.o
