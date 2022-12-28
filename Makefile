TOP := $(shell /bin/pwd)
BUILD_OS := $(shell uname)
BINDIR=$(TOP)/build
TESTDIR=$(TOP)/test

INC+=-I$(TOP) -I$(TOP)/murmur2
LIB+=-lm
CFLAGS+=-Wall
CFLAGS+=-fPIC


ifeq ($(DEBUG),1)
OPT=-g $(DEBUGOPT)
else
OPT?=-O3
endif


ifeq ($(BUILD_OS),$(filter $(BUILD_OS), GNU/kFreeBSD GNU Linux))
RPATH=-Wl,-rpath,$(BINDIR)
endif

ifeq ($(BUILD_OS),SunOS)
RPATH=-R$(BINDIR)
CC=gcc
endif

ifeq ($(BUILD_OS),OpenBSD)
RPATH=-R$(BINDIR)
endif

ifeq ($(BUILD_OS),Darwin)
MAC=-install_name $(BINDIR)/libbloom.dylib \
RPATH=-Xlinker -rpath -Xlinker $(BINDIR)
SO=dylib
LD_SONAME=
endif



all: $(BINDIR)/$(SO_VERSIONED) $(BINDIR)/libbloom.a

$(BINDIR)/$(SO_VERSIONED): $(BINDIR)/murmurhash2.o $(BINDIR)/bloom.o
	(cd $(BINDIR) && \
	    $(CC) $(OPT) $(LDFLAGS) bloom.o murmurhash2.o -shared \
	    $(LIB) $(MAC) $(LD_SONAME) -o $(SO_VERSIONED) && \
	rm -f $(BLOOM_SONAME) && \
	ln -s $(SO_VERSIONED) $(BLOOM_SONAME) && \
	rm -f libbloom.$(SO) && \
	ln -s $(BLOOM_SONAME) libbloom.$(SO) )

$(BINDIR)/libbloom.a: $(BINDIR)/murmurhash2.o $(BINDIR)/bloom.o
	(cd $(BINDIR) && ar rcs libbloom.a bloom.o murmurhash2.o)




$(BINDIR)/test-basic: $(TESTDIR)/basic.c $(BINDIR)/libbloom.a
	$(CC) $(CFLAGS) $(OPT) $(INC) $(TESTDIR)/basic.c \
	    $(BINDIR)/libbloom.a $(LIB) -o $(BINDIR)/test-basic

$(BINDIR)/%.o: %.c
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(OPT) $(INC) -c $< -o $@

$(BINDIR)/murmurhash2.o: murmur2/MurmurHash2.c murmur2/murmurhash2.h
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(OPT) $(INC) \
	    -c murmur2/MurmurHash2.c -o $(BINDIR)/murmurhash2.o

clean:
	rm -rf $(BINDIR)

test: $(BINDIR)/test-basic
	$(BINDIR)/test-basic