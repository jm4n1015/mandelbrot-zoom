IDIR=include
LDIR=lib
SDIR=src
ODIR=src/obj
LIBS=-L$(LDIR) -Wl,-rpath,$(LDIR)

CFLAGS=-I$(IDIR) $(LIBS) --std=c99 -W -Wall -Wextra -Werror -O2

CC=gcc

_DEPS=
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ=main.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

a.out: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	find $(ODIR) -type f -name '*.o' -exec rm {} +
