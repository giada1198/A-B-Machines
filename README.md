# A/B Machines

![header](/img/header.jpg)

Photo Credit: [Louis Stein](https://www.instagram.com/lbs_photo/)

A/B Machines is an experimental theatrical experience where people explore their dream, identity and basic human impulse to see and be seen using cameras and selfie machines. The script was adapted from the works of Andy Warhol and inspired by his philosophy.

## Folder Structure

* **photobooth-controller** - Arduino
* **photobooth-screen** - openFrameworks
* **photobooth-player** - openFrameworks
* **lip-sync-prototype** - openFrameworks
* **camera-switch** - Python Tkinter

## Getting Started

### Install openFrameworks

Download the latest version from [Official Website](https://openframeworks.cc/download/) and do not setup any IDE.

### Compile openFrameworks using Xcode 10

openFramewroks seems have troubles with being compiled by [Xcode 10](https://forum.openframeworks.cc/t/xcode-10-0-build-errors/30447/6) using default setting and getting access to camera in [macOS 10.4 Mojave](https://www.apple.com/macos/mojave/). You have to modify some settings manually.

#### Environment
* oepnFrameworks 10.0
* Xcode 10.0

In both the project and `openFrameworksLib.xcodeproj`, goto `Build Settings` and show `All` settings, then change **Architectures** to `Standard Architectures (64-bit Intel)` and remove `i386` from **Valid Architectures**.

![build_settings](/img/build_settings.png)

Remove the two frameworks in `CoreOF.xcconfig`:

```
-framework QuickTime -framework QTKit
```

Goto `openFrameworks-Info.plist` file to add the privacy key named `NSCameraUsageDescription`, and it will automatically be renamed `Privacy - Camera Usage Description`.

![add_camera_access](/img/add_camera_access.png)

If Xcode has troubles with finding out addons during compiling, use Project Generator to reload all the addons.

Build the project. Once the camera access permission poped out, you succeed.

![camera_access](/img/camera_access.png)

### Compile openFrameworks without Xcode

Another solution is removing Xcode and only using Command Line Tools to compile and run projects.

Launch the Terminal and type the following command string:

```
xcode-select --install
```

Use `cd` to the directory of your project and run:

```
make
```

After compiling is done, in `bin` folder, right click the application you just compiled and select `Show Package Contents`. In `Contents` folder, goto `info.plist` file to add the privacy key within `<dict>` below:

```
<key>NSCameraUsageDescription</key>
<string>${PRODUCT_NAME} Camera Usage</string>
```

Now you can launch the project by double clicking the application or run:

```
make RunRelease
```

To activate debug mode, run:

```
make Debug
make RunDebug
```

Launch the application, and now you can allow the openFrameworks access to camera:

![camera_access](/img/camera_access.png)


### Install Addons

#### ofxOpenPose

[ofxOpenFace](https://github.com/antimodular/ofxOpenFace/tree/quick) is a toolkit wrapping [OpenFace](https://github.com/TadasBaltrusaitis/OpenFace). It's capable of facial landmark detection, head pose estimation, facial action unit recognition, and eye-gaze estimation.

First, use [homebrew](https://brew.sh) to install [TBB](https://www.threadingbuildingblocks.org/), openBlas and [openCV](https://opencv.orgGI):

```
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"; brew install tbb; brew install opencv; brew install openblas;
```

##### Fix Bugs

Goto `addons/ofxOpenFace/src` folder and edit `ofxOpenFaceSingle.h` and `ofxOpenFaceMulti.h`, replacing all `return 1` to `return`.

#### Others
* [ofxCV](https://github.com/kylemcdonald/ofxCv)
* [ofxVideoRecorder](https://github.com/timscaffidi/ofxVideoRecorder)

## Media Design Team
* **Giada Sun** - *Lead Media Design* - [Website](http://giadasun.com)
* **Sean Leo** - *Assistant Media Design* - [Website](https://www.seanbyrumleo.com/)
* **SooA Kim** - *Media Engineer* - [Website](https://www.sooakim.com/)
