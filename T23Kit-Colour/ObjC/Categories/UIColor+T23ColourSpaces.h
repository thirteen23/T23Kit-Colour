/*
 *  UIColor+T23ColourSpaces.h
 *  T23Kit-Colour
 *
 *  Created by Michael Van Milligan on 4/11/14.
 *  Copyright (c) 2014 Thirteen23. All rights reserved.
 */

#import <UIKit/UIKit.h>
#include "colourspaces.h"

/*
 * Colour conversion category for UIColor.
 *
 *  Much prop's to:
 *    Wikipedia (http://en.wikipedia.org/wiki/Color_difference)
 *    http://www.brucelindbloom.com
 *    http://www.easyrgb.com
 */

/* Distance formula options */
typedef NS_ENUM(NSInteger, T23UIColourDistanceOptions) {
  T23UIColourDistanceFormulaCIE76 = 0, /* default */
  T23UIColourDistanceFormulaCMC1984_1_1,
  T23UIColourDistanceFormulaCMC1984_2_1,
  T23UIColourDistanceFormulaCIE94_GRAPHICS,
  T23UIColourDistanceFormulaCIE94_TEXTILES,
  T23UIColourDistanceFormulaCIEDE2000
};

@interface UIColor (T23ColourSpaces)

/*!
 *  @abstract Acquire an autoreleased UIColor with RYB colour values
 *  @discussion UIColor returned is generated from RYB->RGB conversion. (Please
 *  see: http://vis.computer.org/vis2004/DVD/infovis/papers/gossett.pdf)
 */
+ (UIColor *)colorWithRed:(CGFloat)red
                   yellow:(CGFloat)yellow
                     blue:(CGFloat)blue
                    alpha:(CGFloat)alpha;

/*!
 *  @abstract Acquire an autoreleased UIColor from a Hex colour value
 *  @discussion Proper value regex: ^(#{0,2}|0x{0,1})([a-f]|[A-F]|\d){1,6}
 */
+ (UIColor *)colorWithHexString:(NSString *)hexString
                          alpha:(CGFloat)alpha;

/*!
 *  @abstract Initialize a UIColor with RYB colour values
 *  @discussion UIColor returned is generated from RYB->RGB conversion. (Please
 *  see http://vis.computer.org/vis2004/DVD/infovis/papers/gossett.pdf)
 */
- (UIColor *)initWithRed:(CGFloat)red
                  yellow:(CGFloat)yellow
                    blue:(CGFloat)blue
                   alpha:(CGFloat)alpha;

/*!
 *  @abstract Initialize a UIColor from a Hex colour value
 *  @discussion Proper value regex: ^(#{0,2}|0x{0,1})([a-f]|[A-F]|\d){1,6}
 */
- (UIColor *)initWithHexString:(NSString *)hexString
                         alpha:(CGFloat)alpha;

/*!
 *  @abstract Convert current colour to the XYZ colour space
 *  @discussion Values returned are nominal values and therefore aren't
 *  normalized in any way, e.g., not a ratio or percentage.
 *
 *  The conversions are from RGB space and uses the sRGB matrix for calculation,
 *  i.e, sRGB matrices to XYZ and back with (Observer = 2°, Illuminant = D65)
 *  using Bradford adaptation.
 */
- (BOOL)getX:(CGFloat *)coneResponse
           Y:(CGFloat *)luminance
           Z:(CGFloat *)quasiBlue
       alpha:(CGFloat *)alpha;

/*!
 *  @abstract Convert current colour to the Hunter 1948 L, a, b colour space
 *  @discussion Values returned are nominal values and therefore aren't
 *  normalized in any way, e.g., not a ratio or percentage.
 */
- (BOOL)getHunterLStar:(CGFloat *)lightness
                 aStar:(CGFloat *)greenToMagenta
                 bStar:(CGFloat *)yellowtoBlue
                 alpha:(CGFloat *)alpha;

/*!
 *  @abstract Convert current colour to the CIELAB colour space
 *  @discussion Values returned are nominal values and therefore aren't
 *  normalized in any way, e.g., not a ratio or percentage.
 */
- (BOOL)getLStar:(CGFloat *)lightness
           aStar:(CGFloat *)greenToMagenta
           bStar:(CGFloat *)yellowtoBlue
           alpha:(CGFloat *)alpha;

/*!
 *  @abstract Convert current colour to the CIELUV colour space
 *  @discussion Values returned are nominal values and therefore aren't
 *  normalized in any way, e.g., not a ratio or percentage.
 */
- (BOOL)getLStar:(CGFloat *)lightness
           uStar:(CGFloat *)greenToMagenta
           vStar:(CGFloat *)yellowtoBlue
           alpha:(CGFloat *)alpha;

/*!
 *  @abstract Convert current colour to the CIELCH(ab) colour space
 *  @discussion Values returned are nominal values and therefore aren't
 *  normalized in any way, e.g., not a ratio or percentage.
 */
- (BOOL)getLStar:(CGFloat *)lightness
           cStar:(CGFloat *)chroma
         hStarAB:(CGFloat *)hue
           alpha:(CGFloat *)alpha;

/*!
 *  @abstract Convert current colour to the CIELCH(uv) colour space
 *  @discussion Values returned are nominal values and therefore aren't
 *  normalized in any way, e.g., not a ratio or percentage.
 */
- (BOOL)getLStar:(CGFloat *)lightness
           cStar:(CGFloat *)chroma
         hStarUV:(CGFloat *)hue
           alpha:(CGFloat *)alpha;

/*!
 *  @abstract Convert current colour to the CMYK colour space
 *  @discussion Values returned are normalized percentage values 0.0f-1.0f
 */
- (BOOL)getCyan:(CGFloat *)cyan
        magenta:(CGFloat *)magenta
         yellow:(CGFloat *)yellow
          black:(CGFloat *)black
          alpha:(CGFloat *)alpha;

/*!
 *  @abstract Convert current colour to the HSL colour space
 *  @discussion Values returned are normalized percentage values 0.0f-1.0f. The
 *  Hue value is the radian angle over 2π. Saturation and lightness are both
 *  just the normalized percentage.
 */
- (BOOL)getHue:(CGFloat *)hue
    saturation:(CGFloat *)saturation
     lightness:(CGFloat *)lightness
         alpha:(CGFloat *)alpha;

/*!
 *  @abstract Convert current colour to the HSI colour space
 *  @discussion Values returned are normalized percentage values 0.0f-1.0f. The
 *  Hue value is the radian angle over 2π. Saturation and intensity are both
 *  just the normalized percentage.
 */
- (BOOL)getHue:(CGFloat *)hue
    saturation:(CGFloat *)saturation
     intensity:(CGFloat *)intensity
         alpha:(CGFloat *)alpha;

/*!
 *  @abstract Grab the triadic set from the HSV space
 *  @discussion NSArray contains 3 UIColors: The original colour and 2 others
 *  where the hue is shifted ±120º while keeping S and V held constant
 */
- (NSArray *)getTriadic;

/*!
 *  @abstract Grab the split complements from the HSV space
 *  @discussion NSArray contains 3 UIColors: The original colour and 2 others
 *  where the hue is shifted ±150º while keeping S and V held constant
 */
- (NSArray *)getSplitComplements;

/*!
 *  @abstract Grab the analogous set from the HSV space
 *  @discussion NSArray contains 3 UIColors: The original colour and 2 others
 *  where the hue is shifted ±30º while keeping S and V held constant
 */
- (NSArray *)getAnalogous;

/*!
 *  @abstract Grab the complement from the HSV space
 *  @discussion UIColor returned is the colour rotated H by 180º while keeping S
 *  and V held constant
 */
- (UIColor *)getComplement;

/*!
 *  @abstract Acquire colour distance measure between current UIColor object and
 *  the UIColor compare parameter.
 *
 *  @discussion Return values vary based on what distance metric used. However,
 *  all distance values returned base closeness to 0.0f as closeness in colour
 *  range. (See literature on the topic)
 *
 *  Colour Distance Formulas:
 *
 *  T23UIColourDistanceFormulaCEI76: This is the formula based on the CIE76
 *  standard
 *
 *  T23UIColourDistanceFormulaCMC1984_1_1: This is the formula based on the CMC
 *  l:c (1984) standard where l=1 & c=1
 *
 *  T23UIColourDistanceFormulaCMC1984_2_1: This is the formula based on the CMC
 *  l:c (1984) standard where l=2 & c=1
 *
 *  T23UIColourDistanceFormulaCEI94_GRAPHICS: This is the formula based on the
 *  CIE94 standard using the graphics KL, K1 and K2 values of: {1, 0.045, 0.015}
 *
 *  T23UIColourDistanceFormulaCEI94_TEXTILES: This is the formula based on the
 *  CIE94 standard using the textiles KL, K1 and K2 values of {2, 0.048, 0.014}
 *
 *  T23UIColourDistanceFormulaCIEDE2000: This is the formula based on the
 *  CIEDE2000 standard
 */
- (CGFloat)getDistanceMetricBetweenUIColor:(UIColor *)compare
                               withOptions:(T23UIColourDistanceOptions)options;

@end
