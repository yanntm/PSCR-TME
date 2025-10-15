# TME4

This project resizes images in the specified input folder (default: `input_images`) to half their size and saves them in the specified output folder (default: `output_images`).

## Dependencies

This project requires Qt6. Install it as follows:

- **macOS**: `brew install qt6`
- **Ubuntu/Debian**: `sudo apt install qt6-base-dev`
- **Fedora/RHEL**: `sudo dnf install qt6-qtbase-devel`
- **Windows with WSL**: Follow Ubuntu instructions inside WSL.

## Build

1. Configure with CMake (Debug by default):
   ```
   mkdir build
   cd build
   cmake ..
   ```
   For Release build: `cmake -DCMAKE_BUILD_TYPE=Release ..`

2. Build:
   ```
   make
   ```

## Run

From the build directory:
```
./TME4 [input_folder] [output_folder]
```

The program reduces the size of images in the input folder to half their size and saves them in the output folder. If no arguments are provided, it defaults to "input_images" for input and "output_images" for output.

To have a test set, we provide `download_images.sh` that downloads a set of 175 MB of images to work with.
Or you can use your own photos if you prefer.

## Troubleshooting QT issues 

On some platforms, despite build working, vscode cannot find your includes for QT.
"CMake Tools" extension is supposed to read from CMake config, but it does not always work.
(Some people report that using "clangd" from llvm-vs-code-extensions instead of Microsoft Intellisense/CMake Tools patches the issue).

A workaround is to edit `.vscode/settings.json` which contains include folders :

```
    "C_Cpp.default.includePath": [
        "${workspaceFolder}/src",
        "/usr/include/qt6",
        "/usr/include/qt6/QtCore",
        "/usr/include/qt6/QtGui"
    ],
```

On my linux, adding these install folders to `C_Cpp.default.includePath` patched the issue.