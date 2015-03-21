# KallistiOS ##version##
#
# utils/bincnv/Makefile
# (c)2000 Dan Potter
#

all: bincnv

bincnv: pacutils.c
	gcc -g -std=c99 -o  pacutils pacutils.c  -lz

