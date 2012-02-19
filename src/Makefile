#
# Trivial Makefile to build the "Geanyuniq" Geany plugin.
#

BASENAME=geanyuniq

CFLAGS=`pkg-config --cflags geany` -fPIC -Wall -pedantic -g
LDLIBS=`pkg-config --libs geany`

.PHONY:		clean

# --------------------------------------------------------------

$(BASENAME).so:	$(BASENAME).o
		gcc -shared -o $@ $(LDLIBS) $<

$(BASENAME).o:	$(BASENAME).c

# --------------------------------------------------------------

clean:
	rm -f *.o *.so
