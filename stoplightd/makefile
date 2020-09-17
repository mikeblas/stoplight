all: stoplightd

stoplightd: stoplightd.o regularlightmode.o selectormode.o
	g++ -Wall -Wextra -o $@  $^ -L /usr/lib -l daemonize-1.0 -l pthread -l gpiod

stoplightd.o: stoplightd.cpp lights.h buzzer.h buttons.h
	g++ -c -Wall -Wextra -o $@ $< -I /usr/include/libdaemonize-1.0

regularlightmode.o: regularlightmode.cpp regularlightmode.h lights.h
	g++ -c -Wall -Wextra -o $@ $< -I /usr/include/libdaemonize-1.0

selectormode.o: selectormode.cpp selectormode.h lights.h
	g++ -c -Wall -Wextra -o $@ $< -I /usr/include/libdaemonize-1.0


clean:
	-rm -f stoplightd.o
	-rm -f stoplightd
	-rm -f regularlightmode.o
	-rm -f selectormode.o

