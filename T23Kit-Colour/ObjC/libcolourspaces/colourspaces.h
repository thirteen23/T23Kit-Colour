/*
 *  colourspaces.h
 *  T23Kit-Colour
 *
 *  Created by Michael Van Milligan on 4/11/14.
 *  Copyright (c) 2014 Thirteen23. All rights reserved.
 */

#ifdef __cplusplus
extern "C" {
#endif
  
#ifndef LIBCOLOURSPACES_H
#define LIBCOLOURSPACES_H

#define epsilon 0.008856f
#define kappa 7.787f

#define M_2PI (2.0f * M_PI)
#define DEG_TO_RAD(A) ((A) * (M_PI / 180.0))
#define RAD_TO_DEG(R) ((R) * (180.0 / M_PI))

#define COLOURSPACE_LOG(format, ...) fprintf(stderr, format, __VA_ARGS__)
  
#define COLOURSPACE_OPTION_REFERENCE_WHITE(flags) ((flags & 0xFFFF0000) >> 16)
#define COLOURSPACE_OPTION_RGB_PROFILE(flags) (flags & 0x0000FFFF)
  
#define LIBCOLORSPACES_GRAPHICS (1)
#define LIBCOLORSPACES_TEXTILES (-1)
  
#define RGB p.rgb
#define RGB_R p.rgb.a
#define RGB_G p.rgb.b
#define RGB_B p.rgb.c
  
#define RYB p.ryb
#define RYB_R p.ryb.a
#define RYB_Y p.ryb.b
#define RYB_B p.ryb.c
  
#define HSL p.hsl
#define HSL_H p.hsl.a
#define HSL_S p.hsl.b
#define HSL_L p.hsl.c
  
#define HSV p.hsv
#define HSV_H p.hsv.a
#define HSV_S p.hsv.b
#define HSV_V p.hsv.c
  
#define LCH_AB p.lch_ab
#define LCH_AB_L p.lch_ab.a
#define LCH_AB_C p.lch_ab.b
#define LCH_AB_H p.lch_ab.c
  
#define LCH_UV p.lch_uv
#define LCH_UV_L p.lch_uv.a
#define LCH_UV_C p.lch_uv.b
#define LCH_UV_H p.lch_uv.c
  
#define HSI p.hsi
#define HSI_H p.hsi.a
#define HSI_S p.hsi.b
#define HSI_I p.hsi.c
  
#define HLAB p.hlab
#define HLAB_L p.hlab.a
#define HLAB_A p.hlab.b
#define HLAB_B p.hlab.c
  
#define LAB p.lab
#define LAB_L p.lab.a
#define LAB_A p.lab.b
#define LAB_B p.lab.c
  
#define LUV p.luv
#define LUV_L p.luv.a
#define LUV_U p.luv.b
#define LUV_V p.luv.c
  
#define CMYK p.cmyk
#define CMYK_C p.cmyk.a
#define CMYK_M p.cmyk.b
#define CMYK_Y p.cmyk.c
#define CMYK_K p.cmyk.d
  
#define XYZ p.xyz
#define XYZ_X p.xyz.a
#define XYZ_Y p.xyz.b
#define XYZ_Z p.xyz.c
  
#define XYY p.xyY
#define XYY_X p.xyY.a
#define XYY_Y p.xyY.b
#define XYY_Y2 p.xyY.c
  
  /* To RGB */

#define HSL_2_RGB _hsl2rgb
#define HSV_2_RGB _hsv2rgb
#define LCH_AB_2_RGB _lch_ab2rgb
#define LCH_UV_2_RGB _lch_uv2rgb
#define HSI_2_RGB _hsi2rgb
#define LAB_2_RGB _lab2rgb
#define CMYK_2_RGB _cmyk2rgb
#define XYZ_2_RGB _xyz2rgb
#define RYB_2_RGB _ryb2rgb
  
  /* To CMYK */
#define RGB_2_CMYK _rgb2cmyk
  
  /* To HSV */
#define RGB_2_HSV _rgb2hsv
  
  /* To HSL */
#define RGB_2_HSL _rgb2hsl
  
  /* To HSI */
#define RGB_2_HSI _rgb2hsi
  
  /* To HLAB */
#define RGB_2_HLAB _rgb2hlab
#define XYZ_2_HLAB _xyz2hlab
  
  /* To LAB */
#define RGB_2_LAB _rgb2lab
#define XYZ_2_LAB _xyz2lab
#define LCH_AB_2_LAB _lch_ab2lab
#define LCH_UV_2_LAB _lch_uv2lab
  
  /* To LUV */
#define RGB_2_LUV _rgb2luv
#define XYZ_2_LUV _xyz2luv
#define LCH_AB_2_LUV _lch_ab2luv
#define LCH_UV_2_LUV _lch_uv2luv
  
  /* To LCH(AB) */
#define RGB_2_LCH_AB _rgb2lch
#define LUV_2_LCH_AB _luv2lch_ab
#define LAB_2_LCH_AB _lab2lch_ab
  
  /* To LCH(UV) */
#define RGB_2_LCH_UV _rgb2lch_uv
#define LUV_2_LCH_UV _luv2lch_uv
#define LAB_2_LCH_UV _lab2lch_uv
  
  /* to XYZ */
#define XYY_2_XYZ _xyY2xyz
#define RGB_2_XYZ _rgb2xyz
#define HLAB_2_XYZ _hlab2xyz
#define LAB_2_XYZ _lab2xyz
#define LUV_2_XYZ _luv2xyz
  
  /* Helpers */
#define PRINT_PIXEL _print_pixel
  
  /* Distance Metrics */
#define CIE_76 _cie76
#define CMC_1984 _cmc1984
#define CIE_94 _cie94
#define CIEDE_2000 _ciede2000
  
/* Types for now */
typedef double colour_val_t;

typedef struct pixel_t {
  colour_val_t a;
  colour_val_t b;
  colour_val_t c;
  colour_val_t d;
} pixel_t;

typedef struct colour_t {
  union {
    pixel_t rgb;
    pixel_t ryb;
    pixel_t hsl;
    pixel_t hsv;
    pixel_t lch_ab;
    pixel_t lch_uv;
    pixel_t hsi;
    pixel_t hlab;
    pixel_t lab;
    pixel_t luv;
    pixel_t cmyk;
    pixel_t xyz;
    pixel_t xyY;
  } p;
} colour_t;

typedef uint32_t colourspace_option_flags;

typedef enum colourspace_reference_white {
  colourspace_reference_white_A = (0 << 16),
  colourspace_reference_white_B = (1 << 16),
  colourspace_reference_white_C = (2 << 16),
  colourspace_reference_white_D50 = (3 << 16),
  colourspace_reference_white_D55 = (4 << 16),
  colourspace_reference_white_D65 = (5 << 16),
  colourspace_reference_white_D75 = (6 << 16),
  colourspace_reference_white_E = (7 << 16),
  colourspace_reference_white_F2 = (8 << 16),
  colourspace_reference_white_F7 = (9 << 16),
  colourspace_reference_white_F11 = (10 << 16),
  colourspace_reference_white_max = (11 << 16)
} colourspace_reference_white;

typedef enum colourspace_rgb_profile {
  colourspace_rgb_profile_adobe_d65 = (0 | colourspace_reference_white_D65),
  colourspace_rgb_profile_apple_d65 = (1 | colourspace_reference_white_D65),
  colourspace_rgb_profile_best_d50 = (2 | colourspace_reference_white_D50),
  colourspace_rgb_profile_beta_d50 = (3 | colourspace_reference_white_D50),
  colourspace_rgb_profile_bruce_d65 = (4 | colourspace_reference_white_D65),
  colourspace_rgb_profile_cie_e = (5 | colourspace_reference_white_E),
  colourspace_rgb_profile_colormatch_d50 =
  (6 | colourspace_reference_white_D50),
  colourspace_rgb_profile_don_d50 = (7 | colourspace_reference_white_D50),
  colourspace_rgb_profile_eci_d50 = (8 | colourspace_reference_white_D50),
  colourspace_rgb_profile_ekta_space_d50 =
  (9 | colourspace_reference_white_D50),
  colourspace_rgb_profile_ntsc_c = (10 | colourspace_reference_white_C),
  colourspace_rgb_profile_pal_secam_d65 =
  (11 | colourspace_reference_white_D65),
  colourspace_rgb_profile_prophoto_d50 = (12 | colourspace_reference_white_D50),
  colourspace_rgb_profile_smpte_c_d65 = (13 | colourspace_reference_white_D65),
  colourspace_rgb_profile_srgb_d65 = (14 | colourspace_reference_white_D65),
  colourspace_rgb_profile_wide_gamut_d50 =
  (15 | colourspace_reference_white_D50),
  colourspace_rgb_profile_adobe_d50 = (16 | colourspace_reference_white_D50),
  colourspace_rgb_profile_apple_d50 = (17 | colourspace_reference_white_D50),
  colourspace_rgb_profile_bruce_d50 = (18 | colourspace_reference_white_D50),
  colourspace_rgb_profile_cie_d50 = (19 | colourspace_reference_white_D50),
  colourspace_rgb_profile_ntsc_d50 = (20 | colourspace_reference_white_D50),
  colourspace_rgb_profile_pal_secam_d50 =
  (21 | colourspace_reference_white_D50),
  colourspace_rgb_profile_smpte_c_d50 = (22 | colourspace_reference_white_D50),
  colourspace_rgb_profile_srgb_d50 = (23 | colourspace_reference_white_D50),
  colourspace_rgb_profile_max = (24 | colourspace_reference_white_max),
} colourspace_rgb_profile;

extern void _hsl2rgb(pixel_t hsl, pixel_t *rgb,
                                         colourspace_option_flags flags);

extern void _hsv2rgb(pixel_t hsv, pixel_t *rgb,
                                         colourspace_option_flags flags);

extern void _lch_ab2rgb(pixel_t lch_ab, pixel_t *rgb,
                                            colourspace_option_flags flags);

extern void _lch_uv2rgb(pixel_t lch_uv, pixel_t *rgb,
                                            colourspace_option_flags flags);

extern void _hsi2rgb(pixel_t hsi, pixel_t *rgb,
                                         colourspace_option_flags flags);

extern void _lab2rgb(pixel_t lab, pixel_t *rgb,
                                         colourspace_option_flags flags);

extern void _hlab2rgb(pixel_t hlab, pixel_t *rgb,
                                          colourspace_option_flags flags);

extern void _cmyk2rgb(pixel_t cmyk, pixel_t *rgb,
                                          colourspace_option_flags flags);

extern void _xyz2rgb(pixel_t xyz, pixel_t *rgb,
                                         colourspace_option_flags flags);

extern void _ryb2rgb(pixel_t ryb, pixel_t *rgb,
                                         colourspace_option_flags flags);

extern void _rgb2cmyk(pixel_t rgb, pixel_t *cmyk,
                                          colourspace_option_flags flags);

extern void _rgb2hsv(pixel_t rgb, pixel_t *hsv,
                                         colourspace_option_flags flags);

extern void _rgb2hsl(pixel_t rgb, pixel_t *hsl,
                                         colourspace_option_flags flags);

extern void _rgb2hsi(pixel_t rgb, pixel_t *hsi,
                                         colourspace_option_flags flags);

extern void _rgb2hlab(pixel_t rgb, pixel_t *hlab,
                                          colourspace_option_flags flags);

extern void _xyz2hlab(pixel_t xyz, pixel_t *hlab,
                                          colourspace_option_flags flags);

extern void _rgb2lab(pixel_t rgb, pixel_t *lab,
                                         colourspace_option_flags flags);

extern void _xyz2lab(pixel_t xyz, pixel_t *lab,
                                         colourspace_option_flags flags);

extern void _lch_ab2lab(pixel_t lch_ab, pixel_t *lab,
                                            colourspace_option_flags flags);

extern void _lch_uv2lab(pixel_t lch_uv, pixel_t *lab,
                                            colourspace_option_flags flags);

extern void _rgb2luv(pixel_t rgb, pixel_t *luv,
                                         colourspace_option_flags flags);

extern void _xyz2luv(pixel_t xyz, pixel_t *luv,
                                         colourspace_option_flags flags);

extern void _lch_ab2luv(pixel_t lch_ab, pixel_t *luv,
                                            colourspace_option_flags flags);

extern void _lch_uv2luv(pixel_t lch_uv, pixel_t *luv,
                                            colourspace_option_flags flags);

extern void _rgb2lch(pixel_t rgb, pixel_t *lch_ab,
                                            colourspace_option_flags flags);

extern void _lab2lch_ab(pixel_t lab, pixel_t *lch_ab,
                                            colourspace_option_flags flags);

extern void _luv2lch_ab(pixel_t luv, pixel_t *lch_ab,
                                            colourspace_option_flags flags);

extern void _rgb2lch_uv(pixel_t rgb, pixel_t *lch_uv,
                                            colourspace_option_flags flags);

extern void _luv2lch_uv(pixel_t luv, pixel_t *lch_uv,
                                            colourspace_option_flags flags);

extern void _lab2lch_uv(pixel_t lab, pixel_t *lch_uv,
                                            colourspace_option_flags flags);

extern void _xyY2xyz(pixel_t xyY, pixel_t *xyz,
                                         colourspace_option_flags flags);

extern void _rgb2xyz(pixel_t rgb, pixel_t *xyz,
                                         colourspace_option_flags flags);

extern void _hlab2xyz(pixel_t hlab, pixel_t *xyz,
                                          colourspace_option_flags flags);

extern void _lab2xyz(pixel_t lab, pixel_t *xyz,
                                         colourspace_option_flags flags);

extern void _luv2xyz(pixel_t luv, pixel_t *xyz,
                                         colourspace_option_flags flags);

/* Helpers */
extern void _print_pixel(pixel_t p);

/* Distance Metrics */
extern void _cie76(pixel_t lab, pixel_t plab,
                                       colour_val_t *dl);

extern void _cmc1984(pixel_t lab, pixel_t plab,
                                         colour_val_t l, colour_val_t c,
                                         colour_val_t *dl);

extern void _cie94(pixel_t lab, pixel_t plab, int media,
                                       colour_val_t *dl);

extern void _ciede2000(pixel_t lab, pixel_t plab,
                                           colour_val_t kl, colour_val_t kc,
                                           colour_val_t kh,
                                           colour_val_t *dl);


#endif /* LIBCOLOURSPACES_H */

#ifdef __cplusplus
}
#endif
