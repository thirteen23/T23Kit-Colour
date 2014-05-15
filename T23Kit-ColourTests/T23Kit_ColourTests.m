//
//  T23Kit_ColourTests.m
//  T23Kit-ColourTests
//
//  Created by Michael Van Milligan on 5/13/14.
//  Copyright (c) 2014 Thirteen23. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "UIColor+T23ColorSpaces.h"
#include "colorspaces.h"

static pixel_t LAB_TEST_SET[] = {
    /* 1 */
    {50.0000, 2.6772, -79.7751},
    {50.0000, 0.0000, -82.7485},
    /* 2 */
    {50.0000, 3.1571, -77.2803},
    {50.0000, 0.0000, -82.7485},
    /* 3 */
    {50.0000, 2.8361, -74.02},
    {50.0000, 0.0000, -82.7485},
    /* 4 */
    {50.0000, -1.3802, -84.2814},
    {50.0000, 0.0000, -82.7485},
    /* 5 */
    {50.0000, -1.1848, -84.8006},
    {50.0000, 0.0000, -82.7485},
    /* 6 */
    {50.0000, -0.9009, -85.5211},
    {50.0000, 0.0000, -82.7485},
    /* 7 */
    {50.0000, 0.0000, 0.0000},
    {50.0000, -1.0000, 2.0000},
    /* 8 */
    {50.0000, -1.0000, 2.0000},
    {50.0000, 0.0000, 0.0000},
    /* 9 */
    {50.0000, 2.4900, -0.0010},
    {50.0000, -2.4900, 0.0009},
    /* 10 */
    {50.0000, 2.4900, -0.0010},
    {50.0000, -2.4900, 0.0010},
    /* 11 */
    {50.0000, 2.4900, -0.0010},
    {50.0000, -2.4900, 0.0011},
    /* 12 */
    {50.0000, 2.4900, -0.0010},
    {50.0000, -2.4900, 0.0012},
    /* 13 */
    {50.0000, -0.0010, 2.4900},
    {50.0000, 0.0009, -2.49},
    /* 14 */
    {50.0000, -0.0010, 2.49},
    {50.0000, 0.0010, -2.49},
    /* 15 */
    {50.0000, -0.0010, 2.49},
    {50.0000, 0.0011, -2.49},
    /* 16 */
    {50.0000, 2.5000, 0.0000},
    {50.0000, 0.0000, -2.5000},
    /* 17 */
    {50.0000, 2.5000, 0.0000},
    {73.0000, 25.0000, -18.0000},
    /* 18 */
    {50.0000, 2.5000, 0.0000},
    {61.0000, -5.0000, 29.0000},
    /* 19 */
    {50.0000, 2.5000, 0.0000},
    {56.0000, -27.0000, -3.0000},
    /* 20 */
    {50.0000, 2.5000, 0.0000},
    {58.0000, 24.0000, 15.0000},
    /* 21 */
    {50.0000, 2.5000, 0.0000},
    {50.0000, 3.1736, 0.5854},
    /* 22 */
    {50.0000, 2.5000, 0.0000},
    {50.0000, 3.2972, 0.0000},
    /* 23 */
    {50.0000, 2.5000, 0.0000},
    {50.0000, 1.8634, 0.5757},
    /* 24 */
    {50.0000, 2.5000, 0.0000},
    {50.0000, 3.2592, 0.3350},
    /* 25 */
    {60.2574, -34.0099, 36.2677},
    {60.4626, -34.1751, 39.4387},
    /* 26 */
    {63.0109, -31.0961, -5.8663},
    {62.8187, -29.7946, -4.0864},
    /* 27 */
    {61.2901, 3.7196, -5.3901},
    {61.4292, 2.2480, -4.9620},
    /* 28 */
    {35.0831, -44.1164, 3.7933},
    {35.0232, -40.0716, 1.5901},
    /* 29 */
    {22.7233, 20.0904, -46.6940},
    {23.0331, 14.9730, -42.5619},
    /* 30 */
    {36.4612, 47.8580, 18.3852},
    {36.2715, 50.5065, 21.2231},
    /* 31 */
    {90.8027, -2.0831, 1.4410},
    {91.1528, -1.6435, 0.0447},
    /* 32 */
    {90.9257, -0.5406, -0.9208},
    {88.6381, -0.8985, -0.7239},
    /* 33 */
    {6.7747, -0.2908, -2.4247},
    {5.8714, -0.0985, -2.2286},
    /* 34 */
    {2.0776, 0.0795, -1.1350},
    {0.9033, -0.0636, -0.5514}};

static color_val_t ciede2000D[] = {
    2.0425,  2.8615,  3.4412, 1.0000, 1.0000, 1.0000, 2.3669, 2.3669,  7.1792,
    7.1792,  7.2195,  7.2195, 4.8045, 4.8045, 4.7461, 4.3065, 27.1492, 22.8977,
    31.9030, 19.4535, 1.0000, 1.0000, 1.0000, 1.0000, 1.2644, 1.2630,  1.8731,
    1.8645,  2.0373,  1.4146, 1.4441, 1.5381, 0.6377, 0.9082};

@interface T23Kit_ColourTests : XCTestCase

@end

@implementation T23Kit_ColourTests

- (void)setUp {
  [super setUp];
  // Put setup code here. This method is called before the invocation of each
  // test method in the class.
}

- (void)tearDown {
  // Put teardown code here. This method is called after the invocation of each
  // test method in the class.
  [super tearDown];
}

- (void)testUIColor {
  UIColor *blueTestColor = [UIColor blueColor];
  UIColor *redTestColor = [UIColor redColor];

  NSLog(@"Distance between red and blue is: %f",
        [blueTestColor
            getDistanceMetricBetweenColor:redTestColor
                              withOptions:T23UIColorDistanceFormulaCEI76]);

  NSLog(@"Distance between red and red is: %f",
        [blueTestColor
            getDistanceMetricBetweenColor:blueTestColor
                              withOptions:T23UIColorDistanceFormulaCEI76]);

  NSLog(@"Distance between blue and blue is: %f",
        [redTestColor
            getDistanceMetricBetweenColor:redTestColor
                              withOptions:T23UIColorDistanceFormulaCEI76]);
}

- (void)testLibColorspaces {

  CGFloat distance = 0.0f;

  pixel_t *lab_1 = NULL, *lab_2 = NULL;
  size_t test_length = sizeof(ciede2000D) / sizeof(ciede2000D[0]);

  NSLog(@"Running %lu tests.", test_length);

  for (size_t i = 0; i < test_length; i++) {
    lab_1 = &LAB_TEST_SET[i * 2];
    lab_2 = &LAB_TEST_SET[(i * 2) + 1];

    NSLog(@"Tests: %lu", i);

    CIEDE_2000(*lab_1, *lab_2, 1.0f, 1.0f, 1.0f, &distance);

    XCTAssertEqualWithAccuracy(distance, ciede2000D[i], 0.0001,
                               @"Failed test: %lu", i);

    NSLog(@"CIEDE200 Distance: %.7g", distance);

    CIE_94(*lab_1, *lab_2, LIBCOLORSPACES_GRAPHICS, &distance);

    NSLog(@"CIE_94_GRAPHICS Distance: %.7g", distance);

    CIE_94(*lab_1, *lab_2, LIBCOLORSPACES_TEXTILES, &distance);

    NSLog(@"CIE_94_TEXTILES Distance: %.7g", distance);

    CMC_1984(*lab_1, *lab_2, 1.0f, 1.0f, &distance);

    NSLog(@"CMC_1984_1_1 Distance: %.7g", distance);

    CMC_1984(*lab_1, *lab_2, 2.0f, 1.0f, &distance);

    NSLog(@"CMC_1984_2_1 Distance: %.7g", distance);

    CIE_76(*lab_1, *lab_2, &distance);

    NSLog(@"CIE_76 Distance: %.7g", distance);
  }
}

@end
