# Proton2D

<p align="center">
<img alt="GitHub last Main commit" src="https://img.shields.io/github/last-commit/deepspire/Proton2D/main">
<img alt="GitHub repo size" src="https://img.shields.io/github/repo-size/deepspire/Proton2D">
</p>

<p align="center">
<img alt="Windows support" src="https://img.shields.io/badge/Windows-works-green">
<img alt="Linux support" src="https://img.shields.io/badge/Linux-works-green">
<img alt="Android support" src="https://img.shields.io/badge/Android-TODO-red">
</p>

## Cloning
```bash
git clone --recurse-submodules https://github.com/deepspire/Proton2D.git
cd Proton2D
```

if external submodule updated/first time clone:
```bash
git submodule update --init --recursive
```

for VSCode, use [Clangd](https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd) extension instead of default Microsoft one.

## Android building (not working with static build)
you can do it the same way as on arch, any other linux distro or even windows, because it's almost same

You'll need to install android-sdk, jdk-17, jdk-8 and android-ndk (jdk8 is needed for sdkmanager licenses)
```bash
sudo pacman -S jdk17-openjdk jdk8-openjdk
yay -S android-sdk android-ndk android-sdk-platform-tools
```

Next, add env paths, for linux, in `~/.bashrc` (or whatever you use) also its necesarry to add:
```bash
export ANDROID_NDK_HOME=/opt/android-ndk
export ANDROID_HOME=/opt/android-sdk
export PATH=$PATH:$ANDROID_HOME/platform-tools
```

arch **JDK8** path is commonly `/usr/lib/jvm/java-8-openjdk`

also you may need to get access for /opt/android-sdk and /opt/android-ndk

(commonly, user:group is just a username, for example, santer:santer)
```bash
sudo chown -R user:group /opt/android-sdk
sudo chown -R user:group /opt/android-ndk
```

agree with licenses (just input y):
```bash
JAVA_HOME="/usr/lib/jvm/java-8-openjdk" sdkmanager --licenses
```

for debug build:
```bash
cd android_build
JAVA_HOME=/usr/lib/jvm/java-17-openjdk ./gradlew assembleDebug
```

for release build:
```bash
JAVA_HOME=/usr/lib/jvm/java-17-openjdk ./gradlew assembleRelease
```

for installing debug apk via ADB:
```bash
JAVA_HOME=/usr/lib/jvm/java-17-openjdk ./gradlew installDebug
```

for installing release apk via ADB:
```bash
JAVA_HOME=/usr/lib/jvm/java-17-openjdk ./gradlew releaseDebug
```

## Windows build guide
install `msys2`, `ninja`. (choco: `choco install msys2 ninja -y`)

open mingw64 terminal ("C:\tools\msys64\mingw64.exe" choco path)

input `pacman -Syu` and `pacman -S mingw-w64-x86_64-toolchain`. (hit enter if it asks selection)

now, with CMake vscode extension, select kit: `[Scan recursively for kits in specific directories (max depth: 5)]`, input `C:\tools\msys64\mingw64`. after scan, select mingw64 GCC.

now you can press `Build`, `Debug` and `Run` buttons in vscode.

## Dependencies
- SDL3
- SDL3_image
- SDL3_ttf
- [miniaudio](https://github.com/mackron/miniaudio)
- Ninja build system
