#!/usr/bin/env bash

[ $# -eq 0 ] && { echo "Usage: $0 output_dir"; exit 1; }

if grep -q Microsoft /proc/version; then
    # force overwrite
    ./ffmpeg.exe -y \
    -r 30 \
    -i ${1}/output_%d.png \
    -c:v libx264 -qp 1 -profile:v high444 -preset fast -pix_fmt yuv420p \
    result.mp4
else
    # force overwrite
    ffmpeg -y \
    -r 30 \
    -i ${1}/output_%d.png \
    -c:v libx264 -qp 1 -profile:v high444 -preset fast -pix_fmt yuv420p \
    result.mp4
fi

# force overwrite
# ffmpeg -y \
# -r 30 \
# -i ${1}/output_%d.png \
# -c:v libx264 -qp 1 -profile:v high444 -preset fast -pix_fmt yuv420p \
# result.mp4

# xdg-open result.mp4

# side by side
# ffmpeg -i unreachable_damped_least_squares.mp4 -i unreachable_pseudoinverse.mp4 -filter_complex "[0:v:0]pad=iw*2:ih[bg]; [bg][1:v:0]overlay=w" unreachable_trajectory.mp4
