# mvBlueFOX_Webcam

Gets raw image data from a Matrix Vision mvBlueFox camera and publishes it to a /dev/video* device so other applications can use it as a webcam source. This is obviously not a practical way of getting a webcam to work but I've made this as a platform for some openCV face detection experiments.

### Getting it to work

This project is done for Linux and I have no intentions to port it. You'll the following stuff installed:

- g++ >=9 
- cmake 
- build-essential 
- libopencv-dev major version 4
- (libyuv)[https://chromium.googlesource.com/libyuv/libyuv] (compiled, installed from the repo)
- (mvImpact Acquire)[https://www.matrix-vision.com/USB2.0-industriekamera-mvbluefox.html] (installed from package)
- v4l2loopback-dkms, v4l2loopback-utils

To compile do

```bash
# inside project root
cmake .
make
```

Have the camera(s) plugged in via usb. Load the v4l2loopback kernel module but be aware that there are problems with webcam detection in certain circumstances. If you experience some of those try setting *exclusive_caps* to 1.
Increase *devices* if you have more than one camera.

```bash
sudo modprobe v4l2loopback devices=1 exclusive_caps=0 
./webcam
```


