# Built in CI (fork-build.yml, hatari-slidetest job) with the
# m68k-atari-mint cross toolchain plus gemlib.
CC = m68k-atari-mint-gcc

slidetest.prg: slidetest.c
	$(CC) -O2 -Wall -o $@ $< -lgem

clean:
	rm -f slidetest.prg
