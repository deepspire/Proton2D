# Proton2D

<p align="center">
<img alt="GitHub last Main commit" src="https://img.shields.io/github/last-commit/deepspire/Proton2D/main">
<img alt="GitHub repo size" src="https://img.shields.io/github/repo-size/deepspire/Proton2D">
</p>

<p align="center">
<img alt="Windows support" src="https://img.shields.io/badge/Windows-SUPPORTED-green">
<img alt="Linux support" src="https://img.shields.io/badge/Linux-SUPPORTED-green">
</p>

## Cloning
```pwsh
git clone --recurse-submodules https://github.com/deepspire/Proton2D.git
cd Proton2D
git submodule update --init --recursive
```

if any new external submodule added:
```pwsh
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
