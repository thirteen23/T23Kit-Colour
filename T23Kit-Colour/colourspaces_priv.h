/*
 *  colourspaces_priv.h
 *  T23Kit-Colour
 *
 *  Created by Michael Van Milligan on 4/11/14.
 *  Copyright (c) 2014 Thirteen23. All rights reserved.
 */

#ifdef __cplusplus
extern "C" {
#endif

#import <CoreGraphics/CoreGraphics.h>

#ifndef LIBCOLORSPACES_PRIV_H
#define LIBCOLORSPACES_PRIV_H

#define LIBCOLORSPACES_EXTERN extern

/* Types for now */
typedef CGFloat colour_val_t;

typedef struct pixel_t {
  colour_val_t a;
  colour_val_t b;
  colour_val_t c;
  colour_val_t d;
} pixel_t;

typedef enum colourspace_rgb_profile {
  colourspace_rgb_profile_adobe_d65,
  colourspace_rgb_profile_apple_d65,
  colourspace_rgb_profile_best_d50,
  colourspace_rgb_profile_beta_d50,
  colourspace_rgb_profile_bruce_d65,
  colourspace_rgb_profile_cie_e,
  colourspace_rgb_profile_colormatch_d50,
  colourspace_rgb_profile_don_d50,
  colourspace_rgb_profile_eci_d50,
  colourspace_rgb_profile_ekta_space_d50,
  colourspace_rgb_profile_ntsc_c,
  colourspace_rgb_profile_pal_secam_d65,
  colourspace_rgb_profile_prophoto_d50,
  colourspace_rgb_profile_smpte_c_d65,
  colourspace_rgb_profile_srgb_d65,
  colourspace_rgb_profile_wide_gamut_d50,
  colourspace_rgb_profile_adobe_d50,
  colourspace_rgb_profile_apple_d50,
  colourspace_rgb_profile_bruce_d50,
  colourspace_rgb_profile_cie_d50,
  colourspace_rgb_profile_ntsc_d50,
  colourspace_rgb_profile_pal_secam_d50,
  colourspace_rgb_profile_smpte_c_d50,
  colourspace_rgb_profile_srgb_d50,
  colourspace_rgb_profile_max,
} colourspace_rgb_profile;

LIBCOLORSPACES_EXTERN void _hsl2rgb_double_(pixel_t hsl, pixel_t *rgb);
LIBCOLORSPACES_EXTERN void _hsl2rgb_float_(pixel_t hsl, pixel_t *rgb);

LIBCOLORSPACES_EXTERN void _hsv2rgb_double_(pixel_t hsv, pixel_t *rgb);
LIBCOLORSPACES_EXTERN void _hsv2rgb_float_(pixel_t hsv, pixel_t *rgb);

LIBCOLORSPACES_EXTERN void _lch_ab2rgb_double_(pixel_t lch_ab, pixel_t *rgb,
                                               colourspace_rgb_profile prof);
LIBCOLORSPACES_EXTERN void _lch_ab2rgb_float_(pixel_t lch_ab, pixel_t *rgb,
                                              colourspace_rgb_profile prof);

LIBCOLORSPACES_EXTERN void _lch_uv2rgb_double_(pixel_t lch_uv, pixel_t *rgb,
                                               colourspace_rgb_profile prof);
LIBCOLORSPACES_EXTERN void _lch_uv2rgb_float_(pixel_t lch_uv, pixel_t *rgb,
                                              colourspace_rgb_profile prof);

LIBCOLORSPACES_EXTERN void _hsi2rgb_double_(pixel_t hsi, pixel_t *rgb);
LIBCOLORSPACES_EXTERN void _hsi2rgb_float_(pixel_t hsi, pixel_t *rgb);

LIBCOLORSPACES_EXTERN void _lab2rgb_double_(pixel_t lab, pixel_t *rgb,
                                            colourspace_rgb_profile prof);
LIBCOLORSPACES_EXTERN void _lab2rgb_float_(pixel_t lab, pixel_t *rgb,
                                           colourspace_rgb_profile prof);

LIBCOLORSPACES_EXTERN void _hlab2rgb_double_(pixel_t hlab, pixel_t *rgb);
LIBCOLORSPACES_EXTERN void _hlab2rgb_float_(pixel_t hlab, pixel_t *rgb);

LIBCOLORSPACES_EXTERN void _cmyk2rgb_double_(pixel_t cmyk, pixel_t *rgb);
LIBCOLORSPACES_EXTERN void _cmyk2rgb_float_(pixel_t cmyk, pixel_t *rgb);

LIBCOLORSPACES_EXTERN void _xyz2rgb_double_(pixel_t xyz, pixel_t *rgb,
                                            colourspace_rgb_profile prof);
LIBCOLORSPACES_EXTERN void _xyz2rgb_float_(pixel_t xyz, pixel_t *rgb,
                                           colourspace_rgb_profile prof);

LIBCOLORSPACES_EXTERN void _ryb2rgb_double_(pixel_t ryb, pixel_t *rgb);
LIBCOLORSPACES_EXTERN void _ryb2rgb_float_(pixel_t ryb, pixel_t *rgb);

LIBCOLORSPACES_EXTERN void _rgb2cmyk_double_(pixel_t rgb, pixel_t *cmyk);
LIBCOLORSPACES_EXTERN void _rgb2cmyk_float_(pixel_t rgb, pixel_t *cmyk);

LIBCOLORSPACES_EXTERN void _rgb2hsv_double_(pixel_t rgb, pixel_t *hsv);
LIBCOLORSPACES_EXTERN void _rgb2hsv_float_(pixel_t rgb, pixel_t *hsv);

LIBCOLORSPACES_EXTERN void _rgb2hsl_double_(pixel_t rgb, pixel_t *hsl);
LIBCOLORSPACES_EXTERN void _rgb2hsl_float_(pixel_t rgb, pixel_t *hsl);

LIBCOLORSPACES_EXTERN void _rgb2hsi_double_(pixel_t rgb, pixel_t *hsi);
LIBCOLORSPACES_EXTERN void _rgb2hsi_float_(pixel_t rgb, pixel_t *hsi);

LIBCOLORSPACES_EXTERN void _rgb2hlab_double_(pixel_t rgb, pixel_t *hlab,
                                             colourspace_rgb_profile prof);
LIBCOLORSPACES_EXTERN void _rgb2hlab_float_(pixel_t rgb, pixel_t *hlab,
                                            colourspace_rgb_profile prof);

LIBCOLORSPACES_EXTERN void _xyz2hlab_double_(pixel_t xyz, pixel_t *hlab);
LIBCOLORSPACES_EXTERN void _xyz2hlab_float_(pixel_t xyz, pixel_t *hlab);

LIBCOLORSPACES_EXTERN void _rgb2lab_double_(pixel_t rgb, pixel_t *lab,
                                            colourspace_rgb_profile prof);
LIBCOLORSPACES_EXTERN void _rgb2lab_float_(pixel_t rgb, pixel_t *lab,
                                           colourspace_rgb_profile prof);

LIBCOLORSPACES_EXTERN void _xyz2lab_double_(pixel_t xyz, pixel_t *lab);
LIBCOLORSPACES_EXTERN void _xyz2lab_float_(pixel_t xyz, pixel_t *lab);

LIBCOLORSPACES_EXTERN void _lch_ab2lab_double_(pixel_t lch_ab, pixel_t *lab);
LIBCOLORSPACES_EXTERN void _lch_ab2lab_float_(pixel_t lch_ab, pixel_t *lab);

LIBCOLORSPACES_EXTERN void _lch_uv2lab_double_(pixel_t lch_uv, pixel_t *lab);
LIBCOLORSPACES_EXTERN void _lch_uv2lab_float_(pixel_t lch_uv, pixel_t *lab);

LIBCOLORSPACES_EXTERN void _rgb2luv_double_(pixel_t rgb, pixel_t *luv,
                                            colourspace_rgb_profile prof);
LIBCOLORSPACES_EXTERN void _rgb2luv_float_(pixel_t rgb, pixel_t *luv,
                                           colourspace_rgb_profile prof);

LIBCOLORSPACES_EXTERN void _xyz2luv_double_(pixel_t xyz, pixel_t *luv);
LIBCOLORSPACES_EXTERN void _xyz2luv_float_(pixel_t xyz, pixel_t *luv);

LIBCOLORSPACES_EXTERN void _lch_ab2luv_double_(pixel_t lch_ab, pixel_t *luv);
LIBCOLORSPACES_EXTERN void _lch_ab2luv_float_(pixel_t lch_ab, pixel_t *luv);

LIBCOLORSPACES_EXTERN void _lch_uv2luv_double_(pixel_t lch_uv, pixel_t *luv);
LIBCOLORSPACES_EXTERN void _lch_uv2luv_float_(pixel_t lch_uv, pixel_t *luv);

LIBCOLORSPACES_EXTERN void _rgb2lch_ab_double_(pixel_t rgb, pixel_t *lch_ab,
                                               colourspace_rgb_profile prof);
LIBCOLORSPACES_EXTERN void _rgb2lch_ab_float_(pixel_t rgb, pixel_t *lch_ab,
                                              colourspace_rgb_profile prof);

LIBCOLORSPACES_EXTERN void _lab2lch_ab_double_(pixel_t lab, pixel_t *lch_ab);
LIBCOLORSPACES_EXTERN void _lab2lch_ab_float_(pixel_t lab, pixel_t *lch_ab);

LIBCOLORSPACES_EXTERN void _luv2lch_ab_double_(pixel_t luv, pixel_t *lch_ab);
LIBCOLORSPACES_EXTERN void _luv2lch_ab_float_(pixel_t luv, pixel_t *lch_ab);

LIBCOLORSPACES_EXTERN void _rgb2lch_uv_double_(pixel_t rgb, pixel_t *lch_uv,
                                               colourspace_rgb_profile prof);
LIBCOLORSPACES_EXTERN void _rgb2lch_uv_float_(pixel_t rgb, pixel_t *lch_uv,
                                              colourspace_rgb_profile prof);

LIBCOLORSPACES_EXTERN void _luv2lch_uv_double_(pixel_t luv, pixel_t *lch_uv);
LIBCOLORSPACES_EXTERN void _luv2lch_uv_float_(pixel_t luv, pixel_t *lch_uv);

LIBCOLORSPACES_EXTERN void _lab2lch_uv_double_(pixel_t lab, pixel_t *lch_uv);
LIBCOLORSPACES_EXTERN void _lab2lch_uv_float_(pixel_t lab, pixel_t *lch_uv);

LIBCOLORSPACES_EXTERN void _xyY2xyz_double_(pixel_t xyY, pixel_t *xyz);
LIBCOLORSPACES_EXTERN void _xyY2xyz_float_(pixel_t xyY, pixel_t *xyz);

LIBCOLORSPACES_EXTERN void _rgb2xyz_double_(pixel_t rgb, pixel_t *xyz,
                                            colourspace_rgb_profile prof);
LIBCOLORSPACES_EXTERN void _rgb2xyz_float_(pixel_t rgb, pixel_t *xyz,
                                           colourspace_rgb_profile prof);

LIBCOLORSPACES_EXTERN void _hlab2xyz_double_(pixel_t hlab, pixel_t *xyz);
LIBCOLORSPACES_EXTERN void _hlab2xyz_float_(pixel_t hlab, pixel_t *xyz);

LIBCOLORSPACES_EXTERN void _lab2xyz_double_(pixel_t lab, pixel_t *xyz);
LIBCOLORSPACES_EXTERN void _lab2xyz_float_(pixel_t lab, pixel_t *xyz);

LIBCOLORSPACES_EXTERN void _luv2xyz_double_(pixel_t luv, pixel_t *xyz);
LIBCOLORSPACES_EXTERN void _luv2xyz_float_(pixel_t luv, pixel_t *xyz);

/* Helpers */
LIBCOLORSPACES_EXTERN void _print_pixel_double_(pixel_t p);
LIBCOLORSPACES_EXTERN void _print_pixel_float_(pixel_t p);

/* Distance Metrics */
LIBCOLORSPACES_EXTERN void _cie76_double_(pixel_t lab, pixel_t plab,
                                          colour_val_t *dl);
LIBCOLORSPACES_EXTERN void _cie76_float_(pixel_t lab, pixel_t plab,
                                         colour_val_t *dl);

LIBCOLORSPACES_EXTERN void _cmc1984_double_(pixel_t lab, pixel_t plab,
                                            colour_val_t l, colour_val_t c,
                                            colour_val_t *dl);
LIBCOLORSPACES_EXTERN void _cmc1984_float_(pixel_t lab, pixel_t plab,
                                           colour_val_t l, colour_val_t c,
                                           colour_val_t *dl);

LIBCOLORSPACES_EXTERN void _cie94_double_(pixel_t lab, pixel_t plab, int media,
                                          colour_val_t *dl);
LIBCOLORSPACES_EXTERN void _cie94_float_(pixel_t lab, pixel_t plab, int media,
                                         colour_val_t *dl);

LIBCOLORSPACES_EXTERN void _ciede2000_double_(pixel_t lab, pixel_t plab,
                                              colour_val_t kl, colour_val_t kc,
                                              colour_val_t kh,
                                              colour_val_t *dl);
LIBCOLORSPACES_EXTERN void _ciede2000_float_(pixel_t lab, pixel_t plab,
                                             colour_val_t kl, colour_val_t kc,
                                             colour_val_t kh, colour_val_t *dl);

#endif /* LIBCOLORSPACES_PRIV_H */

#ifdef __cplusplus
}
#endif
