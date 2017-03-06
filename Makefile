GXX=gcc
PTHREAD=-lpthread
FLAGS=-Wall -Wextra -Wpedantic

deadlock: deadlock.c
	$(GXX) deadlock.c -o deadlock $(PTHREAD) $(FLAGS)

