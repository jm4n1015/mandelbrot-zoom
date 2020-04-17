#!/bin/bash

./a.out | ffmpeg -y -framerate 30 -i - output.mp4
