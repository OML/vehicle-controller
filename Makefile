CPP=g++
CPPFLAGS=-Wall -c -std=c++0x
LD=g++
LDFLAGS=

MKDIR=mkdir
RMDIR=rmdir
RM=rm -f
CP=cp -r
MV=mv
TOUCH=touch
TEST=test
TEST_DIR=-d

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

default: directories build

clean:
	@echo "RM    " $(OBJECTS) $(OUTPUT);\
	$(RM) $(OBJECTS) $(OUTPUT)
	@echo "RMDIR " $(BINDIR) $(OBJDIR);\
	$(RMDIR) $(BINDIR) $(OBJDIR)

build: $(OUTPUT)

directories:	
	@if [ $(shell $(TEST) $(TEST_DIR) $(OBJDIR); echo $$?;) -eq 1 ]; then \
		echo "MKDIR " $(OBJDIR)\
		$(shell $(MKDIR) $(OBJDIR)); 	\
	fi
	@if [ $(shell $(TEST) $(TEST_DIR) $(BINDIR); echo $$?;) -eq 1 ]; then \
		echo "MKDIR " $(BINDIR)\
		$(shell $(MKDIR) $(BINDIR)); \
	fi

$(OUTPUT): $(OBJECTS)
	@echo "LD     $<";\
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "CPP    $<";\
	$(CPP) $(INCLUDES) $(CPPFLAGS) $< -o $@
