# Interactive GUI

## Description

This is a simple interactive GUI app. This should be considered as a basic template to build interactive apps using Qt.

## Architecture

All the modules are located in the Application object. It allocates all the required resources and uses Qt event system to interact with the environment. The components of the Applications are connected via Observer pattern. The observer pattern uses Qt event system to send messages. Currently the connection does not support sending messages accross different threads. The application object consists of the following modules:

- Kernel:

This part of the application contains the buisness logic and internal data.

1) model_ represents the state of the field with items.

2) geom_model_, geometric model, represents the state of the model that can be drawn

3) field_controller_ is a controller for the module_

These components act as members of MVC pattern. geom_model_ receives the actual state of the model_ and creates a geometric state that can be drawn. In case of any action of a mouse, geometric_model_ notifies the field_controller_. The field_controller_ chooses the the appropriate action of the model_.

- GUI:

This part of the application contains all the GUI related components. It consists of main_window_ only but it contains a view_ object that can draw on the main_window_. This module exposes the main_window_ and the view_ via protected methods.

- Impl:

Implementation layer of the Application connects the kernel and the GUI using controllers and subscribtions. It contains a controller that receives the signals from the view_ object in the main_window_ and chooses appropriate actions of the geometric model. The triple geom_model_, view_, controller_ act as participants of MVC pattern. After all the connections between modules are established the main_window_ is shown to the user to avoid flickering.

The following connections are made
```
model_ --> geom_model_ --> view_
  ^           |     ^       |
  ^           |     ^       |
  |           v     |       v
field_controller_  controller
```

Here --> represents the observalbe-->observer connection and ->> represents controller->>model connection. The observable-->observer connection is non-blocking. It sends messages using Qt event system.

## How to Build on Windows

Currently the project uses Qwt with static linking with Qt and Qwt. You may try to reconfigurate the conanfile and the pro file of the project if you want to switch to dynamic linking with Qt and Qwt.

### Prerequisites

- [Visual Studio 2019](https://visualstudio.microsoft.com/downloads/)

- [Qt 5.15.7](https://www.qt.io/download) with source code

- [Conan 1.59](https://conan.io/)


### Prerequisites

- [Visual Studio 2019](https://visualstudio.microsoft.com/downloads/)

- [Qt 5.15.7](https://www.qt.io/download) with source code

- [Conan 1.59](https://conan.io/)

### Setup your Environment

1. Install Visual Studio 2019. You basically need the compiler and the debugger.

2. Install Conan 1.59. I did not test the project with the conan v2. Probably it will work. It is also worth setting the default profile. Usually it is done by default for Visual Studio. In case it is not, here is an example
```
[settings]
os=Windows
os_build=Windows
arch=x86_64
arch_build=x86_64
compiler=Visual Studio
compiler.version=16
build_type=Release
[options]
[build_requires]
[env]
```

3. Install Qt. Install Qt Creator and the source code of Qt 5.15.7. Currently it is impossible to build the project with a newer version of Qt because qwt depends on this version.

### Build Qt Statically

There are two options: build with static runtime library of with dynamic runtime library. The first option is not recommended but allows you to have one executable file with no dependencies.

#### Build Qt Statically with Static Runtime Library

1. Open Visual Studio command line x64 Native.

2. Suppose Qt_Source is the path to your Qt source code, Qt_Build is your current folder and Qt_Static is the install folder.

3. First you need to configure Qt using command

Qt_Source/configure.bat -debug-and-release -static -static-runtime -opengl desktop -platform win32-msvc -prefix "Qt_Static" -opensource -confirm-license -skip webengine -make libs -nomake tools -nomake tests -nomake examples

4. use
```
set CL=/MP
```
to make nmake multi-threaded

5. Build Qt using (or you may use jom here)
```
nmake
```

6. Install Qt using (or you may use jom here)
```
nmake install
```

7. Delete Qt_Build folder

#### Build Qt Statically with Dynamic Runtime Library

1. Open Visual Studio command line x64 Native.

2. Suppose Qt_Source is the path to your Qt source code, Qt_Build is your current folder and Qt_Static is the install folder.

3. First you need to configure Qt using command
```
Qt_Source/configure.bat -debug-and-release -static -opengl desktop -platform win32-msvc -prefix "Qt_Static" -opensource -confirm-license -skip webengine -make libs -nomake tools -nomake tests -nomake examples
```

4. use
```
set CL=/MP
```
to make nmake multi-threaded

5. Build Qt using (or you may use jom here)
```
nmake
```

6. Install Qt using (or you may use jom here)
```
nmake install
```

7. Delete Qt_Build folder


### Configure QtCreator

1. Run QtCreator

2. Go to Tools->Options->Qt Versions and add statically build version of Qt 5.15.7.

3. Go to Tools->Options->Kits and add a kit for static version of the Qt. You should choose Microsoft Visual C++ Compiler (amd64) for C and C++ code. Make sure "Force UTF-8 MSVC compiler output" flag is checked. Use statically build Qt version for this kit.

4. Open TypingAnalysis.pro file in QtCreator.

5. Go to Project(icon on the left panel) Add "Conan install" as the first step before qmake for "Debug", "Release", and "Profile" modes.

6. If you compiled with static runtime, you should add an additional argument to Connan (in each building mode):
```
-s "compiler.runtime=MT"
```

### Build:

1. Clone the repo:

```
git clone --recurse-submodules https://github.com/DimaTrushin/InteractiveGUI.git
```

2. Open project in QtCreator and build it.
