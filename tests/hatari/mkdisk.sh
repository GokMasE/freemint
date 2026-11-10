#!/bin/bash
# mkdisk.sh <snapshot.zip|dir> <out.img> [extra files for C:\ ...]
#
# Builds a bootable IDE disk image (MBR, one FAT16 partition = C:) from a
# FreeMiNT snapshot archive, e.g. the freemint-tt_falcon_clones artifact
# of fork-build.yml. Extra files land in C:\ and every *.prg among them
# is added as a "run" line to xaaes.cnf so XaAES starts it.
set -eu
SRC="$1"; IMG="$2"; shift 2
SIZE_MB="${DISK_MB:-256}"

tmp="$(mktemp -d)"; trap 'rm -rf "$tmp"' EXIT
root="$tmp/root"; mkdir -p "$root"
if [ -d "$SRC" ]; then
	cp -a "$SRC/." "$root/"
else
	unzip -q "$SRC" -d "$tmp/zip"
	# The artifact may be a zip holding the snapshot zip.
	inner="$(find "$tmp/zip" -maxdepth 1 -name '*.zip' | head -1)"
	if [ -n "$inner" ]; then unzip -q "$inner" -d "$root"; else cp -a "$tmp/zip/." "$root/"; fi
fi
[ -d "$root/auto" ] || { echo "mkdisk: no auto/ folder in $SRC" >&2; exit 1; }

cnf="$(find "$root/mint" -name xaaes.cnf | head -1)"
for f in "$@"; do
	cp "$f" "$root/"
	case "$f" in
	*.prg|*.PRG)
		b="$(basename "$f" | tr a-z A-Z)"
		# Keep the screen predictable: no extra toswin2 window.
		sed -i 's/^run u:\\opt\\GEM\\toswin2/#&/' "$cnf"
		printf '\r\nrun u:\\c\\%s\r\n' "$b" >> "$cnf" ;;
	esac
done

rm -f "$IMG"
truncate -s "${SIZE_MB}M" "$IMG"
printf 'label: dos\nstart=2048, type=6\n' | sfdisk -q "$IMG"
mkfs.fat -F 16 --offset 2048 -n FREEMINT "$IMG" $(( (SIZE_MB * 1024) - 1024 )) >/dev/null
MTOOLS_NO_VFAT=0 mcopy -s -i "$IMG@@1M" "$root"/* ::/
echo "mkdisk: $IMG ($(du -sh "$root" | cut -f1) on C:)"
