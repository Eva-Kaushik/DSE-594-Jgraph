#!/usr/bin/env bash
set -e

for jgr in step_*.jgr; do
  base="${jgr%.jgr}"
  echo "Processing $jgr ..."

  # 1) Jgraph -> PostScript
  jgraph "$jgr" > "${base}.ps"

  # 2) PostScript -> PNG (ImageMagick 7)
  magick -density 300 "${base}.ps" -quality 100 "${base}.png"
done
