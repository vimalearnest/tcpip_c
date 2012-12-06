program_NAME := client
program_BUILDDIR := ../build
program_BINDIR := $(program_BUILDDIR)/bin

program_C_SRCS := $(wildcard *.c)
program_CXX_SRCS := $(wildcard *.cpp)
program_C_OBJS := ${program_C_SRCS:.c=.o}
program_CXX_OBJS := ${program_CXX_SRCS:.cpp=.o}
program_OBJS := $(program_C_OBJS) $(program_CXX_OBJS)
program_INCLUDE_DIRS := $(program_BUILDDIR)/include
program_LIBRARY_DIRS := $(program_BUILDDIR)/lib
program_LIBRARIES := tcp

CPPFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir))
LDFLAGS += $(foreach librarydir,$(program_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(program_LIBRARIES),-l$(library))

.PHONY: all clean distclean

all: $(program_NAME)

$(program_NAME): $(program_OBJS)
	@mkdir -p $(program_BINDIR)
	$(LINK.cc) $(program_OBJS) -o $(program_BINDIR)/$(program_NAME)

clean:
	@- $(RM) $(program_BINDIR)/$(program_NAME)
	@- $(RM) $(program_OBJS)w

distclean: clean
	@- $(RM) *~

