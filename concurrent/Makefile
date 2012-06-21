server_NAME := server
server_C_SRCS := server.c
server_C_OBJS := ${server_C_SRCS:.c=.o}
server_OBJS := $(server_C_OBJS)

client_NAME := client
client_C_SRCS := client.c
client_C_OBJS := ${client_C_SRCS:.c=.o}
client_OBJS := $(client_C_OBJS)

.PHONY: all clean distclean

all: $(server_NAME) $(client_NAME)

$(server_NAME): $(server_OBJS)
	$(LINK.cc) $(server_OBJS) -o $(server_NAME)

$(client_NAME): $(client_OBJS)
	$(LINK.cc) $(client_OBJS) -o $(client_NAME)

clean:
	@- $(RM) $(client_NAME) $(server_NAME)
	@- $(RM) $(client_OBJS) $(server_OBJS)

distclean: clean
	@- $(RM) *~

