#ifndef OPACITY_H_
    #define OPACITY_H_

    #include <stdlib.h>
    #include <stdio.h>
    #include <math.h>

    #define FILE_MAX_BRIGHTNESS "/sys/class/backlight/intel_backlight/max_brightness"
    #define FILE_BRIGHTNESS "/sys/class/backlight/intel_backlight/brightness"

    double getOpacity();

#endif