/*
 *  UIColor+T23ColorSpaces.h
 *  T23Kit-Colour
 *
 *  Created by Michael Van Milligan on 4/11/14.
 *  Copyright (c) 2014 Thirteen23. All rights reserved.
 */

#import <UIKit/UIKit.h>
#include "colorspaces.h"

/*
 * Color conversion category for UIColor.
 *
 *  Much prop's to:
 *    Wikipedia (http://en.wikipedia.org/wiki/Color_difference)
 *    http://www.brucelindbloom.com
 *    http://www.easyrgb.com
 */

/* Distance formula options */
typedef NS_ENUM(NSInteger, T23UIColorDistanceOptions) {
  T23UIColorDistanceFormulaCEI76 = 0, /* default */
  T23UIColorDistanceFormulaCMC1984_1_1,
  T23UIColorDistanceFormulaCMC1984_2_1,
  T23UIColorDistanceFormulaCEI94_GRAPHICS,
  T23UIColorDistanceFormulaCEI94_TEXTILES,
  T23UIColorDistanceFormulaCIEDE2000
};

@interface UIColor (T23ColorSpaces)

/*!
 *  @abstract Convert current color to the XYZ color space
 *  @discussion Values returned are nominal values and therefore aren't
 *  normalized in any way, e.g., percentage.
 *
 *  The conversions are from RGB space and uses the sRGB matrix for calculation,
 *  i.e, sRGB matrices to XYZ and back with (Observer = 2°, Illuminant = D65)
 */
- (BOOL)getX:(CGFloat *)coneResponse
           Y:(CGFloat *)luminance
           Z:(CGFloat *)quasiBlue
       alpha:(CGFloat *)alpha;

/*!
 *  @abstract Convert current color to the Hunter 1948 L, a, b color space
 *  @discussion Values returned are nominal values and therefore aren't
 *  normalized in any way, e.g., percentage.
 */
- (BOOL)getHunterLStar:(CGFloat *)lightness
                 aStar:(CGFloat *)greenToMagenta
                 bStar:(CGFloat *)yellowtoBlue
                 alpha:(CGFloat *)alpha;

/*!
 *  @abstract Convert current color to the CIELAB color space
 *  @discussion Values returned are nominal values and therefore aren't
 *  normalized in any way, e.g., percentage.
 */
- (BOOL)getLStar:(CGFloat *)lightness
           aStar:(CGFloat *)greenToMagenta
           bStar:(CGFloat *)yellowtoBlue
           alpha:(CGFloat *)alpha;

/*!
 *  @abstract Convert current color to the CIELUV color space
 *  @discussion Values returned are nominal values and therefore aren't
 *  normalized in any way, e.g., percentage.
 */
- (BOOL)getLStar:(CGFloat *)lightness
           uStar:(CGFloat *)greenToMagenta
           vStar:(CGFloat *)yellowtoBlue
           alpha:(CGFloat *)alpha;

/*!
 *  @abstract Convert current color to the CIELCH(ab) color space
 *  @discussion Values returned are nominal values and therefore aren't
 *  normalized in any way, e.g., percentage.
 */
- (BOOL)getLStar:(CGFloat *)lightness
           cStar:(CGFloat *)chroma
         hStarAB:(CGFloat *)hue
           alpha:(CGFloat *)alpha;

/*!
 *  @abstract Convert current color to the CIELCH(uv) color space
 *  @discussion Values returned are nominal values and therefore aren't
 *  normalized in any way, e.g., percentage.
 */
- (BOOL)getLStar:(CGFloat *)lightness
           cStar:(CGFloat *)chroma
         hStarUV:(CGFloat *)hue
           alpha:(CGFloat *)alpha;

/*!
 *  @abstract Convert current color to the CMYK color space
 *  @discussion Values returned are normalized percentage values 0.0f-1.0f
 */
- (BOOL)getCyan:(CGFloat *)cyan
        magenta:(CGFloat *)magenta
         yellow:(CGFloat *)yellow
          black:(CGFloat *)black
          alpha:(CGFloat *)alpha;

/*!
 *  @abstract Convert current color to the HSL color space
 *  @discussion Values returned are normalized percentage values 0.0f-1.0f. The
 *  Hue value is the fraction angle over 2π. Saturation and lightness are both
 *  just the normalized the percentage.
 */
- (BOOL)getHue:(CGFloat *)hue
    saturation:(CGFloat *)saturation
     lightness:(CGFloat *)lightness
         alpha:(CGFloat *)alpha;

/*!
 *  @abstract Convert current color to the HSI color space
 *  @discussion Values returned are normalized percentage values 0.0f-1.0f. The
 *  Hue value is the fraction angle over 2π. Saturation and intensity are both
 *  just the normalized the percentage.
 */
- (BOOL)getHue:(CGFloat *)hue
    saturation:(CGFloat *)saturation
     intensity:(CGFloat *)intensity
         alpha:(CGFloat *)alpha;

/*!
 *  @abstract Acquire color distance measure between current UIColor object and
 *  the UIColor compare parameter.

 *  @discussion Return values vary based on what distance metric used. However,
 *  all distance values returned base closeness to 0.0f as closeness in color
 *  range.
 *
 *  Color Distance Formulas:
 *
 *  T23UIColorDistanceFormulaCEI76: This is the formula based on the CIE76
 *  standard
 *
 *  T23UIColorDistanceFormulaCMC1984_1_1: This is the formula based on the CMC
 *  l:c (1984) standard where l=1 & c=1
 *
 *  T23UIColorDistanceFormulaCMC1984_2_1: This is the formula based on the CMC
 *  l:c (1984) standard where l=2 & c=1
 *
 *  T23UIColorDistanceFormulaCEI94_GRAPHICS: This is the formula based on the
 *  CIE94 standard using the graphics KL, K1 and K2 values of: {1, 0.045, 0.015}
 *
 *  T23UIColorDistanceFormulaCEI94_TEXTILES: This is the formula based on the
 *  CIE94 standard using the textiles KL, K1 and K2 values of {2, 0.048, 0.014}
 *
 *  T23UIColorDistanceFormulaCIEDE2000: This is the formula based on the
 *  CIEDE2000 standard
 */
- (CGFloat)getDistanceMetricBetweenColor:(UIColor *)compare
                             withOptions:(T23UIColorDistanceOptions)options;

@end
