//
//  colorspaces.h
//  FrickBits
//
//  Created by Michael Van Milligan on 3/4/14.
//  Copyright (c) 2014 Michael Van Milligan. All rights reserved.
//

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LIBCOLORSPACES_H
#define LIBCOLORSPACES_H

#import <CoreGraphics/CoreGraphics.h>
#include "colorspaces_priv.h"

typedef struct color_t {
  union {
    pixel_t rgb;
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
} color_t;

#define LIBCOLORSPACES_GRAPHICS (1)
#define LIBCOLORSPACES_TEXTILES (-1)

#define RGB p.rgb
#define RGB_R p.rgb.a
#define RGB_G p.rgb.b
#define RGB_B p.rgb.c

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
#if CGFLOAT_IS_DOUBLE
#define HSL_2_RGB _hsl2rgb_double_
#else
#define HSL_2_RGB _hsl2rgb_float_
#endif

#if CGFLOAT_IS_DOUBLE
#define HSV_2_RGB _hsv2rgb_double_
#else
#define HSV_2_RGB _hsv2rgb_float_
#endif

#if CGFLOAT_IS_DOUBLE
#define LCH_AB_2_RGB _lch_ab2rgb_double_
#else
#define LCH_AB_2_RGB _lch_ab2rgb_float_
#endif

#if CGFLOAT_IS_DOUBLE
#define LCH_UV_2_RGB _lch_uv2rgb_double_
#else
#define LCH_UV_2_RGB _lch_uv2rgb_float_
#endif

#if CGFLOAT_IS_DOUBLE
#define HSI_2_RGB _hsi2rgb_double_
#else
#define HSI_2_RGB _hsi2rgb_float_
#endif

#if CGFLOAT_IS_DOUBLE
#define LAB_2_RGB _lab2rgb_double_
#else
#define LAB_2_RGB _lab2rgb_float_
#endif

#if CGFLOAT_IS_DOUBLE
#define CMYK_2_RGB _cmyk2rgb_double_
#else
#define CMYK_2_RGB _cmyk2rgb_float_
#endif

#if CGFLOAT_IS_DOUBLE
#define XYZ_2_RGB _xyz2rgb_double_
#else
#define XYZ_2_RGB _xyz2rgb_float_
#endif

/* To CMYK */
#if CGFLOAT_IS_DOUBLE
#define RGB_2_CMYK _rgb2cmyk_double_
#else
#define RGB_2_CMYK _rgb2cmyk_float_
#endif

/* To HSV */
#if CGFLOAT_IS_DOUBLE
#define RGB_2_HSV _rgb2hsv_double_
#else
#define RGB_2_HSV _rgb2hsv_float_
#endif

/* To HSL */
#if CGFLOAT_IS_DOUBLE
#define RGB_2_HSL _rgb2hsl_double_
#else
#define RGB_2_HSL _rgb2hsl_float_
#endif

/* To HSI */
#if CGFLOAT_IS_DOUBLE
#define RGB_2_HSI _rgb2hsi_double_
#else
#define RGB_2_HSI _rgb2hsi_float_
#endif

/* To HLAB */
#if CGFLOAT_IS_DOUBLE
#define RGB_2_HLAB _rgb2hlab_double_
#else
#define RGB_2_HLAB _rgb2hlab_float_
#endif

#if CGFLOAT_IS_DOUBLE
#define XYZ_2_HLAB _xyz2hlab_double_
#else
#define XYZ_2_HLAB _xyz2hlab_float_
#endif

/* To LAB */
#if CGFLOAT_IS_DOUBLE
#define RGB_2_LAB _rgb2lab_double_
#else
#define RGB_2_LAB _rgb2lab_float_
#endif

#if CGFLOAT_IS_DOUBLE
#define XYZ_2_LAB _xyz2lab_double_
#else
#define XYZ_2_LAB _xyz2lab_float_
#endif

#if CGFLOAT_IS_DOUBLE
#define LCH_AB_2_LAB _lch_ab2lab_double_
#else
#define LCH_AB_2_LAB _lch_ab2lab_float_
#endif

#if CGFLOAT_IS_DOUBLE
#define LCH_UV_2_LAB _lch_uv2lab_double_
#else
#define LCH_UV_2_LAB _lch_uv2lab_float_
#endif

/* To LUV */
#if CGFLOAT_IS_DOUBLE
#define RGB_2_LUV _rgb2luv_double_
#else
#define RGB_2_LUV _rgb2luv_float_
#endif

#if CGFLOAT_IS_DOUBLE
#define XYZ_2_LUV _xyz2luv_double_
#else
#define XYZ_2_LUV _xyz2luv_float_
#endif

#if CGFLOAT_IS_DOUBLE
#define LCH_AB_2_LUV _lch_ab2luv_double_
#else
#define LCH_AB_2_LUV _lch_ab2luv_float_
#endif

#if CGFLOAT_IS_DOUBLE
#define LCH_UV_2_LUV _lch_uv2luv_double_
#else
#define LCH_UV_2_LUV _lch_uv2luv_float_
#endif

/* To LCH(AB) */
#if CGFLOAT_IS_DOUBLE
#define RGB_2_LCH_AB _rgb2lch_ab_double_
#else
#define RGB_2_LCH_AB _rgb2lch_ab_float_
#endif

#if CGFLOAT_IS_DOUBLE
#define LUV_2_LCH_AB _luv2lch_ab_double_
#else
#define LUV_2_LCH_AB _luv2lch_ab_float_
#endif

#if CGFLOAT_IS_DOUBLE
#define LAB_2_LCH_AB _lab2lch_ab_double_
#else
#define LAB_2_LCH_AB _lab2lch_ab_float_
#endif

/* To LCH(UV) */
#if CGFLOAT_IS_DOUBLE
#define RGB_2_LCH_UV _rgb2lch_uv_double_
#else
#define RGB_2_LCH_UV _rgb2lch_uv_float_
#endif

#if CGFLOAT_IS_DOUBLE
#define LUV_2_LCH_UV _luv2lch_uv_double_
#else
#define LUV_2_LCH_UV _luv2lch_uv_float_
#endif

#if CGFLOAT_IS_DOUBLE
#define LAB_2_LCH_UV _lab2lch_uv_double_
#else
#define LAB_2_LCH_UV _lab2lch_uv_float_
#endif

/* to XYZ */
#if CGFLOAT_IS_DOUBLE
#define XYY_2_XYZ _xyY2xyz_double_
#else
#define XYY_2_XYZ _xyY2xyz_float_
#endif

#if CGFLOAT_IS_DOUBLE
#define RGB_2_XYZ _rgb2xyz_double_
#else
#define RGB_2_XYZ _rgb2xyz_float_
#endif

#if CGFLOAT_IS_DOUBLE
#define HLAB_2_XYZ _hlab2xyz_double_
#else
#define HLAB_2_XYZ _hlab2xyz_float_
#endif

#if CGFLOAT_IS_DOUBLE
#define LAB_2_XYZ _lab2xyz_double_
#else
#define LAB_2_XYZ _lab2xyz_float_
#endif

#if CGFLOAT_IS_DOUBLE
#define LUV_2_XYZ _luv2xyz_double_
#else
#define LUV_2_XYZ _luv2xyz_float_
#endif

/* Helpers */
#if CGFLOAT_IS_DOUBLE
#define PRINT_PIXEL _print_pixel_double_
#else
#define PRINT_PIXEL _print_pixel_float_
#endif

/* Distance Metrics */
#if CGFLOAT_IS_DOUBLE
#define CIE_76 _cie76_double_
#else
#define CIE_76 _cie76_float_
#endif

#if CGFLOAT_IS_DOUBLE
#define CMC_1984 _cmc1984_double_
#else
#define CMC_1984 _cmc1984_float_
#endif

#if CGFLOAT_IS_DOUBLE
#define CIE_94 _cie94_double_
#else
#define CIE_94 _cie94_float_
#endif

#if CGFLOAT_IS_DOUBLE
#define CIEDE_2000 _ciede2000_double_
#else
#define CIEDE_2000 _ciede2000_float_
#endif

#endif /* LIBCOLORSPACES_H */

#ifdef __cplusplus
}
#endif
