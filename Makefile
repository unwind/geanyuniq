#
# Top-level Makefile for the Geanyuniq Geany plugin. Not much to see, here.
#

.PHONY:	clean

# --------------------------------------------------------------

all:
	$(MAKE) -C src

# --------------------------------------------------------------

clean:
	$(MAKE) -C src clean
