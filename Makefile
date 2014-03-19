CC = gcc
CFLAGS = -g -Wall -pthread -lm

all: mtsudoku
	$(CC) $(CFLAGS) mtsudoku.c
