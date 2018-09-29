# A/B Machines

An immersive theater project that explores people chasing American dream and their self-identity, inspired by Andy Warhol and his philosophy.

## Getting Started

### Compile openFrameworks using Command Line Tools without Xcode

openFramewroks has troubles with being compiled by [Xcode 10](https://forum.openframeworks.cc/t/xcode-10-0-build-errors/30447/6) and getting access to camera in macOS 10.4 Mojave. The simplest solution is Removing Xcode and only using Command Line Tools to compile and run projects.

Launch the Terminal and type the following command string:

```
xcode-select --install
```

Use `cd` to the directory of your openFramewroks project and run:

```
make
```

### Install openFrameworks Addons

#### ofxOpenPose

[ofxOpenFace](https://github.com/antimodular/ofxOpenFace/tree/quick) is a toolkit wrapping [OpenFace](https://github.com/TadasBaltrusaitis/OpenFace). It's capable of facial landmark detection, head pose estimation, facial action unit recognition, and eye-gaze estimation.

At first, use [homebrew](https://brew.sh) to install [TBB](https://www.threadingbuildingblocks.org/), openBlas and [openCV](https://opencv.orgGI):

```
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"; brew install tbb; brew install opencv; brew install openblas;
```








## Media Design Team
* **Giada Sun** - *Lead Media Design* - [Website](http://giadasun.com)
* **Sean Leo** - *Assistant Media Design* - [Website](https://www.seanbyrumleo.com/)
* **SooA Kim** - *Media Engineer* - [Website](https://www.sooakim.com/)
