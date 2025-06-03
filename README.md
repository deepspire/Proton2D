# Burger2D
Burger2D

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

xmake will prefer to use msvc, to prevent this you need input this command
```pwsh
xmake f --toolchain=mingw
```
now you can build! this command will build as release
```pwsh
xmake
```

project not static so you need to add SDL's .dll in same folder as burger2d .exe

## Base requirements
- SDL3
- SDL3_image
- SDL3_ttf
