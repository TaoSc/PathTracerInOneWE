# PathTracerInOneWE

My implementation of https://raytracing.github.io/books/RayTracingInOneWeekend.html.

Saves results in a png image file and has a lot of command-line options to tweak its output.

Doesn't have any dependency and uses standard <ins>**C++ 17**</ins> code only, so it should easily compile on your platform of choice.

## Default Output
![Default Scene](https://github.com/TaoSc/PathTracerInOneWE/raw/master/output.png)

## Usage and command line options

```
PathTracerInOneWE [-f **filename** -t **threads** -w **width** -h **height** -s **samples per pixel** -b **max ray bounces** -fov **field of view** -cam **camera position** -look **camera look at position** -a **aperture** -focus **focus distance**]
```

#### Command line Options

- **filename** *(-f)*: Output filename without file extension (the program will automatically add .png). Default value: "output".
- **threads** *(-t)*: Numbers of threads on which to run the rendering. If **samples per pixel** < **threads**, processor utilization will not be maximized. If set to 0, will equal the number of physical threads exposed by your machine. Default value: 0
- **width** *(-w)*: Viewport width in pixels. Default value: 400
- **height** *(-h)*: Viewport height in pixels. Default value: 200
- **samples per pixel** *(-s)*: Self-explanatory. Dramatically affects rendering time. Default value: 50
- **max ray bounces** *(-b)*: Number of bounces till ray is aborted. 1 bounce corresponds to direct lightning only. Default value: 5
- **field of view** *(-fov)*: A float value for the camera field of view. Default value: 90
- **camera position** *(-cam)*: Three consecutive floats for the camera position. Default value: 0 0 0
- **camera look at position** *(-look)*: Three consecutive floats for the point the camera is pointing at. Default value: 0 0 -1
- **aperture** *(-a)*: The aperture size of the lens (float). Default value: 0
- **focus distance** *(-focus)*: Any object at this distance will be in focus (float). If set to 0, will equal the distance between the camera and the look at point. Default value: 0

Example command line and resulting output:
```
PathTracerInOneWE -s 40 -b 50 -fov 50 -cam -2 2 -3 -look 0 0 -1 -w 1280 -h 720
```
![Example Output](https://github.com/TaoSc/PathTracerInOneWE/raw/master/example.png)

*Rendering time: 33s (Core i5-4440@3.30 GHz)*

## References
My project uses stb_image_write.h to save the result image to png and stb_image.h to load texture images:
[stb](https://github.com/nothings/stb) (Public Domain)
