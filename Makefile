CPP=g++
CPPFLAGS=-Wall -c -ggdb3
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
	RMDIR=rm -rf
	RM=rm -rf
	CP=cp -r
	MV=mv
	TOUCH=touch
	TEST=test
	TEST_DIR=-d
endif

BINDIR=bin
OBJDIR=obj
SRCDIR=src

INCLUDES=-Isrc/protocols -Isrc/ -Isrc/gptk/
OUTPUT=$(BINDIR)/server
GPTK_SOURCES=\
		gptk/event_loop.cpp \
		gptk/ufile.cpp \
		gptk/file.cpp \
		gptk/unixpipe.cpp \
		gptk/config_file.cpp
SOURCES=$(GPTK_SOURCES) \
	server.cpp \
	client.cpp \
	main.cpp \
	protocols/carma.cpp \
	protocols/protocol.cpp \
	mainboard.cpp
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
