//
//  T23Kit-Colour.swift
//  T23Kit-Colour
//
//  Created by Michael Van Milligan on 12/11/14.
//  Copyright (c) 2014 Thirteen23. All rights reserved.
//

import UIKit
import Foundation
import Darwin

// MARK: - Constants
let M_2PI:CGFloat = CGFloat(M_PI) * 2.0
let ɛ:CGFloat = 0.008856
let κ:CGFloat = 7.787

// MARK: - Enums

public enum CIE94 {
  case graphics
  case textiles
}

public enum RGBWorkingSpace: String {
  case AdobeD65 = "AdobeD65"
  case AppleD65 = "AppleD65"
  case BestD50 = "BestD50"
  case BetaD50 = "BetaD50"
  case BruceD65 = "BruceD65"
  case CIEE = "CIEE"
  case ColormatchD50 = "ColormatchD50"
  case DonD50 = "DonD50"
  case ECID50 = "ECID50"
  case EktaSpaceD50 = "EktaSpaceD50"
  case NTSCC = "NTSCC"
  case PalSecamD65 = "PalSecamD65"
  case ProPhotoD50 = "ProPhotoD50"
  case SMPTECD65 = "SMPTECD65"
  case sRGBD65 = "sRGBD65"
  case WideGamutD50 = "WideGamutD50"
  case AdobeD50 = "AdobeD50"
  case AppleD50 = "AppleD50"
  case BruceD50 = "BruceD50"
  case CEID50 = "CEID50"
  case NTSCD50 = "NTSCD50"
  case PalSecamD50 = "PalSecamD50"
  case SMPTECD50 = "SMPTECD50"
  case sRGBD50 = "sRGBD50"
  
  var referenceWhite:ReferenceWhite {
    get {
      var rw:ReferenceWhite
      switch self {
        case NTSCC:
          rw = .C
        case BestD50:
          fallthrough
        case BetaD50:
          fallthrough
        case ColormatchD50:
          fallthrough
        case DonD50:
          fallthrough
        case ECID50:
          fallthrough
        case EktaSpaceD50:
          fallthrough
        case ProPhotoD50:
          fallthrough
        case WideGamutD50:
          fallthrough
        case AdobeD50:
          fallthrough
        case AppleD50:
          fallthrough
        case BruceD50:
          fallthrough
        case CEID50:
          fallthrough
        case NTSCD50:
          fallthrough
        case PalSecamD50:
          fallthrough
        case SMPTECD50:
          fallthrough
        case sRGBD50:
          rw = .D50
        case AdobeD65:
          fallthrough
        case AppleD65:
          fallthrough
        case BruceD65:
          fallthrough
        case PalSecamD65:
          fallthrough
        case SMPTECD65:
          fallthrough
        case sRGBD65:
          rw = .D65
        case CIEE:
          rw = .E
        default:
          rw = .D65
      }
      return rw
    }
  }
}

public enum ReferenceWhite: String {
  case A = "A"
  case B = "B"
  case C = "C"
  case D50 = "D50"
  case D55 = "D55"
  case D65 = "D65"
  case D75 = "D75"
  case E = "E"
  case F2 = "F2"
  case F7 = "F7"
  case F11 = "F11"
}

// MARK: - Structures

struct ReferenceWhiteMatrix {
  var Xw:CGFloat = 0.0
  var Yw:CGFloat = 0.0
  var Zw:CGFloat = 0.0
}

struct RGBWorkingSpaceMatrix {
  var _00:CGFloat = 0.0
  var _01:CGFloat = 0.0
  var _02:CGFloat = 0.0
  var _10:CGFloat = 0.0
  var _11:CGFloat = 0.0
  var _12:CGFloat = 0.0
  var _20:CGFloat = 0.0
  var _21:CGFloat = 0.0
  var _22:CGFloat = 0.0
  
  func applyWorkingSpace(xr: CGFloat, yg: CGFloat, zb: CGFloat) -> (xr: CGFloat, yg: CGFloat, zb: CGFloat) {
    let a:CGFloat = (xr * self._00) + (yg * self._01) + (zb * self._02)
    let b:CGFloat = (xr * self._10) + (yg * self._11) + (zb * self._12)
    let c:CGFloat = (xr * self._20) + (yg * self._21) + (zb * self._22)
    return (a, b, c)
  }
}

public struct RGB {
  public var r:CGFloat = 0.0
  public var g:CGFloat = 0.0
  public var b:CGFloat = 0.0
  
  public init() {}
  
  public init(r: CGFloat, g: CGFloat, b: CGFloat) {
    self.r = r
    self.g = g
    self.b = b
  }
  
  mutating func sanitize() -> RGB {
    self.r = 0 != signbit(self.r) ? self.r * -1.0 : self.r
    self.r = (1.0 > (self.r * (100000.0))) ? 0.0 : (1.0 < self.r) ? 1.0 : self.r
    self.g = 0 != (signbit(self.g)) ? self.g * -1.0 : self.g;
    self.g = (1.0 > (self.g * (100000.0))) ? 0.0 : (1.0 < self.g) ? 1.0 : self.g;
    self.b = 0 != (signbit(self.b)) ? self.b * -1.0 : self.b;
    self.b = (1.0 > (self.b * (100000.0))) ? 0.0 : (1.0 < self.b) ? 1.0 : self.b;
    
    return self
  }
  
  public func toCMYK() -> CMYK {
    var cmyk:CMYK = CMYK()
    
    var m:CGFloat = max(max(self.r, self.g), self.b)
    
    cmyk.k = 1.0 - m
    cmyk.c = (1.0 - self.r - cmyk.k) / (1.0 - cmyk.k)
    cmyk.m = (1.0 - self.g - cmyk.k) / (1.0 - cmyk.k)
    cmyk.y = (1.0 - self.b - cmyk.k) / (1.0 - cmyk.k)
    
    return cmyk
  }
  
  public func toHSL() -> HSL {
    var hsl:HSL = HSL()
    
    let mn:CGFloat = min(min(self.r, self.g), self.b)
    let mx:CGFloat = max(max(self.r, self.g), self.b)
    let delta:CGFloat = mx - mn
    
    hsl.l = (mx + mn) / 2.0
    
    if 0.0 == mx {
      hsl.h = 0.0
      hsl.s = 0.0
    } else {
      hsl.s = (0.5 > hsl.l) ? delta / (mx + mn) : delta / (2.0 - mx - mn)
      
      let dr:CGFloat = (((mx - self.r) / 6.0) + (delta / 2.0)) / delta
      let dg:CGFloat = (((mx - self.g) / 6.0) + (delta / 2.0)) / delta
      let db:CGFloat = (((mx - self.b) / 6.0) + (delta / 2.0)) / delta
      
      if self.r == mx {
        hsl.h = db - dg
      } else if self.g == mx {
        hsl.h = (1.0 / 3.0) + dr - db
      } else if self.b == mx {
        hsl.h = (2.0 / 3.0) + dg - dr
      }
      
      hsl.h = (0.0 > hsl.h) ? hsl.h + 1.0 : (1.0 < hsl.h) ? hsl.h - 1.0 : hsl.h
    }
    
    return hsl
  }
  
  public func toHSI() -> HSI {
    var hsi:HSI = HSI()
    
    let mn:CGFloat = min(min(self.r, self.g), self.b)
    let mx:CGFloat = max(max(self.r, self.g), self.b)
    let delta:CGFloat = mx - mn
    
    hsi.i = (1.0 / 3.0) * (self.r + self.b + self.g)
    if 0.0 == delta {
      hsi.h = 0.0
      hsi.s = 0.0
    } else {
      hsi.h = (mx == self.r)
        ? fmod(((self.g - self.b) / delta), 6.0)
        : (mx == self.g)
          ? (self.b - self.r) / delta + 2.0
          : (self.r - self.g) / delta + 4.0
      
      hsi.h *= 60.0
      hsi.h = hsi.h.radians / M_2PI
      hsi.h = (0.0 > hsi.h) ? hsi.h + 1.0 : (1.0 < hsi.h) ? hsi.h - 1.0 : hsi.h
      hsi.s = 1.0 - (mn / (hsi.i))
    }
    
    return hsi
  }
  
  public func toXYZ(rgbSpace: RGBWorkingSpace) -> XYZ {
    var xyz:XYZ = XYZ()
    
    let workingMatrix:[RGBWorkingSpaceMatrix] = Matrices.WorkingMatrices[rgbSpace]!
    let matrix:RGBWorkingSpaceMatrix = workingMatrix.first!
    let gamma:CGFloat = Gamma.RGB[rgbSpace]!
    
    xyz.x = self.r
    xyz.y = self.g
    xyz.z = self.b
    
    let sR:CGFloat = 0 != signbit(xyz.x) ? -1.0 : 1.0
    let sG:CGFloat = 0 != signbit(xyz.y) ? -1.0 : 1.0
    let sB:CGFloat = 0 != signbit(xyz.z) ? -1.0 : 1.0
    
    if 0.0 < gamma {
      xyz.x -**= gamma
      xyz.y -**= gamma
      xyz.z -**= gamma
    } else if 0.0 > gamma {
      
      func translate(xyz: CGFloat) -> CGFloat {
        return (0.04045 >= fabs(xyz)) ? fabs(xyz) / 12.92 : ((fabs(xyz) + 0.055) / 1.055) -** 2.4
      }
      
      xyz.x = translate(xyz.x)
      xyz.y = translate(xyz.y)
      xyz.z = translate(xyz.z)
      
      xyz.x *= sR
      xyz.y *= sG
      xyz.z *= sB
    } else {
      
      func translate(xyz: CGFloat) -> CGFloat {
        let lte:CGFloat = (2700.0 * fabs(xyz) / 24389.0)
        let gt:CGFloat =  ((((1000000.0 * fabs(xyz) + 480000.0) * fabs(xyz) + 76800.0) * fabs(xyz) + 4096.0) / 1560896.0)
        return (0.08 >= fabs(xyz)) ? lte : gt
      }
      
      xyz.x = translate(xyz.x)
      xyz.y = translate(xyz.y)
      xyz.z = translate(xyz.z)
      
      xyz.x *= sR
      xyz.y *= sG
      xyz.z *= sB
    }
    
    xyz.x *= 100.0
    xyz.y *= 100.0
    xyz.z *= 100.0
    
    let apply = matrix.applyWorkingSpace(xyz.x, yg: xyz.y, zb: xyz.z)
    
    xyz.x = apply.xr
    xyz.y = apply.yg
    xyz.z = apply.zb
    
    return xyz
  }
  
  public func toHLAB(rgbSpace: RGBWorkingSpace) -> HLAB {
    return self.toXYZ(rgbSpace).toHLAB()
  }
  
  public func toLAB(rgbSpace: RGBWorkingSpace) -> LAB {
    return self.toXYZ(rgbSpace).toLAB(rgbSpace.referenceWhite)
  }
  
  public func toLUV(rgbSpace: RGBWorkingSpace) -> LUV {
    return self.toXYZ(rgbSpace).toLUV(rgbSpace.referenceWhite)
  }
  
  public func toLCHab(rgbSpace: RGBWorkingSpace) -> LCHab {
    return self.toXYZ(rgbSpace).toLAB(rgbSpace.referenceWhite).toLCHab()
  }
  
  public func toLCHuv(rgbSpace: RGBWorkingSpace) -> LCHuv {
    return self.toXYZ(rgbSpace).toLUV(rgbSpace.referenceWhite).toLCHuv()
  }
}

public struct XYZ {
  public var x:CGFloat = 0.0
  public var y:CGFloat = 0.0
  public var z:CGFloat = 0.0
  
  public init() {}
  
  public init(x: CGFloat, y: CGFloat, z: CGFloat) {
    self.x = x
    self.y = y
    self.z = z
  }
  
  public func toHLAB() -> HLAB {
    var hlab:HLAB = HLAB()
    
    hlab.l = 10.0 * sqrt(self.y)
    hlab.a = 17.5 * (((1.02 * self.x) - self.y) / sqrt(self.y))
    hlab.b = 7.0 * ((self.y - (0.847 * self.z)) / sqrt(self.y))
    
    return hlab
  }
  
  public func toLAB(refWhite: ReferenceWhite) -> LAB {
    var lab:LAB = LAB()
    
    let matrix:ReferenceWhiteMatrix = Matrices.ReferenceWhiteMatrices[refWhite]!
    
    var X:CGFloat = self.x / (matrix.Xw * 100.0)
    var Y:CGFloat = self.y / (matrix.Yw * 100.0)
    var Z:CGFloat = self.z / (matrix.Zw * 100.0)
    
    X = (X > ɛ) ? X ** (1.0 / 3.0) : (κ * X) + (16.0 / 116.0)
    Y = (Y > ɛ) ? Y ** (1.0 / 3.0) : (κ * Y) + (16.0 / 116.0)
    Z = (Z > ɛ) ? Z ** (1.0 / 3.0) : (κ * Z) + (16.0 / 116.0)
    
    lab.l = (116.0 * Y) - 16.0
    lab.a = 500.0 * (X - Y)
    lab.b = 200.0 * (Y - Z)
    
    return lab
  }
  
  public func toLUV(refWhite: ReferenceWhite) -> LUV {
    var luv:LUV = LUV()
    
    let matrix:ReferenceWhiteMatrix = Matrices.ReferenceWhiteMatrices[refWhite]!
    
    luv.u = (4.0 * self.x) / (self.x + (15.0 * self.y) + (3.0 * self.z))
    luv.v = (9.0 * self.y) / (self.x + (15.0 * self.y) + (3.0 * self.z))
    
    luv.l = self.y / 100.0
    luv.l = (ɛ < luv.l) ? luv.l ** (1.0 / 3.0) : (κ * luv.l) + (16.0 / 116.0)
    
    var refU:CGFloat = (4.0 * (matrix.Xw * 100.0)) /
      ((matrix.Xw * 100.0) +
        (15.0 * (matrix.Yw * 100.0)) +
        (3.0 * (matrix.Zw * 100.0)))
    var refV:CGFloat = (9.0 * (matrix.Yw * 100.0)) /
      ((matrix.Xw * 100.0) +
        (15.0 * (matrix.Yw * 100.0)) +
        (3.0 * (matrix.Zw * 100.0)))
    
    luv.l = (116.0 * luv.l) - 16.0
    luv.u = 13.0 * luv.l * (luv.u - refU)
    luv.v = 13.0 * luv.l * (luv.v - refV)
    
    return luv
  }
  
  public func toRGB(rgbSpace: RGBWorkingSpace) -> RGB {
    var rgb:RGB = RGB()
    
    /*
    * We want all these impliclty unwrapped optionals to crash immediately because it means
    * that we have messed up our matrices somehow.
    */
    let workingMatrix:[RGBWorkingSpaceMatrix] = Matrices.WorkingMatrices[rgbSpace]!
    let matrix = workingMatrix.last!
    let gamma:CGFloat = Gamma.RGB[rgbSpace]!
    
    let apply = matrix.applyWorkingSpace(self.x / 100.0, yg: self.y / 100.0, zb: self.z / 100.0)
    
    rgb.r = apply.xr
    rgb.g = apply.yg
    rgb.b = apply.zb
    
    let sR:CGFloat = 0 != signbit(rgb.r) ? -1.0 : 1.0
    let sG:CGFloat = 0 != signbit(rgb.g) ? -1.0 : 1.0
    let sB:CGFloat = 0 != signbit(rgb.b) ? -1.0 : 1.0
    
    if 0.0 < gamma {
      rgb.r -**= (1.0 / gamma)
      rgb.g -**= (1.0 / gamma)
      rgb.b -**= (1.0 / gamma)
    } else if 0.0 > gamma {
      rgb.r = (abs(rgb.r) <= 0.0031308)
        ? 12.92 * abs(rgb.r)
        : 1.055 * (abs(rgb.r) -** (1.0 / 2.4)) - 0.055
      rgb.g = (abs(rgb.g) <= 0.0031308)
        ? 12.92 * abs(rgb.g)
        : 1.055 * (abs(rgb.g) -** (1.0 / 2.4)) - 0.055
      rgb.b = (abs(rgb.b) <= 0.0031308)
        ? 12.92 * abs(rgb.b)
        : 1.055 * (abs(rgb.b) -** (1.0 / 2.4)) - 0.055
      
      rgb.r *= sR
      rgb.g *= sG
      rgb.b *= sB
    } else {
      rgb.r = (abs(rgb.r) <= (216.0 / 24389.0))
        ? (abs(rgb.r) * 24389.0 / 2700.0)
        : (1.16 * (fabs(rgb.r) -** (1.0 / 3.0)) - 0.16)
      rgb.g = (abs(rgb.g) <= (216.0 / 24389.0))
        ? (fabs(rgb.g) * 24389.0 / 2700.0)
        : (1.16 * (abs(rgb.g) -** (1.0 / 3.0)) - 0.16)
      rgb.b = (abs(rgb.b) <= (216.0 / 24389.0))
        ? (abs(rgb.b) * 24389.0 / 2700.0)
        : (1.16 * (abs(rgb.b) -** (1.0 / 3.0)) - 0.16)
      
      rgb.r *= sR
      rgb.g *= sG
      rgb.b *= sB
    }
    
    return rgb.sanitize()
  }
  
}

public struct RYB {
  public var r:CGFloat = 0.0
  public var y:CGFloat = 0.0
  public var b:CGFloat = 0.0
  
  public init() {}
  
  public init(r: CGFloat, y: CGFloat, b: CGFloat) {
    self.r = r
    self.y = y
    self.b = b
  }
  
  public func toRGB() -> RGB {
    var rgb:RGB = RGB()
    
    func cubicInterpolation(t: CGFloat, a: CGFloat, b: CGFloat) -> CGFloat {
      return (a + (t * t * (3.0 - 2.0 * t)) * (b - a))
    }
    
    var x0 = cubicInterpolation(self.b, 1.0, 0.163)
    var x1 = cubicInterpolation(self.b, 1.0, 0.0)
    var x2 = cubicInterpolation(self.b, 1.0, 0.5)
    var x3 = cubicInterpolation(self.b, 1.0, 0.2)
    var y0 = cubicInterpolation(self.y, x0, x1)
    var y1 = cubicInterpolation(self.y, x2, x3)
    rgb.r = cubicInterpolation(self.r, y0, y1)
    
    x0 = cubicInterpolation(self.b, 1.0, 0.373)
    x1 = cubicInterpolation(self.b, 1.0, 0.66)
    x2 = cubicInterpolation(self.b, 0.0, 0.0)
    x3 = cubicInterpolation(self.b, 0.5, 0.094)
    y0 = cubicInterpolation(self.y, x0, x1)
    y1 = cubicInterpolation(self.y, x2, x3)
    rgb.g = cubicInterpolation(self.r, y0, y1)
    
    x0 = cubicInterpolation(self.b, 1.0, 0.6)
    x1 = cubicInterpolation(self.b, 0.0, 0.2)
    x2 = cubicInterpolation(self.b, 0.0, 0.5)
    x3 = cubicInterpolation(self.b, 0.0, 0.0)
    y0 = cubicInterpolation(self.y, x0, x1)
    y1 = cubicInterpolation(self.y, x2, x3)
    rgb.b = cubicInterpolation(self.r, y0, y1)
    
    return rgb.sanitize()
  }
}

public struct HSL {
  public var h:CGFloat = 0.0
  public var s:CGFloat = 0.0
  public var l:CGFloat = 0.0
  
  public init() {}
  
  public init(h: CGFloat, s: CGFloat, l: CGFloat) {
    self.h = h
    self.s = s
    self.l = l
  }
  
  public func toRGB() -> RGB {
    var rgb:RGB = RGB(r: 1.0, g: 1.0, b: 1.0)
    
    func hue2rgb(v1: CGFloat, v2: CGFloat, H: CGFloat) -> CGFloat {
      var vH:CGFloat = H
      if (0.0 > vH) {
        vH += 1.0
      }
      
      if (1.0 < vH) {
        vH -= 1.0
      }
      
      return (1.0 > (6.0 * vH))
                ? (v1 + (v2 - v1) * 6.0 * vH)
                : (1.0 > (2.0 * vH))
                  ? (v2)
                  : (2.0 > (3.0 * vH))
                    ? (v1 + (v2 - v1) * ((2.0 / 3.0) - vH) * 6.0)
                    : (v1)
    }
    
    if 0.0 != self.s {
      let var_2:CGFloat = (0.5 > self.l) ? self.l * (1.0 + self.s)
        : (self.l + self.s) - (self.s * self.l);
      
      let var_1:CGFloat = 2.0 * self.l - var_2;
      
      rgb.r = hue2rgb(var_1, var_2, self.h + (1.0 / 3.0));
      rgb.g = hue2rgb(var_1, var_2, self.h);
      rgb.b = hue2rgb(var_1, var_2, self.h - (1.0 / 3.0));
    }
    
    return rgb.sanitize()
  }
}

public struct HSV {
  public var h:CGFloat = 0.0
  public var s:CGFloat = 0.0
  public var v:CGFloat = 0.0
  
  public init() {}
  
  public init(h: CGFloat, s: CGFloat, v: CGFloat) {
    self.h = h
    self.s = s
    self.v = v
  }
  
  public func toRGB() -> RGB {
    var rgb:RGB = RGB()
    
    let h:CGFloat = (self.h * CGFloat(M_2PI)).degrees
    let i:Int = Int(floor(h))
    let f:CGFloat = h - CGFloat(i)
    let p:CGFloat = self.v * (1.0 - self.s)
    let q:CGFloat = self.v * (1.0 - self.s * f)
    let t:CGFloat = self.v * (1.0 - self.s * (1.0 - f))
    
    switch (i) {
    case 0:
      rgb.r = v
      rgb.g = t
      rgb.b = p
    case 1:
      rgb.r = q
      rgb.g = v
      rgb.b = p
    case 2:
      rgb.r = p
      rgb.g = v
      rgb.b = t
    case 3:
      rgb.r = p
      rgb.g = q
      rgb.b = v
    case 4:
      rgb.r = t
      rgb.g = p
      rgb.b = v
    default:
      rgb.r = v
      rgb.g = p
      rgb.b = q
    }
    
    return rgb.sanitize()
  }
}

public struct LCHab {
  public var l:CGFloat = 0.0
  public var c:CGFloat = 0.0
  public var h:CGFloat = 0.0
  
  public init() {}
  
  public init(l: CGFloat, c: CGFloat, h: CGFloat) {
    self.l = l
    self.c = c
    self.h = h
  }
  
  public func toRGB() -> RGB {
    return RGB()
  }
}

public struct LCHuv {
  public var l:CGFloat = 0.0
  public var c:CGFloat = 0.0
  public var h:CGFloat = 0.0
  
  public init() {}
  
  public init(l: CGFloat, c: CGFloat, h: CGFloat) {
    self.l = l
    self.c = c
    self.h = h
  }
  
  public func toRGB() -> RGB {
    return RGB()
  }
}

public struct HSI {
  public var h:CGFloat = 0.0
  public var s:CGFloat = 0.0
  public var i:CGFloat = 0.0
  
  public init() {}
  
  public init(h: CGFloat, s: CGFloat, i: CGFloat) {
    self.h = h
    self.s = s
    self.i = i
  }
  
  public func toRGB() -> RGB {
    var rgb:RGB = RGB()
    
    if 0.0 <= self.h && (M_2PI / 3.0) >= self.h {
      rgb.b = (1.0 / 3.0) * (1.0 - self.s)
      rgb.r = (1.0 / 3.0) * ((self.s * cos(self.h)) / cos((M_2PI / 6.0) - self.h))
      rgb.g = 1.0 - (rgb.b + rgb.r)
    } else if (M_2PI / 3.0) < h && ((2.0 * M_2PI) / 3.0) >= self.h {
      let hA:CGFloat = self.h - (M_2PI / 3.0)
      rgb.r = (1.0 / 3.0) * (1.0 - self.s)
      rgb.g = (1.0 / 3.0) * ((self.s * cos(hA)) / cos((M_2PI / 6.0) - hA))
      rgb.b = 1.0 - (rgb.g + rgb.r)
    } else {
      let hA:CGFloat = self.h - (2.0 * M_2PI / 3.0)
      rgb.g = (1.0 / 3.0) * (1.0 - self.s)
      rgb.b = (1.0 / 3.0) * ((self.s * cos(hA)) / cos((M_2PI / 6.0) - hA))
      rgb.r = 1.0 - ((rgb.g) + (rgb.b))
    }
    
    return rgb.sanitize()
  }
}

public struct HLAB {
  public var l:CGFloat = 0.0
  public var a:CGFloat = 0.0
  public var b:CGFloat = 0.0
  
  public init() {}
  
  public init(l: CGFloat, a: CGFloat, b: CGFloat) {
    self.l = l
    self.a = a
    self.b = b
  }
  
  public func toXYZ() -> XYZ {
    var xyz:XYZ = XYZ()
    
    xyz.y = self.l / 10.0
    xyz.x = self.a / 17.5 * self.l / 10.0
    xyz.z = self.b / 7.0 * self.l / 10.0
    
    xyz.y **= 2.0
    xyz.x = (xyz.x + xyz.y) / 1.02
    xyz.z = -1 * (xyz.z - xyz.y) / 0.847
    
    return xyz
  }
}

public struct LAB {
  public var l:CGFloat = 0.0
  public var a:CGFloat = 0.0
  public var b:CGFloat = 0.0
  
  public init() {}
  
  public init(l: CGFloat, a: CGFloat, b: CGFloat) {
    self.l = l
    self.a = a
    self.b = b
  }
  
  public func toXYZ(refWhite: ReferenceWhite) -> XYZ {
    var xyz:XYZ = XYZ()
    
    let matrix:ReferenceWhiteMatrix = Matrices.ReferenceWhiteMatrices[refWhite]!
    
    xyz.y = (self.l + 16.0) / 116.0
    xyz.x = self.a / 500.0 + xyz.y
    xyz.z = xyz.y - self.b / 200.0
    
    xyz.y = (ɛ < (xyz.y ** 3.0)) ? xyz.y ** 3.0 : (xyz.y - 16.0 / 116.0) / κ
    xyz.x = (ɛ < (xyz.x ** 3.0)) ? xyz.x ** 3.0 : (xyz.x - 16.0 / 116.0) / κ
    xyz.z = (ɛ < (xyz.z ** 3.0)) ? xyz.z ** 3.0 : (xyz.z - 16.0 / 116.0) / κ
    
    xyz.x *= matrix.Xw * 100.0
    xyz.y *= matrix.Yw * 100.0
    xyz.z *= matrix.Zw * 100.0
    
    return xyz
  }
  
  public func toLCHab() -> LCHab {
    var lch:LCHab = LCHab()
    
    lch.l = self.l
    lch.c = sqrt((self.a ** 2.0) + (self.b ** 2.0))
    lch.h = atan2(self.b, self.a).degrees
    
    while 0.0 > lch.h {
      lch.h += M_2PI.degrees
    }
    while M_2PI.degrees < lch.h {
      lch.h -= M_2PI.degrees
    }
    
    lch.h /= M_2PI.degrees
    
    return lch
  }
  
  public func toRGB(rgbSpace: RGBWorkingSpace) -> RGB {
    return self.toXYZ(rgbSpace.referenceWhite).toRGB(rgbSpace)
  }
}

public struct LUV {
  public var l:CGFloat = 0.0
  public var u:CGFloat = 0.0
  public var v:CGFloat = 0.0
  
  public init() {}
  
  public init(l: CGFloat, u: CGFloat, v: CGFloat) {
    self.l = l
    self.u = u
    self.v = v
  }
  
  public func toLCHuv() -> LCHuv {
    var lch:LCHuv = LCHuv()
    
    lch.l = self.l
    lch.c = sqrt((self.u ** 2.0) + (self.v ** 2.0))
    lch.h = atan2(self.v, self.u).degrees
    
    while 0.0 > lch.h {
      lch.h += M_2PI.degrees
    }
    while M_2PI.degrees < lch.h {
      lch.h -= M_2PI.degrees
    }
    
    lch.h /= M_2PI.degrees
    
    return lch
  }
}

public struct XYY {
  public var x:CGFloat = 0.0
  public var y:CGFloat = 0.0
  public var Y:CGFloat = 0.0
  
  public init() {}
  
  public init(x: CGFloat, y: CGFloat, Y: CGFloat) {
    self.x = x
    self.y = y
    self.Y = Y
  }
}

public struct CMYK {
  public var c:CGFloat = 0.0
  public var m:CGFloat = 0.0
  public var y:CGFloat = 0.0
  public var k:CGFloat = 0.0
  
  public init() {}
  
  public init(c: CGFloat, m: CGFloat, y: CGFloat, k: CGFloat) {
    self.c = c
    self.m = m
    self.y = y
    self.k = k
  }
  
  public func toRGB() -> RGB {
    var rgb:RGB = RGB()
    
    rgb.r = (1.0 - self.c) * (1.0 - self.k)
    rgb.g = (1.0 - self.m) * (1.0 - self.k)
    rgb.b = (1.0 - self.y) * (1.0 - self.k)
    
    return rgb
  }
}

// MARK: - Constants
struct Numerics {
  static let ε:CGFloat = 0.008856
  static let κ:CGFloat = 7.787
}

struct Gamma {
  static let RGB:[RGBWorkingSpace:CGFloat] =
  [ .AdobeD65       : 2.2,
    .AppleD65       : 1.8,
    .BestD50        : 2.2,
    .BetaD50        : 2.2,
    .BruceD65       : 2.2,
    .CIEE           : 2.2,
    .ColormatchD50  : 1.8,
    .DonD50         : 2.2,
    .ECID50         : 0.0,
    .EktaSpaceD50   : 2.2,
    .NTSCC          : 2.2,
    .PalSecamD65    : 2.2,
    .ProPhotoD50    : 1.8,
    .SMPTECD65      : 2.2,
    .sRGBD65        : -2.2,
    .WideGamutD50   : 2.2,
    .AdobeD50       : 2.2,
    .AppleD50       : 1.8,
    .BruceD50       : 2.2,
    .CEID50         : 2.2,
    .NTSCD50        : 2.2,
    .PalSecamD50    : 2.2,
    .SMPTECD50      : 2.2,
    .sRGBD50        : -2.2
  ]
}

struct Matrices {
  
  static let ReferenceWhiteMatrices:[ReferenceWhite:ReferenceWhiteMatrix] =
    [ .A    : ReferenceWhiteMatrix(Xw: 1.09850, Yw: 1.0, Zw: 0.35585),
      .B    : ReferenceWhiteMatrix(Xw: 0.99072, Yw: 1.0, Zw: 0.85223),
      .C    : ReferenceWhiteMatrix(Xw: 0.98074, Yw: 1.0, Zw: 1.18232),
      .D50  : ReferenceWhiteMatrix(Xw: 0.96422, Yw: 1.0, Zw: 0.82521),
      .D55  : ReferenceWhiteMatrix(Xw: 0.95682, Yw: 1.0, Zw: 0.92149),
      .D65  : ReferenceWhiteMatrix(Xw: 0.95047, Yw: 1.0, Zw: 1.08883),
      .D75  : ReferenceWhiteMatrix(Xw: 0.94972, Yw: 1.0, Zw: 1.22638),
      .E    : ReferenceWhiteMatrix(Xw: 1.0,     Yw: 1.0, Zw: 1.0),
      .F2   : ReferenceWhiteMatrix(Xw: 0.99186, Yw: 1.0, Zw: 0.67393),
      .F7   : ReferenceWhiteMatrix(Xw: 0.95041, Yw: 1.0, Zw: 1.08747),
      .F11  : ReferenceWhiteMatrix(Xw: 1.00962, Yw: 1.0, Zw: 0.64350)
    ]
  
  static let WorkingMatrices:[RGBWorkingSpace:[RGBWorkingSpaceMatrix]] =
    [ .AdobeD65      : [  RGBWorkingSpaceMatrix(  _00: 0.5767309,   _01: 0.1855540,   _02: 0.1881852,
                                                  _10: 0.2973769,   _11: 0.6273491,   _12: 0.0752741,
                                                  _20: 0.0270343,   _21: 0.0706872,   _22: 0.9911085),
                          RGBWorkingSpaceMatrix(  _00: 2.0413690,   _01: -0.5649464,  _02: -0.3446944,
                                                  _10: -0.9692660,  _11: 1.8760108,   _12: 0.0415560,
                                                  _20: 0.0134474,   _21: -0.1183897,  _22: 1.0154096)],
    
      .AppleD65      : [  RGBWorkingSpaceMatrix(  _00: 0.4497288,   _01: 0.3162486,   _02: 0.1844926,
                                                  _10: 0.2446525,   _11: 0.6720283,   _12: 0.0833192,
                                                  _20: 0.0251848,   _21: 0.1411824,   _22: 0.9224628),
                          RGBWorkingSpaceMatrix(  _00: 2.9515373,   _01: -1.2894116,  _02: -0.4738445,
                                                  _10: -1.0851093,  _11: 1.9908566,   _12: 0.0372026,
                                                  _20: 0.0854934,   _21: -0.2694964,  _22: 1.0912975)],
    
      .BestD50       : [  RGBWorkingSpaceMatrix(  _00: 0.6326696,   _01: 0.2045558,   _02: 0.1269946,
                                                  _10: 0.2284569,   _11: 0.7373523,   _12: 0.0341908,
                                                  _20: 0.0000000,   _21: 0.0095142,   _22: 0.8156958),
                          RGBWorkingSpaceMatrix(  _00: 1.7552599,   _01: -0.4836786,  _02: -0.2530000,
                                                  _10: -0.5441336,  _11: 1.5068789,   _12: 0.0215528,
                                                  _20: 0.0063467,   _21: -0.0175761,  _22: 1.2256959)],
    
      .BetaD50       : [  RGBWorkingSpaceMatrix(  _00: 0.6712537,   _01: 0.1745834,   _02: 0.1183829,
                                                  _10: 0.3032726,   _11: 0.6637861,   _12: 0.0329413,
                                                  _20: 0.0000000,   _21: 0.0407010,   _22: 0.7845090),
                          RGBWorkingSpaceMatrix(  _00: 1.6832270,   _01: -0.4282363,  _02: -0.2360185,
                                                  _10: -0.7710229,  _11: 1.7065571,   _12: 0.0446900,
                                                  _20: 0.0400013,   _21: -0.0885376,  _22: 1.2723640)],
    
      .BruceD65      : [  RGBWorkingSpaceMatrix(  _00: 0.4674162,   _01: 0.2944512,   _02: 0.1886026,
                                                  _10: 0.2410115,   _11: 0.6835475,   _12: 0.0754410,
                                                  _20: 0.0219101,   _21: 0.0736128,   _22: 0.9933071),
                          RGBWorkingSpaceMatrix(  _00: 2.7454669,   _01: -1.1358136,  _02: -0.4350269,
                                                  _10: -0.9692660,  _11: 1.8760108,   _12: 0.0415560,
                                                  _20: 0.0112723,   _21: -0.1139754,  _22: 1.0132541)],
    
      .CIEE          : [  RGBWorkingSpaceMatrix(  _00: 0.4887180,   _01: 0.3106803,   _02: 0.2006017,
                                                  _10: 0.1762044,   _11: 0.8129847,   _12: 0.0108109,
                                                  _20: 0.0000000,   _21: 0.0102048,   _22: 0.9897952),
                          RGBWorkingSpaceMatrix(  _00: 2.3706743,   _01: -0.9000405,  _02: -0.4706338,
                                                  _10: -0.5138850,  _11: 1.4253036,   _12: 0.0885814,
                                                  _20: 0.0052982,   _21: -0.0146949,  _22: 1.0093968)],
    
      .ColormatchD50 : [  RGBWorkingSpaceMatrix(  _00: 0.5093439,   _01: 0.3209071,   _02: 0.1339691,
                                                  _10: 0.2748840,   _11: 0.6581315,   _12: 0.0669845,
                                                  _20: 0.0242545,   _21: 0.1087821,   _22: 0.6921735),
                          RGBWorkingSpaceMatrix(  _00: 2.6422874,   _01: -1.2234270,  _02: -0.3930143,
                                                  _10: -1.1119763,  _11: 2.0590183,   _12: 0.0159614,
                                                  _20: 0.0821699,   _21: -0.2807254,  _22: 1.4559877)],
      
      .DonD50        : [  RGBWorkingSpaceMatrix(  _00: 0.6457711,   _01: 0.1933511,   _02: 0.1250978,
                                                  _10: 0.2783496,   _11: 0.6879702,   _12: 0.0336802,
                                                  _20: 0.0037113,   _21: 0.0179861,   _22: 0.8035125),
                          RGBWorkingSpaceMatrix(  _00: 1.7603902,   _01: -0.4881198,  _02: -0.2536126,
                                                  _10: -0.7126288,  _11: 1.6527432,   _12: 0.0416715,
                                                  _20: 0.0078207,   _21: -0.0347411,  _22: 1.2447743)],
      
      .ECID50        : [  RGBWorkingSpaceMatrix(  _00: 0.6502043,   _01: 0.1780774,   _02: 0.1359384,
                                                  _10: 0.3202499,   _11: 0.6020711,   _12: 0.0776791,
                                                  _20: 0.0000000,   _21: 0.0678390,   _22: 0.7573710),
                          RGBWorkingSpaceMatrix(  _00: 1.7827618,   _01: -0.4969847,  _02: -0.2690101,
                                                  _10: -0.9593623,  _11: 1.9477962,   _12: -0.0275807,
                                                  _20: 0.0859317,   _21: -0.1744674,  _22: 1.3228273)],
      
      .EktaSpaceD50  : [  RGBWorkingSpaceMatrix(  _00: 0.5938914,   _01: 0.2729801,   _02: 0.0973485,
                                                  _10: 0.2606286,   _11: 0.7349465,   _12: 0.0044249,
                                                  _20: 0.0000000,   _21: 0.0419969,   _22: 0.7832131),
                          RGBWorkingSpaceMatrix(  _00: 2.0043819,   _01: -0.7304844,  _02: -0.2450052,
                                                  _10: -0.7110285,  _11: 1.6202126,   _12: 0.0792227,
                                                  _20: 0.0381263,   _21: -0.0868780,  _22: 1.2725438)],
      
      .NTSCC         : [  RGBWorkingSpaceMatrix(  _00: 0.6068909,   _01: 0.1735011,   _02: 0.2003480,
                                                  _10: 0.2989164,   _11: 0.5865990,   _12: 0.1144845,
                                                  _20: 0.0000000,   _21: 0.0660957,   _22: 1.1162243),
                          RGBWorkingSpaceMatrix(  _00: 1.9099961,   _01: -0.5324542,  _02: -0.2882091,
                                                  _10: -0.9846663,  _11: 1.9991710,   _12: -0.0283082,
                                                  _20: 0.0583056,   _21: -0.1183781,  _22: 0.8975535)],
      
      .PalSecamD65   : [  RGBWorkingSpaceMatrix(  _00: 0.4306190,   _01: 0.3415419,   _02: 0.1783091,
                                                  _10: 0.2220379,   _11: 0.7066384,   _12: 0.0713236,
                                                  _20: 0.0201853,   _21: 0.1295504,   _22: 0.9390944),
                          RGBWorkingSpaceMatrix(  _00: 3.0628971,   _01: -1.3931791,  _02: -0.4757517,
                                                  _10: -0.9692660,  _11: 1.8760108,   _12: 0.0415560,
                                                  _20: 0.0678775,   _21: -0.2288548,  _22: 1.0693490)],
      
      .ProPhotoD50   : [  RGBWorkingSpaceMatrix(  _00: 0.7976749,   _01: 0.1351917,   _02: 0.0313534,
                                                  _10: 0.2880402,   _11: 0.7118741,   _12: 0.0000857,
                                                  _20: 0.0000000,   _21: 0.0000000,   _22: 0.8252100),
                          RGBWorkingSpaceMatrix(  _00: 1.3459433,   _01: -0.2556075,  _02: -0.0511118,
                                                  _10: -0.5445989,  _11: 1.5081673,   _12: 0.0205351,
                                                  _20: 0.0000000,   _21: 0.0000000,   _22: 1.2118128)],
      
      .SMPTECD65     : [  RGBWorkingSpaceMatrix(  _00: 0.3935891,   _01: 0.3652497,   _02: 0.1916313,
                                                  _10: 0.2124132,   _11: 0.7010437,   _12: 0.0865432,
                                                  _20: 0.0187423,   _21: 0.1119313,   _22: 0.9581563),
                          RGBWorkingSpaceMatrix(  _00: 3.5053960,   _01: -1.7394894,  _02: -0.5439640,
                                                  _10: -1.0690722,  _11: 1.9778245,   _12: 0.0351722,
                                                  _20: 0.0563200,   _21: -0.1970226,  _22: 1.0502026)],
      
      .sRGBD65       : [  RGBWorkingSpaceMatrix(  _00: 0.4124564,   _01: 0.3575761,   _02: 0.1804375,
                                                  _10: 0.2126729,   _11: 0.7151522,   _12: 0.0721750,
                                                  _20: 0.0193339,   _21: 0.1191920,   _22: 0.9503041),
                          RGBWorkingSpaceMatrix(  _00: 3.2404542,   _01: -1.5371385,  _02: -0.4985314,
                                                  _10: -0.9692660,  _11: 1.8760108,   _12: 0.0415560,
                                                  _20: 0.0556434,   _21: -0.2040259,  _22: 1.0572252)],
      
      .WideGamutD50  : [  RGBWorkingSpaceMatrix(  _00: 0.7161046,   _01: 0.1009296,   _02: 0.1471858,
                                                  _10: 0.2581874,   _11: 0.7249378,   _12: 0.0168748,
                                                  _20: 0.0000000,   _21: 0.0517813,   _22: 0.7734287),
                          RGBWorkingSpaceMatrix(  _00: 1.4628067,   _01: -0.1840623,  _02: -0.2743606,
                                                  _10: -0.5217933,  _11: 1.4472381,   _12: 0.0677227,
                                                  _20: 0.0349342,   _21: -0.0968930,  _22: 1.2884099)],
      
      .AdobeD50      : [  RGBWorkingSpaceMatrix(  _00: 0.6097559,   _01: 0.2052401,   _02: 0.1492240,
                                                  _10: 0.3111242,   _11: 0.6256560,   _12: 0.0632197,
                                                  _20: 0.0194811,   _21: 0.0608902,   _22: 0.7448387),
                          RGBWorkingSpaceMatrix(  _00: 1.9624274,   _01: -0.6105343,  _02: -0.3413404,
                                                  _10: -0.9787684,  _11: 1.9161415,   _12: 0.0334540,
                                                  _20: 0.0286869,   _21: -0.1406752,  _22: 1.3487655)],
      
      .AppleD50      : [  RGBWorkingSpaceMatrix(  _00: 0.4755678,   _01: 0.3396722,   _02: 0.1489800,
                                                  _10: 0.2551812,   _11: 0.6725693,   _12: 0.0722496,
                                                  _20: 0.0184697,   _21: 0.1133771,   _22: 0.6933632),
                          RGBWorkingSpaceMatrix(  _00: 2.8510695,   _01: -1.3605261,  _02: -0.4708281,
                                                  _10: -1.0927680,  _11: 2.0348871,   _12: 0.0227598,
                                                  _20: 0.1027403,   _21: -0.2964984,  _22: 1.4510659)],
      
      .BruceD50      : [  RGBWorkingSpaceMatrix(  _00: 0.4941816,   _01: 0.3204834,   _02: 0.1495550,
                                                  _10: 0.2521531,   _11: 0.6844869,   _12: 0.0633600,
                                                  _20: 0.0157886,   _21: 0.0629304,   _22: 0.7464909),
                          RGBWorkingSpaceMatrix(  _00: 2.6502856,   _01: -1.2014485,  _02: -0.4289936,
                                                  _10: -0.9787684,  _11: 1.9161415,   _12: 0.0334540,
                                                  _20: 0.0264570,   _21: -0.1361227,  _22: 1.3458542)],
      
      .CEID50        : [  RGBWorkingSpaceMatrix(  _00: 0.4868870,   _01: 0.3062984,   _02: 0.1710347,
                                                  _10: 0.1746583,   _11: 0.8247541,   _12: 0.0005877,
                                                  _20: -0.0012563,  _21: 0.0169832,   _22: 0.8094831),
                          RGBWorkingSpaceMatrix(  _00: 2.3638081,   _01: -0.8676030,  _02: -0.4988161,
                                                  _10: -0.5005940,  _11: 1.3962369,   _12: 0.1047562,
                                                  _20: 0.0141712,   _21: -0.0306400,  _22: 1.2323842)],
      
      .NTSCD50       : [  RGBWorkingSpaceMatrix(  _00: 0.6343706,   _01: 0.1852204,   _02: 0.1446290,
                                                  _10: 0.3109496,   _11: 0.5915984,   _12: 0.0974520,
                                                  _20: -0.0011817,  _21: 0.0555518,   _22: 0.7708399),
                          RGBWorkingSpaceMatrix(  _00: 1.8464881,   _01: -0.5521299,  _02: -0.2766458,
                                                  _10: -0.9826630,  _11: 2.0044755,   _12: -0.0690396,
                                                  _20: 0.0736477,   _21: -0.1453020,  _22: 1.3018376)],
      
      .PalSecamD50   : [  RGBWorkingSpaceMatrix(  _00: 0.4552773,   _01: 0.3675500,   _02: 0.1413926,
                                                  _10: 0.2323025,   _11: 0.7077956,   _12: 0.0599019,
                                                  _20: 0.0145457,   _21: 0.1049154,   _22: 0.7057489),
                          RGBWorkingSpaceMatrix(  _00: 2.9603944,   _01: -1.4678519,  _02: -0.4685105,
                                                  _10: -0.9787684,  _11: 1.9161415,   _12: 0.0334540,
                                                  _20: 0.0844874,   _21: -0.2545973,  _22: 1.4216174)],
      
      .SMPTECD50     : [  RGBWorkingSpaceMatrix(  _00: 0.4163290,   _01: 0.3931464,   _02: 0.1547446,
                                                  _10: 0.2216999,   _11: 0.7032549,   _12: 0.0750452,
                                                  _20: 0.0136576,   _21: 0.0913604,   _22: 0.7201920),
                          RGBWorkingSpaceMatrix(  _00: 3.3921940,   _01: -1.8264027,  _02: -0.5385522,
                                                  _10: -1.0770996,  _11: 2.0213975,   _12: 0.0207989,
                                                  _20: 0.0723073,   _21: -0.2217902,  _22: 1.3960932)],
      
      .sRGBD50       : [  RGBWorkingSpaceMatrix(  _00: 0.4360747,   _01: 0.3850649,   _02: 0.1430804,
                                                  _10: 0.2225045,   _11: 0.7168786,   _12: 0.0606169,
                                                  _20: 0.0139322,   _21: 0.0971045,   _22: 0.7141733),
                          RGBWorkingSpaceMatrix(  _00: 3.1338561,   _01: -1.6168667,  _02: -0.4906146,
                                                  _10: -0.9787684,  _11: 1.9161415,   _12: 0.0334540,
                                                  _20: 0.0719453,   _21: -0.2289914,  _22: 1.4052427)]
    ]
}

// MARK: - Colour Distance Functions

public func cie76(lhs: LAB, rhs: LAB) -> CGFloat {
  return sqrt(((lhs.l - rhs.l) ** 2.0) + ((lhs.a - rhs.a) ** 2.0) + ((lhs.b - rhs.b) ** 2.0))
}

public func cmc1984(lhs: LAB, rhs: LAB, l: CGFloat, c: CGFloat) -> CGFloat {
  
  let L_d:CGFloat = (lhs.l - rhs.l)
  
  let C_1:CGFloat = sqrt((lhs.a ** 2.0) + (lhs.b ** 2.0))
  let C_2:CGFloat = sqrt((rhs.a ** 2.0) + (rhs.b ** 2.0))
  let C_d:CGFloat = (C_1 - C_2)
  
  let H_d_ab2:CGFloat =
    ((lhs.a - rhs.a) ** 2.0) + ((lhs.b - rhs.b) ** 2.0) - (C_d ** 2.0)
  
  let F:CGFloat = sqrt((C_1 ** 4.0) / ((C_1 ** 4.0) + 1900.0))
  
  let S_L:CGFloat = (16.0 <= lhs.l)
                      ? (lhs.l * 0.040975) / (1.0 + (lhs.l * 0.01765))
                      : 0.511
  
  let S_C:CGFloat = ((C_1 * 0.0638) / (1.0 + (C_1 * 0.0131))) + 0.638
  
  var H_1:CGFloat = atan2(lhs.b, lhs.a).degrees
  while H_1 < 0.0 {
    H_1 += M_2PI.degrees
  }
  while H_1 > M_2PI.degrees {
    H_1 -= M_2PI.degrees
  }
  
  let T:CGFloat = (164.0 <= H_1 && 345.0 >= H_1)
                    ? 0.56 + abs(0.2 * cos((H_1 + 168.0).radians))
                    : 0.36 + abs(0.4 * cos((H_1 + 35.0).radians))
  
  let S_H:CGFloat = S_C * (F * T + 1.0 - F)
  
  /*
   * Commonly used values are 2:1 for acceptability and 1:1 for the threshold
   * of imperceptibility.
   */
  
  return sqrt(((L_d / (l * S_L)) ** 2.0) + ((C_d / (c * S_C)) ** 2.0) + (H_d_ab2 / (S_H ** 2.0)))
}

public func cie94(lhs: LAB, rhs: LAB, media: CIE94) -> CGFloat {
  let L_d:CGFloat = (lhs.l - rhs.l)
  
  let C_1:CGFloat = sqrt((lhs.a ** 2.0) + (lhs.b ** 2.0))
  let C_2:CGFloat = sqrt((rhs.a ** 2.0) + (rhs.b ** 2.0))
  let C_d:CGFloat = (C_1 - C_2)
  
  let D_H_ab2:CGFloat = (((lhs.a - rhs.a) ** 2.0) + ((lhs.b - rhs.b) ** 2.0) - (C_d ** 2.0))
  
  let K_L:CGFloat = (.textiles != media) ? 1.0 : 2.0
  let K_C:CGFloat = 1.0
  let K_H:CGFloat = K_C
  
  let K_1:CGFloat = (.textiles != media) ? 0.045 : 0.048
  let K_2:CGFloat = (.textiles != media) ? 0.015 : 0.014
  
  let S_L:CGFloat = 1.0
  let S_C:CGFloat = 1.0 + (K_1 * C_1)
  let S_H:CGFloat = 1.0 + (K_2 * C_1)
  
  let term0:CGFloat = ((L_d / (K_L * S_L)) ** 2.0)
  let term1:CGFloat = ((C_d / (K_C * S_C)) ** 2.0)
  let term2:CGFloat = (D_H_ab2 / ((K_H ** 2.0) * (S_H ** 2.0)))
  
  return sqrt(term0 + term1 + term2)
}

public func ciede2000(lhs: LAB, rhs: LAB, kl: CGFloat, kc: CGFloat, kh: CGFloat) -> CGFloat {
  /* Calculate C_i_p, h_i_p */
  
  let C_1_s:CGFloat = sqrt((lhs.a ** 2.0) + (lhs.b ** 2.0))
  let C_2_s:CGFloat = sqrt((rhs.a ** 2.0) + (rhs.b ** 2.0))
  let C_sb_p:CGFloat = (C_1_s + C_2_s) / 2.0
  
  let G:CGFloat = 0.5 * (1.0 - sqrt((C_sb_p ** 7.0) / ((C_sb_p ** 7.0) + (25.0 ** 7.0))))
  
  let a_1_p:CGFloat = lhs.a + (G * lhs.a)
  let a_2_p:CGFloat = rhs.a + (G * rhs.a)
  
  let C_1_p:CGFloat = sqrt((a_1_p ** 2.0) + (lhs.b ** 2.0))
  let C_2_p:CGFloat = sqrt((a_2_p ** 2.0) + (rhs.b ** 2.0))
  
  /* Apparently you must work in degrees from here on out? */
  var h_1_p:CGFloat = (0.0 == C_1_p) ? 0.0 : atan2(lhs.b, a_1_p).degrees
  while h_1_p < 0.0 {
    h_1_p += M_2PI.degrees
  }
  while (h_1_p > M_2PI.degrees) {
    h_1_p -= M_2PI.degrees
  }
  
  var h_2_p:CGFloat = (0.0 == C_2_p) ? 0.0 : atan2(rhs.b, a_2_p).degrees
  while h_2_p < 0.0 {
    h_2_p += M_2PI.degrees
  }
  while h_2_p > M_2PI.degrees {
    h_2_p -= M_2PI.degrees
  }
  
  /* Calculate L_d_p, C_d_p, and H_d_p */
  
  let L_d_p:CGFloat = (rhs.l - lhs.l)
  let C_d_p:CGFloat = (C_2_p - C_1_p)
  
  var h_d_p:CGFloat = 0.0
  if 0.0 != (C_1_p * C_2_p) {
    h_d_p = (180.0 >= abs(h_2_p - h_1_p))
      ? (h_2_p - h_1_p)
      : (180.0 < (h_2_p - h_1_p))
        ? ((h_2_p - h_1_p) - M_2PI.degrees)
        : (-180.0 > (h_2_p - h_1_p))
          ? ((h_2_p - h_1_p) + M_2PI.degrees)
          : 0.0
  }
  
  let H_d_p:CGFloat = 2.0 * sqrt(C_1_p * C_2_p) * sin((h_d_p / 2.0).radians)
  
  /* Calculate CIEDE2000 Color-Difference E_d_00 */
  
  let L_b_p:CGFloat = (lhs.l + rhs.l) / 2.0
  let C_b_p:CGFloat = (C_1_p + C_2_p) / 2.0
  
  let h_d_p_abs:CGFloat = abs(h_1_p - h_2_p)
  let h_d_p_sum:CGFloat = (h_1_p + h_2_p)
  
  var h_b_p:CGFloat = h_d_p_sum
  if (0.0 != (C_1_p * C_2_p)) {
    h_b_p = (180.0 >= h_d_p_abs)
              ? (h_d_p_sum / 2.0)
              : ((180.0 < h_d_p_abs) && (M_2PI.degrees > h_d_p_sum))
                ? ((h_d_p_sum + M_2PI.degrees) / 2.0)
                : ((180.0 < h_d_p_abs) && (M_2PI.degrees <= h_d_p_sum))
                  ? ((h_d_p_sum - M_2PI.degrees) / 2.0)
                  : CGFloat.NaN
  }
  
  let t0:CGFloat = (0.17 * cos((h_b_p - 30.0).radians))
  let t1:CGFloat = (0.24 * cos((2.0 * h_b_p).radians))
  let t2:CGFloat = (0.32 * cos(((3.0 * h_b_p) + 6.0).radians))
  let t3:CGFloat = (0.20 * cos(((4.0 * h_b_p) - 63.0).radians))
  let T:CGFloat = 1.0 - t0 + t1 + t2 - t3
  
  let T_d:CGFloat = 30.0 * exp(-1.0 * (((h_b_p - 275.0) / 25.0) ** 2.0))
  
  let R_C:CGFloat = 2.0 * sqrt((C_b_p ** 7.0) / (((C_b_p ** 7.0) + (25.0 ** 7.0))))
  
  let S_L = 1.0 + ((0.015 * ((L_b_p - 50.0) ** 2.0)) / sqrt(20.0 + ((L_b_p - 50.0) ** 2.0)))
  
  let S_C:CGFloat = 1.0 + (0.045 * C_b_p)
  
  let S_H:CGFloat = 1.0 + (0.015 * C_b_p * T)
  
  let R_T:CGFloat = -1.0 * sin((2.0 * T_d).radians) * R_C
  
  let L_term:CGFloat = ((L_d_p / (kl * S_L)) ** 2.0)
  
  let C_term:CGFloat = ((C_d_p / (kc * S_C)) ** 2.0)
  
  let H_term:CGFloat = ((H_d_p / (kh * S_H)) ** 2.0)
  
  let R_term:CGFloat = R_T * (C_d_p / (kc * S_C)) * (H_d_p / (kh * S_H))
  
  return sqrt(L_term + C_term + H_term + R_term)
}

