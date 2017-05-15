#include <math.h>

void hsv_to_rgb(int h, float s, float v, int* r, int* g, int* b)
{
    double p, q, t;

    int part = h / 60;
    int rem = h % 60;

    float fr, fg, fb;

    //double hh = h / 60.0;
    //int i = (int)hh;

    double ff = (float)rem / 60.;

    p = v * (1.0 - s);
    q = v * (1.0 - (s * ff));
    t = v * (1.0 - (s * (1.0 - ff)));

    switch(part)
    {
    case 0:
        fr = v;
        fg = t;
        fb = p;
        break;
    case 1:
        fr = q;
        fg = v;
        fb = p;
        break;
    case 2:
        fr = p;
        fg = v;
        fb = t;
        break;

    case 3:
        fr = p;
        fg = q;
        fb = v;
        break;
    case 4:
        fr = t;
        fg = p;
        fb = v;
        break;
    case 5:
    default:
        fr = v;
        fg = p;
        fb = q;
        break;
    }

    //scale for further conversion to 565
    *r = floor (fr * 31.0);
    *g = floor (fg * 63.0);
    *b = floor (fb * 31.0);
}
