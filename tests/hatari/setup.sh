#!/bin/bash
# Install what the Hatari test harness needs (Ubuntu 24.04 / Debian).
# Safe to run more than once. EmuTOS is cached in $HATARI_CACHE.
set -eu
HERE="$(cd "$(dirname "$0")" && pwd)"
. "$HERE/config.sh"

SUDO=; [ "$(id -u)" = 0 ] || SUDO=sudo
need=
for p in hatari Xvfb xdotool mcopy mkfs.fat sfdisk unzip; do
	command -v $p >/dev/null || need=1
done
if [ -n "$need" ]; then
	$SUDO apt-get update -q
	$SUDO env DEBIAN_FRONTEND=noninteractive apt-get install -y -q \
		hatari xvfb xdotool mtools dosfstools fdisk unzip python3-pil
fi

mkdir -p "$HATARI_CACHE"
if [ ! -f "$EMUTOS_IMG" ]; then
	curl -fsSL -o "$HATARI_CACHE/emutos.zip" "$EMUTOS_URL"
	unzip -o -q "$HATARI_CACHE/emutos.zip" -d "$HATARI_CACHE"
fi
echo "hatari: $(hatari --version 2>&1 | grep -m1 Hatari)"
echo "emutos: $EMUTOS_IMG"
