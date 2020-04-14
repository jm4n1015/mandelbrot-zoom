IDIR=include
LDIR=lib
SDIR=src
ODIR=src/obj
LIBS=-L$(LDIR) -Wl,-rpath,$(LDIR) -lmpc -lmpfr

CFLAGS=-I$(IDIR) $(LIBS) --std=c99 -W -Wall -Wextra -Werror -g

CC=gcc

_DEPS=mandelbrot.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ=main.o mandelbrot.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

a.out: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	find $(ODIR) -type f -name '*.o' -exec rm {} +
