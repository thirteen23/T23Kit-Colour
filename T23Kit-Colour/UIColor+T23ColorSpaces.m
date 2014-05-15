#import "UIColor+T23ColorSpaces.h"

@implementation UIColor (T23ColorSpaces)

- (BOOL)getHunterLStar:(CGFloat *)lightness
                 aStar:(CGFloat *)greenToMagenta
                 bStar:(CGFloat *)yellowtoBlue
                 alpha:(CGFloat *)alpha {

  color_t hlab = {0.0f}, rgb = {0.0f};
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

  color_t lab = {0.0f}, rgb = {0.0f};
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

- (BOOL)getCyan:(CGFloat *)cyan
        magenta:(CGFloat *)magenta
         yellow:(CGFloat *)yellow
          black:(CGFloat *)black
          alpha:(CGFloat *)alpha {

  color_t cmyk = {0.0f}, rgb = {0.0f};
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
  color_t xyz = {0.0f}, rgb = {0.0f};
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
  color_t hsl = {0.0f}, rgb = {0.0f};
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
  color_t hsi = {0.0f}, rgb = {0.0f};
  CGFloat r, g, b, a;

  [self getRed:&r green:&g blue:&b alpha:&a];
  rgb.RGB_R = r;
  rgb.RGB_G = g;
  rgb.RGB_B = b;

  RGB_2_HSL(rgb.RGB, &hsi.HSL);
  *hue = hsi.HSI_H;
  *saturation = hsi.HSI_S;
  *intensity = hsi.HSI_I;
  *alpha = a;

  return YES;
}

- (CGFloat)getDistanceMetricBetweenColor:(UIColor *)compare
                             withOptions:(T23UIColorDistanceOptions)options {

  CGFloat distance = 0.0f;
  CGFloat r, g, b, a;
  color_t rgb = {0.0f}, self_lab = {0.0f}, comp_lab = {0.0f};

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
  case T23UIColorDistanceFormulaCEI76:
    CIE_76(self_lab.LAB, comp_lab.LAB, (color_val_t *)&distance);
    break;

  case T23UIColorDistanceFormulaCMC1984_1_1:
    CMC_1984(self_lab.LAB, comp_lab.LAB, 1.0f, 1.0f, (color_val_t *)&distance);
    break;

  case T23UIColorDistanceFormulaCMC1984_2_1:
    CMC_1984(self_lab.LAB, comp_lab.LAB, 2.0f, 1.0f, (color_val_t *)&distance);
    break;

  case T23UIColorDistanceFormulaCEI94_GRAPHICS:
    CIE_94(self_lab.LAB, comp_lab.LAB, LIBCOLORSPACES_GRAPHICS,
           (color_val_t *)&distance);
    break;

  case T23UIColorDistanceFormulaCEI94_TEXTILES:
    CIE_94(self_lab.LAB, comp_lab.LAB, LIBCOLORSPACES_TEXTILES,
           (color_val_t *)&distance);
    break;

  case T23UIColorDistanceFormulaCIEDE2000:
    CIEDE_2000(self_lab.LAB, comp_lab.LAB, 1.0f, 1.0f, 1.0f,
               (color_val_t *)&distance);
    break;

  default:
    CIE_76(self_lab.LAB, comp_lab.LAB, (color_val_t *)&distance);
    break;
  }

  return distance;
}

@end
