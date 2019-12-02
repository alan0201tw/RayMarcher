#!/usr/bin/env bash

[ $# -eq 0 ] && { echo "Usage: $0 <video1> <video2>"; exit 1; }

if grep -q Microsoft /proc/version; then
./ffmpeg.exe -i $1 -i $2 -filter_complex "[0:v:0]pad=iw*2:ih[bg]; [bg][1:v:0]overlay=w" SBS_result.mp4
else
ffmpeg -i $1 -i $2 -filter_complex "[0:v:0]pad=iw*2:ih[bg]; [bg][1:v:0]overlay=w" SBS_result.mp4
fi