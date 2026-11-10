#!/bin/bash -eu
# One-step local setup of the m68k-atari-mint cross toolchain + libraries,
# using the same snapshot packages as CI (https://atari.joska.no/snapshots).
# Needs: sudo, wget, bzip2, curl (Debian/Ubuntu: apt install wget bzip2 curl).
# Needs network access to atari.joska.no and mikro.atari.org.
#
# Usage:  ./.scripts/install-toolchain-local.sh
# Then:   M68K_ATARI_MINT_CROSS=yes make
#         (top-level `make` builds all default targets; see .scripts/build.sh
#          for per-CPU targets via before_script.sh)

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
export CROSS_TOOL=${CROSS_TOOL:-m68k-atari-mint}
# CI scripts append to $GITHUB_ENV; locally we just collect it in a file.
export GITHUB_ENV=${GITHUB_ENV:-$HOME/.freemint-toolchain.env}
: > "$GITHUB_ENV"

SYSROOT_DIR="/" "$SCRIPT_DIR/install-freemint.sh" binutils gcc mintbin
"$SCRIPT_DIR/install-freemint.sh" mintlib fdlibm gemlib cflib gemma
"$SCRIPT_DIR/install-hcp.sh"   # help compiler (XaAES/kernel docs)
"$SCRIPT_DIR/install-udo.sh"

echo "M68K_ATARI_MINT_CROSS=yes" >> "$GITHUB_ENV"
echo
echo "Toolchain installed: $(command -v ${CROSS_TOOL}-gcc || echo "${CROSS_TOOL}-gcc not on PATH")"
echo "Env (HCP, UDO, ...) written to $GITHUB_ENV; load with: set -a; . $GITHUB_ENV; set +a"
