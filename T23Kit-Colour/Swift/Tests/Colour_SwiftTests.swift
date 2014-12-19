//
//  Colour_SwiftTests.swift
//  Colour-SwiftTests
//
//  Created by Michael Van Milligan on 12/15/14.
//  Copyright (c) 2014 Thirteen23. All rights reserved.
//

import UIKit
import XCTest

import ColourSwift

let XYZ_TEST:[XYZ] =
  [ XYZ(x: 41.2456, y: 21.2673, z: 1.9334),
    XYZ(x: 35.7576, y: 71.5152, z: 11.9192),
    XYZ(x: 18.0437, y: 7.2175, z: 95.0304),
    XYZ(x: 77.0033, y: 92.7825, z: 13.8526),
    XYZ(x: 53.8014, y: 78.7327, z: 106.9496),
    XYZ(x: 59.2894, y: 28.4848, z: 96.9638),
    XYZ(x: 58.6741, y: 40.3185, z: 85.5640),
    XYZ(x: 31.5647, y: 16.4604, z: 27.8951),
    XYZ(x: 23.7210, y: 44.5683, z: 10.3694),
    XYZ(x: 16.0399, y: 22.5929, z: 5.1056),
    XYZ(x: 84.5460, y: 94.5854, z: 108.3908),
    XYZ(x: 20.8174, y: 16.6611, z: 73.3384)
  ]

let RGB_TEST:[RGB] =
  [ RGB(r: 1.000000, g: 0.000000, b: 0.000000),
    RGB(r: 0.000000, g: 1.000000, b: 0.000000),
    RGB(r: 0.000000, g: 0.000000, b: 1.000000),
    RGB(r: 1.000000, g: 1.000000, b: 0.000000),
    RGB(r: 0.000000, g: 1.000000, b: 1.000000),
    RGB(r: 1.000000, g: 0.000000, b: 1.000000),
    RGB(r: 0.933333, g: 0.509804, b: 0.933333),
    RGB(r: 0.815686, g: 0.125490, b: 0.564706),
    RGB(r: 0.196078, g: 0.803922, b: 0.196078),
    RGB(r: 0.419608, g: 0.556863, b: 0.137255),
    RGB(r: 0.878431, g: 1.000000, b: 1.000000),
    RGB(r: 0.254902, g: 0.411765, b: 0.882353)
  ]

let HLAB_TEST:[HLAB] =
  [ HLAB(l: 46.116470, a: 78.942963, b: 29.795856),
    HLAB(l: 84.566666, a: -72.515923, b: 50.840080),
    HLAB(l: 26.865406, a: 72.872406, b: -190.919449),
    HLAB(l: 96.323685, a: -25.869644, b: 58.899899),
    HLAB(l: 88.731461, a: -47.048512, b: -9.351272),
    HLAB(l: 53.371143, a: 104.894089, b: -70.357269),
    HLAB(l: 63.496826, a: 53.822945, b: -35.447365),
    HLAB(l: 40.571369, a: 67.874077, b: -12.365271),
    HLAB(l: 66.759521, a: -53.404575, b: 37.522484),
    HLAB(l: 47.532005, a: -22.945438, b: 26.903835),
    HLAB(l: 97.255035, a: -15.022249, b: 1.999814),
    HLAB(l: 40.817955, a: 19.604727, b: -77.954865)
  ]

let LAB_TEST:[LAB] =
  [ LAB(l: 53.2408, a: 80.0925, b: 67.2032),
    LAB(l: 87.7347, a: -86.1827, b: 83.1793),
    LAB(l: 32.2970, a: 79.1875, b: -107.8602),
    LAB(l: 97.1393, a: -21.5537, b: 94.4780),
    LAB(l: 91.1132, a: -48.0875, b: -14.1312),
    LAB(l: 60.3242, a: 98.2343, b: -60.8249),
    LAB(l: 69.6958, a: 56.3566, b: -36.8098),
    LAB(l: 47.5728, a: 72.2307, b: -17.4159),
    LAB(l: 72.6067, a: -67.1256, b: 61.4373),
    LAB(l: 54.6505, a: -28.2218, b: 49.6907),
    LAB(l: 97.8674, a: -9.9445, b: -3.3751),
    LAB(l: 47.8301, a: 26.2631, b: -65.2636)
  ]

let LCH_AB_TEST:[LCHab] =
  [ LCHab(l: 53.2408, c: 104.5518, h: 0.1111),
    LCHab(l: 87.7347, c: 119.7759, h: 0.3778),
    LCHab(l: 32.2970, c: 133.8076, h: 0.8508),
    LCHab(l: 97.1393, c: 96.9054, h: 0.2857),
    LCHab(l: 91.1132, c: 50.1209, h: 0.5455),
    LCHab(l: 60.3242, c: 115.5407, h: 0.9118),
    LCHab(l: 69.6958, c: 67.3129, h: 0.9079),
    LCHab(l: 47.5728, c: 74.3006, h: 0.9623),
    LCHab(l: 72.6067, c: 90.9966, h: 0.3820),
    LCHab(l: 54.6505, c: 57.1458, h: 0.3322),
    LCHab(l: 97.8674, c: 10.5016, h: 0.5521),
    LCHab(l: 47.8301, c: 70.3498, h: 0.8109)
  ]

let LUV_TEST:[LUV] =
  [ LUV(l: 53.2408, u: 175.0151, v: 37.7564),
    LUV(l: 87.7347, u: -83.0776, v: 107.3986),
    LUV(l: 32.2970, u: -9.4054, v: -130.3423),
    LUV(l: 97.1393, u: 7.7056, v: 106.7866),
    LUV(l: 91.1132, u: -70.4773, v: -15.2042),
    LUV(l: 60.3242, u: 84.0714, v: -108.6833),
    LUV(l: 69.6958, u: 51.8485, v: -67.0272),
    LUV(l: 47.5728, u: 93.2568, v: -36.6593),
    LUV(l: 72.6067, u: -62.9268, v: 81.3486),
    LUV(l: 54.6505, u: -17.4439, v: 57.4400),
    LUV(l: 97.8674, u: -16.3973, v: -3.5374),
    LUV(l: 47.8301, u: -17.5102, v: -101.2174)
  ]

let LCH_UV_TEST:[LCHuv] =
  [ LCHuv(l: 53.2408, c: 179.0414, h: 0.0338),
    LCHuv(l: 87.7347, c: 135.7804, h: 0.3548),
    LCHuv(l: 32.2970, c: 130.6812, h: 0.7385),
    LCHuv(l: 97.1393, c: 107.0643, h: 0.2385),
    LCHuv(l: 91.1132, c: 72.0987, h: 0.5338),
    LCHuv(l: 60.3242, c: 137.4048, h: 0.8548),
    LCHuv(l: 69.6958, c: 84.7403, h: 0.8548),
    LCHuv(l: 47.5728, c: 100.2035, h: 0.9404),
    LCHuv(l: 72.6067, c: 102.8464, h: 0.3548),
    LCHuv(l: 54.6505, c: 60.0304, h: 0.2969),
    LCHuv(l: 97.8674, c: 16.7746, h: 0.5338),
    LCHuv(l: 47.8301, c: 102.7208, h: 0.7227)
  ]

let CMYK_TEST:[CMYK] =
  [ CMYK(c: 0.000000, m: 1.000000, y: 1.000000, k: 0.000000),
    CMYK(c: 1.000000, m: 0.000000, y: 1.000000, k: 0.000000),
    CMYK(c: 1.000000, m: 1.000000, y: 0.000000, k: 0.000000),
    CMYK(c: 0.000000, m: 0.000000, y: 1.000000, k: 0.000000),
    CMYK(c: 1.000000, m: 0.000000, y: 0.000000, k: 0.000000),
    CMYK(c: 0.000000, m: 1.000000, y: 0.000000, k: 0.000000),
    CMYK(c: 0.000000, m: 0.453781, y: 0.000000, k: 0.066667),
    CMYK(c: 0.000000, m: 0.846154, y: 0.307692, k: 0.184314),
    CMYK(c: 0.756098, m: 0.000000, y: 0.756098, k: 0.196078),
    CMYK(c: 0.246479, m: 0.000000, y: 0.753521, k: 0.443137),
    CMYK(c: 0.121569, m: 0.000000, y: 0.000000, k: 0.000000),
    CMYK(c: 0.711111, m: 0.533333, y: 0.000000, k: 0.117647)
  ]

let HSL_TEST:[HSL] =
  [ HSL(h: 0.000000, s: 1.000000, l: 0.500000),
    HSL(h: 0.333333, s: 1.000000, l: 0.500000),
    HSL(h: 0.666667, s: 1.000000, l: 0.500000),
    HSL(h: 0.166667, s: 1.000000, l: 0.500000),
    HSL(h: 0.500000, s: 1.000000, l: 0.500000),
    HSL(h: 0.833333, s: 1.000000, l: 0.500000),
    HSL(h: 0.833333, s: 0.760562, l: 0.721568),
    HSL(h: 0.893939, s: 0.733334, l: 0.470588),
    HSL(h: 0.333333, s: 0.607844, l: 0.500000),
    HSL(h: 0.221184, s: 0.604520, l: 0.347059),
    HSL(h: 0.500000, s: 1.000000, l: 0.939216),
    HSL(h: 0.625000, s: 0.727273, l: 0.568627)
  ]

let HSI_TEST:[HSI] =
  [ HSI(h: 0.000000, s: 1.000000, i: 0.333333),
    HSI(h: 0.333333, s: 1.000000, i: 0.333333),
    HSI(h: 0.666667, s: 1.000000, i: 0.333333),
    HSI(h: 0.166667, s: 1.000000, i: 0.666667),
    HSI(h: 0.500000, s: 1.000000, i: 0.666667),
    HSI(h: 0.833333, s: 1.000000, i: 0.666667),
    HSI(h: 0.833333, s: 0.356435, i: 0.792157),
    HSI(h: 0.893939, s: 0.750000, i: 0.501961),
    HSI(h: 0.333333, s: 0.508198, i: 0.398693),
    HSI(h: 0.221184, s: 0.630282, i: 0.371242),
    HSI(h: 0.500000, s: 0.084469, i: 0.959477),
    HSI(h: 0.625000, s: 0.506329, i: 0.516340)
  ]

class Colour_SwiftTests: XCTestCase {
  
    override func setUp() {
        super.setUp()
        // Put setup code here. This method is called before the invocation of each test method in the class.
    }
  
    override func tearDown() {
        // Put teardown code here. This method is called after the invocation of each test method in the class.
        super.tearDown()
    }
    
    func testUIColor() {
      
      for (index, rgb) in enumerate(RGB_TEST) {
        
        let xyz:XYZ = rgb.toXYZ(.sRGBD65)
        
        XCTAssertEqualWithAccuracy(xyz.x, XYZ_TEST[index].x, 0.001)
        XCTAssertEqualWithAccuracy(xyz.y, XYZ_TEST[index].y, 0.001)
        XCTAssertEqualWithAccuracy(xyz.z, XYZ_TEST[index].z, 0.001)
        
        let hlab:HLAB = rgb.toHLAB(.sRGBD65)
        
        XCTAssertEqualWithAccuracy(hlab.l, HLAB_TEST[index].l, 0.001)
        XCTAssertEqualWithAccuracy(hlab.a, HLAB_TEST[index].a, 0.001)
        XCTAssertEqualWithAccuracy(hlab.b, HLAB_TEST[index].b, 0.001)
        
        let lab:LAB = rgb.toLAB(.sRGBD65)
        
        XCTAssertEqualWithAccuracy(lab.l, LAB_TEST[index].l, 0.001)
        XCTAssertEqualWithAccuracy(lab.a, LAB_TEST[index].a, 0.001)
        XCTAssertEqualWithAccuracy(lab.b, LAB_TEST[index].b, 0.001)
        
        let lchab:LCHab = rgb.toLCHab(.sRGBD65)
        
        XCTAssertEqualWithAccuracy(lchab.l, LCH_AB_TEST[index].l, 0.001)
        XCTAssertEqualWithAccuracy(lchab.c, LCH_AB_TEST[index].c, 0.001)
        XCTAssertEqualWithAccuracy(lchab.h, LCH_AB_TEST[index].h, 0.001)
        
        let luv:LUV = rgb.toLUV(.sRGBD65)
        
        XCTAssertEqualWithAccuracy(luv.l, LUV_TEST[index].l, 0.001)
        XCTAssertEqualWithAccuracy(luv.u, LUV_TEST[index].u, 0.001)
        XCTAssertEqualWithAccuracy(luv.v, LUV_TEST[index].v, 0.001)
        
        let lchuv:LCHuv = rgb.toLCHuv(.sRGBD65)
        
        XCTAssertEqualWithAccuracy(lchuv.l, LCH_UV_TEST[index].l, 0.001)
        XCTAssertEqualWithAccuracy(lchuv.c, LCH_UV_TEST[index].c, 0.001)
        XCTAssertEqualWithAccuracy(lchuv.h, LCH_UV_TEST[index].h, 0.001)
        
        let cmyk:CMYK = rgb.toCMYK()
        
        XCTAssertEqualWithAccuracy(cmyk.c, CMYK_TEST[index].c, 0.001)
        XCTAssertEqualWithAccuracy(cmyk.m, CMYK_TEST[index].m, 0.001)
        XCTAssertEqualWithAccuracy(cmyk.y, CMYK_TEST[index].y, 0.001)
        XCTAssertEqualWithAccuracy(cmyk.k, CMYK_TEST[index].k, 0.001)
        
        let hsl:HSL = rgb.toHSL()
        
        XCTAssertEqualWithAccuracy(hsl.h, HSL_TEST[index].h, 0.001)
        XCTAssertEqualWithAccuracy(hsl.s, HSL_TEST[index].s, 0.001)
        XCTAssertEqualWithAccuracy(hsl.l, HSL_TEST[index].l, 0.001)
        
        let hsi:HSI = rgb.toHSI()
        
        XCTAssertEqualWithAccuracy(hsi.h, HSI_TEST[index].h, 0.001)
        XCTAssertEqualWithAccuracy(hsi.s, HSI_TEST[index].s, 0.001)
        XCTAssertEqualWithAccuracy(hsi.i, HSI_TEST[index].i, 0.001)
        
      }
    }
    
    func testPerformanceExample() {
        // This is an example of a performance test case.
        self.measureBlock() {
            // Put the code you want to measure the time of here.
        }
    }
    
}
