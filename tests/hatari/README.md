# Hatari test harness

Boots a FreeMiNT snapshot with XaAES in [Hatari](https://hatari.tuxfamily.org)
and drives it with real mouse events, so GUI bugs can be reproduced and
regression tested without hardware.

- Machine: Falcon 030 with MMU and FPU, 14 MB, VGA monitor, EmuTOS 1.4.
- Disk: an IDE image with one FAT16 partition (C:) built from the
  `freemint-tt_falcon_clones` snapshot zip.
- Display: Hatari runs on a private Xvfb display. Mouse input is sent with
  `xdotool` (window pixels map 1:1 to Atari pixels), other control goes
  through Hatari's command FIFO (fast-forward, screenshots).
- Output: `poptest.prg` (in `poptest/`) logs every event through Hatari
  NatFeats `NF_STDERR`, so lines starting with `POPTEST:` show up in
  `hatari.log` and cases assert on them.

## CI

The `hatari-test` job in `.github/workflows/fork-build.yml` runs after
the builds on every push to a non-master branch. It uploads
`hatari-test-results` (screenshots, Hatari log, `results.txt`).

## Running locally

```sh
tests/hatari/setup.sh                 # apt packages + EmuTOS (cached)
make -C tests/hatari/poptest          # needs m68k-atari-mint-gcc + gemlib
tests/hatari/run-tests.sh freemint-*-tt_falcon_clones.zip [case ...]
```

Results land in `./hatari-out` (override with `HATARI_WORK`). Machine
options, EmuTOS version and display number are in `config.sh`.

## Cases

Each file in `cases/` is sourced in the same boot, with the helpers from
`lib.sh` (`mouse_move`, `mouse_down`, `mouse_up`, `key`, `emu_shot`,
`emu_wait`, `prog_log`). Return non-zero to fail.

- `popup_213.sh`: XaAES issue #213. Hold left on the popup button,
  press right, release left, move to an entry, release right. Exactly one
  popup and one button event are allowed; a normal click must still work.
