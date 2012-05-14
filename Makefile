CPP=g++
CPPFLAGS=-Wall -c -std=c++0x
LD=g++
LDFLAGS=

ifdef SystemRoot # Windows
	MKDIR=mkdir
	MKDIR_P=$(MKDIR)
	RMDIR=rmdis /S
	RM=rm /Q
	CP=copy
	MV=move
	
else	# Linux
	MKDIR=mkdir
	MKDIR_P=$(MKDIR) -p
	RMDIR=rmdir
	RM=rm -f
	CP=cp -r
	MV=mv
	TOUCH=touch
	TEST=test
	TEST_DIR=-d
endif

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

default: build

clean:
	@echo "RM    " $(OBJECTS) $(OUTPUT);\
	$(RM) $(OBJECTS) $(OUTPUT)
	@echo "RMDIR " $(BINDIR) $(OBJDIR);\
	$(RMDIR) $(BINDIR) $(OBJDIR)

build: $(OUTPUT)

assure_dir=@$(MKDIR_P) $(@D)

$(OUTPUT): $(OBJECTS)
	$(assure_dir)
	@echo "LD     $@";\
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(assure_dir)
	@echo "CPP    $@";\
	$(CPP) $(INCLUDES) $(CPPFLAGS) $< -o $@
