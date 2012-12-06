library_NAME := libtcp.so
library_C_SRCS := $(wildcard *.c)
library_CXX_SRCS := $(wildcard *.cpp)
library_C_OBJS := ${library_C_SRCS:.c=.o}
library_CXX_OBJS := ${library_CXX_SRCS:.cpp=.o}
library_OBJS := $(library_C_OBJS) $(library_CXX_OBJS)
library_INCLUDE_DIRS :=
library_LIBRARY_DIRS :=
library_LIBRARIES :=
library_BUILDDIR := ../build
library_LIBDIR := $(library_BUILDDIR)/lib
library_INCDIR := $(library_BUILDDIR)/include

CPPFLAGS += $(foreach includedir,$(library_INCLUDE_DIRS),-I$(includedir))
CFLAGS += -fPIC
LDFLAGS + =$(foreach librarydir,$(library_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(library_LIBRARIES),-l$(library))

.PHONY: all clean distclean

all: $(library_NAME)

$(library_NAME): $(library_OBJS)
	mkdir -p $(library_INCDIR)
	mkdir -p $(library_LIBDIR)
	$(LINK.cc) -shared -Wl,-soname,$(library_NAME) -o $(library_LIBDIR)/$(library_NAME) $(library_OBJS)
	cp *.h  $(library_INCDIR)

clean:
	@- $(RM) $(library_LIBDIR)/$(library_NAME).1.0
	@- $(RM) $(library_OBJS)

distclean: clean
	@- $(RM) -rf *~

