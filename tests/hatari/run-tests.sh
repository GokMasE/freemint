#!/bin/bash
# run-tests.sh <snapshot.zip|dir> [case ...]
#
# Boots a FreeMiNT snapshot (with XaAES) in Hatari and runs test cases
# from cases/*.sh (all of them by default). Needs tests/hatari/setup.sh
# done first and poptest/poptest.prg built. Results, the Hatari log and
# screenshots go to $HATARI_WORK (default ./hatari-out).
#
# Each case is a bash snippet that is sourced with lib.sh loaded; it
# returns non-zero on failure. Cases run one after another in the same
# boot, so each must leave the screen as it found it.
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"
. "$HERE/config.sh"
. "$HERE/lib.sh"

SNAP="$1"; shift
CASES="${*:-$(cd "$HERE/cases" && ls *.sh | sed 's/\.sh$//')}"
BOOT_TIMEOUT="${BOOT_TIMEOUT:-300}"

mkdir -p "$HATARI_WORK"
rm -rf "$HATARI_WORK/shots"
"$HERE/mkdisk.sh" "$SNAP" "$HATARI_WORK/disk.img" "$HERE/poptest/poptest.prg" || exit 1

trap emu_stop EXIT
emu_start "$HATARI_WORK/disk.img" || exit 1
emu_log "booting (timeout ${BOOT_TIMEOUT}s)"
if ! emu_wait '^POPTEST: READY' "$BOOT_TIMEOUT"; then
	emu_shot boot-timeout
	tail -40 "$HATARI_WORK/hatari.log"
	exit 1
fi
emu_fast off
sleep 2
emu_shot booted

fail=0
for c in $CASES; do
	emu_log "=== case $c"
	if ( . "$HERE/cases/$c.sh" ); then
		emu_log "PASS $c"; echo "PASS $c" >> "$HATARI_WORK/results.txt"
	else
		emu_log "FAIL $c"; echo "FAIL $c" >> "$HATARI_WORK/results.txt"; fail=1
		emu_shot "fail-$c"
	fi
done

key q
emu_wait '^POPTEST: DONE' 20 || true
echo "--- program log"
prog_log POPTEST
if [ "$fail" != 0 ] && [ -n "${DUMP_SHOTS:-}" ]; then
	# Screenshots as text in the log, for when artifacts can't be fetched.
	python3 "$HERE/png2txt.py" "$HATARI_WORK"/shots/*.png
fi
exit $fail
