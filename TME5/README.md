# TME5

This project is a ray tracer that renders a scene with random spheres to a BMP image.

## Dependencies

None.

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
./TME5 [options]
```

The program renders a scene with random spheres and saves the image to a BMP file. Default output is `spheres.bmp` with 2000x2000 resolution and 250 spheres.

Options:
- `-o,--output`: Output BMP file name (default: spheres.bmp)
- `-W,--width`: Image width (default: 2000)
- `-H,--height`: Image height (default: 2000)
- `-s,--spheres`: Number of random spheres (default: 250)
- `-m,--mode`: Processing mode (default: sequential, options: sequential, ThreadPerPixel, ThreadPerRow, ThreadManual, PoolPixel, PoolRow, PoolFunctionalRow)
- `-n,--nbthread`: Number of threads (default: 4, used for threaded modes)