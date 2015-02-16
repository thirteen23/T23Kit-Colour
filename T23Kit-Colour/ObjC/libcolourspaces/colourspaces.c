/*
 *  colourspaces.c
 *  T23Kit-Colour
 *
 *  Created by Michael Van Milligan on 4/11/14.
 *  Copyright (c) 2014 Thirteen23. All rights reserved.
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <tgmath.h>
#include "colourspaces.h"

typedef struct rgb_working_matrix {
  pixel_t rgb2xyz[3];
  pixel_t xyz2rgb[3];
} rgb_working_matrix;

/*
 *  RGB workspace matrices for converting to/from XYZ using Bradford adaptation
 *
 *  See http://www.brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html
 *
 *  Note: currently only sRGB D65 is supported but anyone reading this code can
 *  easily massage it for anything else.
 *
 */
static rgb_working_matrix rgb_working_matrices[colourspace_rgb_profile_max] = {
    /* 0 */
    { /* Adobe RGB (1998) D65 */
     {/* rgb2xyz */
      {0.5767309f, 0.1855540f, 0.1881852f, 0.0f},
      {0.2973769f, 0.6273491f, 0.0752741f, 0.0f},
      {0.0270343f, 0.0706872f, 0.9911085f, 0.0f}},
     {/* xyz2rgb */
      {2.0413690f, -0.5649464f, -0.3446944f, 0.0f},
      {-0.9692660f, 1.8760108f, 0.0415560f, 0.0f},
      {0.0134474f, -0.1183897f, 1.0154096f, 0.0f}}},
    /* 1 */
    { /* AppleRGB D65 */
     {/* rgb2xyz */
      {0.4497288f, 0.3162486f, 0.1844926f, 0.0f},
      {0.2446525f, 0.6720283f, 0.0833192f, 0.0f},
      {0.0251848f, 0.1411824f, 0.9224628f, 0.0f}},
     {/* xyz2rgb */
      {2.9515373f, -1.2894116f, -0.4738445f, 0.0f},
      {-1.0851093f, 1.9908566f, 0.0372026f, 0.0f},
      {0.0854934f, -0.2694964f, 1.0912975f, 0.0f}}},
    /* 2 */
    { /* Best RGB D50 */
     {/* rgb2xyz */
      {0.6326696f, 0.2045558f, 0.1269946f, 0.0f},
      {0.2284569f, 0.7373523f, 0.0341908f, 0.0f},
      {0.0000000f, 0.0095142f, 0.8156958f, 0.0f}},
     {/* xyz2rgb */
      {1.7552599f, -0.4836786f, -0.2530000f, 0.0f},
      {-0.5441336f, 1.5068789f, 0.0215528f, 0.0f},
      {0.0063467f, -0.0175761f, 1.2256959f, 0.0f}}},
    /* 3 */
    { /* Beta RGB D50 */
     {/* rgb2xyz */
      {0.6712537f, 0.1745834f, 0.1183829f, 0.0f},
      {0.3032726f, 0.6637861f, 0.0329413f, 0.0f},
      {0.0000000f, 0.0407010f, 0.7845090f, 0.0f}},
     {/* xyz2rgb */
      {1.6832270f, -0.4282363f, -0.2360185f, 0.0f},
      {-0.7710229f, 1.7065571f, 0.0446900f, 0.0f},
      {0.0400013f, -0.0885376f, 1.2723640f, 0.0f}}},
    /* 4 */
    { /* Bruce RGB D65 */
     {/* rgb2xyz */
      {0.4674162f, 0.2944512f, 0.1886026f, 0.0f},
      {0.2410115f, 0.6835475f, 0.0754410f, 0.0f},
      {0.0219101f, 0.0736128f, 0.9933071f, 0.0f}},
     {/* xyz2rgb */
      {2.7454669f, -1.1358136f, -0.4350269f, 0.0f},
      {-0.9692660f, 1.8760108f, 0.0415560f, 0.0f},
      {0.0112723f, -0.1139754f, 1.0132541f, 0.0f}}},
    /* 5 */
    { /* CIE RGB E */
     {/* rgb2xyz */
      {0.4887180f, 0.3106803f, 0.2006017f, 0.0f},
      {0.1762044f, 0.8129847f, 0.0108109f, 0.0f},
      {0.0000000f, 0.0102048f, 0.9897952f, 0.0f}},
     {/* xyz2rgb */
      {2.3706743f, -0.9000405f, -0.4706338f, 0.0f},
      {-0.5138850f, 1.4253036f, 0.0885814f, 0.0f},
      {0.0052982f, -0.0146949f, 1.0093968f, 0.0f}}},
    /* 6 */
    { /* ColorMatch RGB D50 */
     {/* rgb2xyz */
      {0.5093439f, 0.3209071f, 0.1339691f, 0.0f},
      {0.2748840f, 0.6581315f, 0.0669845f, 0.0f},
      {0.0242545f, 0.1087821f, 0.6921735f, 0.0f}},
     {/* xyz2rgb */
      {2.6422874f, -1.2234270f, -0.3930143f, 0.0f},
      {-1.1119763f, 2.0590183f, 0.0159614f, 0.0f},
      {0.0821699f, -0.2807254f, 1.4559877f, 0.0f}}},
    /* 7 */
    { /* Don RGB 4 D50 */
     {/* rgb2xyz */
      {0.6457711f, 0.1933511f, 0.1250978f, 0.0f},
      {0.2783496f, 0.6879702f, 0.0336802f, 0.0f},
      {0.0037113f, 0.0179861f, 0.8035125f, 0.0f}},
     {/* xyz2rgb */
      {1.7603902f, -0.4881198f, -0.2536126f, 0.0f},
      {-0.7126288f, 1.6527432f, 0.0416715f, 0.0f},
      {0.0078207f, -0.0347411f, 1.2447743f, 0.0f}}},
    /* 8 */
    { /* ECI RGB D50 */
     {/* rgb2xyz */
      {0.6502043f, 0.1780774f, 0.1359384f, 0.0f},
      {0.3202499f, 0.6020711f, 0.0776791f, 0.0f},
      {0.0000000f, 0.0678390f, 0.7573710f, 0.0f}},
     {/* xyz2rgb */
      {1.7827618f, -0.4969847f, -0.2690101f, 0.0f},
      {-0.9593623f, 1.9477962f, -0.0275807f, 0.0f},
      {0.0859317f, -0.1744674f, 1.3228273f, 0.0f}}},
    /* 9 */
    { /* Ekta Space PS5 D50 */
     {/* rgb2xyz */
      {0.5938914f, 0.2729801f, 0.0973485f, 0.0f},
      {0.2606286f, 0.7349465f, 0.0044249f, 0.0f},
      {0.0000000f, 0.0419969f, 0.7832131f, 0.0f}},
     {/* xyz2rgb */
      {2.0043819f, -0.7304844f, -0.2450052f, 0.0f},
      {-0.7110285f, 1.6202126f, 0.0792227f, 0.0f},
      {0.0381263f, -0.0868780f, 1.2725438f, 0.0f}}},
    /* 10 */
    { /* NTSC RGB C */
     {/* rgb2xyz */
      {0.6068909f, 0.1735011f, 0.2003480f, 0.0f},
      {0.2989164f, 0.5865990f, 0.1144845f, 0.0f},
      {0.0000000f, 0.0660957f, 1.1162243f, 0.0f}},
     {/* xyz2rgb */
      {1.9099961f, -0.5324542f, -0.2882091f, 0.0f},
      {-0.9846663f, 1.9991710f, -0.0283082f, 0.0f},
      {0.0583056f, -0.1183781f, 0.8975535f, 0.0f}}},
    /* 11 */
    { /* PAL/SECAM RGB D65 */
     {/* rgb2xyz */
      {0.4306190f, 0.3415419f, 0.1783091f, 0.0f},
      {0.2220379f, 0.7066384f, 0.0713236f, 0.0f},
      {0.0201853f, 0.1295504f, 0.9390944f, 0.0f}},
     {/* xyz2rgb */
      {3.0628971f, -1.3931791f, -0.4757517f, 0.0f},
      {-0.9692660f, 1.8760108f, 0.0415560f, 0.0f},
      {0.0678775f, -0.2288548f, 1.0693490, 0.0f}}},
    /* 12 */
    { /* ProPhoto RGB D50 */
     {/* rgb2xyz */
      {0.7976749f, 0.1351917f, 0.0313534f, 0.0f},
      {0.2880402f, 0.7118741f, 0.0000857f, 0.0f},
      {0.0000000f, 0.0000000f, 0.8252100f, 0.0f}},
     {/* xyz2rgb */
      {1.3459433f, -0.2556075f, -0.0511118f, 0.0f},
      {-0.5445989f, 1.5081673f, 0.0205351f, 0.0f},
      {0.0000000f, 0.0000000f, 1.2118128f, 0.0f}}},
    /* 13 */
    { /* SMPTE-C RGB D65 */
     {/* rgb2xyz */
      {0.3935891f, 0.3652497f, 0.1916313f, 0.0f},
      {0.2124132f, 0.7010437f, 0.0865432f, 0.0f},
      {0.0187423f, 0.1119313f, 0.9581563f, 0.0f}},
     {/* xyz2rgb */
      {3.5053960f, -1.7394894f, -0.5439640f, 0.0f},
      {-1.0690722f, 1.9778245f, 0.0351722f, 0.0f},
      {0.0563200f, -0.1970226f, 1.0502026f, 0.0f}}},
    /* 14 */
    { /* sRGB D65 */
     {/* rgb2xyz */
      {0.4124564f, 0.3575761f, 0.1804375f, 0.0f},
      {0.2126729f, 0.7151522f, 0.0721750f, 0.0f},
      {0.0193339f, 0.1191920f, 0.9503041f, 0.0f}},
     {/* xyz2rgb */
      {3.2404542f, -1.5371385f, -0.4985314f, 0.0f},
      {-0.9692660f, 1.8760108f, 0.0415560f, 0.0f},
      {0.0556434f, -0.2040259f, 1.0572252f, 0.0f}}},
    /* 15 */
    { /* Wide Gamut RGB D50 */
     {/* rgb2xyz */
      {0.7161046f, 0.1009296f, 0.1471858f, 0.0f},
      {0.2581874f, 0.7249378f, 0.0168748f, 0.0f},
      {0.0000000f, 0.0517813f, 0.7734287f, 0.0f}},
     {/* xyz2rgb */
      {1.4628067f, -0.1840623f, -0.2743606f, 0.0f},
      {-0.5217933f, 1.4472381f, 0.0677227f, 0.0f},
      {0.0349342f, -0.0968930f, 1.2884099f, 0.0f}}},
    /* 16 */
    { /* Adobe RGB (1998) D50 */
     {/* rgb2xyz */
      {0.6097559f, 0.2052401f, 0.1492240f, 0.0f},
      {0.3111242f, 0.6256560f, 0.0632197f, 0.0f},
      {0.0194811f, 0.0608902f, 0.7448387f, 0.0f}},
     {/* xyz2rgb */
      {1.9624274f, -0.6105343f, -0.3413404f, 0.0f},
      {-0.9787684f, 1.9161415f, 0.0334540f, 0.0f},
      {0.0286869f, -0.1406752f, 1.3487655f, 0.0f}}},
    /* 17 */
    { /* AppleRGB D50 */
     {/* rgb2xyz */
      {0.4755678f, 0.3396722f, 0.1489800f, 0.0f},
      {0.2551812f, 0.6725693f, 0.0722496f, 0.0f},
      {0.0184697f, 0.1133771f, 0.6933632f, 0.0f}},
     {/* xyz2rgb */
      {2.8510695f, -1.3605261f, -0.4708281f, 0.0f},
      {-1.0927680f, 2.0348871f, 0.0227598f, 0.0f},
      {0.1027403f, -0.2964984f, 1.4510659f, 0.0f}}},
    /* 18 */
    { /* Bruce RGB D50 */
     {/* rgb2xyz */
      {0.4941816f, 0.3204834f, 0.1495550f, 0.0f},
      {0.2521531f, 0.6844869f, 0.0633600f, 0.0f},
      {0.0157886f, 0.0629304f, 0.7464909f, 0.0f}},
     {/* xyz2rgb */
      {2.6502856f, -1.2014485f, -0.4289936f, 0.0f},
      {-0.9787684f, 1.9161415f, 0.0334540f, 0.0f},
      {0.0264570f, -0.1361227f, 1.3458542f, 0.0f}}},
    /* 19 */
    { /* CIE RGB D50 */
     {/* rgb2xyz */
      {0.4868870f, 0.3062984f, 0.1710347f, 0.0f},
      {0.1746583f, 0.8247541f, 0.0005877f, 0.0f},
      {-0.0012563f, 0.0169832f, 0.8094831f, 0.0f}},
     {/* xyz2rgb */
      {2.3638081f, -0.8676030f, -0.4988161f, 0.0f},
      {-0.5005940f, 1.3962369f, 0.1047562f, 0.0f},
      {0.0141712f, -0.0306400f, 1.2323842f, 0.0f}}},
    /* 20 */
    { /* NTSC RGB D50 */
     {/* rgb2xyz */
      {0.6343706f, 0.1852204f, 0.1446290f, 0.0f},
      {0.3109496f, 0.5915984f, 0.0974520f, 0.0f},
      {-0.0011817f, 0.0555518f, 0.7708399f, 0.0f}},
     {/* xyz2rgb */
      {1.8464881f, -0.5521299f, -0.2766458f, 0.0f},
      {-0.9826630f, 2.0044755f, -0.0690396f, 0.0f},
      {0.0736477f, -0.1453020f, 1.3018376f, 0.0f}}},
    /* 21 */
    { /* PAL/SECAM RGB D50 */
     {/* rgb2xyz */
      {0.4552773f, 0.3675500f, 0.1413926f, 0.0f},
      {0.2323025f, 0.7077956f, 0.0599019f, 0.0f},
      {0.0145457f, 0.1049154f, 0.7057489f, 0.0f}},
     {/* xyz2rgb */
      {2.9603944f, -1.4678519f, -0.4685105f, 0.0f},
      {-0.9787684f, 1.9161415f, 0.0334540f, 0.0f},
      {0.0844874f, -0.2545973f, 1.4216174f, 0.0f}}},
    /* 22 */
    { /* SMPTE-C RGB D50 */
     {/* rgb2xyz */
      {0.4163290f, 0.3931464f, 0.1547446f, 0.0f},
      {0.2216999f, 0.7032549f, 0.0750452f, 0.0f},
      {0.0136576f, 0.0913604f, 0.7201920f, 0.0f}},
     {/* xyz2rgb */
      {3.3921940f, -1.8264027f, -0.5385522f, 0.0f},
      {-1.0770996f, 2.0213975f, 0.0207989f, 0.0f},
      {0.0723073f, -0.2217902f, 1.3960932f, 0.0f}}},
    /* 23 */
    { /* sRGB D50 */
     {/* rgb2xyz */
      {0.4360747f, 0.3850649f, 0.1430804f, 0.0f},
      {0.2225045f, 0.7168786f, 0.0606169f, 0.0f},
      {0.0139322f, 0.0971045f, 0.7141733f, 0.0f}},
     {/* xyz2rgb */
      {3.1338561f, -1.6168667f, -0.4906146f, 0.0f},
      {-0.9787684f, 1.9161415f, 0.0334540f, 0.0f},
      {0.0719453f, -0.2289914f, 1.4052427f, 0.0f}}}
    /* ### */
};

static pixel_t reference_white_matrix[11] = {
    /* A (ASTM E308-01) */
    {1.09850f, 1.0f, 0.35585f},
    /* B (Wyszecki & Stiles, p. 769) */
    {0.99072f, 1.0f, 0.85223f},
    /* C (ASTM E308-01) */
    {0.98074f, 1.0f, 1.18232f},
    /* D50 (ASTM E308-01) */
    {0.96422f, 1.0f, 0.82521f},
    /* D55 (ASTM E308-01) */
    {0.95682f, 1.0f, 0.92149f},
    /* D65 (ASTM E308-01) */
    {0.95047f, 1.0f, 1.08883f},
    /* D75 (ASTM E308-01) */
    {0.94972f, 1.0f, 1.22638f},
    /* E (ASTM E308-01) */
    {1.0f, 1.0f, 1.0f},
    /* F2 (ASTM E308-01) */
    {0.99186f, 1.0f, 0.67393f},
    /* F7 (ASTM E308-01) */
    {0.95041f, 1.0f, 1.08747f},
    /* F11 (ASTM E308-01) */
    {1.00962f, 1.0f, 0.64350f}};

static colour_val_t rgb_model_gamma[colourspace_rgb_profile_max] = {
    2.2f, 1.8f, 2.2f,  2.2f, 2.2f, 2.2f, 1.8f, 2.2f, 0.0f, 2.2f, 2.2f, 2.2f,
    1.8f, 2.2f, -2.2f, 2.2f, 2.2f, 1.8f, 2.2f, 2.2f, 2.2f, 2.2f, 2.2f, -2.2f};

#pragma mark - Prototypes

#ifdef CGFLOAT_IS_DOUBLE
static inline colour_val_t neg_pow(colour_val_t a, colour_val_t p);
#else
static inline colour_val_t neg_powf(colour_val_t a, colour_val_t p);
#endif

static inline void sanitize_rgb(pixel_t *rgb);

static inline void apply_working_space_matrix(pixel_t p, pixel_t m0, pixel_t m1,
                                              pixel_t m2, pixel_t *result);
static inline colour_val_t colour_min(colour_val_t a, colour_val_t b,
                                      colour_val_t c);
static inline colour_val_t colour_max(colour_val_t a, colour_val_t b,
                                      colour_val_t c);

static colour_val_t hue2rgb(colour_val_t v1, colour_val_t v2, colour_val_t vH);

static colour_val_t cubic_interpolation(colour_val_t t, colour_val_t a,
                                        colour_val_t b);

#pragma mark - Utilities

#ifdef CGFLOAT_IS_DOUBLE
static inline colour_val_t neg_pow(colour_val_t b, colour_val_t e) {
  return (0.0f <= b) ? pow(b, e) : -1.0f * pow(-1.0f * b, e);
}
#else
static inline colour_val_t neg_powf(colour_val_t b, colour_val_t e) {
  return (0.0f <= b) ? powf(b, e) : -1.0f * powf(-1.0f * b, e);
}
#endif

static inline void sanitize_rgb(pixel_t *rgb) {
  colour_val_t *r = &(rgb->a), *g = &(rgb->b), *b = &(rgb->c);

  *r = (signbit(*r)) ? *r * -1.0f : *r;
  *r = (1.0f > (*r * (100000.0f))) ? 0.0f : (1.0f < *r) ? 1.0f : *r;
  *g = (signbit(*g)) ? *g * -1.0f : *g;
  *g = (1.0f > (*g * (100000.0f))) ? 0.0f : (1.0f < *g) ? 1.0f : *g;
  *b = (signbit(*b)) ? *b * -1.0f : *b;
  *b = (1.0f > (*b * (100000.0f))) ? 0.0f : (1.0f < *b) ? 1.0f : *b;
}

static inline void apply_working_space_matrix(pixel_t p, pixel_t m0, pixel_t m1,
                                              pixel_t m2, pixel_t *result) {
  result->a = (p.a * m0.a) + (p.b * m0.b) + (p.c * m0.c);
  result->b = (p.a * m1.a) + (p.b * m1.b) + (p.c * m1.c);
  result->c = (p.a * m2.a) + (p.b * m2.b) + (p.c * m2.c);
}

static inline colour_val_t colour_min(colour_val_t a, colour_val_t b,
                                      colour_val_t c) {
  colour_val_t m = a;
  if (m > b) m = b;
  if (m > c) m = c;
  return m;
}

static inline colour_val_t colour_max(colour_val_t a, colour_val_t b,
                                      colour_val_t c) {
  colour_val_t m = a;
  if (m < b) m = b;
  if (m < c) m = c;
  return m;
}

colour_val_t hue2rgb(colour_val_t v1, colour_val_t v2, colour_val_t vH) {
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

static inline colour_val_t cubic_interpolation(colour_val_t t, colour_val_t a,
                                               colour_val_t b) {
  return (colour_val_t)(a + (t * t * (3.0f - 2.0f * t)) * (b - a));
}

#pragma mark - To XYZ

void xyY2xyz(pixel_t xyY, pixel_t *xyz, colourspace_option_flags flags) {
  xyz->b = xyY.c;
  
  if (xyY.b != 0.0f) {
    xyz->a = xyY.a * xyY.c / xyY.b;
    xyz->c = (1.0f - xyY.a - xyY.b) * xyY.c / xyY.b;
  } else {
    xyz->a = xyz->b = xyz->c = 0.0f;
  }
}

#ifdef CGFLOAT_IS_DOUBLE
void _xyY2xyz_double_(pixel_t xyY, pixel_t *xyz,
                      colourspace_option_flags flags) {
  xyY2xyz(xyY, xyz, flags);
}
#else
void _xyY2xyz_float_(pixel_t xyY, pixel_t *xyz,
                     colourspace_option_flags flags) {
  xyY2xyz(xyY, xyz, flags);
}
#endif

#ifdef CGFLOAT_IS_DOUBLE
void _rgb2xyz_double_(pixel_t rgb, pixel_t *xyz,
                      colourspace_option_flags flags) {
  pixel_t p = {0.0f};
  colour_val_t s_r = 0.0f, s_g = 0.0f, s_b = 0.0f;
  
  p.a = rgb.a;
  p.b = rgb.b;
  p.c = rgb.c;
  
  s_r = signbit(p.a) ? -1.0f : 1.0f;
  s_g = signbit(p.a) ? -1.0f : 1.0f;
  s_b = signbit(p.a) ? -1.0f : 1.0f;
  
  uint16_t rgb_model_idx =
  (COLOURSPACE_OPTION_RGB_PROFILE(colourspace_rgb_profile_max) <
   COLOURSPACE_OPTION_RGB_PROFILE(flags))
  ? COLOURSPACE_OPTION_RGB_PROFILE(colourspace_rgb_profile_srgb_d65)
  : COLOURSPACE_OPTION_RGB_PROFILE(flags);
  
  if (0.0f < rgb_model_gamma[rgb_model_idx]) {
    p.a = neg_pow(p.a, rgb_model_gamma[rgb_model_idx]);
    p.b = neg_pow(p.b, rgb_model_gamma[rgb_model_idx]);
    p.c = neg_pow(p.c, rgb_model_gamma[rgb_model_idx]);
    
  } else if (0.0f > rgb_model_gamma[rgb_model_idx]) {
    p.a = (fabs(p.a) <= 0.04045f)
    ? fabs(p.a) / 12.92f
    : neg_pow((fabs(p.a) + 0.055f) / 1.055f, 2.4f);
    p.b = (fabs(p.b) <= 0.04045f)
    ? fabs(p.b) / 12.92f
    : neg_pow((fabs(p.b) + 0.055f) / 1.055f, 2.4f);
    p.c = (fabs(p.c) <= 0.04045f)
    ? fabs(p.c) / 12.92f
    : neg_pow((fabs(p.c) + 0.055f) / 1.055f, 2.4f);
    
    p.a *= s_r;
    p.b *= s_g;
    p.c *= s_b;
    
  } else {
    p.a =
    (fabs(p.a) <= 0.08f)
    ? (2700.0f * fabs(p.a) / 24389.0f)
    : ((((1000000.0f * fabs(p.a) + 480000.0f) * fabs(p.a) + 76800.0f) *
        fabs(p.a) +
        4096.0f) /
       1560896.0f);
    p.b =
    (fabs(p.b) <= 0.08f)
    ? (2700.0f * fabs(p.b) / 24389.0f)
    : ((((1000000.0f * fabs(p.b) + 480000.0f) * fabs(p.b) + 76800.0f) *
        fabs(p.b) +
        4096.0f) /
       1560896.0f);
    p.c =
    (fabs(p.c) <= 0.08f)
    ? (2700.0f * fabs(p.c) / 24389.0f)
    : ((((1000000.0f * fabs(p.c) + 480000.0f) * fabs(p.c) + 76800.0f) *
        fabs(p.c) +
        4096.0f) /
       1560896.0f);
    
    p.a *= s_r;
    p.b *= s_g;
    p.c *= s_b;
  }
  
  p.a *= 100.0f;
  p.b *= 100.0f;
  p.c *= 100.0f;
  
  apply_working_space_matrix(p, rgb_working_matrices[rgb_model_idx].rgb2xyz[0],
                             rgb_working_matrices[rgb_model_idx].rgb2xyz[1],
                             rgb_working_matrices[rgb_model_idx].rgb2xyz[2],
                             xyz);
}
#else
void _rgb2xyz_float_(pixel_t rgb, pixel_t *xyz,
                     colourspace_option_flags flags) {
  pixel_t p = {0.0f};
  colour_val_t s_r = 0.0f, s_g = 0.0f, s_b = 0.0f;
  
  p.a = rgb.a;
  p.b = rgb.b;
  p.c = rgb.c;
  
  s_r = signbit(p.a) ? -1.0f : 1.0f;
  s_g = signbit(p.a) ? -1.0f : 1.0f;
  s_b = signbit(p.a) ? -1.0f : 1.0f;
  
  uint16_t rgb_model_idx =
  (COLOURSPACE_OPTION_RGB_PROFILE(colourspace_rgb_profile_max) <
   COLOURSPACE_OPTION_RGB_PROFILE(flags))
  ? COLOURSPACE_OPTION_RGB_PROFILE(colourspace_rgb_profile_srgb_d65)
  : COLOURSPACE_OPTION_RGB_PROFILE(flags);
  
  if (0.0f < rgb_model_gamma[rgb_model_idx]) {
    p.a = neg_powf(p.a, rgb_model_gamma[rgb_model_idx]);
    p.b = neg_powf(p.b, rgb_model_gamma[rgb_model_idx]);
    p.c = neg_powf(p.c, rgb_model_gamma[rgb_model_idx]);
    
  } else if (0.0f > rgb_model_gamma[rgb_model_idx]) {
    p.a = (fabsf(p.a) <= 0.04045f)
    ? fabsf(p.a) / 12.92f
    : neg_powf((fabsf(p.a) + 0.055f) / 1.055f, 2.4f);
    p.b = (fabsf(p.b) <= 0.04045f)
    ? fabsf(p.b) / 12.92f
    : neg_powf((fabsf(p.b) + 0.055f) / 1.055f, 2.4f);
    p.c = (fabsf(p.c) <= 0.04045f)
    ? fabsf(p.c) / 12.92f
    : neg_powf((fabsf(p.c) + 0.055f) / 1.055f, 2.4f);
    
    p.a *= s_r;
    p.b *= s_g;
    p.c *= s_b;
    
  } else {
    p.a = (fabsf(p.a) <= 0.08f)
    ? (2700.0f * fabsf(p.a) / 24389.0f)
    : ((((1000000.0f * fabsf(p.a) + 480000.0f) * fabsf(p.a) +
         76800.0f) *
        fabsf(p.a) +
        4096.0f) /
       1560896.0f);
    p.b = (fabsf(p.b) <= 0.08f)
    ? (2700.0f * fabsf(p.b) / 24389.0f)
    : ((((1000000.0f * fabsf(p.b) + 480000.0f) * fabsf(p.b) +
         76800.0f) *
        fabsf(p.b) +
        4096.0f) /
       1560896.0f);
    p.c = (fabsf(p.c) <= 0.08f)
    ? (2700.0f * fabsf(p.c) / 24389.0f)
    : ((((1000000.0f * fabsf(p.c) + 480000.0f) * fabsf(p.c) +
         76800.0f) *
        fabsf(p.c) +
        4096.0f) /
       1560896.0f);
    
    p.a *= s_r;
    p.b *= s_g;
    p.c *= s_b;
  }
  
  p.a *= 100.0f;
  p.b *= 100.0f;
  p.c *= 100.0f;
  
  apply_working_space_matrix(p, rgb_working_matrices[rgb_model_idx].rgb2xyz[0],
                             rgb_working_matrices[rgb_model_idx].rgb2xyz[1],
                             rgb_working_matrices[rgb_model_idx].rgb2xyz[2],
                             xyz);
}
#endif

#ifdef CGFLOAT_IS_DOUBLE
void _hlab2xyz_double_(pixel_t hlab, pixel_t *xyz,
                       colourspace_option_flags flags) {
  colour_val_t *x = NULL, *y = NULL, *z = NULL, var_X, var_Y, var_Z;
  
  var_Y = hlab.a / 10.0f;
  var_X = hlab.b / 17.5f * hlab.a / 10.0f;
  var_Z = hlab.c / 7.0f * hlab.a / 10.0f;
  
  *y = pow(var_Y, 2.0f);
  *x = (var_X + *y) / 1.02f;
  *z = -1 * (var_Z - *y) / 0.847f;
}
#else
void _hlab2xyz_float_(pixel_t hlab, pixel_t *xyz,
                      colourspace_option_flags flags) {
  colour_val_t *x = NULL, *y = NULL, *z = NULL, var_X, var_Y, var_Z;
  
  var_Y = hlab.a / 10.0f;
  var_X = hlab.b / 17.5f * hlab.a / 10.0f;
  var_Z = hlab.c / 7.0f * hlab.a / 10.0f;
  
  *y = powf(var_Y, 2.0f);
  *x = (var_X + *y) / 1.02f;
  *z = -1 * (var_Z - *y) / 0.847f;
}
#endif

#ifdef CGFLOAT_IS_DOUBLE
void _lab2xyz_double_(pixel_t lab, pixel_t *xyz,
                      colourspace_option_flags flags) {
  colour_val_t x, y, z;
  
  uint16_t reference_white_idx =
  (COLOURSPACE_OPTION_REFERENCE_WHITE(colourspace_reference_white_max) <
   COLOURSPACE_OPTION_REFERENCE_WHITE(flags))
  ? COLOURSPACE_OPTION_REFERENCE_WHITE(colourspace_reference_white_D65)
  : COLOURSPACE_OPTION_REFERENCE_WHITE(flags);
  
  y = (lab.a + 16.0f) / 116.0f;
  x = lab.b / 500.0f + y;
  z = y - lab.c / 200.0f;
  
  y = (pow(y, 3.0f) > epsilon) ? pow(y, 3.0f) : (y - 16.0f / 116.0f) / kappa;
  x = (pow(x, 3.0f) > epsilon) ? pow(x, 3.0f) : (x - 16.0f / 116.0f) / kappa;
  z = (pow(z, 3.0f) > epsilon) ? pow(z, 3.0f) : (z - 16.0f / 116.0f) / kappa;
  
  xyz->a = x * (reference_white_matrix[reference_white_idx].a * 100.0f);
  xyz->b = y * (reference_white_matrix[reference_white_idx].b * 100.0f);
  xyz->c = z * (reference_white_matrix[reference_white_idx].c * 100.0f);
}
#else
void _lab2xyz_float_(pixel_t lab, pixel_t *xyz,
                     colourspace_option_flags flags) {
  colour_val_t x, y, z;
  
  uint16_t reference_white_idx =
  (COLOURSPACE_OPTION_REFERENCE_WHITE(colourspace_reference_white_max) <
   COLOURSPACE_OPTION_REFERENCE_WHITE(flags))
  ? COLOURSPACE_OPTION_REFERENCE_WHITE(colourspace_reference_white_D65)
  : COLOURSPACE_OPTION_REFERENCE_WHITE(flags);
  
  y = (lab.a + 16.0f) / 116.0f;
  x = lab.b / 500.0f + y;
  z = y - lab.c / 200.0f;
  
  y = (powf(y, 3.0f) > epsilon) ? powf(y, 3.0f) : (y - 16.0f / 116.0f) / kappa;
  x = (powf(x, 3.0f) > epsilon) ? powf(x, 3.0f) : (x - 16.0f / 116.0f) / kappa;
  z = (powf(z, 3.0f) > epsilon) ? powf(z, 3.0f) : (z - 16.0f / 116.0f) / kappa;
  
  xyz->a = x * (reference_white_matrix[reference_white_idx].a * 100.0f);
  xyz->b = y * (reference_white_matrix[reference_white_idx].b * 100.0f);
  xyz->c = z * (reference_white_matrix[reference_white_idx].c * 100.0f);
}
#endif

#ifdef CGFLOAT_IS_DOUBLE
void _luv2xyz_double_(pixel_t luv, pixel_t *xyz,
                      colourspace_option_flags flags) {
  colour_val_t var_Y, var_U, ref_U, var_V, ref_V, *x = NULL, *y = NULL,
  *z = NULL;
  
  uint16_t reference_white_idx =
  (COLOURSPACE_OPTION_REFERENCE_WHITE(colourspace_reference_white_max) <
   COLOURSPACE_OPTION_REFERENCE_WHITE(flags))
  ? COLOURSPACE_OPTION_REFERENCE_WHITE(colourspace_reference_white_D65)
  : COLOURSPACE_OPTION_REFERENCE_WHITE(flags);
  
  var_Y = (luv.a + 16.0f) / 116.0f;
  
  var_Y = (epsilon < pow(var_Y, 3.0f)) ? pow(var_Y, 3.0f)
  : (var_Y - 16.0f / 116.0f) / kappa;
  
  ref_U = (4.0f * (reference_white_matrix[reference_white_idx].a * 100.0f)) /
  ((reference_white_matrix[reference_white_idx].a * 100.0f) +
   (15.0f * (reference_white_matrix[reference_white_idx].b * 100.0f)) +
   (3.0f * (reference_white_matrix[reference_white_idx].c * 100.0f)));
  ref_V = (9.0f * (reference_white_matrix[reference_white_idx].b * 100.0f)) /
  ((reference_white_matrix[reference_white_idx].a * 100.0f) +
   (15.0f * (reference_white_matrix[reference_white_idx].b * 100.0f)) +
   (3.0f * (reference_white_matrix[reference_white_idx].c * 100.0f)));
  
  var_U = luv.b / (13.0f * luv.a) + ref_U;
  var_V = luv.c / (13.0f * luv.a) + ref_V;
  
  *y = var_Y * 100.0f;
  *x = -1.0f * (9.0f * (*y) * var_U) / ((var_U - 4.0f) * var_V - var_U * var_V);
  *z = (9.0f * (*y) - (15.0f * var_V * (*y)) - (var_V * (*x))) / (3.0f * var_V);
}
#else
void _luv2xyz_float_(pixel_t luv, pixel_t *xyz,
                     colourspace_option_flags flags) {
  colour_val_t var_Y, var_U, ref_U, var_V, ref_V, *x = NULL, *y = NULL,
  *z = NULL;
  
  uint16_t reference_white_idx =
  (COLOURSPACE_OPTION_REFERENCE_WHITE(colourspace_reference_white_max) <
   COLOURSPACE_OPTION_REFERENCE_WHITE(flags))
  ? COLOURSPACE_OPTION_REFERENCE_WHITE(colourspace_reference_white_D65)
  : COLOURSPACE_OPTION_REFERENCE_WHITE(flags);
  
  var_Y = (luv.a + 16.0f) / 116.0f;
  
  var_Y = (epsilon < powf(var_Y, 3.0f)) ? powf(var_Y, 3.0f)
  : (var_Y - 16.0f / 116.0f) / kappa;
  
  ref_U = (4.0f * (reference_white_matrix[reference_white_idx].a * 100.0f)) /
  ((reference_white_matrix[reference_white_idx].a * 100.0f) +
   (15.0f * (reference_white_matrix[reference_white_idx].b * 100.0f)) +
   (3.0f * (reference_white_matrix[reference_white_idx].c * 100.0f)));
  ref_V = (9.0f * (reference_white_matrix[reference_white_idx].b * 100.0f)) /
  ((reference_white_matrix[reference_white_idx].a * 100.0f) +
   (15.0f * (reference_white_matrix[reference_white_idx].b * 100.0f)) +
   (3.0f * (reference_white_matrix[reference_white_idx].c * 100.0f)));
  
  var_U = luv.b / (13.0f * luv.a) + ref_U;
  var_V = luv.c / (13.0f * luv.a) + ref_V;
  
  *y = var_Y * 100.0f;
  *x = -1.0f * (9.0f * (*y) * var_U) / ((var_U - 4.0f) * var_V - var_U * var_V);
  *z = (9.0f * (*y) - (15.0f * var_V * (*y)) - (var_V * (*x))) / (3.0f * var_V);
}
#endif

#pragma mark - to Hunter LAB

#ifdef CGFLOAT_IS_DOUBLE
void _xyz2hlab_double_(pixel_t xyz, pixel_t *hlab,
                       colourspace_option_flags flags) {
  colour_val_t *hl, *a, *b;
  
  hl = &(hlab->a);
  a = &(hlab->b);
  b = &(hlab->c);
  
  *hl = 10.0f * sqrt(xyz.b);
  *a = 17.5f * (((1.02f * xyz.a) - xyz.b) / sqrt(xyz.b));
  *b = 7.0f * ((xyz.b - (0.847f * xyz.c)) / sqrt(xyz.b));
}
#else
void _xyz2hlab_float_(pixel_t xyz, pixel_t *hlab,
                      colourspace_option_flags flags) {
  colour_val_t *hl, *a, *b;
  
  hl = &(hlab->a);
  a = &(hlab->b);
  b = &(hlab->c);
  
  *hl = 10.0f * sqrtf(xyz.b);
  *a = 17.5f * (((1.02f * xyz.a) - xyz.b) / sqrtf(xyz.b));
  *b = 7.0f * ((xyz.b - (0.847f * xyz.c)) / sqrtf(xyz.b));
}
#endif

#ifdef CGFLOAT_IS_DOUBLE
void _rgb2hlab_double_(pixel_t rgb, pixel_t *hlab,
                       colourspace_option_flags flags) {
  pixel_t xyz = {0.0f};
  
  _rgb2xyz_double_(rgb, &xyz, flags);
  _xyz2hlab_double_(xyz, hlab, flags);
}
#else
void _rgb2hlab_float_(pixel_t rgb, pixel_t *hlab,
                      colourspace_option_flags flags) {
  pixel_t xyz = {0.0f};
  
  _rgb2xyz_float_(rgb, &xyz, flags);
  _xyz2hlab_float_(xyz, hlab, flags);
}
#endif

#pragma mark - To LAB

#ifdef CGFLOAT_IS_DOUBLE
void _xyz2lab_double_(pixel_t xyz, pixel_t *lab,
                      colourspace_option_flags flags) {
  colour_val_t x, y, z;
  
  uint16_t reference_white_idx =
  (COLOURSPACE_OPTION_REFERENCE_WHITE(colourspace_reference_white_max) <
   COLOURSPACE_OPTION_REFERENCE_WHITE(flags))
  ? COLOURSPACE_OPTION_REFERENCE_WHITE(colourspace_reference_white_D65)
  : COLOURSPACE_OPTION_REFERENCE_WHITE(flags);
  
  x = xyz.a / (reference_white_matrix[reference_white_idx].a * 100.0f);
  y = xyz.b / (reference_white_matrix[reference_white_idx].b * 100.0f);
  z = xyz.c / (reference_white_matrix[reference_white_idx].c * 100.0f);
  
  x = (x > epsilon) ? pow(x, 1.0f / 3.0f) : (kappa * x) + (16.0f / 116.0f);
  y = (y > epsilon) ? pow(y, 1.0f / 3.0f) : (kappa * y) + (16.0f / 116.0f);
  z = (z > epsilon) ? pow(z, 1.0f / 3.0f) : (kappa * z) + (16.0f / 116.0f);
  
  lab->a = (116.0f * y) - 16.0f;
  lab->b = 500.0f * (x - y);
  lab->c = 200.0f * (y - z);
}
#else
void _xyz2lab_float_(pixel_t xyz, pixel_t *lab,
                     colourspace_option_flags flags) {
  colour_val_t x, y, z;
  
  uint16_t reference_white_idx =
  (COLOURSPACE_OPTION_REFERENCE_WHITE(colourspace_reference_white_max) <
   COLOURSPACE_OPTION_REFERENCE_WHITE(flags))
  ? COLOURSPACE_OPTION_REFERENCE_WHITE(colourspace_reference_white_D65)
  : COLOURSPACE_OPTION_REFERENCE_WHITE(flags);
  
  x = xyz.a / (reference_white_matrix[reference_white_idx].a * 100.0f);
  y = xyz.b / (reference_white_matrix[reference_white_idx].b * 100.0f);
  z = xyz.c / (reference_white_matrix[reference_white_idx].c * 100.0f);
  
  x = (x > epsilon) ? powf(x, 1.0f / 3.0f) : (kappa * x) + (16.0f / 116.0f);
  y = (y > epsilon) ? powf(y, 1.0f / 3.0f) : (kappa * y) + (16.0f / 116.0f);
  z = (z > epsilon) ? powf(z, 1.0f / 3.0f) : (kappa * z) + (16.0f / 116.0f);
  
  lab->a = (116.0f * y) - 16.0f;
  lab->b = 500.0f * (x - y);
  lab->c = 200.0f * (y - z);
}
#endif

#ifdef CGFLOAT_IS_DOUBLE
void _rgb2lab_double_(pixel_t rgb, pixel_t *lab,
                      colourspace_option_flags flags) {
  pixel_t xyz = {0.0f};
  
  _rgb2xyz_double_(rgb, &xyz, flags);
  _xyz2lab_double_(xyz, lab, flags);
}
#else
void _rgb2lab_float_(pixel_t rgb, pixel_t *lab,
                     colourspace_option_flags flags) {
  pixel_t xyz = {0.0f};
  
  _rgb2xyz_float_(rgb, &xyz, flags);
  _xyz2lab_float_(xyz, lab, flags);
}
#endif

#ifdef CGFLOAT_IS_DOUBLE
void _lch_ab2lab_double_(pixel_t lch_ab, pixel_t *lab,
                         colourspace_option_flags flags) {
  colour_val_t l_, c, h, *l, *a, *b;
  
  l_ = lch_ab.a;
  c = lch_ab.b;
  h = (lch_ab.c * M_2PI);
  
  l = &(lab->a);
  a = &(lab->b);
  b = &(lab->c);
  
  *l = l_;
  *a = c * cos(h);
  *b = c * sin(h);
}
#else
void _lch_ab2lab_float_(pixel_t lch_ab, pixel_t *lab,
                        colourspace_option_flags flags) {
  colour_val_t l_, c, h, *l, *a, *b;
  
  l_ = lch_ab.a;
  c = lch_ab.b;
  h = (lch_ab.c * M_2PI);
  
  l = &(lab->a);
  a = &(lab->b);
  b = &(lab->c);
  
  *l = l_;
  *a = c * cosf(h);
  *b = c * sinf(h);
}
#endif

#ifdef CGFLOAT_IS_DOUBLE
void _lch_uv2lab_double_(pixel_t lch_uv, pixel_t *lab,
                         colourspace_option_flags flags) {
  pixel_t luv = {0.0f}, xyz = {0.0f};
  
  _lch_uv2luv_double_(lch_uv, &luv, flags);
  _luv2xyz_double_(luv, &xyz, flags);
  _xyz2lab_double_(xyz, lab, flags);
}
#else
void _lch_uv2lab_float_(pixel_t lch_uv, pixel_t *lab,
                        colourspace_option_flags flags) {
  pixel_t luv = {0.0f}, xyz = {0.0f};
  
  _lch_uv2luv_float_(lch_uv, &luv, flags);
  _luv2xyz_float_(luv, &xyz, flags);
  _xyz2lab_float_(xyz, lab, flags);
}
#endif

#pragma mark - To LUV

#ifdef CGFLOAT_IS_DOUBLE
void _xyz2luv_double_(pixel_t xyz, pixel_t *luv,
                      colourspace_option_flags flags) {
  colour_val_t var_Y, var_U, ref_U, var_V, ref_V, *l, *u, *v;
  
  uint16_t reference_white_idx =
  (COLOURSPACE_OPTION_REFERENCE_WHITE(colourspace_reference_white_max) <
   COLOURSPACE_OPTION_REFERENCE_WHITE(flags))
  ? COLOURSPACE_OPTION_REFERENCE_WHITE(colourspace_reference_white_D65)
  : COLOURSPACE_OPTION_REFERENCE_WHITE(flags);
  
  l = &(luv->a);
  u = &(luv->b);
  v = &(luv->c);
  
  var_U = (4.0f * xyz.a) / (xyz.a + (15.0f * xyz.b) + (3.0f * xyz.c));
  var_V = (9.0f * xyz.b) / (xyz.a + (15.0f * xyz.b) + (3.0f * xyz.c));
  
  var_Y = (xyz.b / 100.0f);
  var_Y = (epsilon < var_Y) ? pow(var_Y, (1.0f / 3.0f))
  : (kappa * var_Y) + (16.0f / 116.0f);
  
  ref_U = (4.0f * (reference_white_matrix[reference_white_idx].a * 100.0f)) /
  ((reference_white_matrix[reference_white_idx].a * 100.0f) +
   (15.0f * (reference_white_matrix[reference_white_idx].b * 100.0f)) +
   (3.0f * (reference_white_matrix[reference_white_idx].c * 100.0f)));
  ref_V = (9.0f * (reference_white_matrix[reference_white_idx].b * 100.0f)) /
  ((reference_white_matrix[reference_white_idx].a * 100.0f) +
   (15.0f * (reference_white_matrix[reference_white_idx].b * 100.0f)) +
   (3.0f * (reference_white_matrix[reference_white_idx].c * 100.0f)));
  
  *l = (116.0f * var_Y) - 16.0f;
  *u = 13.0f * (*l) * (var_U - ref_U);
  *v = 13.0f * (*l) * (var_V - ref_V);
}
#else
void _xyz2luv_float_(pixel_t xyz, pixel_t *luv,
                     colourspace_option_flags flags) {
  colour_val_t var_Y, var_U, ref_U, var_V, ref_V, *l, *u, *v;
  
  uint16_t reference_white_idx =
  (COLOURSPACE_OPTION_REFERENCE_WHITE(colourspace_reference_white_max) <
   COLOURSPACE_OPTION_REFERENCE_WHITE(flags))
  ? COLOURSPACE_OPTION_REFERENCE_WHITE(colourspace_reference_white_D65)
  : COLOURSPACE_OPTION_REFERENCE_WHITE(flags);
  
  l = &(luv->a);
  u = &(luv->b);
  v = &(luv->c);
  
  var_U = (4.0f * xyz.a) / (xyz.a + (15.0f * xyz.b) + (3.0f * xyz.c));
  var_V = (9.0f * xyz.b) / (xyz.a + (15.0f * xyz.b) + (3.0f * xyz.c));
  
  var_Y = (xyz.b / 100.0f);
  var_Y = (epsilon < var_Y) ? powf(var_Y, (1.0f / 3.0f))
  : (kappa * var_Y) + (16.0f / 116.0f);
  
  ref_U = (4.0f * (reference_white_matrix[reference_white_idx].a * 100.0f)) /
  ((reference_white_matrix[reference_white_idx].a * 100.0f) +
   (15.0f * (reference_white_matrix[reference_white_idx].b * 100.0f)) +
   (3.0f * (reference_white_matrix[reference_white_idx].c * 100.0f)));
  ref_V = (9.0f * (reference_white_matrix[reference_white_idx].b * 100.0f)) /
  ((reference_white_matrix[reference_white_idx].a * 100.0f) +
   (15.0f * (reference_white_matrix[reference_white_idx].b * 100.0f)) +
   (3.0f * (reference_white_matrix[reference_white_idx].c * 100.0f)));
  
  *l = (116.0f * var_Y) - 16.0f;
  *u = 13.0f * (*l) * (var_U - ref_U);
  *v = 13.0f * (*l) * (var_V - ref_V);
}
#endif

#ifdef CGFLOAT_IS_DOUBLE
void _rgb2luv_double_(pixel_t rgb, pixel_t *luv,
                      colourspace_option_flags flags) {
  pixel_t xyz = {0.0f};
  
  _rgb2xyz_double_(rgb, &xyz, flags);
  _xyz2luv_double_(xyz, luv, flags);
}
#else
void _rgb2luv_float_(pixel_t rgb, pixel_t *luv,
                     colourspace_option_flags flags) {
  pixel_t xyz = {0.0f};
  
  _rgb2xyz_float_(rgb, &xyz, flags);
  _xyz2luv_float_(xyz, luv, flags);
}
#endif

#ifdef CGFLOAT_IS_DOUBLE
void _lch_ab2luv_double_(pixel_t lch_ab, pixel_t *luv,
                         colourspace_option_flags flags) {
  pixel_t lab = {0.0f}, xyz = {0.0f};
  
  _lch_ab2lab_double_(lch_ab, &lab, flags);
  _lab2xyz_double_(lab, &xyz, flags);
  _xyz2luv_double_(xyz, luv, flags);
}
#else
void _lch_ab2luv_float_(pixel_t lch_ab, pixel_t *luv,
                        colourspace_option_flags flags) {
  pixel_t lab = {0.0f}, xyz = {0.0f};
  
  _lch_ab2lab_float_(lch_ab, &lab, flags);
  _lab2xyz_float_(lab, &xyz, flags);
  _xyz2luv_float_(xyz, luv, flags);
}
#endif

#ifdef CGFLOAT_IS_DOUBLE
void _lch_uv2luv_double_(pixel_t lch_uv, pixel_t *luv,
                         colourspace_option_flags flags) {
  colour_val_t l_, c, h, *l, *u, *v;
  
  l_ = lch_uv.a;
  c = lch_uv.b;
  h = (lch_uv.c * M_2PI);
  
  l = &(luv->a);
  u = &(luv->b);
  v = &(luv->c);
  
  *l = l_;
  *u = c * cos(h);
  *v = c * sin(h);
}
#else
void _lch_uv2luv_float_(pixel_t lch_uv, pixel_t *luv,
                        colourspace_option_flags flags) {
  colour_val_t l_, c, h, *l, *u, *v;
  
  l_ = lch_uv.a;
  c = lch_uv.b;
  h = (lch_uv.c * M_2PI);
  
  l = &(luv->a);
  u = &(luv->b);
  v = &(luv->c);
  
  *l = l_;
  *u = c * cosf(h);
  *v = c * sinf(h);
}
#endif

#pragma mark - To LCH(AB)

#ifdef CGFLOAT_IS_DOUBLE
void _lab2lch_ab_double_(pixel_t lab, pixel_t *lch_ab,
                         colourspace_option_flags flags) {
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
#else
void _lab2lch_ab_float_(pixel_t lab, pixel_t *lch_ab,
                        colourspace_option_flags flags) {
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
#endif

#ifdef CGFLOAT_IS_DOUBLE
void _rgb2lch_ab_double_(pixel_t rgb, pixel_t *lch_ab,
                         colourspace_option_flags flags) {
  pixel_t lab = {0.0f};
  
  _rgb2lab_double_(rgb, &lab, flags);
  _lab2lch_ab_double_(lab, lch_ab, flags);
}
#else
void _rgb2lch_ab_float_(pixel_t rgb, pixel_t *lch_ab,
                        colourspace_option_flags flags) {
  pixel_t lab = {0.0f};
  
  _rgb2lab_float_(rgb, &lab, flags);
  _lab2lch_ab_float_(lab, lch_ab, flags);
}
#endif

#ifdef CGFLOAT_IS_DOUBLE
void _luv2lch_ab_double_(pixel_t luv, pixel_t *lch_ab,
                         colourspace_option_flags flags) {
  pixel_t lab = {0.0f}, xyz = {0.0f};
  
  _luv2xyz_double_(luv, &xyz, flags);
  _xyz2lab_double_(xyz, &lab, flags);
  _lab2lch_ab_double_(lab, lch_ab, flags);
}
#else
void _luv2lch_ab_float_(pixel_t luv, pixel_t *lch_ab,
                        colourspace_option_flags flags) {
  pixel_t lab = {0.0f}, xyz = {0.0f};
  
  _luv2xyz_float_(luv, &xyz, flags);
  _xyz2lab_float_(xyz, &lab, flags);
  _lab2lch_ab_float_(lab, lch_ab, flags);
}
#endif

#pragma mark - To LCH(UV)

#ifdef CGFLOAT_IS_DOUBLE
void _luv2lch_uv_double_(pixel_t luv, pixel_t *lch_uv,
                         colourspace_option_flags flags) {
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
#else
void _luv2lch_uv_float_(pixel_t luv, pixel_t *lch_uv,
                        colourspace_option_flags flags) {
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
#endif

#ifdef CGFLOAT_IS_DOUBLE
void _lab2lch_uv_double_(pixel_t lab, pixel_t *lch_uv,
                         colourspace_option_flags flags) {
  pixel_t luv = {0.0f}, xyz = {0.0f};
  
  _lab2xyz_double_(lab, &xyz, flags);
  _xyz2luv_double_(xyz, &luv, flags);
  _luv2lch_uv_double_(luv, lch_uv, flags);
}
#else
void _lab2lch_uv_float_(pixel_t lab, pixel_t *lch_uv,
                        colourspace_option_flags flags) {
  pixel_t luv = {0.0f}, xyz = {0.0f};
  
  _lab2xyz_float_(lab, &xyz, flags);
  _xyz2luv_float_(xyz, &luv, flags);
  _luv2lch_uv_float_(luv, lch_uv, flags);
}
#endif

#ifdef CGFLOAT_IS_DOUBLE
void _rgb2lch_uv_double_(pixel_t rgb, pixel_t *lch_uv,
                         colourspace_option_flags flags) {
  pixel_t lab = {0.0f};
  
  _rgb2lab_double_(rgb, &lab, flags);
  _lab2lch_uv_double_(lab, lch_uv, flags);
}
#else
void _rgb2lch_uv_float_(pixel_t rgb, pixel_t *lch_uv,
                        colourspace_option_flags flags) {
  pixel_t lab = {0.0f};
  
  _rgb2lab_float_(rgb, &lab, flags);
  _lab2lch_uv_float_(lab, lch_uv, flags);
}
#endif

#pragma mark - To RGB

#ifdef CGFLOAT_IS_DOUBLE
void _xyz2rgb_double_(pixel_t xyz, pixel_t *rgb,
                      colourspace_option_flags flags) {
  colour_val_t *r, *g, *b, R, G, B, s_r = 0.0f, s_g = 0.0f, s_b = 0.0f;
  pixel_t var_xyz = {0.0f};
  
  uint16_t rgb_model_idx =
  (COLOURSPACE_OPTION_RGB_PROFILE(colourspace_rgb_profile_max) <
   COLOURSPACE_OPTION_RGB_PROFILE(flags))
  ? COLOURSPACE_OPTION_RGB_PROFILE(colourspace_rgb_profile_srgb_d65)
  : COLOURSPACE_OPTION_RGB_PROFILE(flags);
  
  var_xyz.a = xyz.a / 100.0f;
  var_xyz.b = xyz.b / 100.0f;
  var_xyz.c = xyz.c / 100.0f;
  
  apply_working_space_matrix(
                             var_xyz, rgb_working_matrices[rgb_model_idx].xyz2rgb[0],
                             rgb_working_matrices[rgb_model_idx].xyz2rgb[1],
                             rgb_working_matrices[rgb_model_idx].xyz2rgb[2], rgb);
  
  r = &(rgb->a);
  g = &(rgb->b);
  b = &(rgb->c);
  
  R = rgb->a;
  G = rgb->b;
  B = rgb->c;
  
  s_r = signbit(R) ? -1.0f : 1.0f;
  s_g = signbit(G) ? -1.0f : 1.0f;
  s_b = signbit(B) ? -1.0f : 1.0f;
  
  if (0.0f < rgb_model_gamma[rgb_model_idx]) {
    R = neg_pow(R, 1.0f / rgb_model_gamma[rgb_model_idx]);
    G = neg_pow(G, 1.0f / rgb_model_gamma[rgb_model_idx]);
    B = neg_pow(B, 1.0f / rgb_model_gamma[rgb_model_idx]);
    
  } else if (0.0f > rgb_model_gamma[rgb_model_idx]) {
    R = (fabs(R) <= 0.0031308f)
    ? 12.92f * fabs(R)
    : 1.055f * neg_pow(fabs(R), (1.0f / 2.4f)) - 0.055f;
    G = (fabs(G) <= 0.0031308f)
    ? 12.92f * fabs(G)
    : 1.055f * neg_pow(fabs(G), (1.0f / 2.4f)) - 0.055f;
    B = (fabs(B) <= 0.0031308f)
    ? 12.92f * fabs(B)
    : 1.055f * neg_pow(fabs(B), (1.0f / 2.4f)) - 0.055f;
    
    R *= s_r;
    G *= s_g;
    B *= s_b;
    
  } else {
    R = (fabs(R) <= (216.0f / 24389.0f))
    ? (fabs(R) * 24389.0f / 2700.0f)
    : (1.16f * neg_pow(fabs(R), 1.0f / 3.0f) - 0.16f);
    G = (fabs(G) <= (216.0f / 24389.0f))
    ? (fabs(G) * 24389.0f / 2700.0f)
    : (1.16f * neg_pow(fabs(G), 1.0f / 3.0f) - 0.16f);
    B = (fabs(B) <= (216.0f / 24389.0f))
    ? (fabs(B) * 24389.0f / 2700.0f)
    : (1.16f * neg_pow(fabs(B), 1.0f / 3.0f) - 0.16f);
    
    R *= s_r;
    G *= s_g;
    B *= s_b;
  }
  
  *r = R;
  *g = G;
  *b = B;
  
  sanitize_rgb(rgb);
}
#else
void _xyz2rgb_float_(pixel_t xyz, pixel_t *rgb,
                     colourspace_option_flags flags) {
  colour_val_t *r, *g, *b, R, G, B, s_r = 0.0f, s_g = 0.0f, s_b = 0.0f;
  pixel_t var_xyz = {0.0f};
  
  uint16_t rgb_model_idx =
  (COLOURSPACE_OPTION_RGB_PROFILE(colourspace_rgb_profile_max) <
   COLOURSPACE_OPTION_RGB_PROFILE(flags))
  ? COLOURSPACE_OPTION_RGB_PROFILE(colourspace_rgb_profile_srgb_d65)
  : COLOURSPACE_OPTION_RGB_PROFILE(flags);
  
  var_xyz.a = xyz.a / 100.0f;
  var_xyz.b = xyz.b / 100.0f;
  var_xyz.c = xyz.c / 100.0f;
  
  apply_working_space_matrix(
                             var_xyz, rgb_working_matrices[rgb_model_idx].xyz2rgb[0],
                             rgb_working_matrices[rgb_model_idx].xyz2rgb[1],
                             rgb_working_matrices[rgb_model_idx].xyz2rgb[2], rgb);
  
  r = &(rgb->a);
  g = &(rgb->b);
  b = &(rgb->c);
  
  R = rgb->a;
  G = rgb->b;
  B = rgb->c;
  
  s_r = signbit(R) ? -1.0f : 1.0f;
  s_g = signbit(G) ? -1.0f : 1.0f;
  s_b = signbit(B) ? -1.0f : 1.0f;
  
  if (0.0f < rgb_model_gamma[rgb_model_idx]) {
    R = neg_powf(R, 1.0f / rgb_model_gamma[rgb_model_idx]);
    G = neg_powf(G, 1.0f / rgb_model_gamma[rgb_model_idx]);
    B = neg_powf(B, 1.0f / rgb_model_gamma[rgb_model_idx]);
    
  } else if (0.0f > rgb_model_gamma[rgb_model_idx]) {
    R = (fabsf(R) <= 0.0031308f)
    ? 12.92f * fabsf(R)
    : 1.055f * neg_powf(fabsf(R), (1.0f / 2.4f)) - 0.055f;
    G = (fabsf(G) <= 0.0031308f)
    ? 12.92f * fabsf(G)
    : 1.055f * neg_powf(fabsf(G), (1.0f / 2.4f)) - 0.055f;
    B = (fabsf(B) <= 0.0031308f)
    ? 12.92f * fabsf(B)
    : 1.055f * neg_powf(fabsf(B), (1.0f / 2.4f)) - 0.055f;
    
    R *= s_r;
    G *= s_g;
    B *= s_b;
    
  } else {
    R = (fabsf(R) <= (216.0f / 24389.0f))
    ? (fabsf(R) * 24389.0f / 2700.0f)
    : (1.16f * neg_powf(fabsf(R), 1.0f / 3.0f) - 0.16f);
    G = (fabsf(G) <= (216.0f / 24389.0f))
    ? (fabsf(G) * 24389.0f / 2700.0f)
    : (1.16f * neg_powf(fabsf(G), 1.0f / 3.0f) - 0.16f);
    B = (fabsf(B) <= (216.0f / 24389.0f))
    ? (fabsf(B) * 24389.0f / 2700.0f)
    : (1.16f * neg_powf(fabsf(B), 1.0f / 3.0f) - 0.16f);
    
    R *= s_r;
    G *= s_g;
    B *= s_b;
  }
  
  *r = R;
  *g = G;
  *b = B;
  
  sanitize_rgb(rgb);
}
#endif

void hsl2rgb(pixel_t hsl, pixel_t *rgb, colourspace_option_flags flags) {
  colour_val_t var_1, var_2, *r, *g, *b;

  r = &(rgb->a);
  g = &(rgb->b);
  b = &(rgb->c);

  if (0.0f == hsl.b) {
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

#ifdef CGFLOAT_IS_DOUBLE
void _hsl2rgb_double_(pixel_t hsl, pixel_t *rgb,
                      colourspace_option_flags flags) {
  hsl2rgb(hsl, rgb, flags);
}
#else
void _hsl2rgb_float_(pixel_t hsl, pixel_t *rgb,
                     colourspace_option_flags flags) {
  hsl2rgb(hsl, rgb, flags);
}
#endif

#ifdef CGFLOAT_IS_DOUBLE
void _hsv2rgb_double_(pixel_t hsv, pixel_t *rgb,
                      colourspace_option_flags flags) {
  colour_val_t h, s, v, f, p, q, t, *r, *g, *b;
  int i;

  h = (hsv.a * M_2PI);
  h = RAD_TO_DEG(h);
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
#else
void _hsv2rgb_float_(pixel_t hsv, pixel_t *rgb,
                     colourspace_option_flags flags) {
  colour_val_t h, s, v, f, p, q, t, *r, *g, *b;
  int i;

  h = (hsv.a * M_2PI);
  h = RAD_TO_DEG(h);
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
#endif

#ifdef CGFLOAT_IS_DOUBLE
void _lab2rgb_double_(pixel_t lab, pixel_t *rgb,
                      colourspace_option_flags flags) {
  pixel_t xyz = {0.0f};
  
  _lab2xyz_double_(lab, &xyz, flags);
  _xyz2rgb_double_(xyz, rgb, flags);
}
#else
void _lab2rgb_float_(pixel_t lab, pixel_t *rgb,
                     colourspace_option_flags flags) {
  pixel_t xyz = {0.0f};
  
  _lab2xyz_float_(lab, &xyz, flags);
  _xyz2rgb_float_(xyz, rgb, flags);
}
#endif

#ifdef CGFLOAT_IS_DOUBLE
void _lch_ab2rgb_double_(pixel_t lch_ab, pixel_t *rgb,
                         colourspace_option_flags flags) {
  pixel_t lab = {0.0f};

  _lch_ab2lab_double_(lch_ab, &lab, flags);
  _lab2rgb_double_(lab, rgb, flags);
}
#else
void _lch_ab2rgb_float_(pixel_t lch_ab, pixel_t *rgb,
                        colourspace_option_flags flags) {
  pixel_t lab = {0.0f};

  _lch_ab2lab_float_(lch_ab, &lab, flags);
  _lab2rgb_float_(lab, rgb, flags);
}
#endif

#ifdef CGFLOAT_IS_DOUBLE
void _lch_uv2rgb_double_(pixel_t lch_uv, pixel_t *rgb,
                         colourspace_option_flags flags) {
  pixel_t lab = {0.0f};

  _lch_uv2lab_double_(lch_uv, &lab, flags);
  _lab2rgb_double_(lab, rgb, flags);
}
#else
void _lch_uv2rgb_float_(pixel_t lch_uv, pixel_t *rgb,
                        colourspace_option_flags flags) {
  pixel_t lab = {0.0f};

  _lch_uv2lab_float_(lch_uv, &lab, flags);
  _lab2rgb_float_(lab, rgb, flags);
}
#endif

#ifdef CGFLOAT_IS_DOUBLE
void _hsi2rgb_double_(pixel_t hsi, pixel_t *rgb,
                      colourspace_option_flags flags) {
  colour_val_t *r, *g, *b, h, s, i;

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
    h -= (M_2PI / 3.0f);
    *r = (1.0f / 3.0f) * (1.0f - s);
    *g = (1.0f / 3.0f) * ((s * cos(h)) / cos((M_2PI / 6.0f) - h));
    *b = 1.0f - ((*g) + (*r));

  } else {
    h -= (2.0f * M_2PI / 3.0f);
    *g = (1.0f / 3.0f) * (1.0f - s);
    *b = (1.0f / 3.0f) * ((s * cos(h)) / cos((M_2PI / 6.0f) - h));
    *r = 1.0f - ((*g) + (*b));
  }

  sanitize_rgb(rgb);
}
#else
void _hsi2rgb_float_(pixel_t hsi, pixel_t *rgb,
                     colourspace_option_flags flags) {
  colour_val_t *r, *g, *b, h, s, i;

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
    h -= (M_2PI / 3.0f);
    *r = (1.0f / 3.0f) * (1.0f - s);
    *g = (1.0f / 3.0f) * ((s * cosf(h)) / cosf((M_2PI / 6.0f) - h));
    *b = 1.0f - ((*g) + (*r));

  } else {
    h -= (2.0f * M_2PI / 3.0f);
    *g = (1.0f / 3.0f) * (1.0f - s);
    *b = (1.0f / 3.0f) * ((s * cosf(h)) / cosf((M_2PI / 6.0f) - h));
    *r = 1.0f - ((*g) + (*b));
  }

  sanitize_rgb(rgb);
}
#endif

#ifdef CGFLOAT_IS_DOUBLE
void _hlab2rgb_double_(pixel_t hlab, pixel_t *rgb,
                       colourspace_option_flags flags) {}
#else
void _hlab2rgb_float_(pixel_t hlab, pixel_t *rgb,
                      colourspace_option_flags flags) {}
#endif

#ifdef CGFLOAT_IS_DOUBLE
void _cmyk2rgb_double_(pixel_t cmyk, pixel_t *rgb,
                       colourspace_option_flags flags) {
  colour_val_t c, m, y, k, *r, *g, *b;

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
#else
void _cmyk2rgb_float_(pixel_t cmyk, pixel_t *rgb,
                      colourspace_option_flags flags) {
  colour_val_t c, m, y, k, *r, *g, *b;

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
#endif

void ryb2rgb(pixel_t ryb, pixel_t *rgb, colourspace_option_flags flags) {
  colour_val_t x0, x1, x2, x3, y0, y1, *r = NULL, *g = NULL, *b = NULL;

  r = &(rgb->a);
  g = &(rgb->b);
  b = &(rgb->c);

  // R
  x0 = cubic_interpolation(ryb.c, 1.0f, 0.163f);
  x1 = cubic_interpolation(ryb.c, 1.0f, 0.0f);
  x2 = cubic_interpolation(ryb.c, 1.0f, 0.5f);
  x3 = cubic_interpolation(ryb.c, 1.0f, 0.2f);
  y0 = cubic_interpolation(ryb.b, x0, x1);
  y1 = cubic_interpolation(ryb.b, x2, x3);
  *r = cubic_interpolation(ryb.a, y0, y1);

  // G
  x0 = cubic_interpolation(ryb.c, 1.0f, 0.373f);
  x1 = cubic_interpolation(ryb.c, 1.0f, 0.66f);
  x2 = cubic_interpolation(ryb.c, 0.0f, 0.0f);
  x3 = cubic_interpolation(ryb.c, 0.5f, 0.094f);
  y0 = cubic_interpolation(ryb.b, x0, x1);
  y1 = cubic_interpolation(ryb.b, x2, x3);
  *g = cubic_interpolation(ryb.a, y0, y1);

  // B
  x0 = cubic_interpolation(ryb.c, 1.0f, 0.6f);
  x1 = cubic_interpolation(ryb.c, 0.0f, 0.2f);
  x2 = cubic_interpolation(ryb.c, 0.0f, 0.5f);
  x3 = cubic_interpolation(ryb.c, 0.0f, 0.0f);
  y0 = cubic_interpolation(ryb.b, x0, x1);
  y1 = cubic_interpolation(ryb.b, x2, x3);
  *b = cubic_interpolation(ryb.a, y0, y1);
}

#ifdef CGFLOAT_IS_DOUBLE
void _ryb2rgb_double_(pixel_t ryb, pixel_t *rgb,
                      colourspace_option_flags flags) {
  ryb2rgb(ryb, rgb, flags);
}
#else
void _ryb2rgb_float_(pixel_t ryb, pixel_t *rgb,
                     colourspace_option_flags flags) {
  ryb2rgb(ryb, rgb, flags);
}
#endif

#pragma mark - To CMYK

void rgb2cmyk(pixel_t rgb, pixel_t *cmyk, colourspace_option_flags flags) {
  colour_val_t max, *c, *m, *y, *k;

  c = &(cmyk->a);
  m = &(cmyk->b);
  y = &(cmyk->c);
  k = &(cmyk->d);

  max = colour_max(rgb.a, rgb.b, rgb.c);

  *k = 1.0f - max;
  *c = (1.0f - rgb.a - *k) / (1.0f - *k);
  *m = (1.0f - rgb.b - *k) / (1.0f - *k);
  *y = (1.0f - rgb.c - *k) / (1.0f - *k);
}

#ifdef CGFLOAT_IS_DOUBLE
void _rgb2cmyk_double_(pixel_t rgb, pixel_t *cmyk,
                       colourspace_option_flags flags) {
  rgb2cmyk(rgb, cmyk, flags);
}
#else
void _rgb2cmyk_float_(pixel_t rgb, pixel_t *cmyk,
                      colourspace_option_flags flags) {
  rgb2cmyk(rgb, cmyk, flags);
}
#endif

#pragma mark - To HSV

void rgb2hsv(pixel_t rgb, pixel_t *hsv, colourspace_option_flags flags) {
  colour_val_t max, min, delta, r, g, b, *h, *s, *v;

  h = &(hsv->a);
  s = &(hsv->b);
  v = &(hsv->c);

  r = rgb.a;
  g = rgb.b;
  b = rgb.c;

  min = colour_min(r, g, b);
  max = colour_max(r, g, b);

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
  *h /= M_2PI;
}

#ifdef CGFLOAT_IS_DOUBLE
void _rgb2hsv_double_(pixel_t rgb, pixel_t *hsv,
                      colourspace_option_flags flags) {
  rgb2hsv(rgb, hsv, flags);
}
#else
void _rgb2hsv_float_(pixel_t rgb, pixel_t *hsv,
                     colourspace_option_flags flags) {
  rgb2hsv(rgb, hsv, flags);
}
#endif

#pragma mark - To HSL

void rgb2hsl(pixel_t rgb, pixel_t *hsl, colourspace_option_flags flags) {
  colour_val_t max, min, delta, *h, *s, *l, dr, dg, db;

  h = &(hsl->a);
  s = &(hsl->b);
  l = &(hsl->c);

  min = colour_min(rgb.a, rgb.b, rgb.c);
  max = colour_max(rgb.a, rgb.b, rgb.c);
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

#ifdef CGFLOAT_IS_DOUBLE
void _rgb2hsl_double_(pixel_t rgb, pixel_t *hsl,
                      colourspace_option_flags flags) {
  rgb2hsl(rgb, hsl, flags);
}
#else
void _rgb2hsl_float_(pixel_t rgb, pixel_t *hsl,
                     colourspace_option_flags flags) {
  rgb2hsl(rgb, hsl, flags);
}
#endif

#pragma mark - To HSI

#ifdef CGFLOAT_IS_DOUBLE
void _rgb2hsi_double_(pixel_t rgb, pixel_t *hsi,
                      colourspace_option_flags flags) {
  colour_val_t *h, *s, *i, r, g, b, max, min, delta;

  h = &(hsi->a);
  s = &(hsi->b);
  i = &(hsi->c);

  r = rgb.a;
  g = rgb.b;
  b = rgb.c;

  min = colour_min(r, g, b);
  max = colour_max(r, g, b);
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
#else
void _rgb2hsi_float_(pixel_t rgb, pixel_t *hsi,
                     colourspace_option_flags flags) {
  colour_val_t *h, *s, *i, r, g, b, max, min, delta;

  h = &(hsi->a);
  s = &(hsi->b);
  i = &(hsi->c);

  r = rgb.a;
  g = rgb.b;
  b = rgb.c;

  min = colour_min(r, g, b);
  max = colour_max(r, g, b);
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
#endif

#pragma mark - Distance Formulas

#ifdef CGFLOAT_IS_DOUBLE
void _cie76_double_(pixel_t lab, pixel_t plab, colour_val_t *dl) {
  *dl = sqrt(pow(plab.a - lab.a, 2.0f) + pow(plab.b - lab.b, 2.0f) +
             pow(plab.c - lab.c, 2.0f));
}
#else
void _cie76_float_(pixel_t lab, pixel_t plab, colour_val_t *dl) {
  *dl = sqrtf(powf(plab.a - lab.a, 2.0f) + powf(plab.b - lab.b, 2.0f) +
              powf(plab.c - lab.c, 2.0f));
}
#endif

#ifdef CGFLOAT_IS_DOUBLE
void _cmc1984_double_(pixel_t lab, pixel_t plab, colour_val_t l, colour_val_t c,
                      colour_val_t *dl) {
  colour_val_t C_1, C_2, C_d, H_1, H_d_ab2, L_d, S_L, S_C, S_H, F, T;

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
#else
void _cmc1984_float_(pixel_t lab, pixel_t plab, colour_val_t l, colour_val_t c,
                     colour_val_t *dl) {
  colour_val_t C_1, C_2, C_d, H_1, H_d_ab2, L_d, S_L, S_C, S_H, F, T;

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
#endif

#ifdef CGFLOAT_IS_DOUBLE
void _cie94_double_(pixel_t lab, pixel_t plab, int media, colour_val_t *dl) {
  colour_val_t D_H_ab2, S_L, S_C, S_H, L_d, C_1, C_2, C_d, K_1, K_2, K_L, K_C,
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
#else
void _cie94_float_(pixel_t lab, pixel_t plab, int media, colour_val_t *dl) {
  colour_val_t D_H_ab2, S_L, S_C, S_H, L_d, C_1, C_2, C_d, K_1, K_2, K_L, K_C,
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
#endif

#ifdef CGFLOAT_IS_DOUBLE
void _ciede2000_double_(pixel_t lab, pixel_t plab, colour_val_t kl,
                        colour_val_t kc, colour_val_t kh, colour_val_t *dl) {
  colour_val_t L_b_p, L_d_p, L_term, a_1_p, a_2_p, C_1_p, C_2_p, C_1_s, C_2_s,
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
}
#else
void _ciede2000_float_(pixel_t lab, pixel_t plab, colour_val_t kl,
                       colour_val_t kc, colour_val_t kh, colour_val_t *dl) {
  colour_val_t L_b_p, L_d_p, L_term, a_1_p, a_2_p, C_1_p, C_2_p, C_1_s, C_2_s,
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
}
#endif