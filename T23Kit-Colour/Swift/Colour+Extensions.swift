//
//  T23Kit-Colour+Extensions.swift
//  T23Kit-Colour
//
//  Created by Michael Van Milligan on 12/11/14.
//  Copyright (c) 2014 Thirteen23. All rights reserved.
//

import UIKit

// MARK: - Operators

infix operator ** { associativity left precedence 160 }
func ** (left: Double, right: Double) -> Double {
  return pow(left, right)
}

func ** (left: CGFloat, right: CGFloat) -> CGFloat {
  return pow(left, right)
}

func ** (left: Float, right: Float) -> Float {
  return powf(left, right)
}

infix operator **= { associativity right precedence 90 }
func **= (inout left: Double, right: Double) {
  left = left ** right
}

func **= (inout left: CGFloat, right: CGFloat) {
  left = left ** right
}

func **= (inout left: Float, right: Float) {
  left = left ** right
}

infix operator -** { associativity left precedence 160 }
func -** (left: Double, right: Double) -> Double {
  return (0.0 <= left) ? pow(left, right) : -1.0 * pow(-1.0 * left, right)
}

func -** (left: CGFloat, right: CGFloat) -> CGFloat {
  return (0.0 <= left) ? pow(left, right) : -1.0 * pow(-1.0 * left, right)
}

func -** (left: Float, right: Float) -> Float {
  return (0.0 <= left) ? powf(left, right) : -1.0 * powf(-1.0 * left, right)
}

infix operator -**= { associativity right precedence 90 }
func -**= (inout left: Double, right: Double) {
  left = left -** right
}

func -**= (inout left: CGFloat, right: CGFloat) {
  left = left -** right
}

func -**= (inout left: Float, right: Float) {
  left = left -** right
}

extension CGFloat {
  var radians:CGFloat {
    get {
      return (CGFloat(M_PI) / 180.0) * self
    }
  }
  
  var degrees:CGFloat {
    get {
      return (180.0 / CGFloat(M_PI)) * self
    }
  }
}

extension Double {
  var radians:Double {
    get {
      return (M_PI / 180.0) * self
    }
  }
  
  var degrees:Double {
    get {
      return (180.0 / M_PI) * self
    }
  }
}

extension Float {
  var radians:Float {
    get {
      return (Float(M_PI) / 180.0) * self
    }
  }
  
  var degrees:Float {
    get {
      return (180.0 / Float(M_PI)) * self
    }
  }
}

extension Array {
  var last: T? {
    get {
      return self.isEmpty ? nil : self[self.count - 1]
    }
  }
  
  mutating func shuffle() {
    self.sort { (_,_) in 0 == arc4random_uniform(UInt32.max) % 2 }
  }
  
  mutating func reverseInPlace() {
    self = self.reverse()
  }
  
  func enumerateUsingClosure(closure: (index: Int, element: T, inout stop: Bool) -> Void) {
    
    if (self.isEmpty) {
      return
    }
    
    for (index, element) in enumerate(self) {
      var stopper:Bool = false
      closure(index: index, element: element, stop: &stopper)
      if (stopper) { break }
    }
  }
  
  func find(fn: (T) -> Bool) -> Int {
    var i:Int = -1
    for (index, element) in enumerate(self) {
      if fn(element) {
        i = index
        break
      }
    }
    return i
  }
}

extension UIColor {
  
  var hue:CGFloat {
    get {
      return self.getHSB().h
    }
  }
  
  var saturation:CGFloat {
    get {
      return self.getHSB().s
    }
  }
  
  var brightness:CGFloat {
    get {
      return self.getHSB().b
    }
  }
  
  var red:CGFloat {
    get {
      return self.getRGB().r
    }
  }
  
  var green:CGFloat {
    get {
      return self.getRGB().g
    }
  }
  
  var blue:CGFloat {
    get {
      return self.getRGB().b
    }
  }
  
  func getHSB() -> (h: CGFloat, s: CGFloat, b: CGFloat) {
    var h:CGFloat = 0.0
    var s:CGFloat = 0.0
    var b:CGFloat = 0.0
    var a:CGFloat = 0.0
    
    self.getHue(&h, saturation: &s, brightness: &b, alpha: &a)
    return (h, s, b)
  }
  
  func getRGB() -> (r: CGFloat, g: CGFloat, b: CGFloat) {
    var r:CGFloat = 0.0
    var g:CGFloat = 0.0
    var b:CGFloat = 0.0
    var a:CGFloat = 0.0
    
    self.getRed(&r, green: &g, blue: &b, alpha: &a)
    return (r, g, b)
  }
  
  func hexString() -> String {
    let rgb = self.getRGB()
    var R:UInt8 = UInt8(255.0 * rgb.r)
    var G:UInt8 = UInt8(255.0 * rgb.g)
    var B:UInt8 = UInt8(255.0 * rgb.b)
    
    return NSString(format: "%02x%02x%02x", R, G, B)
  }
}