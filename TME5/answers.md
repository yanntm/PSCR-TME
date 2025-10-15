# TME5 Correction: Parallelization of a Ray Tracer

## Baseline Sequential

### Question 1
Extracted TME5.zip from Moodle, added to repo, committed and pushed.

Configured project with CMake as previous TMEs. No dependencies, all handmade.

### Question 2

Ran `build/TME5` and generated `spheres.bmp`.

Platform: 

Temps/baseline choisi :

## With Manual Threads

### Question 3
Implemented `void renderThreadPerPixel(const Scene& scene, Image& img)` in Renderer.

mesures


### Question 4
Implemented `void renderThreadPerRow(const Scene& scene, Image& img)` in Renderer.

mesures


### Question 5
Implemented `void renderThreadManual(const Scene& scene, Image& img, int nbthread)` in Renderer.

mesures


## With Thread Pool

### Question 6
Queue class: blocking by default, can switch to non-blocking.

### Question 7
Pool class: constructor with queue size, start, stop.
Job abstract class with virtual run().

### Question 8
PixelJob: derives from Job, captures ?TODO?

renderPoolPixel: 

Mode "-m PoolPixel" with -n.

mesures

### Question 9
LineJob: derives from Job, captures TODO

renderPoolRow: 

Mode "-m PoolRow -n nbthread".

mesures

### Question 10
Best:

## Bonus

### Question 11

pool supportant soumission de lambda.