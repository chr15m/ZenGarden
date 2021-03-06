my-dir = $(dir $(lastword $(MAKEFILE_LIST)))
OS_MAKEFILES = $(shell ls Makefile.OS.*)

include Makefile.sourcefiles

OBJS := $(foreach FILE,$(LOCAL_SRC_FILES),$(FILE:.cpp=.o))

SNDFILE_INCLUDE = `pkg-config --cflags sndfile`
SNDFILE_LIB = `pkg-config --libs sndfile`

CXXFLAGS = -g -Os -fPIC -Wall -I$(my-dir) $(SNDFILE_INCLUDE)

# figure out what platform we're on

OS=$(shell ./platform)

ifeq (,$(findstring Makefile.OS.$(OS),$(OS_MAKEFILES)))

### Help message
all %:
	@echo OS is set to [$(OS)]
	@echo Please set the OS environment variable to one of the following:
	@ls Makefile.OS.*
	@echo for example:
	@echo '`OS=Linux-i386 make`'
	@echo or else make a Makefile.$(OS)

else

include Makefile.OS.$(OS)

all: platform libzengarden libjnizengarden examplegarden

platform: Makefile.OS.$(OS)
	@echo "Building for the $(OS) platform"
	@mkdir -p ../libs/$(OS)

clean:
	rm -rf $(LOCAL_MODULE).so *.o me/rjdj/zengarden/*.class ../ZenGarden.jar ../libs/$(OS)/*

libjnizengarden: ../libs/$(OS)/libjnizengarden.$(JNI_EXTENSION)

../libs/$(OS)/libjnizengarden.$(JNI_EXTENSION): ./me/rjdj/zengarden/jnizengarden.cpp $(OBJS)
	$(call MAKE_SO, $@, $<, $(OBJS))

libzengarden: ../libs/$(OS)/libzengarden.$(SO_EXTENSION)

../libs/$(OS)/libzengarden.$(SO_EXTENSION): ./me/rjdj/zengarden/jnizengarden.cpp $(OBJS)
	$(call MAKE_SO, $@, $<, $(OBJS))

examplegarden: ../ZenGarden.jar

../ZenGarden.jar: me/rjdj/zengarden/*.java
	cd me/rjdj/zengarden/ && javac *.java
	jar cfm ../ZenGarden.jar me/rjdj/zengarden/manifest me/rjdj/zengarden/*.class

endif
