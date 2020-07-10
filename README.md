# IShowTimeProbe
Web Controls for a BeagleBone Blue Project

 - This code is meant to be run on the beaglebone blue 
 - BBBlue drivers come from https://github.com/beagleboard/librobotcontrol
 - Robot uses a 4 wheel mecanum drive frame and webcam

# Webcam Server Instructions:

Webcam runs Mjpg-streamer 

    MJPG-streamer setup
    1. mkdir MJPG-Streamer
    2. cd MJPG-Streamer
    3. git clone https://github.com/jacksonliam/mjpg-streamer
    4. cd mjpg-streamer/mjpg-streamer-experimental
    5. make

    ./mjpg_streamer -i "./input_uvc.so -d /dev/video0 -n -f 15 -r 640x480" -o ".
    /output_http.so -p 8075 -n -w ./www"
- source https://coreinit.files.wordpress.com/2015/09/camera_streaming_bbb_to_android.pdf

# Website Server Instructions

Server is Flask

    pip3 install Flask

    export FLASK_APP=web_python_program.py
    python3 -m flask run --host=0.0.0.0 
