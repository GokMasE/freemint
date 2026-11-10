# Built in CI (fork-build.yml, hatari-test job) with the m68k-atari-mint
# cross toolchain plus gemlib from .scripts/install-freemint.sh.
CC = m68k-atari-mint-gcc

poptest.prg: poptest.c
	$(CC) -O2 -Wall -o $@ $< -lgem

clean:
	rm -f poptest.prg
