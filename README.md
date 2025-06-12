# Proton2D
Proton2D

## Windows guide
### Requirements
install mingw (i prefer from choco so you wont need to set up `PATH`)
install xmake using powershell via command
```pwsh
Invoke-Expression (Invoke-Webrequest 'https://xmake.io/psget.text' -UseBasicParsing).Content
```

download [SDL3](https://github.com/libsdl-org/SDL/releases/latest), [SDL3 image](https://github.com/libsdl-org/SDL_image/releases/latest) and [SDL3 ttf support](https://github.com/libsdl-org/SDL_ttf/releases/latest).

(you need download the one with devel & mingw in name)

**now unpack all of them in C:/ folder with names `SDL3`, `SDL3_image` and `SDL3_ttf`**

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
- [miniaudio](https://github.com/mackron/miniaudio)
