#T23Kit-Colour
UIColor Category with various colourspace transformations, neighbourhood colour generations, and ∆-E metrics.
_______________
##Discussion

Currently there are three pieces to this library:

1. **UIColor conversions to non-standard colour spaces**
2. **UIColor neighbourhood colour generations**
3. **UIColor distance (∆-E) metrics**

###Colour Conversions

Below are the currently supported colourspace conversions:
*   CIE XYZ
*   Hunter 1948 (L, a, b)
*   CIE 1976 (L\*, a\*, b\*)
*   CIE 1976 (L\*, u\*, v\*) 
*   CIE 1976 (L\*, C\*, H\*ab)
*   CIE 1976 (L\*, C\*, H\*uv)
*   CMYK
*   HSL
*   HSI

**Note: Please see [UIColor+T23ColourSpaces.h](https://github.com/thirteen23/T23Kit-Colour/blob/master/T23Kit-Colour/UIColor%2BT23ColourSpaces.h) for the format of values that are returned.**

###Neighbourhood Colour Generations
Below are the currently supported neighbourhood generations
*   Triadic
*   Split Complements
*   Analogous
*   Complementary

**Note: Please see [EasyRGB's](http://www.easyrgb.com/index.php?X=WEEL) definition of these neighbourhoods.**
###Colour Distance Formulas (∆-E)
Below are the currently supported colour distance metrics
*   CIE76
*   CIE94
*   CIEDE2000
*   CMC l:c (1984)

**Note: Please see [Wikipedia](http://en.wikipedia.org/wiki/Color_difference) for more information regarding ∆-E**
_______________

##API

**`UIColor+T23ColourSpaces`**

    - (BOOL)getX:Y:Z:alpha:
    - (BOOL)getHunterLStar:aStar:bStar:alpha:
    - (BOOL)getLStar:aStar:bStar:alpha:
    - (BOOL)getLStar:uStar:vStar:alpha:
    - (BOOL)getLStar:cStar:hStarAB:alpha:
    - (BOOL)getLStar:cStar:hStarUV:alpha:
    - (BOOL)getCyan:magenta:yellow:black:alpha:
    - (BOOL)getHue:saturation:lightness:alpha:
    - (BOOL)getHue:saturation:intensity:alpha:
    - (NSArray *)getTriadic
    - (NSArray *)getSplitComplements
    - (NSArray *)getAnalogous
    - (UIColor *)getComplement
    - (CGFloat)getDistanceMetricBetweenUIColor:withOptions:

##Setup

####Using [CocoaPods](http://cocoapods.org):

1.	Add the pod `T23Kit-Colour` to your [Podfile](http://guides.cocoapods.org/using/the-podfile.html).

    	pod 'T23Kit-Colour'

2.	Run `pod install` from Terminal, then open your app's `.xcworkspace` file to launch Xcode.
3.	`#import "UIColor+T23ColourSpaces.h"` wherever you want to use the API.

####Manually from GitHub

1.	Download the source files in the [T23Kit-Colour Source directory](https://github.com/thirteen23/T23Kit-Colour/tree/master/T23Kit-Colour).
2.	Add files to your Xcode project.
3.	`#import "UIColor+T23ColourSpaces.h"` wherever you want to use the API.

##Releases

Releases are tagged in the git commit history using (mostly) [semantic versioning](http://semver.org). Check out the [releases and release notes](https://github.com/thirteen23/T23Kit-Colour/blob/master/RELEASE) for each version.


Designed & maintained by Thirteen23 Developers (current: [Michael Van Milligan](mailto:michael.vanmilligan@thirteen23.com)). Distributed with the MIT license.