# Helpers for driving Hatari headless. Source after config.sh.
#
# Hatari runs on a private Xvfb display. Mouse input goes in through
# xdotool as real X events (Hatari maps window pixels 1:1 to Atari
# screen pixels at --zoom 1), other control through Hatari's command
# FIFO. Program output sent with NatFeats NF_STDERR ends up in
# $HATARI_WORK/hatari.log.

emu_log() { echo "[$(date +%T)] $*"; }

emu_start() {	# emu_start <disk.img> [extra hatari args]
	local img="$1"; shift
	mkdir -p "$HATARI_WORK/shots"
	: > "$HATARI_WORK/hatari.log"
	SHOT_N=0
	Xvfb "$HATARI_DISPLAY" -screen 0 1024x768x24 -nolisten tcp >/dev/null 2>&1 &
	XVFB_PID=$!
	sleep 1
	DISPLAY="$HATARI_DISPLAY" SDL_AUDIODRIVER=dummy \
	hatari $HATARI_MACHINE_ARGS \
		--tos "$EMUTOS_IMG" --ide-master "$img" \
		--natfeats on --fast-forward on --sound off \
		--zoom 1 --statusbar off --drive-led off --mousewarp off \
		--confirm-quit off --screenshot-dir "$HATARI_WORK/shots" \
		--cmd-fifo "$HATARI_WORK/cmd.fifo" "$@" \
		> "$HATARI_WORK/hatari.log" 2>&1 &
	HATARI_PID=$!
	local i
	for i in $(seq 50); do
		HATARI_WIN="$(DISPLAY="$HATARI_DISPLAY" xdotool search --name '^Hatari' 2>/dev/null | head -1)"
		[ -n "$HATARI_WIN" ] && [ -p "$HATARI_WORK/cmd.fifo" ] && break
		sleep 0.2
	done
	[ -n "$HATARI_WIN" ] || { emu_log "hatari window not found"; return 1; }
	emu_log "hatari started (pid $HATARI_PID, window $HATARI_WIN)"
}

emu_stop() {
	[ -n "${HATARI_PID:-}" ] && kill "$HATARI_PID" 2>/dev/null; wait "$HATARI_PID" 2>/dev/null
	[ -n "${XVFB_PID:-}" ] && kill "$XVFB_PID" 2>/dev/null; wait "$XVFB_PID" 2>/dev/null
	HATARI_PID=; XVFB_PID=
	true
}

emu_cmd() { echo "$*" > "$HATARI_WORK/cmd.fifo"; }

emu_fast() { emu_cmd "hatari-option --fast-forward $1"; }

# emu_shot <name>: screenshot to shots/NN-name.png
emu_shot() {
	local before after f
	before="$(ls "$HATARI_WORK/shots" | wc -l)"
	emu_cmd "hatari-shortcut screenshot"
	for _ in $(seq 25); do
		after="$(ls "$HATARI_WORK/shots" | wc -l)"
		[ "$after" -gt "$before" ] && break
		sleep 0.2
	done
	sleep 0.3
	f="$(ls -t "$HATARI_WORK/shots"/grab*.png 2>/dev/null | head -1)"
	SHOT_N=$((SHOT_N + 1))
	[ -n "$f" ] && mv "$f" "$HATARI_WORK/shots/$(printf %02d $SHOT_N)-$1.png"
}

# emu_wait <regex> <seconds>: wait for a line in hatari.log
emu_wait() {
	local t=0
	while ! grep -qE "$1" "$HATARI_WORK/hatari.log"; do
		[ "$t" -ge "$(( $2 * 5 ))" ] && { emu_log "timeout waiting for: $1"; return 1; }
		kill -0 "$HATARI_PID" 2>/dev/null || { emu_log "hatari exited"; return 1; }
		sleep 0.2; t=$((t + 1))
	done
}

# Mouse, in Atari screen pixels.
xdo() { DISPLAY="$HATARI_DISPLAY" xdotool "$@"; }
mouse_move() { xdo mousemove --window "$HATARI_WIN" "$1" "$2"; sleep 0.2; }
mouse_down() { xdo mousedown "$1"; sleep 0.2; }	# 1 = left, 3 = right
mouse_up()   { xdo mouseup "$1"; sleep 0.2; }
mouse_click() { mouse_down "$1"; sleep 0.1; mouse_up "$1"; }
key() { xdo key --window "$HATARI_WIN" "$@"; sleep 0.2; }

# Program log lines (NF_STDERR, "POPTEST: ..." etc.)
prog_log() { grep -a "^$1: " "$HATARI_WORK/hatari.log" | sed "s/^$1: //"; }
