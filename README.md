# PathTracerInOneWE

My implementation of https://raytracing.github.io/books/RayTracingInOneWeekend.html.

Saves results in a png image file and has a lot of command-line options to tweak its output.

Doesn't have any dependency and uses standard C++ code only, should easily compile on your platform of choice.

## Example Output
![Demo Scene](https://github.com/TaoSc/PathTracerInOneWE/raw/master/output.png)

## Usage and command line options

```
PathTracerInOneWE.exe [-f **filename** -w **width** -h **height** -s **samples per pixel** -b **max ray bounces**]
```


## References
My project uses stb_image_write.h to save the result image to png: 
[stb](https://github.com/nothings/stb) (Public Domain)
