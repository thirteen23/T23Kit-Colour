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

  RYB_2_RGB(ryb.RYB, &rgb.RGB);

  return [UIColor colorWithRed:rgb.RGB_R
                         green:rgb.RGB_G
                          blue:rgb.RGB_B
                         alpha:alpha];
}

- (UIColor *)initWithRed:(CGFloat)red
                  yellow:(CGFloat)yellow
                    blue:(CGFloat)blue
                   alpha:(CGFloat)alpha {

  colour_t ryb = {0.0f}, rgb = {0.0f};
  ryb.RYB_R = red;
  ryb.RYB_Y = yellow;
  ryb.RYB_B = blue;

  RYB_2_RGB(ryb.RYB, &rgb.RGB);

  return
      [self initWithRed:rgb.RGB_R green:rgb.RGB_G blue:rgb.RGB_B alpha:alpha];
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

  RGB_2_HLAB(rgb.RGB, &hlab.LAB);
  *lightness = hlab.LAB_L;
  *greenToMagenta = hlab.LAB_A;
  *yellowtoBlue = hlab.LAB_B;
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

  RGB_2_LAB(rgb.RGB, &lab.LAB);
  *lightness = lab.LAB_L;
  *greenToMagenta = lab.LAB_A;
  *yellowtoBlue = lab.LAB_B;
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

  RGB_2_LUV(rgb.RGB, &luv.LUV);
  *lightness = luv.LUV_L;
  *greenToMagenta = luv.LUV_U;
  *yellowtoBlue = luv.LUV_V;
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

  RGB_2_LCH_AB(rgb.RGB, &lch_ab.LCH_AB);
  *lightness = lch_ab.LCH_AB_L;
  *chroma = lch_ab.LCH_AB_C;
  *hue = lch_ab.LCH_AB_H;
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

  RGB_2_LCH_UV(rgb.RGB, &lch_uv.LCH_UV);
  *lightness = lch_uv.LCH_UV_L;
  *chroma = lch_uv.LCH_UV_C;
  *hue = lch_uv.LCH_UV_H;
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

  RGB_2_CMYK(rgb.RGB, &cmyk.CMYK);
  *cyan = cmyk.CMYK_C;
  *magenta = cmyk.CMYK_M;
  *yellow = cmyk.CMYK_Y;
  *black = cmyk.CMYK_K;
  *alpha = a;

  return YES;
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

  RGB_2_XYZ(rgb.RGB, &xyz.XYZ);
  *coneResponse = xyz.XYZ_X;
  *luminance = xyz.XYZ_Y;
  *quasiBlue = xyz.XYZ_Z;
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

  RGB_2_HSL(rgb.RGB, &hsl.HSL);
  *hue = hsl.HSL_H;
  *saturation = hsl.HSL_S;
  *lightness = hsl.HSL_L;
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

  RGB_2_HSI(rgb.RGB, &hsi.HSL);
  *hue = hsi.HSI_H;
  *saturation = hsi.HSI_S;
  *intensity = hsi.HSI_I;
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

  RGB_2_LAB(rgb.RGB, &self_lab.LAB);

  [compare getRed:&r green:&g blue:&b alpha:&a];
  rgb.RGB_R = r;
  rgb.RGB_G = g;
  rgb.RGB_B = b;

  RGB_2_LAB(rgb.RGB, &comp_lab.LAB);

  switch (options) {
  case T23UIColourDistanceFormulaCEI76:
    CIE_76(self_lab.LAB, comp_lab.LAB, (colour_val_t *)&distance);
    break;

  case T23UIColourDistanceFormulaCMC1984_1_1:
    CMC_1984(self_lab.LAB, comp_lab.LAB, 1.0f, 1.0f, (colour_val_t *)&distance);
    break;

  case T23UIColourDistanceFormulaCMC1984_2_1:
    CMC_1984(self_lab.LAB, comp_lab.LAB, 2.0f, 1.0f, (colour_val_t *)&distance);
    break;

  case T23UIColourDistanceFormulaCEI94_GRAPHICS:
    CIE_94(self_lab.LAB, comp_lab.LAB, LIBCOLORSPACES_GRAPHICS,
           (colour_val_t *)&distance);
    break;

  case T23UIColourDistanceFormulaCEI94_TEXTILES:
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
