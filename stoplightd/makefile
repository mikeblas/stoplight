
# CXX      := g++
CXXFLAGS ?= -Wall -Wextra -I /usr/include/libdaemonize-1.0 -g -Wa,-adhln=$*.s
LDFLAGS  := -L /usr/lib -l daemonize-1.0 -l pthread -l gpiod

OBJDIR   := obj

all: stoplightd

stoplightd: $(OBJDIR)/stoplightd.o $(OBJDIR)/regularlightmode.o $(OBJDIR)/selectormode.o $(OBJDIR)/main.o $(OBJDIR)/remomode.o
	$(CXX) $(CXXFLAGS)  -o $@  $^ $(LDFLAGS)

$(OBJDIR)/main.o: main.cpp stoplightd.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/stoplightd.o: stoplightd.cpp lights.h buzzer.h buttons.h smartbuzzer.h stoplightd.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/regularlightmode.o: regularlightmode.cpp regularlightmode.h lights.h modeinterface.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/selectormode.o: selectormode.cpp selectormode.h lights.h modeinterface.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/remomode.o: remomode.cpp remomode.h lights.h modeinterface.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<



debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all



clean:
	-rm -f $(OBJDIR)/stoplightd.o
	-rm -f $(OBJDIR)/stoplightd
	-rm -f $(OBJDIR)/regularlightmode.o
	-rm -f $(OBJDIR)/selectormode.o
	-rm -f $(OBJDIR)main.o
	-rm -f stoplightd



