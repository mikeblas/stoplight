
CXX      := g++
CXXFLAGS := -Wall -Wextra -I /usr/include/libdaemonize-1.0
LDFLAGS  := -L /usr/lib -l daemonize-1.0 -l pthread -l gpiod

all: stoplightd

stoplightd: stoplightd.o regularlightmode.o selectormode.o main.o remomode.o
	$(CXX) $(CXXFLAGS)  -o $@  $^ $(LDFLAGS)

main.o: main.cpp stoplightd.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

stoplightd.o: stoplightd.cpp lights.h buzzer.h buttons.h smartbuzzer.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

regularlightmode.o: regularlightmode.cpp regularlightmode.h lights.h modeinterface.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

selectormode.o: selectormode.cpp selectormode.h lights.h modeinterface.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

remomode.o: remomode.cpp remomode.h lights.h modeinterface.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<



debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all



clean:
	-rm -f stoplightd.o
	-rm -f stoplightd
	-rm -f regularlightmode.o
	-rm -f selectormode.o
	-rm -f main.o

