#thread pool makefile

CXX = g++
CC = $(CXX)

INCLUDE_PATH= ../../inc
INLLUDE_PATH+= ../../inc/inl
SOUREC_PATH= ../../src

CFLAGS = -Wall -Werror
CFLAGS+= -Wextra
CFLAGS+= -pedantic
CFLAGS+= -g3
CFLAGS+= -DDEBUG
CFLAGS += -pthread


CXXFLAGS = $(CFLAGS)
CXXFLAGS += -std=c++17

# flags for preprocessor

LDLIBS = -lpthread
CPPFLAGS= -I$(INCLUDE_PATH)
CPPFLAGS+= -I$(INLLUDE_PATH)

VALGRIND = valgrind

VTARGET= memcheck.log

VFLAGS= --log-file=$(VTARGET) --leak-check=full --track-origins=yes --max-threads=500 -s

OBJECTS = utest.o $(SOUREC_PATH)/thread_pool.o $(SOUREC_PATH)/task_printer.o

TARGET= utest

all: utest

utest: $(OBJECTS)

check: utest
	@./utest -v

clean:
	@$(RM) all $(OBJECTS)

.PHONY: clean all

valgrind:
	$(VALGRIND) $(VFLAGS) ./utest

hell:
	valgrind --tool=helgrind -s ./$(TARGET) 2> hell.log

# dependencies

utest.o:

$(SOUREC_PATH)/thread_pool.o: $(INCLUDE_PATH)/thread_pool.hpp $(INCLUDE_PATH)/blocking_queue.hpp $(INLLUDE_PATH)/blocking_queue.hxx $(INCLUDE_PATH)/task.hpp

$(SOUREC_PATH)/task_printer.o: $(INCLUDE_PATH)/task_printer.hpp $(INCLUDE_PATH)/task.hpp

