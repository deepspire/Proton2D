# Proton2D

<p align="center">
<img alt="GitHub last Main commit" src="https://img.shields.io/github/last-commit/deepspire/Proton2D/main">
<img alt="GitHub repo size" src="https://img.shields.io/github/repo-size/deepspire/Proton2D">
</p>

<p align="center">
<img alt="Windows support" src="https://img.shields.io/badge/Windows-works-green">
<img alt="Linux support" src="https://img.shields.io/badge/Linux-works-green">
<img alt="Android support" src="https://img.shields.io/badge/Android-works-green">
</p>

## андройд тест
скачиваем android-sdk, jdk-17, jdk-8 и android-ndk (jdk8 нужен для принятии sdkmanager лицензии)
```bash
sudo pacman -S jdk17-openjdk jdk8-openjdk
yay -S android-sdk android-ndk android-sdk-platform-tools
```

потом в `~/.bashrc` (или чем вы пользуетесь) надо добавить где-нибудь строчки
```bash
export ANDROID_NDK_HOME=/opt/android-ndk
export ANDROID_HOME=/opt/android-sdk
export PATH=$PATH:$ANDROID_HOME/platform-tools
```

на арче путь до **JDK8** обычно `/usr/lib/jvm/java-8-openjdk`

еще на арче нужно дать себе права на /opt/android-sdk и на /opt/android-ndk

(обычно пользователь:группа это просто имя пользователя, например, santer:santer)
```bash
sudo chown -R пользователь:группа /opt/android-sdk
sudo chown -R пользователь:группа /opt/android-ndk
```

принимаем лицензии (просто отвечайте y):
```bash
JAVA_HOME="/usr/lib/jvm/java-8-openjdk" sdkmanager --licenses
```

для билда:
```bash
cd android_build
JAVA_HOME=/usr/lib/jvm/java-17-openjdk ./gradlew assembleDebug
```

для релиз билда:
```bash
JAVA_HOME=/usr/lib/jvm/java-17-openjdk ./gradlew assembleRelease
```

для инсталла через ADB:
```bash
JAVA_HOME=/usr/lib/jvm/java-17-openjdk ./gradlew installDebug
```

для инсталла релиза через ADB:
```bash
JAVA_HOME=/usr/lib/jvm/java-17-openjdk ./gradlew releaseDebug
```

## Cloning
```bash
git clone --recurse-submodules https://github.com/deepspire/Proton2D.git
cd Proton2D
git submodule update --init --recursive
```

if any new external submodule added:
```bash
git submodule update --remote --recursive
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
