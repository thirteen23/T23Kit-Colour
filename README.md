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
*   RYB *(Only from RYB)*
*   CIE XYZ <sup>1</sup>
*   Hunter 1948 (L, a, b)
*   CIE 1976 (L\*, a\*, b\*)
*   CIE 1976 (L\*, u\*, v\*) 
*   CIE 1976 (L\*, C\*, H\*<sub>ab</sub>)
*   CIE 1976 (L\*, C\*, H\*<sub>uv</sub>)
*   CMYK
*   HSL
*   HSI

**Note: Please see [UIColor+T23ColourSpaces.h](https://github.com/thirteen23/T23Kit-Colour/blob/master/T23Kit-Colour/UIColor%2BT23ColourSpaces.h) for the format of values that are returned.**

*<sup>1</sup> The default currently uses the sRGB working matrices for converting to/from XYZ space. Please see rgb_working_matrices in [colourspaces.c](https://github.com/thirteen23/T23Kit-Colour/blob/master/T23Kit-Colour/colourspaces.c) for more detail*
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

	+ (UIColor *)colorWithRed:yellow:blue:alpha:
	+ (UIColor *)colorWithHexString:alpha:
	- (UIColor *)initWithRed:yellow:blue:alpha:
	- (UIColor *)initWithHexString:alpha:
	- (NSString *)hexString
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

**`UIColor Swift Extension`**

	- init(red: CGFloat, yellow: CGFloat, blue: CGFloat, alpha: CGFloat)
	- init(hexString: String, alpha: CGFloat)
	- var hue:CGFloat { get }
	- var saturation:CGFloat { get }
	- var brightness:CGFloat { get }
	- var red:CGFloat { get }
	- var green:CGFloat { get }
	- var blue:CGFloat { get }
	- var hexString:String { get }
	- func getHSB() -> HSB
	- func getRGB() -> RGB
	- func getXYZ() -> XYZ
	- func getHLAB() -> HLAB
	- func getLAB() -> LAB
	- func getLUV() -> LUV
	- func getLCHab() -> LCHab
	- func getLCHuv() -> LCHuv
	- func getCMYK() -> CMYK
	- func getHSI() -> HSI
	- func getHSL() -> HSL
	- func getTriadic() -> (thisColor: UIColor, right: UIColor, left: UIColor)
	- func getSplitCompliments() -> (thisColor: UIColor, right: UIColor, left: UIColor)
	- func getAnalogous() -> (thisColor: UIColor, right: UIColor, left: UIColor)
	- func getCompliment() -> UIColor
	- func getDistanceBetweenUIColor(compare: UIColor, options: ColourDistanceOptions) -> CGFloat

##Setup

###Objective-C:

#####Using [CocoaPods](http://cocoapods.org):

1.	Add the pod `T23Kit-Colour` to your [Podfile](http://guides.cocoapods.org/using/the-podfile.html).

`pod 'T23Kit-Colour'`

2.	Run `pod install` from Terminal, then open your app's `.xcworkspace` file to launch Xcode.
3.	`#import "UIColor+T23ColourSpaces.h"` wherever you want to use the API.

#####Manually from GitHub

1.	Download the source files in the [T23Kit-Colour Source directory](https://github.com/thirteen23/T23Kit-Colour/tree/master/T23Kit-Colour).
2.	Add files to your Xcode project.
3.	`#import "UIColor+T23ColourSpaces.h"` wherever you want to use the API.

###Swift:

#####Using [CocoaPods](http://cocoapods.org):

CocoaPods aren't supported yet but this project will update as soon as CocoaPods irons it all out.

#####Manually from GitHub

1.	Download the source files in the [T23Kit-Colour Source directory](https://github.com/thirteen23/T23Kit-Colour/tree/master/T23Kit-Colour).
2.	Add `Colour.swift` to your Xcode project.

##Examples 

If you would like to see an example of this library in use check out our other toy project [ColorPalette](https://github.com/thirteen23/ColorPalette).

##Releases

Releases are tagged in the git commit history using (mostly) [semantic versioning](http://semver.org). Check out the [releases and release notes](https://github.com/thirteen23/T23Kit-Colour/blob/master/RELEASE) for each version.


Designed & maintained by [Thirteen23 Developers](mailto:dev@thirteen23.com). Distributed with the MIT license.
