/*
 *  UIColor+T23ColourSpaces.m
 *  T23Kit-Colour
 *
 *  Created by Michael Van Milligan on 4/11/14.
 *  Copyright (c) 2014 Thirteen23. All rights reserved.
 */

#import "UIColor+T23ColourSpaces.h"

@implementation UIColor (T23ColourSpaces)

+ (UIColor *)colorWithRed:(CGFloat)red
                   yellow:(CGFloat)yellow
                     blue:(CGFloat)blue
                    alpha:(CGFloat)alpha {
  colour_t ryb = {0.0f}, rgb = {0.0f};
  ryb.RYB_R = red;
  ryb.RYB_Y = yellow;
  ryb.RYB_B = blue;

  RYB_2_RGB(ryb.RYB, &rgb.RGB, 0x0);

  return [UIColor colorWithRed:(CGFloat)rgb.RGB_R
                         green:(CGFloat)rgb.RGB_G
                          blue:(CGFloat)rgb.RGB_B
                         alpha:alpha];
}

+ (UIColor *)colorWithHexString:(NSString *)hexString
                          alpha:(CGFloat)alpha {
  CGFloat r = 0.0f, g = 0.0f, b = 0.0f;
  unsigned int hexInt = 0;
  
  if (9 > hexString.length) {
    NSScanner *scanner = [NSScanner scannerWithString:hexString];
    scanner.charactersToBeSkipped = [NSCharacterSet characterSetWithCharactersInString:@"#"];
    
    if ([scanner scanHexInt:&hexInt]) {
      r = (CGFloat)(((hexInt & 0xFF0000) >> 16) / 255.0);
      g = (CGFloat)(((hexInt & 0xFF00) >> 8) / 255.0);
      b = (CGFloat)(((hexInt & 0xFF) >> 0) / 255.0);
    }
  }
  
  return [UIColor colorWithRed:r green:g blue:b alpha:alpha];
}

- (UIColor *)initWithRed:(CGFloat)red
                  yellow:(CGFloat)yellow
                    blue:(CGFloat)blue
                   alpha:(CGFloat)alpha {
  colour_t ryb = {0.0f}, rgb = {0.0f};
  ryb.RYB_R = red;
  ryb.RYB_Y = yellow;
  ryb.RYB_B = blue;

  RYB_2_RGB(ryb.RYB, &rgb.RGB, 0x0);

  return
      [self initWithRed:(CGFloat)rgb.RGB_R
                  green:(CGFloat)rgb.RGB_G
                   blue:(CGFloat)rgb.RGB_B alpha:alpha];
}

- (UIColor *)initWithHexString:(NSString *)hexString
                         alpha:(CGFloat)alpha {
  CGFloat r = 0.0f, g = 0.0f, b = 0.0f;
  unsigned int hexInt = 0;
  
  if (9 > hexString.length) {
    NSScanner *scanner = [NSScanner scannerWithString:hexString];
    scanner.charactersToBeSkipped = [NSCharacterSet characterSetWithCharactersInString:@"#"];
    
    if ([scanner scanHexInt:&hexInt]) {
      r = (CGFloat)(((hexInt & 0xFF0000) >> 16) / 255.0);
      g = (CGFloat)(((hexInt & 0xFF00) >> 8) / 255.0);
      b = (CGFloat)(((hexInt & 0xFF) >> 0) / 255.0);
    }
  }
  
  return [self initWithRed:r green:g blue:b alpha:alpha];
}

- (NSString *)hexString {
  CGFloat r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;
  
  [self getRed:&r green:&g blue:&b alpha:&a];
  
  return [NSString stringWithFormat:@"%02x%02x%02x",
          (UInt8)(r * 255), (UInt8)(g * 255), (UInt8)(b * 255)];
}

- (BOOL)getX:(CGFloat *)coneResponse
           Y:(CGFloat *)luminance
           Z:(CGFloat *)quasiBlue
       alpha:(CGFloat *)alpha {
  colour_t xyz = {0.0f}, rgb = {0.0f};
  CGFloat r, g, b, a;
  
  [self getRed:&r green:&g blue:&b alpha:&a];
  rgb.RGB_R = r;
  rgb.RGB_G = g;
  rgb.RGB_B = b;
  
  RGB_2_XYZ(rgb.RGB, &xyz.XYZ, colourspace_rgb_profile_srgb_d65);
  *coneResponse = (CGFloat)xyz.XYZ_X;
  *luminance =    (CGFloat)xyz.XYZ_Y;
  *quasiBlue =    (CGFloat)xyz.XYZ_Z;
  *alpha = a;
  
  return YES;
}

- (BOOL)getHunterLStar:(CGFloat *)lightness
                 aStar:(CGFloat *)greenToMagenta
                 bStar:(CGFloat *)yellowtoBlue
                 alpha:(CGFloat *)alpha {
  colour_t hlab = {0.0f}, rgb = {0.0f};
  CGFloat r, g, b, a;

  [self getRed:&r green:&g blue:&b alpha:&a];
  rgb.RGB_R = r;
  rgb.RGB_G = g;
  rgb.RGB_B = b;

  RGB_2_HLAB(rgb.RGB, &hlab.LAB, colourspace_rgb_profile_srgb_d65);
  *lightness =      (CGFloat)hlab.LAB_L;
  *greenToMagenta = (CGFloat)hlab.LAB_A;
  *yellowtoBlue =   (CGFloat)hlab.LAB_B;
  *alpha = a;

  return YES;
}

- (BOOL)getLStar:(CGFloat *)lightness
           aStar:(CGFloat *)greenToMagenta
           bStar:(CGFloat *)yellowtoBlue
           alpha:(CGFloat *)alpha {
  colour_t lab = {0.0f}, rgb = {0.0f};
  CGFloat r, g, b, a;

  [self getRed:&r green:&g blue:&b alpha:&a];
  rgb.RGB_R = r;
  rgb.RGB_G = g;
  rgb.RGB_B = b;

  RGB_2_LAB(rgb.RGB, &lab.LAB, colourspace_rgb_profile_srgb_d65);
  *lightness =      (CGFloat)lab.LAB_L;
  *greenToMagenta = (CGFloat)lab.LAB_A;
  *yellowtoBlue =   (CGFloat)lab.LAB_B;
  *alpha = a;

  return YES;
}

- (BOOL)getLStar:(CGFloat *)lightness
           uStar:(CGFloat *)greenToMagenta
           vStar:(CGFloat *)yellowtoBlue
           alpha:(CGFloat *)alpha {
  colour_t luv = {0.0f}, rgb = {0.0f};
  CGFloat r, g, b, a;

  [self getRed:&r green:&g blue:&b alpha:&a];
  rgb.RGB_R = r;
  rgb.RGB_G = g;
  rgb.RGB_B = b;

  RGB_2_LUV(rgb.RGB, &luv.LUV, colourspace_rgb_profile_srgb_d65);
  *lightness =      (CGFloat)luv.LUV_L;
  *greenToMagenta = (CGFloat)luv.LUV_U;
  *yellowtoBlue =   (CGFloat)luv.LUV_V;
  *alpha = a;

  return YES;
}

- (BOOL)getLStar:(CGFloat *)lightness
           cStar:(CGFloat *)chroma
         hStarAB:(CGFloat *)hue
           alpha:(CGFloat *)alpha {
  colour_t lch_ab = {0.0f}, rgb = {0.0f};
  CGFloat r, g, b, a;

  [self getRed:&r green:&g blue:&b alpha:&a];
  rgb.RGB_R = r;
  rgb.RGB_G = g;
  rgb.RGB_B = b;

  RGB_2_LCH_AB(rgb.RGB, &lch_ab.LCH_AB, colourspace_rgb_profile_srgb_d65);
  *lightness =  (CGFloat)lch_ab.LCH_AB_L;
  *chroma =     (CGFloat)lch_ab.LCH_AB_C;
  *hue =        (CGFloat)lch_ab.LCH_AB_H;
  *alpha = a;

  return YES;
}

- (BOOL)getLStar:(CGFloat *)lightness
           cStar:(CGFloat *)chroma
         hStarUV:(CGFloat *)hue
           alpha:(CGFloat *)alpha {
  colour_t lch_uv = {0.0f}, rgb = {0.0f};
  CGFloat r, g, b, a;

  [self getRed:&r green:&g blue:&b alpha:&a];
  rgb.RGB_R = r;
  rgb.RGB_G = g;
  rgb.RGB_B = b;

  RGB_2_LCH_UV(rgb.RGB, &lch_uv.LCH_UV, colourspace_rgb_profile_srgb_d65);
  *lightness =  (CGFloat)lch_uv.LCH_UV_L;
  *chroma =     (CGFloat)lch_uv.LCH_UV_C;
  *hue =        (CGFloat)lch_uv.LCH_UV_H;
  *alpha = a;

  return YES;
}

- (BOOL)getCyan:(CGFloat *)cyan
        magenta:(CGFloat *)magenta
         yellow:(CGFloat *)yellow
          black:(CGFloat *)black
          alpha:(CGFloat *)alpha {
  colour_t cmyk = {0.0f}, rgb = {0.0f};
  CGFloat r, g, b, a;

  [self getRed:&r green:&g blue:&b alpha:&a];
  rgb.RGB_R = r;
  rgb.RGB_G = g;
  rgb.RGB_B = b;

  RGB_2_CMYK(rgb.RGB, &cmyk.CMYK, 0x0);
  *cyan =     (CGFloat)cmyk.CMYK_C;
  *magenta =  (CGFloat)cmyk.CMYK_M;
  *yellow =   (CGFloat)cmyk.CMYK_Y;
  *black =    (CGFloat)cmyk.CMYK_K;
  *alpha = a;

  return YES;
}

- (BOOL)getHue:(CGFloat *)hue
    saturation:(CGFloat *)saturation
     lightness:(CGFloat *)lightness
         alpha:(CGFloat *)alpha {
  colour_t hsl = {0.0f}, rgb = {0.0f};
  CGFloat r, g, b, a;

  [self getRed:&r green:&g blue:&b alpha:&a];
  rgb.RGB_R = r;
  rgb.RGB_G = g;
  rgb.RGB_B = b;

  RGB_2_HSL(rgb.RGB, &hsl.HSL, 0x0);
  *hue =        (CGFloat)hsl.HSL_H;
  *saturation = (CGFloat)hsl.HSL_S;
  *lightness =  (CGFloat)hsl.HSL_L;
  *alpha = a;

  return YES;
}

- (BOOL)getHue:(CGFloat *)hue
    saturation:(CGFloat *)saturation
     intensity:(CGFloat *)intensity
         alpha:(CGFloat *)alpha {
  colour_t hsi = {0.0f}, rgb = {0.0f};
  CGFloat r, g, b, a;

  [self getRed:&r green:&g blue:&b alpha:&a];
  rgb.RGB_R = r;
  rgb.RGB_G = g;
  rgb.RGB_B = b;

  RGB_2_HSI(rgb.RGB, &hsi.HSL, 0x0);
  *hue =        (CGFloat)hsi.HSI_H;
  *saturation = (CGFloat)hsi.HSI_S;
  *intensity =  (CGFloat)hsi.HSI_I;
  *alpha = a;

  return YES;
}

- (BOOL)getRed:(CGFloat *)red
        yellow:(CGFloat *)yellow
          blue:(CGFloat *)blue
         alpha:(CGFloat *)alpha {
  return YES;
}

- (NSArray *)getTriadic {
  UIColor *plus120 = nil, *minus120 = nil;
  CGFloat H, S, V, A;
  CGFloat HUE = 0.0f, HUEP, HUEM;

  [self getHue:&H saturation:&S brightness:&V alpha:&A];

  HUE = (H * M_2PI);
  HUEM = (0.0f > HUE - DEG_TO_RAD(120)) ? M_2PI + (HUE - DEG_TO_RAD(120))
                                        : HUE - DEG_TO_RAD(120);
  HUEP = (M_2PI < HUE + DEG_TO_RAD(120)) ? (HUE + DEG_TO_RAD(120)) - M_2PI
                                         : HUE + DEG_TO_RAD(120);

  plus120 = [UIColor colorWithHue:HUEP saturation:S brightness:V alpha:A];
  minus120 = [UIColor colorWithHue:HUEM saturation:S brightness:V alpha:A];

  return @[ plus120, minus120, self ];
}

- (NSArray *)getSplitComplements {
  UIColor *plus150 = nil, *minus150 = nil;
  CGFloat H, S, V, A;
  CGFloat HUE = 0.0f, HUEP, HUEM;

  [self getHue:&H saturation:&S brightness:&V alpha:&A];

  HUE = (H * M_2PI);
  HUEM = (0.0f > HUE - DEG_TO_RAD(150)) ? M_2PI + (HUE - DEG_TO_RAD(150))
                                        : HUE - DEG_TO_RAD(150);
  HUEP = (M_2PI < HUE + DEG_TO_RAD(150)) ? (HUE + DEG_TO_RAD(150)) - M_2PI
                                         : HUE + DEG_TO_RAD(150);

  plus150 = [UIColor colorWithHue:HUEP saturation:S brightness:V alpha:A];
  minus150 = [UIColor colorWithHue:HUEM saturation:S brightness:V alpha:A];

  return @[ plus150, minus150, self ];
}

- (NSArray *)getAnalogous {
  UIColor *plus30 = nil, *minus30 = nil;
  CGFloat H, S, V, A;
  CGFloat HUE = 0.0f, HUEP, HUEM;

  [self getHue:&H saturation:&S brightness:&V alpha:&A];

  HUE = (H * M_2PI);
  HUEM = (0.0f > HUE - DEG_TO_RAD(30)) ? M_2PI + (HUE - DEG_TO_RAD(30))
                                       : HUE - DEG_TO_RAD(30);
  HUEP = (M_2PI < HUE + DEG_TO_RAD(30)) ? (HUE + DEG_TO_RAD(30)) - M_2PI
                                        : HUE + DEG_TO_RAD(30);

  plus30 = [UIColor colorWithHue:HUEP saturation:S brightness:V alpha:A];
  minus30 = [UIColor colorWithHue:HUEM saturation:S brightness:V alpha:A];

  return @[ plus30, minus30, self ];
}

- (UIColor *)getComplement {
  CGFloat H, S, V, A;
  CGFloat HUE = 0.0f;

  [self getHue:&H saturation:&S brightness:&V alpha:&A];

  HUE = (H * M_2PI);

  HUE += M_PI;
  HUE = (HUE > (M_2PI)) ? HUE - M_2PI : HUE;

  H = (HUE / M_2PI);

  return [UIColor colorWithHue:H saturation:S brightness:V alpha:A];
}

- (CGFloat)getDistanceMetricBetweenUIColor:(UIColor *)compare
                               withOptions:(T23UIColourDistanceOptions)options {
  CGFloat distance = 0.0f;
  CGFloat r, g, b, a;
  colour_t rgb = {0.0f}, self_lab = {0.0f}, comp_lab = {0.0f};

  [self getRed:&r green:&g blue:&b alpha:&a];
  rgb.RGB_R = r;
  rgb.RGB_G = g;
  rgb.RGB_B = b;

  RGB_2_LAB(rgb.RGB, &self_lab.LAB, colourspace_rgb_profile_srgb_d65);

  [compare getRed:&r green:&g blue:&b alpha:&a];
  rgb.RGB_R = r;
  rgb.RGB_G = g;
  rgb.RGB_B = b;

  RGB_2_LAB(rgb.RGB, &comp_lab.LAB, colourspace_rgb_profile_srgb_d65);

  switch (options) {
    case T23UIColourDistanceFormulaCIE76:
      CIE_76(self_lab.LAB, comp_lab.LAB, (colour_val_t *)&distance);
      break;

    case T23UIColourDistanceFormulaCMC1984_1_1:
      CMC_1984(self_lab.LAB, comp_lab.LAB, 1.0f, 1.0f,
               (colour_val_t *)&distance);
      break;

    case T23UIColourDistanceFormulaCMC1984_2_1:
      CMC_1984(self_lab.LAB, comp_lab.LAB, 2.0f, 1.0f,
               (colour_val_t *)&distance);
      break;

    case T23UIColourDistanceFormulaCIE94_GRAPHICS:
      CIE_94(self_lab.LAB, comp_lab.LAB, LIBCOLORSPACES_GRAPHICS,
             (colour_val_t *)&distance);
      break;

    case T23UIColourDistanceFormulaCIE94_TEXTILES:
      CIE_94(self_lab.LAB, comp_lab.LAB, LIBCOLORSPACES_TEXTILES,
             (colour_val_t *)&distance);
      break;

    case T23UIColourDistanceFormulaCIEDE2000:
      CIEDE_2000(self_lab.LAB, comp_lab.LAB, 1.0f, 1.0f, 1.0f,
                 (colour_val_t *)&distance);
      break;

    default:
      CIE_76(self_lab.LAB, comp_lab.LAB, (colour_val_t *)&distance);
      break;
  }

  return distance;
}

@end
