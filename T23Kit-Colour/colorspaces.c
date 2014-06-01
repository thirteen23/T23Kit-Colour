/*
 *  colorspaces.c
 *  T23Kit-Colour
 *
 *  Created by Michael Van Milligan on 4/11/14.
 *  Copyright (c) 2014 Thirteen23. All rights reserved.
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <tgmath.h>
#include "colorspaces.h"

#define epsilon 0.008856f
#define kappa 7.787f

#define M_2PI (2.0f * M_PI)
#define DEG_TO_RAD(A) ((A) * (M_PI / 180.0))
#define RAD_TO_DEG(R) ((R) * (180.0 / M_PI))

#define COLORSPACE_LOG(format, ...) fprintf(stderr, format, __VA_ARGS__)

/*
 * sRGB matrices to XYZ with (Observer = 2°, Illuminant = D65)
 *
 * See http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
 */
static pixel_t RGB2XYZ[3] = {{0.4124564f, 0.2126729f, 0.0193339f, 0.0f},
                             {0.3575761f, 0.7151522f, 0.1191920f, 0.0f},
                             {0.1804375f, 0.0721750f, 0.9503041f, 0.0f}};

static pixel_t XYZ2RGB[3] = {{3.2404542f, -0.9692660f, 0.0556434f, 0.0f},
                             {-1.5371385f, 1.8760108f, -0.2040259f, 0.0f},
                             {-0.4985314f, 0.0415560f, 1.0572252f, 0.0f}};

#pragma mark - Prototypes

static inline color_val_t neg_pow(color_val_t a, color_val_t p);
static inline color_val_t neg_powf(color_val_t a, color_val_t p);

static inline void sanitize_rgb(pixel_t *rgb);

static inline void apply_working_space_matrix(pixel_t p, pixel_t m0, pixel_t m1,
                                              pixel_t m2, pixel_t *result);
static inline color_val_t color_min(color_val_t a, color_val_t b,
                                    color_val_t c);
static inline color_val_t color_max(color_val_t a, color_val_t b,
                                    color_val_t c);

static color_val_t hue2rgb(color_val_t v1, color_val_t v2, color_val_t vH);

#pragma mark - Utilities

static inline color_val_t neg_pow(color_val_t b, color_val_t e) {
  return (0.0f <= b) ? pow(b, e) : -1.0f * pow(-1.0f * b, e);
}

static inline color_val_t neg_powf(color_val_t b, color_val_t e) {
  return (0.0f <= b) ? powf(b, e) : -1.0f * powf(-1.0f * b, e);
}

static inline void sanitize_rgb(pixel_t *rgb) {
  color_val_t *r = &(rgb->a), *g = &(rgb->b), *b = &(rgb->c);

  *r = ((0.0f > *r) || (1.0f < *r)) ? -1.0f : *r;
  *g = ((0.0f > *g) || (1.0f < *g)) ? -1.0f : *g;
  *b = ((0.0f > *b) || (1.0f < *b)) ? -1.0f : *b;
}

static inline void apply_working_space_matrix(pixel_t p, pixel_t m0, pixel_t m1,
                                              pixel_t m2, pixel_t *result) {
  result->a = (p.a * m0.a) + (p.b * m1.a) + (p.c * m2.a);
  result->b = (p.a * m0.b) + (p.b * m1.b) + (p.c * m2.b);
  result->c = (p.a * m0.c) + (p.b * m1.c) + (p.c * m2.c);
}

static inline color_val_t color_min(color_val_t a, color_val_t b,
                                    color_val_t c) {
  color_val_t m = a;
  if (m > b)
    m = b;
  if (m > c)
    m = c;
  return m;
}

static inline color_val_t color_max(color_val_t a, color_val_t b,
                                    color_val_t c) {
  color_val_t m = a;
  if (m < b)
    m = b;
  if (m < c)
    m = c;
  return m;
}

color_val_t hue2rgb(color_val_t v1, color_val_t v2, color_val_t vH) {

  if (0.0f > vH) {
    vH += 1.0f;
  }

  if (1.0f < vH) {
    vH -= 1.0f;
  }

  return (1.0f > (6.0f * vH))
             ? (v1 + (v2 - v1) * 6.0f * vH)
             : (1.0f > (2.0f * vH))
                   ? (v2)
                   : (2.0f > (3.0f * vH))
                         ? (v1 + (v2 - v1) * ((2.0f / 3.0f) - vH) * 6.0f)
                         : (v1);
}

void print_pixel(pixel_t p) {
  fprintf(stderr, "A: %f B: %f C: %f\n", p.a, p.b, p.c);
}
void _print_pixel_double_(pixel_t p) { print_pixel(p); }
void _print_pixel_float_(pixel_t p) { print_pixel(p); }

#pragma mark - To RGB

void hsl2rgb(pixel_t hsl, pixel_t *rgb) {
  color_val_t var_1, var_2, *r, *g, *b;

  r = &(rgb->a);
  g = &(rgb->b);
  b = &(rgb->c);

  if (0.0 == hsl.b) {
    *r = *g = *b = 1.0f;
  } else {

    var_2 = (0.5f > hsl.c) ? hsl.c * (1.0f + hsl.b)
                           : (hsl.c + hsl.b) - (hsl.b * hsl.c);

    var_1 = 2.0f * hsl.c - var_2;

    *r = hue2rgb(var_1, var_2, hsl.a + (1.0f / 3.0f));
    *g = hue2rgb(var_1, var_2, hsl.a);
    *b = hue2rgb(var_1, var_2, hsl.a - (1.0f / 3.0f));
  }

  sanitize_rgb(rgb);
}
void _hsl2rgb_double_(pixel_t hsl, pixel_t *rgb) { hsl2rgb(hsl, rgb); }
void _hsl2rgb_float_(pixel_t hsl, pixel_t *rgb) { hsl2rgb(hsl, rgb); }

void _hsv2rgb_double_(pixel_t hsv, pixel_t *rgb) {
  color_val_t h, s, v, f, p, q, t, *r, *g, *b;
  int i;

  h = RAD_TO_DEG(hsv.a);
  s = hsv.b;
  v = hsv.c;

  r = &(rgb->a);
  g = &(rgb->b);
  b = &(rgb->c);

  if (s == 0.0f) {
    *r = *g = *b = v;
    return;
  }

  h /= 60.0f;
  i = (int)floor(h);
  f = h - i;
  p = v * (1.0f - s);
  q = v * (1.0f - s * f);
  t = v * (1.0f - s * (1.0f - f));

  switch (i) {
  case 0:
    *r = v;
    *g = t;
    *b = p;
    break;
  case 1:
    *r = q;
    *g = v;
    *b = p;
    break;
  case 2:
    *r = p;
    *g = v;
    *b = t;
    break;
  case 3:
    *r = p;
    *g = q;
    *b = v;
    break;
  case 4:
    *r = t;
    *g = p;
    *b = v;
    break;
  default:
    *r = v;
    *g = p;
    *b = q;
    break;
  }
  sanitize_rgb(rgb);
}

void _hsv2rgb_float_(pixel_t hsv, pixel_t *rgb) {
  color_val_t h, s, v, f, p, q, t, *r, *g, *b;
  int i;

  h = RAD_TO_DEG(hsv.a);
  s = hsv.b;
  v = hsv.c;

  r = &(rgb->a);
  g = &(rgb->b);
  b = &(rgb->c);

  if (s == 0.0f) {
    *r = *g = *b = v;
    return;
  }

  h /= 60.0f;
  i = (int)floorf(h);
  f = h - i;
  p = v * (1.0f - s);
  q = v * (1.0f - s * f);
  t = v * (1.0f - s * (1.0f - f));

  switch (i) {
  case 0:
    *r = v;
    *g = t;
    *b = p;
    break;
  case 1:
    *r = q;
    *g = v;
    *b = p;
    break;
  case 2:
    *r = p;
    *g = v;
    *b = t;
    break;
  case 3:
    *r = p;
    *g = q;
    *b = v;
    break;
  case 4:
    *r = t;
    *g = p;
    *b = v;
    break;
  default:
    *r = v;
    *g = p;
    *b = q;
    break;
  }
  sanitize_rgb(rgb);
}

void _lch_ab2rgb_double_(pixel_t lch_ab, pixel_t *rgb) {
  pixel_t lab = {0.0f};

  _lch_ab2lab_double_(lch_ab, &lab);
  _lab2rgb_double_(lab, rgb);
}

void _lch_ab2rgb_float_(pixel_t lch_ab, pixel_t *rgb) {
  pixel_t lab = {0.0f};

  _lch_ab2lab_float_(lch_ab, &lab);
  _lab2rgb_float_(lab, rgb);
}

void _lch_uv2rgb_double_(pixel_t lch_uv, pixel_t *rgb) {
  pixel_t lab = {0.0f};

  _lch_uv2lab_double_(lch_uv, &lab);
  _lab2rgb_double_(lab, rgb);
}

void _lch_uv2rgb_float_(pixel_t lch_uv, pixel_t *rgb) {
  pixel_t lab = {0.0f};

  _lch_uv2lab_float_(lch_uv, &lab);
  _lab2rgb_float_(lab, rgb);
}

void _hsi2rgb_double_(pixel_t hsi, pixel_t *rgb) {
  color_val_t *r, *g, *b, h, s, i;

  // h [0, 2π], everyone else [0, 1]

  h = hsi.a;
  s = hsi.b;
  i = hsi.c;

  r = &(rgb->a);
  g = &(rgb->b);
  b = &(rgb->c);

  if (0.0f <= h && (M_2PI / 3.0f) >= h) {

    *b = (1.0f / 3.0f) * (1.0f - s);
    *r = (1.0f / 3.0f) * ((s * cos(h)) / cos((M_2PI / 6.0f) - h));
    *g = 1.0f - ((*b) + (*r));

  } else if ((M_2PI / 3.0f) < h && ((2.0f * M_2PI) / 3.0f) >= h) {

    h -= (M_2PI / 3.0);
    *r = (1.0 / 3.0) * (1.0 - s);
    *g = (1.0 / 3.0) * ((s * cos(h)) / cos((M_2PI / 6.0f) - h));
    *b = 1.0 - ((*g) + (*r));

  } else {

    h -= (2.0 * M_2PI / 3.0);
    *g = (1.0 / 3.0) * (1.0 - s);
    *b = (1.0 / 3.0) * ((s * cos(h)) / cos((M_2PI / 6.0f) - h));
    *r = 1.0 - ((*g) + (*b));
  }

  sanitize_rgb(rgb);
}

void _hsi2rgb_float_(pixel_t hsi, pixel_t *rgb) {
  color_val_t *r, *g, *b, h, s, i;

  // h [0, 2π], everyone else [0, 1]

  h = hsi.a;
  s = hsi.b;
  i = hsi.c;

  r = &(rgb->a);
  g = &(rgb->b);
  b = &(rgb->c);

  if (0.0f <= h && (M_2PI / 3.0f) >= h) {

    *b = (1.0f / 3.0f) * (1.0f - s);
    *r = (1.0f / 3.0f) * ((s * cosf(h)) / cosf((M_2PI / 6.0f) - h));
    *g = 1.0f - ((*b) + (*r));

  } else if ((M_2PI / 3.0f) < h && ((2.0f * M_2PI) / 3.0f) >= h) {

    h -= (M_2PI / 3.0);
    *r = (1.0 / 3.0) * (1.0 - s);
    *g = (1.0 / 3.0) * ((s * cosf(h)) / cosf((M_2PI / 6.0f) - h));
    *b = 1.0 - ((*g) + (*r));

  } else {

    h -= (2.0 * M_2PI / 3.0);
    *g = (1.0 / 3.0) * (1.0 - s);
    *b = (1.0 / 3.0) * ((s * cosf(h)) / cosf((M_2PI / 6.0f) - h));
    *r = 1.0 - ((*g) + (*b));
  }

  sanitize_rgb(rgb);
}

void _lab2rgb_double_(pixel_t lab, pixel_t *rgb) {
  pixel_t xyz = {0.0f};

  _lab2xyz_double_(lab, &xyz);
  _xyz2rgb_double_(xyz, rgb);
  sanitize_rgb(rgb);
}

void _lab2rgb_float_(pixel_t lab, pixel_t *rgb) {
  pixel_t xyz = {0.0f};

  _lab2xyz_float_(lab, &xyz);
  _xyz2rgb_float_(xyz, rgb);
  sanitize_rgb(rgb);
}

void _hlab2rgb_double_(pixel_t hlab, pixel_t *rgb) {}

void _hlab2rgb_float_(pixel_t hlab, pixel_t *rgb) {}

void _cmyk2rgb_double_(pixel_t cmyk, pixel_t *rgb) {
  color_val_t c, m, y, k, *r, *g, *b;

  c = cmyk.a;
  m = cmyk.b;
  y = cmyk.c;
  k = cmyk.d;

  r = &(rgb->a);
  g = &(rgb->b);
  b = &(rgb->c);

  *r = (1.0f - c) * (1.0f - k);
  *g = (1.0f - m) * (1.0f - k);
  *b = (1.0f - y) * (1.0f - k);

  sanitize_rgb(rgb);
}

void _cmyk2rgb_float_(pixel_t cmyk, pixel_t *rgb) {
  color_val_t c, m, y, k, *r, *g, *b;

  c = cmyk.a;
  m = cmyk.b;
  y = cmyk.c;
  k = cmyk.d;

  r = &(rgb->a);
  g = &(rgb->b);
  b = &(rgb->c);

  *r = 1.0f - (c + k);
  *g = 1.0f - (m + k);
  *b = 1.0f - (y + k);

  sanitize_rgb(rgb);
}

void _xyz2rgb_double_(pixel_t xyz, pixel_t *rgb) {
  color_val_t *r, *g, *b, R, G, B;
  pixel_t var_xyz = {0.0f};

  var_xyz.a = xyz.a / 100.0f;
  var_xyz.b = xyz.b / 100.0f;
  var_xyz.c = xyz.c / 100.0f;

  apply_working_space_matrix(var_xyz, XYZ2RGB[0], XYZ2RGB[1], XYZ2RGB[2], rgb);

  r = &(rgb->a);
  g = &(rgb->b);
  b = &(rgb->c);

  R = rgb->a;
  G = rgb->b;
  B = rgb->c;

  R = (fabs(R) <= 0.0031308f) ? 12.92f * R
                              : 1.055f * neg_pow(R, (1.0f / 2.4f)) - 0.055f;
  G = (fabs(G) <= 0.0031308f) ? 12.92f * G
                              : 1.055f * neg_pow(G, (1.0f / 2.4f)) - 0.055f;
  B = (fabs(B) <= 0.0031308f) ? 12.92f * B
                              : 1.055f * neg_pow(B, (1.0f / 2.4f)) - 0.055f;

  *r = R;
  *g = G;
  *b = B;

  sanitize_rgb(rgb);
}

void _xyz2rgb_float_(pixel_t xyz, pixel_t *rgb) {
  color_val_t *r, *g, *b, R, G, B;
  pixel_t var_xyz = {0.0f};

  var_xyz.a = xyz.a / 100.0f;
  var_xyz.b = xyz.b / 100.0f;
  var_xyz.c = xyz.c / 100.0f;

  apply_working_space_matrix(var_xyz, XYZ2RGB[0], XYZ2RGB[1], XYZ2RGB[2], rgb);

  r = &(rgb->a);
  g = &(rgb->b);
  b = &(rgb->c);

  R = rgb->a;
  G = rgb->b;
  B = rgb->c;

  R = (fabsf(R) <= 0.0031308f) ? 12.92f * R
                               : 1.055f * neg_powf(R, (1.0f / 2.4f)) - 0.055f;
  G = (fabsf(G) <= 0.0031308f) ? 12.92f * G
                               : 1.055f * neg_powf(G, (1.0f / 2.4f)) - 0.055f;
  B = (fabsf(B) <= 0.0031308f) ? 12.92f * B
                               : 1.055f * neg_powf(B, (1.0f / 2.4f)) - 0.055f;

  *r = R;
  *g = G;
  *b = B;

  sanitize_rgb(rgb);
}

#pragma mark - To CMYK

void rgb2cmyk(pixel_t rgb, pixel_t *cmyk) {
  color_val_t max, *c, *m, *y, *k;

  c = &(cmyk->a);
  m = &(cmyk->b);
  y = &(cmyk->c);
  k = &(cmyk->d);

  max = color_max(rgb.a, rgb.b, rgb.c);

  *k = 1.0f - max;
  *c = (1.0f - rgb.a - *k) / (1.0f - *k);
  *m = (1.0f - rgb.b - *k) / (1.0f - *k);
  *y = (1.0f - rgb.c - *k) / (1.0f - *k);
}
void _rgb2cmyk_double_(pixel_t rgb, pixel_t *cmyk) { rgb2cmyk(rgb, cmyk); }
void _rgb2cmyk_float_(pixel_t rgb, pixel_t *cmyk) { rgb2cmyk(rgb, cmyk); }

#pragma mark - To HSV

void rgb2hsv(pixel_t rgb, pixel_t *hsv) {
  color_val_t max, min, delta, r, g, b, *h, *s, *v;

  h = &(hsv->a);
  s = &(hsv->b);
  v = &(hsv->c);

  r = rgb.a;
  g = rgb.b;
  b = rgb.c;

  min = color_min(r, g, b);
  max = color_max(r, g, b);

  *v = max;
  delta = (max - min);

  if (0.0f > max) {
    *s = 0.0f;
    *h = NAN;
    return;
  }

  *s = (delta / max);

  if (r >= max) {
    *h = (g - b) / delta;
  } else if (g >= max) {
    *h = 2.0f + (b - r) / delta;
  } else {
    *h = 4.0f + (r - g) / delta;
  }

  *h *= 60.0f;

  if (0.0f > *h) {
    *h += RAD_TO_DEG(M_2PI);
  }

  *h = DEG_TO_RAD(*h);
}
void _rgb2hsv_double_(pixel_t rgb, pixel_t *hsv) { rgb2hsv(rgb, hsv); }
void _rgb2hsv_float_(pixel_t rgb, pixel_t *hsv) { rgb2hsv(rgb, hsv); }

#pragma mark - To HSL

void rgb2hsl(pixel_t rgb, pixel_t *hsl) {
  color_val_t max, min, delta, *h, *s, *l, dr, dg, db;

  h = &(hsl->a);
  s = &(hsl->b);
  l = &(hsl->c);

  min = color_min(rgb.a, rgb.b, rgb.c);
  max = color_max(rgb.a, rgb.b, rgb.c);
  delta = max - min;

  *l = (max + min) / 2.0f;

  if (max == 0.0f) {
    *h = *s = 0.0f;
  } else {
    *s = (0.5f > *l) ? delta / (max + min) : delta / (2 - max - min);

    dr = (((max - rgb.a) / 6.0f) + (delta / 2.0f)) / delta;
    dg = (((max - rgb.b) / 6.0f) + (delta / 2.0f)) / delta;
    db = (((max - rgb.c) / 6.0f) + (delta / 2.0f)) / delta;

    if (rgb.a == max) {
      *h = db - dg;
    } else if (rgb.b == max) {
      *h = (1.0f / 3.0f) + dr - db;
    } else if (rgb.c == max) {
      *h = (2.0f / 3.0f) + dg - dr;
    }

    *h = (0.0f > *h) ? *h += 1.0f : (1.0f < *h) ? *h -= 1.0f : *h;
  }
}
void _rgb2hsl_double_(pixel_t rgb, pixel_t *hsl) { rgb2hsl(rgb, hsl); }
void _rgb2hsl_float_(pixel_t rgb, pixel_t *hsl) { rgb2hsl(rgb, hsl); }

#pragma mark - To HSI

void _rgb2hsi_double_(pixel_t rgb, pixel_t *hsi) {
  color_val_t *h, *s, *i, r, g, b, max, min, delta;

  h = &(hsi->a);
  s = &(hsi->b);
  i = &(hsi->c);

  r = rgb.a;
  g = rgb.b;
  b = rgb.c;

  min = color_min(r, g, b);
  max = color_max(r, g, b);
  delta = max - min;

  *i = (1.0f / 3.0f) * (r + b + g);
  if (0.0f == delta) {
    *h = 0.0f;
    *s = 0.0f;
  } else {

    *h = (max == r)
             ? fmod(((g - b) / delta), 6.0f)
             : (max == g) ? (b - r) / delta + 2.0f : (r - g) / delta + 4.0f;
    *h *= 60.0f;
    *h = (DEG_TO_RAD(*h) / M_2PI);
    *h = (0.0f > *h) ? *h += 1.0f : (1.0f < *h) ? *h -= 1.0f : *h;
    *s = 1.0f - (min / (*i));
  }
}

void _rgb2hsi_float_(pixel_t rgb, pixel_t *hsi) {
  color_val_t *h, *s, *i, r, g, b, max, min, delta;

  h = &(hsi->a);
  s = &(hsi->b);
  i = &(hsi->c);

  r = rgb.a;
  g = rgb.b;
  b = rgb.c;

  min = color_min(r, g, b);
  max = color_max(r, g, b);
  delta = max - min;

  *i = (1.0f / 3.0f) * (r + b + g);
  if (0.0f == delta) {
    *h = 0.0f;
    *s = 0.0f;
  } else {

    *h = (max == r)
             ? fmodf(((g - b) / delta), 6.0f)
             : (max == g) ? (b - r) / delta + 2.0f : (r - g) / delta + 4.0f;
    *h *= 60.0f;
    *h = (DEG_TO_RAD(*h) / M_2PI);
    *h = (0.0f > *h) ? *h += 1.0f : (1.0f < *h) ? *h -= 1.0f : *h;
    *s = 1.0f - (min / (*i));
  }
}

#pragma mark - to Hunter LAB

void _rgb2hlab_double_(pixel_t rgb, pixel_t *hlab) {
  pixel_t xyz = {0.0f};

  _rgb2xyz_double_(rgb, &xyz);
  _xyz2hlab_double_(xyz, hlab);
}

void _rgb2hlab_float_(pixel_t rgb, pixel_t *hlab) {
  pixel_t xyz = {0.0f};

  _rgb2xyz_float_(rgb, &xyz);
  _xyz2hlab_float_(xyz, hlab);
}

void _xyz2hlab_double_(pixel_t xyz, pixel_t *hlab) {
  color_val_t *hl, *a, *b;

  hl = &(hlab->a);
  a = &(hlab->b);
  b = &(hlab->c);

  *hl = 10.0f * sqrt(xyz.b);
  *a = 17.5f * (((1.02f * xyz.a) - xyz.b) / sqrt(xyz.b));
  *b = 7.0f * ((xyz.b - (0.847f * xyz.c)) / sqrt(xyz.b));
}

void _xyz2hlab_float_(pixel_t xyz, pixel_t *hlab) {
  color_val_t *hl, *a, *b;

  hl = &(hlab->a);
  a = &(hlab->b);
  b = &(hlab->c);

  *hl = 10.0f * sqrtf(xyz.b);
  *a = 17.5f * (((1.02f * xyz.a) - xyz.b) / sqrtf(xyz.b));
  *b = 7.0f * ((xyz.b - (0.847f * xyz.c)) / sqrtf(xyz.b));
}

#pragma mark - To LAB

void _rgb2lab_double_(pixel_t rgb, pixel_t *lab) {
  pixel_t xyz = {0.0f};

  _rgb2xyz_double_(rgb, &xyz);
  _xyz2lab_double_(xyz, lab);
}

void _rgb2lab_float_(pixel_t rgb, pixel_t *lab) {
  pixel_t xyz = {0.0f};

  _rgb2xyz_float_(rgb, &xyz);
  _xyz2lab_float_(xyz, lab);
}

void _xyz2lab_double_(pixel_t xyz, pixel_t *lab) {
  color_val_t x, y, z;

  x = xyz.a / 95.047f;
  y = xyz.b / 100.000f;
  z = xyz.c / 108.883f;

  x = (x > epsilon) ? pow(x, 1.0f / 3.0f) : (kappa * x) + (16.0f / 116.0f);
  y = (y > epsilon) ? pow(y, 1.0f / 3.0f) : (kappa * y) + (16.0f / 116.0f);
  z = (z > epsilon) ? pow(z, 1.0f / 3.0f) : (kappa * z) + (16.0f / 116.0f);

  lab->a = (116.0f * y) - 16.0f;
  lab->b = 500.0f * (x - y);
  lab->c = 200.0f * (y - z);
}

void _xyz2lab_float_(pixel_t xyz, pixel_t *lab) {
  color_val_t x, y, z;

  x = xyz.a / 95.047f;
  y = xyz.b / 100.000f;
  z = xyz.c / 108.883f;

  x = (x > epsilon) ? powf(x, 1.0f / 3.0f) : (kappa * x) + (16.0f / 116.0f);
  y = (y > epsilon) ? powf(y, 1.0f / 3.0f) : (kappa * y) + (16.0f / 116.0f);
  z = (z > epsilon) ? powf(z, 1.0f / 3.0f) : (kappa * z) + (16.0f / 116.0f);

  lab->a = (116.0f * y) - 16.0f;
  lab->b = 500.0f * (x - y);
  lab->c = 200.0f * (y - z);
}

void _lch_ab2lab_double_(pixel_t lch_ab, pixel_t *lab) {
  color_val_t l_, c, h, *l, *a, *b;

  l_ = lch_ab.a;
  c = lch_ab.b;
  h = lch_ab.c;

  l = &(lab->a);
  a = &(lab->b);
  b = &(lab->c);

  *l = l_;
  *a = c * cos(DEG_TO_RAD(h));
  *b = c * sin(DEG_TO_RAD(h));
}

void _lch_ab2lab_float_(pixel_t lch_ab, pixel_t *lab) {
  color_val_t l_, c, h, *l, *a, *b;

  l_ = lch_ab.a;
  c = lch_ab.b;
  h = lch_ab.c;

  l = &(lab->a);
  a = &(lab->b);
  b = &(lab->c);

  *l = l_;
  *a = c * cosf(DEG_TO_RAD(h));
  *b = c * sinf(DEG_TO_RAD(h));
}

void _lch_uv2lab_double_(pixel_t lch_uv, pixel_t *lab) {
  pixel_t luv = {0.0f}, xyz = {0.0f};

  _lch_uv2luv_double_(lch_uv, &luv);
  _luv2xyz_double_(luv, &xyz);
  _xyz2lab_double_(xyz, lab);
}

void _lch_uv2lab_float_(pixel_t lch_uv, pixel_t *lab) {
  pixel_t luv = {0.0f}, xyz = {0.0f};

  _lch_uv2luv_float_(lch_uv, &luv);
  _luv2xyz_float_(luv, &xyz);
  _xyz2lab_float_(xyz, lab);
}

#pragma mark - To LUV

void _rgb2luv_double_(pixel_t rgb, pixel_t *luv) {
  pixel_t xyz = {0.0f};

  _rgb2xyz_double_(rgb, &xyz);
  _xyz2luv_double_(xyz, luv);
}

void _rgb2luv_float_(pixel_t rgb, pixel_t *luv) {
  pixel_t xyz = {0.0f};

  _rgb2xyz_float_(rgb, &xyz);
  _xyz2luv_float_(xyz, luv);
}

void _xyz2luv_double_(pixel_t xyz, pixel_t *luv) {
  color_val_t var_Y, var_U, ref_U, var_V, ref_V, *l, *u, *v;

  l = &(luv->a);
  u = &(luv->b);
  v = &(luv->c);

  var_U = (4.0f * xyz.a) / (xyz.a + (15.0f * xyz.b) + (3.0f * xyz.c));
  var_V = (9.0f * xyz.b) / (xyz.a + (15.0f * xyz.b) + (3.0f * xyz.c));

  var_Y = (xyz.b / 100.0f);
  var_Y = (epsilon < var_Y) ? pow(var_Y, (1.0f / 3.0f))
                            : (kappa * var_Y) + (16.0f / 116.0f);

  ref_U = (4.0f * 95.047f) / (95.047f + (15.0f * 100.0f) + (3.0f * 108.883f));
  ref_V = (9.0f * 100.0f) / (95.047f + (15.0f * 100.0f) + (3.0f * 108.883f));

  *l = (116.0f * var_Y) - 16.0f;
  *u = 13.0f * (*l) * (var_U - ref_U);
  *v = 13.0f * (*l) * (var_V - ref_V);
}

void _xyz2luv_float_(pixel_t xyz, pixel_t *luv) {
  color_val_t var_Y, var_U, ref_U, var_V, ref_V, *l, *u, *v;

  l = &(luv->a);
  u = &(luv->b);
  v = &(luv->c);

  var_U = (4.0f * xyz.a) / (xyz.a + (15.0f * xyz.b) + (3.0f * xyz.c));
  var_V = (9.0f * xyz.b) / (xyz.a + (15.0f * xyz.b) + (3.0f * xyz.c));

  var_Y = (xyz.b / 100.0f);
  var_Y = (epsilon < var_Y) ? powf(var_Y, (1.0f / 3.0f))
                            : (kappa * var_Y) + (16.0f / 116.0f);

  ref_U = (4.0f * 95.047f) / (95.047f + (15.0f * 100.0f) + (3.0f * 108.883f));
  ref_V = (9.0f * 100.0f) / (95.047f + (15.0f * 100.0f) + (3.0f * 108.883f));

  *l = (116.0f * var_Y) - 16.0f;
  *u = 13.0f * (*l) * (var_U - ref_U);
  *v = 13.0f * (*l) * (var_V - ref_V);
}

void _lch_ab2luv_double_(pixel_t lch_ab, pixel_t *luv) {
  pixel_t lab = {0.0f}, xyz = {0.0f};

  _lch_ab2lab_double_(lch_ab, &lab);
  _lab2xyz_double_(lab, &xyz);
  _xyz2luv_double_(xyz, luv);
}

void _lch_ab2luv_float_(pixel_t lch_ab, pixel_t *luv) {
  pixel_t lab = {0.0f}, xyz = {0.0f};

  _lch_ab2lab_float_(lch_ab, &lab);
  _lab2xyz_float_(lab, &xyz);
  _xyz2luv_float_(xyz, luv);
}

void _lch_uv2luv_double_(pixel_t lch_uv, pixel_t *luv) {
  color_val_t l_, c, h, *l, *u, *v;

  l_ = lch_uv.a;
  c = lch_uv.b;
  h = lch_uv.c;

  l = &(luv->a);
  u = &(luv->b);
  v = &(luv->c);

  *l = l_;
  *u = c * cos(DEG_TO_RAD(h));
  *v = c * sin(DEG_TO_RAD(h));
}

void _lch_uv2luv_float_(pixel_t lch_uv, pixel_t *luv) {
  color_val_t l_, c, h, *l, *u, *v;

  l_ = lch_uv.a;
  c = lch_uv.b;
  h = lch_uv.c;

  l = &(luv->a);
  u = &(luv->b);
  v = &(luv->c);

  *l = l_;
  *u = c * cosf(DEG_TO_RAD(h));
  *v = c * sinf(DEG_TO_RAD(h));
}

#pragma mark - To LCH(AB)

void _rgb2lch_ab_double_(pixel_t rgb, pixel_t *lch_ab) {
  pixel_t lab = {0.0f};

  _rgb2lab_double_(rgb, &lab);
  _lab2lch_ab_double_(lab, lch_ab);
}

void _rgb2lch_ab_float_(pixel_t rgb, pixel_t *lch_ab) {
  pixel_t lab = {0.0f};

  _rgb2lab_float_(rgb, &lab);
  _lab2lch_ab_float_(lab, lch_ab);
}

void _lab2lch_ab_double_(pixel_t lab, pixel_t *lch_ab) {

  lch_ab->a = lab.a;

  lch_ab->b = sqrt(pow(lab.b, 2.0f) + pow(lab.c, 2.0f));
  lch_ab->c = RAD_TO_DEG(atan2(lab.c, lab.b));

  while (lch_ab->c < 0.0f) {
    lch_ab->c += RAD_TO_DEG(M_2PI);
  }
  while (lch_ab->c > RAD_TO_DEG(M_2PI)) {
    lch_ab->c -= RAD_TO_DEG(M_2PI);
  }

  lch_ab->c /= RAD_TO_DEG(M_2PI);
}

void _lab2lch_ab_float_(pixel_t lab, pixel_t *lch_ab) {

  lch_ab->a = lab.a;

  lch_ab->b = sqrtf(powf(lab.b, 2.0f) + powf(lab.c, 2.0f));
  lch_ab->c = RAD_TO_DEG(atan2f(lab.c, lab.b));

  while (lch_ab->c < 0.0f) {
    lch_ab->c += RAD_TO_DEG(M_2PI);
  }
  while (lch_ab->c > RAD_TO_DEG(M_2PI)) {
    lch_ab->c -= RAD_TO_DEG(M_2PI);
  }

  lch_ab->c /= RAD_TO_DEG(M_2PI);
}

void _luv2lch_ab_double_(pixel_t luv, pixel_t *lch_ab) {
  pixel_t lab = {0.0f}, xyz = {0.0f};

  _luv2xyz_double_(luv, &xyz);
  _xyz2lab_double_(xyz, &lab);
  _lab2lch_ab_double_(lab, lch_ab);
}

void _luv2lch_ab_float_(pixel_t luv, pixel_t *lch_ab) {
  pixel_t lab = {0.0f}, xyz = {0.0f};

  _luv2xyz_float_(luv, &xyz);
  _xyz2lab_float_(xyz, &lab);
  _lab2lch_ab_float_(lab, lch_ab);
}

#pragma mark - To LCH(UV)

void _rgb2lch_uv_double_(pixel_t rgb, pixel_t *lch_uv) {
  pixel_t lab = {0.0f};

  _rgb2lab_double_(rgb, &lab);
  _lab2lch_uv_double_(lab, lch_uv);
}

void _rgb2lch_uv_float_(pixel_t rgb, pixel_t *lch_uv) {
  pixel_t lab = {0.0f};

  _rgb2lab_float_(rgb, &lab);
  _lab2lch_uv_float_(lab, lch_uv);
}

void _luv2lch_uv_double_(pixel_t luv, pixel_t *lch_uv) {
  lch_uv->a = luv.a;

  lch_uv->b = sqrt(pow(luv.b, 2.0f) + pow(luv.c, 2.0f));
  lch_uv->c = RAD_TO_DEG(atan2(luv.c, luv.b));

  while (lch_uv->c < 0.0f) {
    lch_uv->c += RAD_TO_DEG(M_2PI);
  }
  while (lch_uv->c > RAD_TO_DEG(M_2PI)) {
    lch_uv->c -= RAD_TO_DEG(M_2PI);
  }

  lch_uv->c /= RAD_TO_DEG(M_2PI);
}

void _luv2lch_uv_float_(pixel_t luv, pixel_t *lch_uv) {
  lch_uv->a = luv.a;

  lch_uv->b = sqrtf(powf(luv.b, 2.0f) + powf(luv.c, 2.0f));
  lch_uv->c = RAD_TO_DEG(atan2f(luv.c, luv.b));

  while (lch_uv->c < 0.0f) {
    lch_uv->c += RAD_TO_DEG(M_2PI);
  }
  while (lch_uv->c > RAD_TO_DEG(M_2PI)) {
    lch_uv->c -= RAD_TO_DEG(M_2PI);
  }

  lch_uv->c /= RAD_TO_DEG(M_2PI);
}

void _lab2lch_uv_double_(pixel_t lab, pixel_t *lch_uv) {
  pixel_t luv = {0.0f}, xyz = {0.0f};

  _lab2xyz_double_(lab, &xyz);
  _xyz2luv_double_(xyz, &luv);
  _luv2lch_uv_double_(luv, lch_uv);
}

void _lab2lch_uv_float_(pixel_t lab, pixel_t *lch_uv) {
  pixel_t luv = {0.0f}, xyz = {0.0f};

  _lab2xyz_float_(lab, &xyz);
  _xyz2luv_float_(xyz, &luv);
  _luv2lch_uv_float_(luv, lch_uv);
}

#pragma mark - To XYZ

void xyY2xyz(pixel_t xyY, pixel_t *xyz) {
  xyz->b = xyY.c;

  if (xyY.b != 0.0f) {
    xyz->a = xyY.a * xyY.c / xyY.b;
    xyz->c = (1.0f - xyY.a - xyY.b) * xyY.c / xyY.b;
  } else {
    xyz->a = xyz->b = xyz->c = 0.0f;
  }
}
void _xyY2xyz_double_(pixel_t xyY, pixel_t *xyz) { xyY2xyz(xyY, xyz); }
void _xyY2xyz_float_(pixel_t xyY, pixel_t *xyz) { xyY2xyz(xyY, xyz); }

void _rgb2xyz_double_(pixel_t rgb, pixel_t *xyz) {
  pixel_t p = {0.0f};

  p.a = rgb.a;
  p.b = rgb.b;
  p.c = rgb.c;

  p.a = (fabs(p.a) <= 0.04045f) ? p.a / 12.92f
                                : neg_pow((p.a + 0.055f) / 1.055f, 2.4f);
  p.b = (fabs(p.b) <= 0.04045f) ? p.b / 12.92f
                                : neg_pow((p.b + 0.055f) / 1.055f, 2.4f);
  p.c = (fabs(p.c) <= 0.04045f) ? p.c / 12.92f
                                : neg_pow((p.c + 0.055f) / 1.055f, 2.4f);

  p.a *= 100.0f;
  p.b *= 100.0f;
  p.c *= 100.0f;

  apply_working_space_matrix(p, RGB2XYZ[0], RGB2XYZ[1], RGB2XYZ[2], xyz);
}

void _rgb2xyz_float_(pixel_t rgb, pixel_t *xyz) {
  pixel_t p = {0.0f};

  p.a = rgb.a;
  p.b = rgb.b;
  p.c = rgb.c;

  p.a = (fabsf(p.a) <= 0.04045f) ? p.a / 12.92f
                                 : neg_powf((p.a + 0.055f) / 1.055f, 2.4f);
  p.b = (fabsf(p.b) <= 0.04045f) ? p.b / 12.92f
                                 : neg_powf((p.b + 0.055f) / 1.055f, 2.4f);
  p.c = (fabsf(p.c) <= 0.04045f) ? p.c / 12.92f
                                 : neg_powf((p.c + 0.055f) / 1.055f, 2.4f);

  p.a *= 100.0f;
  p.b *= 100.0f;
  p.c *= 100.0f;

  apply_working_space_matrix(p, RGB2XYZ[0], RGB2XYZ[1], RGB2XYZ[2], xyz);
}

void _hlab2xyz_double_(pixel_t hlab, pixel_t *xyz) {
  color_val_t *x, *y, *z, var_X, var_Y, var_Z;

  var_Y = hlab.a / 10.0f;
  var_X = hlab.b / 17.5f * hlab.a / 10.0f;
  var_Z = hlab.c / 7.0f * hlab.a / 10.0f;

  *y = pow(var_Y, 2.0);
  *x = (var_X + *y) / 1.02f;
  *z = -1 * (var_Z - *y) / 0.847f;
}

void _hlab2xyz_float_(pixel_t hlab, pixel_t *xyz) {
  color_val_t *x, *y, *z, var_X, var_Y, var_Z;

  var_Y = hlab.a / 10.0f;
  var_X = hlab.b / 17.5f * hlab.a / 10.0f;
  var_Z = hlab.c / 7.0f * hlab.a / 10.0f;

  *y = powf(var_Y, 2.0);
  *x = (var_X + *y) / 1.02f;
  *z = -1 * (var_Z - *y) / 0.847f;
}

void _lab2xyz_double_(pixel_t lab, pixel_t *xyz) {
  color_val_t x, y, z;

  y = (lab.a + 16.0f) / 116.0f;
  x = lab.b / 500.0f + y;
  z = y - lab.c / 200.0f;

  y = (pow(y, 3.0f) > epsilon) ? pow(y, 3.0f) : (y - 16.0f / 116.0f) / kappa;
  x = (pow(x, 3.0f) > epsilon) ? pow(x, 3.0f) : (x - 16.0f / 116.0f) / kappa;
  z = (pow(z, 3.0f) > epsilon) ? pow(z, 3.0f) : (z - 16.0f / 116.0f) / kappa;

  xyz->a = x * 95.047f;
  xyz->b = y * 100.000f;
  xyz->c = z * 108.883f;
}

void _lab2xyz_float_(pixel_t lab, pixel_t *xyz) {
  color_val_t x, y, z;

  y = (lab.a + 16.0f) / 116.0f;
  x = lab.b / 500.0f + y;
  z = y - lab.c / 200.0f;

  y = (powf(y, 3.0f) > epsilon) ? powf(y, 3.0f) : (y - 16.0f / 116.0f) / kappa;
  x = (powf(x, 3.0f) > epsilon) ? powf(x, 3.0f) : (x - 16.0f / 116.0f) / kappa;
  z = (powf(z, 3.0f) > epsilon) ? powf(z, 3.0f) : (z - 16.0f / 116.0f) / kappa;

  xyz->a = x * 95.047f;
  xyz->b = y * 100.000f;
  xyz->c = z * 108.883f;
}

void _luv2xyz_double_(pixel_t luv, pixel_t *xyz) {
  color_val_t var_Y, var_U, ref_U, var_V, ref_V, *x, *y, *z;

  var_Y = (luv.a + 16.0f) / 116.0f;

  var_Y = (epsilon < pow(var_Y, 3.0f)) ? pow(var_Y, 3.0f)
                                       : (var_Y - 16.0f / 116.0f) / kappa;

  ref_U = (4.0f * 95.047f) / (95.047f + (15.0f * 100.0f) + (3.0f * 108.883f));
  ref_V = (9.0f * 100.0f) / (95.047f + (15.0f * 100.0f) + (3.0f * 108.883f));

  var_U = luv.b / (13.0f * luv.a) + ref_U;
  var_V = luv.c / (13.0f * luv.a) + ref_V;

  *y = var_Y * 100.0f;
  *x = -1.0f * (9.0f * (*y) * var_U) / ((var_U - 4.0f) * var_V - var_U * var_V);
  *z = (9.0f * (*y) - (15.0f * var_V * (*y)) - (var_V * (*x))) / (3.0f * var_V);
}

void _luv2xyz_float_(pixel_t luv, pixel_t *xyz) {
  color_val_t var_Y, var_U, ref_U, var_V, ref_V, *x, *y, *z;

  var_Y = (luv.a + 16.0f) / 116.0f;

  var_Y = (epsilon < powf(var_Y, 3.0f)) ? powf(var_Y, 3.0f)
                                        : (var_Y - 16.0f / 116.0f) / kappa;

  ref_U = (4.0f * 95.047f) / (95.047f + (15.0f * 100.0f) + (3.0f * 108.883f));
  ref_V = (9.0f * 100.0f) / (95.047f + (15.0f * 100.0f) + (3.0f * 108.883f));

  var_U = luv.b / (13.0f * luv.a) + ref_U;
  var_V = luv.c / (13.0f * luv.a) + ref_V;

  *y = var_Y * 100.0f;
  *x = -1.0f * (9.0f * (*y) * var_U) / ((var_U - 4.0f) * var_V - var_U * var_V);
  *z = (9.0f * (*y) - (15.0f * var_V * (*y)) - (var_V * (*x))) / (3.0f * var_V);
}

#pragma mark - Distance Formulas

void _cie76_double_(pixel_t lab, pixel_t plab, color_val_t *dl) {
  *dl = sqrt(pow(plab.a - lab.a, 2.0f) + pow(plab.b - lab.b, 2.0f) +
             pow(plab.c - lab.c, 2.0f));
}

void _cie76_float_(pixel_t lab, pixel_t plab, color_val_t *dl) {
  *dl = sqrtf(powf(plab.a - lab.a, 2.0f) + powf(plab.b - lab.b, 2.0f) +
              powf(plab.c - lab.c, 2.0f));
}

void _cmc1984_double_(pixel_t lab, pixel_t plab, color_val_t l, color_val_t c,
                      color_val_t *dl) {
  color_val_t C_1, C_2, C_d, H_1, H_d_ab2, L_d, S_L, S_C, S_H, F, T;

  L_d = (lab.a - plab.a);

  C_1 = sqrt(pow(lab.b, 2.0f) + pow(lab.c, 2.0f));
  C_2 = sqrt(pow(plab.b, 2.0f) + pow(plab.c, 2.0f));
  C_d = (C_1 - C_2);

  H_d_ab2 =
      (pow(lab.b - plab.b, 2.0f) + pow(lab.c - plab.c, 2.0f) - pow(C_d, 2.0f));

  F = sqrt(pow(C_1, 4.0f) / (pow(C_1, 4.0f) + 1900.0f));

  S_L = (16.0f <= lab.a) ? (lab.a * 0.040975f) / (1.0f + (lab.a * 0.01765f))
                         : 0.511f;

  S_C = ((C_1 * 0.0638f) / (1.0f + (C_1 * 0.0131f))) + 0.638f;

  H_1 = RAD_TO_DEG(atan2(lab.c, lab.b));
  while (H_1 < 0.0f) {
    H_1 += RAD_TO_DEG(M_2PI);
  }
  while (H_1 > RAD_TO_DEG(M_2PI)) {
    H_1 -= RAD_TO_DEG(M_2PI);
  }

  T = (164.0f <= H_1 && 345.0f >= H_1)
          ? 0.56f + fabs(0.2f * cos(DEG_TO_RAD(H_1 + 168.0f)))
          : 0.36f + fabs(0.4f * cos(DEG_TO_RAD(H_1 + 35.0f)));

  S_H = S_C * (F * T + 1 - F);

  /*
   * Commonly used values are 2:1 for acceptability and 1:1 for the threshold
   * of imperceptibility.
   */

  *dl = sqrt(pow((L_d / (l * S_L)), 2.0f) + pow((C_d / (c * S_C)), 2.0f) +
             (H_d_ab2 / pow(S_H, 2.0f)));
}

void _cmc1984_float_(pixel_t lab, pixel_t plab, color_val_t l, color_val_t c,
                     color_val_t *dl) {
  color_val_t C_1, C_2, C_d, H_1, H_d_ab2, L_d, S_L, S_C, S_H, F, T;

  L_d = (lab.a - plab.a);

  C_1 = sqrtf(powf(lab.b, 2.0f) + powf(lab.c, 2.0f));
  C_2 = sqrtf(powf(plab.b, 2.0f) + powf(plab.c, 2.0f));
  C_d = (C_1 - C_2);

  H_d_ab2 = (powf(lab.b - plab.b, 2.0f) + powf(lab.c - plab.c, 2.0f) -
             powf(C_d, 2.0f));

  F = sqrtf(powf(C_1, 4.0f) / (powf(C_1, 4.0f) + 1900.0f));

  S_L = (16.0f <= lab.a) ? (lab.a * 0.040975f) / (1.0f + (lab.a * 0.01765f))
                         : 0.511f;

  S_C = ((C_1 * 0.0638f) / (1.0f + (C_1 * 0.0131f))) + 0.638f;

  H_1 = RAD_TO_DEG(atan2f(lab.c, lab.b));
  while (H_1 < 0.0f) {
    H_1 += RAD_TO_DEG(M_2PI);
  }
  while (H_1 > RAD_TO_DEG(M_2PI)) {
    H_1 -= RAD_TO_DEG(M_2PI);
  }

  T = (164.0f <= H_1 && 345.0f >= H_1)
          ? 0.56f + fabsf(0.2f * cosf(DEG_TO_RAD(H_1 + 168.0f)))
          : 0.36f + fabsf(0.4f * cosf(DEG_TO_RAD(H_1 + 35.0f)));

  S_H = S_C * (F * T + 1 - F);

  /*
   * Commonly used values are 2:1 for acceptability and 1:1 for the threshold
   * of imperceptibility.
   */

  *dl = sqrtf(powf((L_d / (l * S_L)), 2.0f) + powf((C_d / (c * S_C)), 2.0f) +
              (H_d_ab2 / powf(S_H, 2.0f)));
}

void _cie94_double_(pixel_t lab, pixel_t plab, int media, color_val_t *dl) {
  color_val_t D_H_ab2, S_L, S_C, S_H, L_d, C_1, C_2, C_d, K_1, K_2, K_L, K_C,
      K_H;

  L_d = (lab.a - plab.a);

  C_1 = sqrt(pow(lab.b, 2.0f) + pow(lab.c, 2.0f));
  C_2 = sqrt(pow(plab.b, 2.0f) + pow(plab.c, 2.0f));
  C_d = (C_1 - C_2);

  D_H_ab2 = (pow((lab.b - plab.b), 2.0f) + pow((lab.c - plab.c), 2.0f) -
             pow(C_d, 2.0f));

  K_L = (LIBCOLORSPACES_TEXTILES != media) ? 1.0f : 2.0f;
  K_C = K_H = 1.0f;

  K_1 = (LIBCOLORSPACES_TEXTILES != media) ? 0.045f : 0.048f;
  K_2 = (LIBCOLORSPACES_TEXTILES != media) ? 0.015f : 0.014f;

  S_L = 1.0f;
  S_C = 1.0f + (K_1 * C_1);
  S_H = 1.0f + (K_2 * C_1);

  *dl = sqrt(pow(L_d / (K_L * S_L), 2.0f) + pow(C_d / (K_C * S_C), 2.0f) +
             (D_H_ab2 / (pow(K_H, 2.0f) * pow(S_H, 2.0f))));
}

void _cie94_float_(pixel_t lab, pixel_t plab, int media, color_val_t *dl) {
  color_val_t D_H_ab2, S_L, S_C, S_H, L_d, C_1, C_2, C_d, K_1, K_2, K_L, K_C,
      K_H;

  L_d = (lab.a - plab.a);

  C_1 = sqrtf(powf(lab.b, 2.0f) + powf(lab.c, 2.0f));
  C_2 = sqrtf(powf(plab.b, 2.0f) + powf(plab.c, 2.0f));
  C_d = (C_1 - C_2);

  D_H_ab2 = (powf((lab.b - plab.b), 2.0f) + powf((lab.c - plab.c), 2.0f) -
             powf(C_d, 2.0f));

  K_L = (LIBCOLORSPACES_TEXTILES != media) ? 1.0f : 2.0f;
  K_C = K_H = 1.0f;

  K_1 = (LIBCOLORSPACES_TEXTILES != media) ? 0.045f : 0.048f;
  K_2 = (LIBCOLORSPACES_TEXTILES != media) ? 0.015f : 0.014f;

  S_L = 1.0f;
  S_C = 1.0f + (K_1 * C_1);
  S_H = 1.0f + (K_2 * C_1);

  *dl = sqrtf(powf(L_d / (K_L * S_L), 2.0f) + powf(C_d / (K_C * S_C), 2.0f) +
              (D_H_ab2 / (powf(K_H, 2.0f) * powf(S_H, 2.0f))));
}

void _ciede2000_double_(pixel_t lab, pixel_t plab, color_val_t kl,
                        color_val_t kc, color_val_t kh, color_val_t *dl) {
  color_val_t L_b_p, L_d_p, L_term, a_1_p, a_2_p, C_1_p, C_2_p, C_1_s, C_2_s,
      C_sb_p, C_b_p, C_d_p, C_term, G, h_1_p, h_2_p, h_d_p, H_d_p, h_b_p,
      h_d_p_abs, h_d_p_sum, H_term, T, S_L, S_C, S_H, R_C, R_T, R_term, T_d,
      E_d_00;

  /* Calculate C_i_p, h_i_p */

  C_1_s = sqrt(pow(lab.b, 2.0f) + pow(lab.c, 2.0f));
  C_2_s = sqrt(pow(plab.b, 2.0f) + pow(plab.c, 2.0f));
  C_sb_p = (C_1_s + C_2_s) / 2.0f;

  G = 0.5f *
      (1.0f - sqrt(pow(C_sb_p, 7.0f) / (pow(C_sb_p, 7.0f) + pow(25.0f, 7.0f))));

  a_1_p = lab.b + (G * lab.b);
  a_2_p = plab.b + (G * plab.b);

  C_1_p = sqrt(pow(a_1_p, 2.0f) + pow(lab.c, 2.0f));
  C_2_p = sqrt(pow(a_2_p, 2.0f) + pow(plab.c, 2.0f));

  /* Apparently you must work in degrees from here on out? */
  h_1_p = (0.0f == C_1_p) ? 0.0f : RAD_TO_DEG(atan2(lab.c, a_1_p));
  while (h_1_p < 0.0f) {
    h_1_p += RAD_TO_DEG(M_2PI);
  }
  while (h_1_p > RAD_TO_DEG(M_2PI)) {
    h_1_p -= RAD_TO_DEG(M_2PI);
  }

  h_2_p = (0.0f == C_2_p) ? 0.0f : RAD_TO_DEG(atan2(plab.c, a_2_p));
  while (h_2_p < 0.0f) {
    h_2_p += RAD_TO_DEG(M_2PI);
  }
  while (h_2_p > RAD_TO_DEG(M_2PI)) {
    h_2_p -= RAD_TO_DEG(M_2PI);
  }

  /* Calculate L_d_p, C_d_p, and H_d_p */

  L_d_p = (plab.a - lab.a);
  C_d_p = (C_2_p - C_1_p);

  if (0.0f == (C_1_p * C_2_p)) {
    h_d_p = 0.0f;
  } else {
    h_d_p = (180.0f >= fabs(h_2_p - h_1_p))
                ? (h_2_p - h_1_p)
                : (180.0f < (h_2_p - h_1_p))
                      ? ((h_2_p - h_1_p) - RAD_TO_DEG(M_2PI))
                      : (-180.0f > (h_2_p - h_1_p))
                            ? ((h_2_p - h_1_p) + RAD_TO_DEG(M_2PI))
                            : 0.0f;
  }

  H_d_p = 2.0f * sqrt(C_1_p * C_2_p) * sin(DEG_TO_RAD(h_d_p / 2.0f));

  /* Calculate CIEDE2000 Color-Difference E_d_00 */

  L_b_p = (lab.a + plab.a) / 2.0f;
  C_b_p = (C_1_p + C_2_p) / 2.0f;

  h_d_p_abs = fabs(h_1_p - h_2_p);
  h_d_p_sum = (h_1_p + h_2_p);

  if (0.0f == (C_1_p * C_2_p)) {
    h_b_p = h_d_p_sum;
  } else {
    h_b_p =
        (180.0f >= h_d_p_abs)
            ? (h_d_p_sum / 2.0f)
            : ((180.0f < h_d_p_abs) && (RAD_TO_DEG(M_2PI) > h_d_p_sum))
                  ? ((h_d_p_sum + RAD_TO_DEG(M_2PI)) / 2.0f)
                  : ((180.0f < h_d_p_abs) && (RAD_TO_DEG(M_2PI) <= h_d_p_sum))
                        ? ((h_d_p_sum - RAD_TO_DEG(M_2PI)) / 2.0f)
                        : NAN;
  }

  T = 1.0f - (0.17f * cos(DEG_TO_RAD(h_b_p - 30.0f))) +
      (0.24f * cos(DEG_TO_RAD(2.0f * h_b_p))) +
      (0.32f * cos(DEG_TO_RAD((3.0f * h_b_p) + 6.0f))) -
      (0.20f * cos(DEG_TO_RAD((4.0f * h_b_p) - 63.0f)));

  T_d = 30.0f * exp(-1.0f * pow((h_b_p - 275.0f) / 25.0f, 2.0f));

  R_C = 2.0f * sqrt(pow(C_b_p, 7.0f) / ((pow(C_b_p, 7.0f) + pow(25.0f, 7.0f))));

  S_L = 1.0f + ((0.015f * pow(L_b_p - 50.0f, 2.0f)) /
                sqrt(20.0f + pow(L_b_p - 50.0f, 2.0f)));

  S_C = 1.0f + (0.045f * C_b_p);

  S_H = 1.0f + (0.015f * C_b_p * T);

  R_T = -1.0f * sin(DEG_TO_RAD(2.0f * T_d)) * R_C;

  L_term = pow(L_d_p / (kl * S_L), 2.0f);

  C_term = pow(C_d_p / (kc * S_C), 2.0f);

  H_term = pow(H_d_p / (kh * S_H), 2.0f);

  R_term = R_T * (C_d_p / (kc * S_C)) * (H_d_p / (kh * S_H));

  E_d_00 = sqrt(L_term + C_term + H_term + R_term);

  *dl = E_d_00;

  //  COLORSPACE_LOG("L*1: %f\n", lab.a);
  //  COLORSPACE_LOG("L*2: %f\n", plab.a);
  //  COLORSPACE_LOG("a*1: %f\n", lab.b);
  //  COLORSPACE_LOG("a*2: %f\n", plab.b);
  //  COLORSPACE_LOG("b*1: %f\n", lab.c);
  //  COLORSPACE_LOG("b*2: %f\n", plab.c);
  //  COLORSPACE_LOG("a'1: %f\n", a_1_p);
  //  COLORSPACE_LOG("a'2: %f\n", a_2_p);
  //  COLORSPACE_LOG("C'1: %f\n", C_1_p);
  //  COLORSPACE_LOG("C'2: %f\n", C_2_p);
  //  COLORSPACE_LOG("h'1: %f\n", h_1_p);
  //  COLORSPACE_LOG("h'2: %f\n", h_2_p);
  //  COLORSPACE_LOG("hBAR': %f\n", h_b_p);
  //  COLORSPACE_LOG("G: %f\n", G);
  //  COLORSPACE_LOG("T: %f\n", T);
  //  COLORSPACE_LOG("S_L: %f\n", S_L);
  //  COLORSPACE_LOG("S_C: %f\n", S_C);
  //  COLORSPACE_LOG("S_H: %f\n", S_H);
  //  COLORSPACE_LOG("R_T: %f\n", R_T);
  //  COLORSPACE_LOG("E_d_00: %f\n", E_d_00);
}

void _ciede2000_float_(pixel_t lab, pixel_t plab, color_val_t kl,
                       color_val_t kc, color_val_t kh, color_val_t *dl) {
  color_val_t L_b_p, L_d_p, L_term, a_1_p, a_2_p, C_1_p, C_2_p, C_1_s, C_2_s,
      C_sb_p, C_b_p, C_d_p, C_term, G, h_1_p, h_2_p, h_d_p, H_d_p, h_b_p,
      h_d_p_abs, h_d_p_sum, H_term, T, S_L, S_C, S_H, R_C, R_T, R_term, T_d,
      E_d_00;

  /* Calculate C_i_p, h_i_p */

  C_1_s = sqrtf(powf(lab.b, 2.0f) + powf(lab.c, 2.0f));
  C_2_s = sqrtf(powf(plab.b, 2.0f) + powf(plab.c, 2.0f));
  C_sb_p = (C_1_s + C_2_s) / 2.0f;

  G = 0.5f * (1.0f - sqrtf(powf(C_sb_p, 7.0f) /
                           (powf(C_sb_p, 7.0f) + powf(25.0f, 7.0f))));

  a_1_p = lab.b + (G * lab.b);
  a_2_p = plab.b + (G * plab.b);

  C_1_p = sqrtf(powf(a_1_p, 2.0f) + powf(lab.c, 2.0f));
  C_2_p = sqrtf(powf(a_2_p, 2.0f) + powf(plab.c, 2.0f));

  /* Apparently you must work in degrees from here on out? */
  h_1_p = (0.0f == C_1_p) ? 0.0f : RAD_TO_DEG(atan2f(lab.c, a_1_p));
  while (h_1_p < 0.0f) {
    h_1_p += RAD_TO_DEG(M_2PI);
  }
  while (h_1_p > RAD_TO_DEG(M_2PI)) {
    h_1_p -= RAD_TO_DEG(M_2PI);
  }

  h_2_p = (0.0f == C_2_p) ? 0.0f : RAD_TO_DEG(atan2f(plab.c, a_2_p));
  while (h_2_p < 0.0f) {
    h_2_p += RAD_TO_DEG(M_2PI);
  }
  while (h_2_p > RAD_TO_DEG(M_2PI)) {
    h_2_p -= RAD_TO_DEG(M_2PI);
  }

  /* Calculate L_d_p, C_d_p, and H_d_p */

  L_d_p = (plab.a - lab.a);
  C_d_p = (C_2_p - C_1_p);

  if (0.0f == (C_1_p * C_2_p)) {
    h_d_p = 0.0f;
  } else {
    h_d_p = (180.0f >= fabsf(h_2_p - h_1_p))
                ? (h_2_p - h_1_p)
                : (180.0f < (h_2_p - h_1_p))
                      ? ((h_2_p - h_1_p) - RAD_TO_DEG(M_2PI))
                      : (-180.0f > (h_2_p - h_1_p))
                            ? ((h_2_p - h_1_p) + RAD_TO_DEG(M_2PI))
                            : 0.0f;
  }

  H_d_p = 2.0f * sqrtf(C_1_p * C_2_p) * sinf(DEG_TO_RAD(h_d_p / 2.0f));

  /* Calculate CIEDE2000 Color-Difference E_d_00 */

  L_b_p = (lab.a + plab.a) / 2.0f;
  C_b_p = (C_1_p + C_2_p) / 2.0f;

  h_d_p_abs = fabsf(h_1_p - h_2_p);
  h_d_p_sum = (h_1_p + h_2_p);

  if (0.0f == (C_1_p * C_2_p)) {
    h_b_p = h_d_p_sum;
  } else {
    h_b_p =
        (180.0f >= h_d_p_abs)
            ? (h_d_p_sum / 2.0f)
            : ((180.0f < h_d_p_abs) && (RAD_TO_DEG(M_2PI) > h_d_p_sum))
                  ? ((h_d_p_sum + RAD_TO_DEG(M_2PI)) / 2.0f)
                  : ((180.0f < h_d_p_abs) && (RAD_TO_DEG(M_2PI) <= h_d_p_sum))
                        ? ((h_d_p_sum - RAD_TO_DEG(M_2PI)) / 2.0f)
                        : NAN;
  }

  T = 1.0f - (0.17f * cosf(DEG_TO_RAD(h_b_p - 30.0f))) +
      (0.24f * cosf(DEG_TO_RAD(2.0f * h_b_p))) +
      (0.32f * cosf(DEG_TO_RAD((3.0f * h_b_p) + 6.0f))) -
      (0.20f * cosf(DEG_TO_RAD((4.0f * h_b_p) - 63.0f)));

  T_d = 30.0f * expf(-1.0f * powf((h_b_p - 275.0f) / 25.0f, 2.0f));

  R_C = 2.0f *
        sqrtf(powf(C_b_p, 7.0f) / ((powf(C_b_p, 7.0f) + powf(25.0f, 7.0f))));

  S_L = 1.0f + ((0.015f * powf(L_b_p - 50.0f, 2.0f)) /
                sqrtf(20.0f + powf(L_b_p - 50.0f, 2.0f)));

  S_C = 1.0f + (0.045f * C_b_p);

  S_H = 1.0f + (0.015f * C_b_p * T);

  R_T = -1.0f * sinf(DEG_TO_RAD(2.0f * T_d)) * R_C;

  L_term = powf(L_d_p / (kl * S_L), 2.0f);

  C_term = powf(C_d_p / (kc * S_C), 2.0f);

  H_term = powf(H_d_p / (kh * S_H), 2.0f);

  R_term = R_T * (C_d_p / (kc * S_C)) * (H_d_p / (kh * S_H));

  E_d_00 = sqrtf(L_term + C_term + H_term + R_term);

  *dl = E_d_00;

  //  COLORSPACE_LOG("L*1: %f\n", lab.a);
  //  COLORSPACE_LOG("L*2: %f\n", plab.a);
  //  COLORSPACE_LOG("a*1: %f\n", lab.b);
  //  COLORSPACE_LOG("a*2: %f\n", plab.b);
  //  COLORSPACE_LOG("b*1: %f\n", lab.c);
  //  COLORSPACE_LOG("b*2: %f\n", plab.c);
  //  COLORSPACE_LOG("a'1: %f\n", a_1_p);
  //  COLORSPACE_LOG("a'2: %f\n", a_2_p);
  //  COLORSPACE_LOG("C'1: %f\n", C_1_p);
  //  COLORSPACE_LOG("C'2: %f\n", C_2_p);
  //  COLORSPACE_LOG("h'1: %f\n", h_1_p);
  //  COLORSPACE_LOG("h'2: %f\n", h_2_p);
  //  COLORSPACE_LOG("hBAR': %f\n", h_b_p);
  //  COLORSPACE_LOG("G: %f\n", G);
  //  COLORSPACE_LOG("T: %f\n", T);
  //  COLORSPACE_LOG("S_L: %f\n", S_L);
  //  COLORSPACE_LOG("S_C: %f\n", S_C);
  //  COLORSPACE_LOG("S_H: %f\n", S_H);
  //  COLORSPACE_LOG("R_T: %f\n", R_T);
  //  COLORSPACE_LOG("E_d_00: %f\n", E_d_00);
}