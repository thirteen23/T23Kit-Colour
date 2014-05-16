#import <UIKit/UIKit.h>
#include "colorspaces.h"

typedef NS_ENUM(NSInteger, T23UIColorDistanceOptions) {
  T23UIColorDistanceFormulaCEI76 = 0, /* default */
  T23UIColorDistanceFormulaCMC1984_1_1,
  T23UIColorDistanceFormulaCMC1984_2_1,
  T23UIColorDistanceFormulaCEI94_GRAPHICS,
  T23UIColorDistanceFormulaCEI94_TEXTILES,
  T23UIColorDistanceFormulaCIEDE2000
};

@interface UIColor (T23ColorSpaces)

- (BOOL)getHunterLStar:(CGFloat *)lightness
                 aStar:(CGFloat *)greenToMagenta
                 bStar:(CGFloat *)yellowtoBlue
                 alpha:(CGFloat *)alpha;

- (BOOL)getLStar:(CGFloat *)lightness
           aStar:(CGFloat *)greenToMagenta
           bStar:(CGFloat *)yellowtoBlue
           alpha:(CGFloat *)alpha;

- (BOOL)getLStar:(CGFloat *)lightness
           uStar:(CGFloat *)greenToMagenta
           vStar:(CGFloat *)yellowtoBlue
           alpha:(CGFloat *)alpha;

- (BOOL)getCyan:(CGFloat *)cyan
        magenta:(CGFloat *)magenta
         yellow:(CGFloat *)yellow
          black:(CGFloat *)black
          alpha:(CGFloat *)alpha;

- (BOOL)getX:(CGFloat *)coneResponse
           Y:(CGFloat *)luminance
           Z:(CGFloat *)quasiBlue
       alpha:(CGFloat *)alpha;

- (BOOL)getHue:(CGFloat *)hue
    saturation:(CGFloat *)saturation
     lightness:(CGFloat *)lightness
         alpha:(CGFloat *)alpha;

- (BOOL)getHue:(CGFloat *)hue
    saturation:(CGFloat *)saturation
     intensity:(CGFloat *)intensity
         alpha:(CGFloat *)alpha;

- (CGFloat)getDistanceMetricBetweenColor:(UIColor *)compare
                             withOptions:(T23UIColorDistanceOptions)options;

@end
