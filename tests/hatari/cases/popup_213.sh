# XaAES issue #213: hold left on a popup button, press right, release
# left, move onto an entry, release right. The entry must be selected
# once, the popup must not reopen and no phantom button events may
# follow.
X=320; Y=220	# inside poptest's work area (160,120 320x200)
WORK_TITLE_Y=110	# poptest's title bar
mark=$(prog_log POPTEST | wc -l)
new_log() { prog_log POPTEST | tail -n +$((mark + 1)); }

# Top the test window first (click its title bar): a click into an
# untopped XaAES window only tops it and is not passed to the app.
mouse_move $X $((WORK_TITLE_Y)); mouse_click 1; sleep 1
emu_shot topped

# Diagnostics: Hatari's own injected double click, then a plain click.
mouse_move $X $Y
emu_cmd "hatari-event doubleclick"; sleep 1.5
new_log
mouse_click 1; sleep 1.5
new_log

mouse_move $X $Y
mouse_down 1
emu_wait '^POPTEST: POPUP_OPEN' 10 || return 1
sleep 0.5; emu_shot popup-open
mouse_down 3; sleep 1.5
mouse_up 1; sleep 0.5
mouse_move $X $((Y + 40))	# two entries down
sleep 0.5; emu_shot popup-highlight
mouse_up 3
emu_wait '^POPTEST: POPUP_CLOSE' 10 || return 1
sleep 3	# give any phantom clicks time to show up
emu_shot after-release

new_log
opens=$(new_log | grep -c '^POPUP_OPEN')
closes=$(new_log | grep '^POPUP_CLOSE' | head -1)
buttons=$(new_log | grep -c '^BUTTON')
ok=0
[ "$opens" -eq 1 ] || { echo "popup opened $opens times (want 1)"; ok=1; }
[ "$buttons" -eq 1 ] || { echo "$buttons button events (want 1)"; ok=1; }
case "$closes" in *"ret=1 item="[1-4]*) ;; *) echo "no entry selected: $closes"; ok=1 ;; esac

# A normal click afterwards must still work: open, pick entry 1.
mark=$(prog_log POPTEST | wc -l)
mouse_move $X $Y; mouse_click 1
emu_wait '^POPTEST: POPUP_OPEN' 10 || { echo "normal click did not open popup"; return 1; }
sleep 0.5; mouse_click 1
emu_wait '^POPTEST: POPUP_CLOSE' 10 || return 1
sleep 2
new_log
[ "$(new_log | grep -c '^POPUP_OPEN')" -eq 1 ] || { echo "normal click: popup reopened"; ok=1; }
return $ok
