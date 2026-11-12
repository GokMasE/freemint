# Shared settings for the Hatari test harness. Override any of these
# from the environment.
: "${HATARI_CACHE:=$HOME/.cache/freemint-hatari}"
: "${EMUTOS_VER:=1.4}"
: "${EMUTOS_URL:=https://downloads.sourceforge.net/project/emutos/emutos/$EMUTOS_VER/emutos-512k-$EMUTOS_VER.zip}"
: "${EMUTOS_IMG:=$HATARI_CACHE/emutos-512k-$EMUTOS_VER/etos512us.img}"
# Where a run keeps its disk image, logs and screenshots.
: "${HATARI_WORK:=$PWD/hatari-out}"
# Emulated machine: Falcon 030 + IDE disk, VGA monitor (640x480).
: "${HATARI_MACHINE_ARGS:=--machine falcon --cpulevel 3 --mmu on --fpu 68882 -s 14 --monitor vga --dsp off}"
: "${HATARI_DISPLAY:=:42}"
