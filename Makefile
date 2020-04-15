IDIR=include
LDIR=lib
SDIR=src
ODIR=src/obj
LIBS=-L$(LDIR) -Wl,-rpath,$(LDIR) -lmpc -lmpfr -lm

CFLAGS=-I$(IDIR) $(LIBS) --std=c99 -W -Wall -Wextra -Werror -O2

CC=gcc

_DEPS=mandelbrot.h image.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ=main.o mandelbrot.o image.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

a.out: $(OBJ) | $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

$(OBJ): | $(ODIR)

$(ODIR):
	mkdir -p $(ODIR)

.PHONY: clean

clean:
	find $(ODIR) -type f -name '*.o' -exec rm {} +
