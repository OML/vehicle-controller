CPP=g++
CPPFLAGS=-Wall -c -std=c++0x
LD=g++
LDFLAGS=
RM=rm -f


BINDIR=bin
OBJDIR=obj
SRCDIR=src

INCLUDES=-I$(INCDIR)
OUTPUT=$(BINDIR)/server
SOURCES=server.cpp \
	client.cpp \
	event_loop.cpp \
	main.cpp
OBJECTS=$(SOURCES:%.cpp=$(OBJDIR)/%.o)

default: $(OUTPUT)

clean:
	@echo "CLEANUP";\
	$(RM) $(OBJECTS) $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	@echo "LD  $<";\
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "CPP $<";\
	$(CPP) $(INCLUDES) $(CPPFLAGS) $< -o $@
