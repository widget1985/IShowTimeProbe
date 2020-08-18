#!/bin/bash

# export LD_LIBRARY_PATH=/opt/MJPG-Streamer/mjpg-streamer/mjpg-streamer-experimental/
/opt/MJPG-Streamer/mjpg-streamer/mjpg-streamer-experimental/mjpg_streamer -i "/opt/MJPG-Streamer/mjpg-streamer/mjpg-streamer-experimental/input_uvc.so -d /dev/video0 -n -f 15 -r 640x480" -o "/opt/MJPG-Streamer/mjpg-streamer/mjpg-streamer-experimental/output_http.so -p 8075 -n -w ./www" &

export FLASK_APP=/home/debian/IShow2020/IShowTimeProbe/Server/web_python_program.py
python3 -m flask run --host=0.0.0.0 --port=8090 &

