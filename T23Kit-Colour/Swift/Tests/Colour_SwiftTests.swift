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

let LAB_TEST_SET:[LAB] =
  [ /* 1 */
    LAB(l: 50.0000, a: 2.6772, b: -79.7751),
    LAB(l: 50.0000, a: 0.0000, b: -82.7485),
    /* 2 */
    LAB(l: 50.0000, a: 3.1571, b: -77.2803),
    LAB(l: 50.0000, a: 0.0000, b: -82.7485),
    /* 3 */
    LAB(l: 50.0000, a: 2.8361, b: -74.02),
    LAB(l: 50.0000, a: 0.0000, b: -82.7485),
    /* 4 */
    LAB(l: 50.0000, a: -1.3802, b: -84.2814),
    LAB(l: 50.0000, a: 0.0000, b: -82.7485),
    /* 5 */
    LAB(l: 50.0000, a: -1.1848, b: -84.8006),
    LAB(l: 50.0000, a: 0.0000, b: -82.7485),
    /* 6 */
    LAB(l: 50.0000, a: -0.9009, b: -85.5211),
    LAB(l: 50.0000, a: 0.0000, b: -82.7485),
    /* 7 */
    LAB(l: 50.0000, a: 0.0000, b: 0.0000),
    LAB(l: 50.0000, a: -1.0000, b: 2.0000),
    /* 8 */
    LAB(l: 50.0000, a: -1.0000, b: 2.0000),
    LAB(l: 50.0000, a: 0.0000, b: 0.0000),
    /* 9 */
    LAB(l: 50.0000, a: 2.4900, b: -0.0010),
    LAB(l: 50.0000, a: -2.4900, b: 0.0009),
    /* 10 */
    LAB(l: 50.0000, a: 2.4900, b: -0.0010),
    LAB(l: 50.0000, a: -2.4900, b: 0.0010),
    /* 11 */
    LAB(l: 50.0000, a: 2.4900, b: -0.0010),
    LAB(l: 50.0000, a: -2.4900, b: 0.0011),
    /* 12 */
    LAB(l: 50.0000, a: 2.4900, b: -0.0010),
    LAB(l: 50.0000, a: -2.4900, b: 0.0012),
    /* 13 */
    LAB(l: 50.0000, a: -0.0010, b: 2.4900),
    LAB(l: 50.0000, a: 0.0009, b: -2.49),
    /* 14 */
    LAB(l: 50.0000, a: -0.0010, b: 2.49),
    LAB(l: 50.0000, a: 0.0010, b: -2.49),
    /* 15 */
    LAB(l: 50.0000, a: -0.0010, b: 2.49),
    LAB(l: 50.0000, a: 0.0011, b: -2.49),
    /* 16 */
    LAB(l: 50.0000, a: 2.5000, b: 0.0000),
    LAB(l: 50.0000, a: 0.0000, b: -2.5000),
    /* 17 */
    LAB(l: 50.0000, a: 2.5000, b: 0.0000),
    LAB(l: 73.0000, a: 25.0000, b: -18.0000),
    /* 18 */
    LAB(l: 50.0000, a: 2.5000, b: 0.0000),
    LAB(l: 61.0000, a: -5.0000, b: 29.0000),
    /* 19 */
    LAB(l: 50.0000, a: 2.5000, b: 0.0000),
    LAB(l: 56.0000, a: -27.0000, b: -3.0000),
    /* 20 */
    LAB(l: 50.0000, a: 2.5000, b: 0.0000),
    LAB(l: 58.0000, a: 24.0000, b: 15.0000),
    /* 21 */
    LAB(l: 50.0000, a: 2.5000, b: 0.0000),
    LAB(l: 50.0000, a: 3.1736, b: 0.5854),
    /* 22 */
    LAB(l: 50.0000, a: 2.5000, b: 0.0000),
    LAB(l: 50.0000, a: 3.2972, b: 0.0000),
    /* 23 */
    LAB(l: 50.0000, a: 2.5000, b: 0.0000),
    LAB(l: 50.0000, a: 1.8634, b: 0.5757),
    /* 24 */
    LAB(l: 50.0000, a: 2.5000, b: 0.0000),
    LAB(l: 50.0000, a: 3.2592, b: 0.3350),
    /* 25 */
    LAB(l: 60.2574, a: -34.0099, b: 36.2677),
    LAB(l: 60.4626, a: -34.1751, b: 39.4387),
    /* 26 */
    LAB(l: 63.0109, a: -31.0961, b: -5.8663),
    LAB(l: 62.8187, a: -29.7946, b: -4.0864),
    /* 27 */
    LAB(l: 61.2901, a: 3.7196, b: -5.3901),
    LAB(l: 61.4292, a: 2.2480, b: -4.9620),
    /* 28 */
    LAB(l: 35.0831, a: -44.1164, b: 3.7933),
    LAB(l: 35.0232, a: -40.0716, b: 1.5901),
    /* 29 */
    LAB(l: 22.7233, a: 20.0904, b: -46.6940),
    LAB(l: 23.0331, a: 14.9730, b: -42.5619),
    /* 30 */
    LAB(l: 36.4612, a: 47.8580, b: 18.3852),
    LAB(l: 36.2715, a: 50.5065, b: 21.2231),
    /* 31 */
    LAB(l: 90.8027, a: -2.0831, b: 1.4410),
    LAB(l: 91.1528, a: -1.6435, b: 0.0447),
    /* 32 */
    LAB(l: 90.9257, a: -0.5406, b: -0.9208),
    LAB(l: 88.6381, a: -0.8985, b: -0.7239),
    /* 33 */
    LAB(l: 6.7747, a: -0.2908, b: -2.4247),
    LAB(l: 5.8714, a: -0.0985, b: -2.2286),
    /* 34 */
    LAB(l: 2.0776, a: 0.0795, b: -1.1350),
    LAB(l: 0.9033, a: -0.0636, b: -0.5514)
  ]

let cie1976D:[CGFloat] =
  [ 4.001063, 6.314150, 9.177700,  2.062701,  2.369571,  2.915293,  2.236068,
    2.236068, 4.980000, 4.980000,  4.980000,  4.980000,  4.980000,  4.980000,
    4.980000, 3.535534, 36.868008, 31.910030, 30.253099, 27.408940, 0.892429,
    0.797200, 0.858307, 0.829825,  3.181924,  2.213343,  1.538904,  4.606309,
    6.584680, 3.886414, 1.505148,  2.323785,  0.944132,  1.319108
  ]

let cie_graphics_1994D:[CGFloat] =
  [ 1.395039, 1.934101, 2.454336,  0.684492,  0.669563,  0.691945,  2.236068,
    2.031638, 4.800694, 4.800694,  4.800694,  4.800695,  4.800694,  4.800694,
    4.800694, 3.407744, 34.689163, 29.441373, 27.914088, 24.937661, 0.822132,
    0.716584, 0.804875, 0.752844,  1.390995,  1.248089,  1.297958,  1.820451,
    2.556133, 1.424913, 1.419453,  2.322569,  0.938533,  1.306545
  ]

let cie_textiles_1994D:[CGFloat] =
  [ 1.423046, 1.942673, 2.406600,  0.697952,  0.671851,  0.677197,  2.236068,
    2.019331, 4.812245, 4.812246,  4.812246,  4.812246,  4.812245,  4.812246,
    4.812246, 3.415975, 28.250263, 27.730808, 27.328584, 23.807638, 0.819391,
    0.711786, 0.804063, 0.748772,  1.389733,  1.244090,  1.288424,  1.795793,
    2.530989, 1.399092, 1.385811,  1.212342,  0.518211,  0.819075
  ]

let ciede2000D:[CGFloat] =
  [ 2.0425,  2.8615,  3.4412, 1.0000, 1.0000, 1.0000, 2.3669, 2.3669,  7.1792,
    7.1792,  7.2195,  7.2195, 4.8045, 4.8045, 4.7461, 4.3065, 27.1492, 22.8977,
    31.9030, 19.4535, 1.0000, 1.0000, 1.0000, 1.0000, 1.2644, 1.2630,  1.8731,
    1.8645,  2.0373,  1.4146, 1.4441, 1.5381, 0.6377, 0.9082
  ]

let cmc_1_1_1984D:[CGFloat] =
  [ 1.738736, 2.496608, 3.304945,  0.857356,  0.883329,  0.978227,  3.504809,
    2.879300, 6.578381, 6.578381,  6.578381,  6.578381,  6.674923,  6.674923,
    6.674923, 4.668530, 42.108755, 39.458891, 38.360059, 33.936638, 1.144002,
    1.006004, 1.113025, 1.053353,  1.428230,  1.254814,  1.768381,  2.025834,
    3.086955, 1.748935, 1.900952,  1.702581,  1.803172,  2.449344
  ]

let cmc_2_1_1984D:[CGFloat] =
  [ 1.738736, 2.496608, 3.304945,  0.857356,  0.883329,  0.978227,  3.504809,
    2.879300, 6.578381, 6.578381,  6.578381,  6.578381,  6.674923,  6.674923,
    6.674923, 4.668530, 37.923276, 38.475767, 38.061769, 33.334208, 1.144002,
    1.006004, 1.113025, 1.053353,  1.420486,  1.247404,  1.765559,  2.024991,
    3.060441, 1.739572, 1.889083,  0.990070,  0.952801,  1.427773
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
    
  func testLibColorspaces() {
    
    for idx in 0..<Int(LAB_TEST_SET.count / 2) {
      let testLab0:LAB = LAB_TEST_SET[idx * 2]
      let testLab1:LAB = LAB_TEST_SET[(idx * 2) + 1]
      
      XCTAssertEqualWithAccuracy(cie1976D[idx], cie76(testLab0, testLab1), 0.001)
      XCTAssertEqualWithAccuracy(cmc_2_1_1984D[idx], cmc1984(testLab0, testLab1, 2.0, 1.0), 0.001)
      XCTAssertEqualWithAccuracy(cmc_1_1_1984D[idx], cmc1984(testLab0, testLab1, 1.0, 1.0), 0.001)
      XCTAssertEqualWithAccuracy(cie_textiles_1994D[idx], cie94(testLab0, testLab1, .textiles), 0.001)
      XCTAssertEqualWithAccuracy(cie_graphics_1994D[idx], cie94(testLab0, testLab1, .graphics), 0.001)
      XCTAssertEqualWithAccuracy(ciede2000D[idx], ciede2000(testLab0, testLab1, 1.0, 1.0, 1.0), 0.001)
    }
    
  }
    
}
