# Proton2D

<p align="center">
<img alt="GitHub last Main commit" src="https://img.shields.io/github/last-commit/deepspire/Proton2D/main">
<img alt="GitHub repo size" src="https://img.shields.io/github/repo-size/deepspire/Proton2D">
</p>

<p align="center">
<img alt="Windows SUPPORTED" src="https://img.shields.io/badge/Windows-OUTDATED-orange">
<img alt="Linux SUPPORTED" src="https://img.shields.io/badge/Linux-SUPPORTED-green">
</p>

## Windows build guide (OUTDATED)
### Requirements
install mingw (i prefer from choco so you wont need to set up `PATH`)
install xmake using powershell via command
```pwsh
Invoke-Expression (Invoke-Webrequest 'https://xmake.io/psget.text' -UseBasicParsing).Content
```

download [SDL3](https://github.com/libsdl-org/SDL/releases/latest), [SDL3 image](https://github.com/libsdl-org/SDL_image/releases/latest) and [SDL3 ttf support](https://github.com/libsdl-org/SDL_ttf/releases/latest).

(you need download the one with devel & mingw in name)

**now unpack each library to C:/ folder with names `SDL3`, `SDL3_image` and `SDL3_ttf`**

xmake will prefer to use msvc, to prevent this you'll need to input this command
```pwsh
xmake f --toolchain=mingw
```
(if xmake couldn't then find it then use this)
```pwsh
xmake f --toolchain=mingw --mingw=msys64_folder_path/mingw64
```
now you can build! this command will build the release artifact
```pwsh
xmake
```

project is not static so you have to have SDL's .dll in the same folder as Proton2D binary

## Dependencies
- SDL3
- SDL3_image
- SDL3_ttf
- [SDL3_gfx](https://github.com/sabdul-khabir/SDL3_gfx) (you need to build it yourself & install & set: export PKG_CONFIG_PATH="/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH")
- [miniaudio](https://github.com/mackron/miniaudio)
- Ninja build system