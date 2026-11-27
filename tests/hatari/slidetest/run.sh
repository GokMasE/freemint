#!/bin/bash
# run.sh <snapshot.zip|dir>
#
# Slider painting test (fork PR 12, display_slider() in xa_wind.c).
# For each step sldtest.prg changes sliders (some right after a
# WF_CURRXYWH resize, which leaves a frame redraw queued, the case where
# XaAES skips the direct slider paint), or the sizer is dragged with the
# mouse. The window as painted is then compared with the same window
# after a full repaint from scratch (form_dial FMD_FINISH). They must be
# pixel-identical, otherwise a slider was left stale.
set -u
HERE="$(cd "$(dirname "$0")" && pwd)"
HROOT="$(cd "$HERE/.." && pwd)"
. "$HROOT/config.sh"
. "$HROOT/lib.sh"
SNAP="$1"
BOOT_TIMEOUT="${BOOT_TIMEOUT:-300}"
OUT="$HATARI_WORK"
mkdir -p "$OUT"; rm -rf "$OUT/shots"; : > "$OUT/results.txt"
"$HROOT/mkdisk.sh" "$SNAP" "$OUT/disk.img" "$HERE/sldtest.prg" || exit 1
trap emu_stop EXIT
emu_start "$OUT/disk.img" || exit 1
emu_log "booting (timeout ${BOOT_TIMEOUT}s)"
if ! emu_wait '^SLIDETEST: READY' "$BOOT_TIMEOUT"; then
	emu_shot boot-timeout; tail -40 "$OUT/hatari.log"; exit 1
fi
emu_fast off
sleep 3

PARK_X=630; PARK_Y=470	# keep the mouse pointer out of the shots
lastline() { prog_log SLIDETEST | grep "^$1 " | tail -1; }
rect_of() { echo "$1" | sed -n 's/.* rect=\([0-9,-]*\).*/\1/p'; }
shot() { emu_shot "$1"; ls -t "$OUT"/shots/*-"$1".png | head -1; }
nlines() { prog_log SLIDETEST | wc -l; }
wait_new() {	# wait_new <tag> <count-before>
	local t=0
	while [ "$(prog_log SLIDETEST | tail -n +$(( $2 + 1 )) | grep -c "^$1 ")" -eq 0 ]; do
		[ $t -ge 50 ] && { emu_log "timeout waiting for $1"; return 1; }
		sleep 0.2; t=$((t + 1))
	done
}
fail=0
check() {	# check <name> <shot-a> <shot-b> <rect> <want: same|differ>
	local r n
	r="$(python3 "$HERE/compare.py" "$2" "$3" "$4")"; n=$(echo "$r" | awk '{print $2}')
	if { [ "$5" = same ] && [ "$n" -eq 0 ]; } || { [ "$5" = differ ] && [ "$n" -gt 0 ]; }; then
		echo "PASS $1 ($5): $r" | tee -a "$OUT/results.txt"
	else
		echo "FAIL $1 (want $5): $r" | tee -a "$OUT/results.txt"; fail=1
	fi
}
# painted-vs-repainted for the current state
verify() {	# verify <name>
	local m rect a b
	mouse_move $PARK_X $PARK_Y; sleep 1.5
	m=$(nlines); key p; wait_new RECT $m || { fail=1; return; }
	rect="$(rect_of "$(lastline RECT)")"
	a="$(shot "$1-painted")"
	m=$(nlines); key r; wait_new REF $m || { fail=1; return; }
	sleep 1.5
	b="$(shot "$1-repainted")"
	check "$1" "$a" "$b" "$rect" same
	LAST_SHOT="$a"; LAST_RECT="$rect"
}

# Top the window: click its title bar.
r="$(rect_of "$(lastline READY)")"; IFS=, read WX WY WW WH <<< "$r"
mouse_move $((WX + WW / 2)) $((WY + 6)); mouse_click 1; sleep 1
emu_shot topped

# 0. sanity: 'r' must repaint the frame. Scribble, repaint, compare.
verify initial
m=$(nlines); key g; wait_new GARBAGE $m; sleep 1
g="$(shot garbage)"
check sanity-garbage-visible "$LAST_SHOT" "$g" "$LAST_RECT" differ
m=$(nlines); key r; wait_new REF $m; sleep 1.5
c="$(shot garbage-repainted)"
check sanity-repaint-clears "$LAST_SHOT" "$c" "$LAST_RECT" same

# 1..6: scripted slider changes (3..6 with a frame redraw queued).
prev="$LAST_SHOT"
for s in 1 2 3 4 5 6; do
	m=$(nlines); key n; wait_new STEP $m || { fail=1; continue; }
	verify "step$s"
	case $s in 1|2) check "step$s-slider-moved" "$prev" "$LAST_SHOT" "$LAST_RECT" differ ;; esac
	prev="$LAST_SHOT"
done

# 7..8: live resize with the sizer, app sets sliders on every WM_SIZED.
for d in "70 50" "-90 -60" "40 -30"; do
	set -- $d
	IFS=, read WX WY WW WH <<< "$LAST_RECT"
	sx=$((WX + WW - 6)); sy=$((WY + WH - 6))
	mouse_move $sx $sy; mouse_down 1
	for i in 1 2 3 4 5; do mouse_move $((sx + $1 * i / 5)) $((sy + $2 * i / 5)); done
	sleep 0.5; mouse_up 1; sleep 1
	verify "drag${1}_${2}"
done
emu_log "SIZED events: $(prog_log SLIDETEST | grep -c '^SIZED')"

key q; emu_wait '^SLIDETEST: DONE' 20 || true
echo "--- program log"; prog_log SLIDETEST
echo "--- results"; cat "$OUT/results.txt"
if [ "$fail" != 0 ]; then
	python3 "$HROOT/png2txt.py" "$OUT"/shots/*.png | head -2000
fi
exit $fail
