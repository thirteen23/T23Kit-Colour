/*
 *  T23Kit_ColourTests.m
 *  T23Kit-Colour
 *
 *  Created by Michael Van Milligan on 4/11/14.
 *  Copyright (c) 2014 Thirteen23. All rights reserved.
 */

#include "colourspaces.h"
#import <XCTest/XCTest.h>
#import "UIColor+T23ColourSpaces.h"

#define CGFloatAreEqual(X, Y) (fabs(X - Y) <= 0.0001f)

static pixel_t RGB_TEST[] = {{1.000000, 0.000000, 0.000000, 1.0},
                             {0.000000, 1.000000, 0.000000, 1.0},
                             {0.000000, 0.000000, 1.000000, 1.0},
                             {1.000000, 1.000000, 0.000000, 1.0},
                             {0.000000, 1.000000, 1.000000, 1.0},
                             {1.000000, 0.000000, 1.000000, 1.0},
                             {0.933333, 0.509804, 0.933333, 1.0},
                             {0.815686, 0.125490, 0.564706, 1.0},
                             {0.196078, 0.803922, 0.196078, 1.0},
                             {0.419608, 0.556863, 0.137255, 1.0},
                             {0.878431, 1.000000, 1.000000, 1.0},
                             {0.254902, 0.411765, 0.882353, 1.0}};

static pixel_t LAB_TEST[] = {{53.2408, 80.0925, 67.2032, 1.0},
                             {87.7347, -86.1827, 83.1793, 1.0},
                             {32.2970, 79.1875, -107.8602, 1.0},
                             {97.1393, -21.5537, 94.4780, 1.0},
                             {91.1132, -48.0875, -14.1312, 1.0},
                             {60.3242, 98.2343, -60.8249, 1.0},
                             {69.6958, 56.3566, -36.8098, 1.0},
                             {47.5728, 72.2307, -17.4159, 1.0},
                             {72.6067, -67.1256, 61.4373, 1.0},
                             {54.6505, -28.2218, 49.6907, 1.0},
                             {97.8674, -9.9445, -3.3751, 1.0},
                             {47.8301, 26.2631, -65.2636, 1.0}};

static pixel_t LUV_TEST[] = {{53.2408, 175.0151, 37.7564, 1.0},
                             {87.7347, -83.0776, 107.3986, 1.0},
                             {32.2970, -9.4054, -130.3423, 1.0},
                             {97.1393, 7.7056, 106.7866, 1.0},
                             {91.1132, -70.4773, -15.2042, 1.0},
                             {60.3242, 84.0714, -108.6833, 1.0},
                             {69.6958, 51.8485, -67.0272, 1.0},
                             {47.5728, 93.2568, -36.6593, 1.0},
                             {72.6067, -62.9268, 81.3486, 1.0},
                             {54.6505, -17.4439, 57.4400, 1.0},
                             {97.8674, -16.3973, -3.5374, 1.0},
                             {47.8301, -17.5102, -101.2174, 1.0}};

static pixel_t LCH_AB_TEST[] = {{53.2408, 104.5518, 0.1111, 1.0},
                                {87.7347, 119.7759, 0.3778, 1.0},
                                {32.2970, 133.8076, 0.8508, 1.0},
                                {97.1393, 96.9054, 0.2857, 1.0},
                                {91.1132, 50.1209, 0.5455, 1.0},
                                {60.3242, 115.5407, 0.9118, 1.0},
                                {69.6958, 67.3129, 0.9079, 1.0},
                                {47.5728, 74.3006, 0.9623, 1.0},
                                {72.6067, 90.9966, 0.3820, 1.0},
                                {54.6505, 57.1458, 0.3322, 1.0},
                                {97.8674, 10.5016, 0.5521, 1.0},
                                {47.8301, 70.3498, 0.8109, 1.0}};

static pixel_t LCH_UV_TEST[] = {{53.2408, 179.0414, 0.0338, 1.0},
                                {87.7347, 135.7804, 0.3548, 1.0},
                                {32.2970, 130.6812, 0.7385, 1.0},
                                {97.1393, 107.0643, 0.2385, 1.0},
                                {91.1132, 72.0987, 0.5338, 1.0},
                                {60.3242, 137.4048, 0.8548, 1.0},
                                {69.6958, 84.7403, 0.8548, 1.0},
                                {47.5728, 100.2035, 0.9404, 1.0},
                                {72.6067, 102.8464, 0.3548, 1.0},
                                {54.6505, 60.0304, 0.2969, 1.0},
                                {97.8674, 16.7746, 0.5338, 1.0},
                                {47.8301, 102.7208, 0.7227, 1.0}};

static pixel_t HLAB_TEST[] = {{46.116470, 78.942963, 29.795856, 1.0},
                              {84.566666, -72.515923, 50.840080, 1.0},
                              {26.865406, 72.872406, -190.919449, 1.0},
                              {96.323685, -25.869644, 58.899899, 1.0},
                              {88.731461, -47.048512, -9.351272, 1.0},
                              {53.371143, 104.894089, -70.357269, 1.0},
                              {63.496826, 53.822945, -35.447365, 1.0},
                              {40.571369, 67.874077, -12.365271, 1.0},
                              {66.759521, -53.404575, 37.522484, 1.0},
                              {47.532005, -22.945438, 26.903835, 1.0},
                              {97.255035, -15.022249, 1.999814, 1.0},
                              {40.817955, 19.604727, -77.954865, 1.0}};

static pixel_t XYZ_TEST[] = {{41.2456, 21.2673, 1.9334, 1.0},
                             {35.7576, 71.5152, 11.9192, 1.0},
                             {18.0437, 7.2175, 95.0304, 1.0},
                             {77.0033, 92.7825, 13.8526, 1.0},
                             {53.8014, 78.7327, 106.9496, 1.0},
                             {59.2894, 28.4848, 96.9638, 1.0},
                             {58.6741, 40.3185, 85.5640, 1.0},
                             {31.5647, 16.4604, 27.8951, 1.0},
                             {23.7210, 44.5683, 10.3694, 1.0},
                             {16.0399, 22.5929, 5.1056, 1.0},
                             {84.5460, 94.5854, 108.3908, 1.0},
                             {20.8174, 16.6611, 73.3384, 1.0}};

static pixel_t CMYK_TEST[] = {{0.000000, 1.000000, 1.000000, 0.000000},
                              {1.000000, 0.000000, 1.000000, 0.000000},
                              {1.000000, 1.000000, 0.000000, 0.000000},
                              {0.000000, 0.000000, 1.000000, 0.000000},
                              {1.000000, 0.000000, 0.000000, 0.000000},
                              {0.000000, 1.000000, 0.000000, 0.000000},
                              {0.000000, 0.453781, 0.000000, 0.066667},
                              {0.000000, 0.846154, 0.307692, 0.184314},
                              {0.756098, 0.000000, 0.756098, 0.196078},
                              {0.246479, 0.000000, 0.753521, 0.443137},
                              {0.121569, 0.000000, 0.000000, 0.000000},
                              {0.711111, 0.533333, 0.000000, 0.117647}};

static pixel_t HSL_TEST[] = {{0.000000, 1.000000, 0.500000, 1.0},
                             {0.333333, 1.000000, 0.500000, 1.0},
                             {0.666667, 1.000000, 0.500000, 1.0},
                             {0.166667, 1.000000, 0.500000, 1.0},
                             {0.500000, 1.000000, 0.500000, 1.0},
                             {0.833333, 1.000000, 0.500000, 1.0},
                             {0.833333, 0.760562, 0.721568, 1.0},
                             {0.893939, 0.733334, 0.470588, 1.0},
                             {0.333333, 0.607844, 0.500000, 1.0},
                             {0.221184, 0.604520, 0.347059, 1.0},
                             {0.500000, 1.000000, 0.939216, 1.0},
                             {0.625000, 0.727273, 0.568627, 1.0}};

static pixel_t HSI_TEST[] = {{0.000000, 1.000000, 0.333333, 1.0},
                             {0.333333, 1.000000, 0.333333, 1.0},
                             {0.666667, 1.000000, 0.333333, 1.0},
                             {0.166667, 1.000000, 0.666667, 1.0},
                             {0.500000, 1.000000, 0.666667, 1.0},
                             {0.833333, 1.000000, 0.666667, 1.0},
                             {0.833333, 0.356435, 0.792157, 1.0},
                             {0.893939, 0.750000, 0.501961, 1.0},
                             {0.333333, 0.508198, 0.398693, 1.0},
                             {0.221184, 0.630282, 0.371242, 1.0},
                             {0.500000, 0.084469, 0.959477, 1.0},
                             {0.625000, 0.506329, 0.516340, 1.0}};

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

static colour_val_t cie1976D[] = {
    4.001063, 6.314150, 9.177700,  2.062701,  2.369571,  2.915293,  2.236068,
    2.236068, 4.980000, 4.980000,  4.980000,  4.980000,  4.980000,  4.980000,
    4.980000, 3.535534, 36.868008, 31.910030, 30.253099, 27.408940, 0.892429,
    0.797200, 0.858307, 0.829825,  3.181924,  2.213343,  1.538904,  4.606309,
    6.584680, 3.886414, 1.505148,  2.323785,  0.944132,  1.319108};

static colour_val_t cie_graphics_1994D[] = {
    1.395039, 1.934101, 2.454336,  0.684492,  0.669563,  0.691945,  2.236068,
    2.031638, 4.800694, 4.800694,  4.800694,  4.800695,  4.800694,  4.800694,
    4.800694, 3.407744, 34.689163, 29.441373, 27.914088, 24.937661, 0.822132,
    0.716584, 0.804875, 0.752844,  1.390995,  1.248089,  1.297958,  1.820451,
    2.556133, 1.424913, 1.419453,  2.322569,  0.938533,  1.306545};

static colour_val_t cie_textiles_1994D[] = {
    1.423046, 1.942673, 2.406600,  0.697952,  0.671851,  0.677197,  2.236068,
    2.019331, 4.812245, 4.812246,  4.812246,  4.812246,  4.812245,  4.812246,
    4.812246, 3.415975, 28.250263, 27.730808, 27.328584, 23.807638, 0.819391,
    0.711786, 0.804063, 0.748772,  1.389733,  1.244090,  1.288424,  1.795793,
    2.530989, 1.399092, 1.385811,  1.212342,  0.518211,  0.819075};

static colour_val_t ciede2000D[] = {
    2.0425,  2.8615,  3.4412, 1.0000, 1.0000, 1.0000, 2.3669, 2.3669,  7.1792,
    7.1792,  7.2195,  7.2195, 4.8045, 4.8045, 4.7461, 4.3065, 27.1492, 22.8977,
    31.9030, 19.4535, 1.0000, 1.0000, 1.0000, 1.0000, 1.2644, 1.2630,  1.8731,
    1.8645,  2.0373,  1.4146, 1.4441, 1.5381, 0.6377, 0.9082};

static colour_val_t cmc_1_1_1984D[] = {
    1.738736, 2.496608, 3.304945,  0.857356,  0.883329,  0.978227,  3.504809,
    2.879300, 6.578381, 6.578381,  6.578381,  6.578381,  6.674923,  6.674923,
    6.674923, 4.668530, 42.108755, 39.458891, 38.360059, 33.936638, 1.144002,
    1.006004, 1.113025, 1.053353,  1.428230,  1.254814,  1.768381,  2.025834,
    3.086955, 1.748935, 1.900952,  1.702581,  1.803172,  2.449344};

static colour_val_t cmc_2_1_1984D[] = {
    1.738736, 2.496608, 3.304945,  0.857356,  0.883329,  0.978227,  3.504809,
    2.879300, 6.578381, 6.578381,  6.578381,  6.578381,  6.674923,  6.674923,
    6.674923, 4.668530, 37.923276, 38.475767, 38.061769, 33.334208, 1.144002,
    1.006004, 1.113025, 1.053353,  1.420486,  1.247404,  1.765559,  2.024991,
    3.060441, 1.739572, 1.889083,  0.990070,  0.952801,  1.427773};

/* XYZ answers for {0.254902, 0.411765, 0.882353, 1.0} */
static pixel_t rgb_2_xyz_working_space_matrix_test_results[] = {
    /* Adobe RGB (1998) D65 */
    {19.7744, 16.0927, 76.3921},
    /* Apple RGB D65 */
    {24.9717, 22.3474, 76.7122},
    /* Best RGB D50 */
    {15.6745, 14.1944, 62.0708},
    /* Beta RGB D50 */
    {14.7858, 13.4249, 60.1456},
    /* Bruce RGB D65 */
    {20.8118, 16.6247, 76.5752},
    /* CIE RGB E */
    {22.0586, 13.2347, 75.3000},
    /* Color Match RGB D50 */
    {21.5420, 21.0203, 57.6646},
    /* Don RGB D50 */
    {15.4361, 13.7011, 61.2844},
    /* ECI RGB D50 */
    {14.9707, 14.3113, 55.7638},
    /* Ekta Space RGB D50 */
    {14.2033, 12.0592, 60.0656},
    /* NTSC RGB C */
    {20.6759, 18.4990, 85.6933},
    /* PAL/SECAM RGB D65 */
    {20.5169, 16.5461, 73.2445},
    /* ProPhoto RGB D50 */
    {12.0525, 16.8804, 65.8750},
    /* SMPTE-C RGB D65 */
    {21.6820, 17.5747, 74.4346},
    /* sRGB D65 */
    {20.8174, 16.6611, 73.3384},
    /* Wide Gamut RGB D50 */
    {16.1488, 12.8503, 59.4616},
    /* Adobe RGB (1998) D50 */
    {17.2588, 15.2214, 57.5164},
    /* Apple RGB D50 */
    {22.8318, 21.5646, 57.8032},
    /* Bruce RGB D50 */
    {18.3489, 15.7758, 57.6526},
    /* CIE RGB D50 */
    {19.7423, 12.6179, 61.6989},
    /* NTSC RGB D50 */
    {16.7474, 17.3362, 59.3127},
    /* PAL/SECAM RGB D50 */
    {18.2050, 15.7460, 55.1490},
    /* SMPTE-C RGB D50 */
    {19.3897, 16.7790, 56.0488},
    /* sRGB D50 */
    {18.5178, 15.8672, 55.2185}};

static colour_val_t rgb_workspace_options[] = {
    colourspace_rgb_profile_adobe_d65,
    colourspace_rgb_profile_apple_d65,
    colourspace_rgb_profile_best_d50,
    colourspace_rgb_profile_beta_d50,
    colourspace_rgb_profile_bruce_d65,
    colourspace_rgb_profile_cie_e,
    colourspace_rgb_profile_colormatch_d50,
    colourspace_rgb_profile_don_d50,
    colourspace_rgb_profile_eci_d50,
    colourspace_rgb_profile_ekta_space_d50,
    colourspace_rgb_profile_ntsc_c,
    colourspace_rgb_profile_pal_secam_d65,
    colourspace_rgb_profile_prophoto_d50,
    colourspace_rgb_profile_smpte_c_d65,
    colourspace_rgb_profile_srgb_d65,
    colourspace_rgb_profile_wide_gamut_d50,
    colourspace_rgb_profile_adobe_d50,
    colourspace_rgb_profile_apple_d50,
    colourspace_rgb_profile_bruce_d50,
    colourspace_rgb_profile_cie_d50,
    colourspace_rgb_profile_ntsc_d50,
    colourspace_rgb_profile_pal_secam_d50,
    colourspace_rgb_profile_smpte_c_d50,
    colourspace_rgb_profile_srgb_d50};

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
  CGFloat x, y, z, k, a;
  size_t test_length = sizeof(RGB_TEST) / sizeof(RGB_TEST[0]);

  NSLog(@"Running %lu tests...", test_length);

  for (size_t i = 0; i < test_length; i++) {
    UIColor *testColor = [UIColor colorWithRed:RGB_TEST[i].a
                                         green:RGB_TEST[i].b
                                          blue:RGB_TEST[i].c
                                         alpha:RGB_TEST[i].d];

    [testColor getHunterLStar:&x aStar:&y bStar:&z alpha:&a];

    XCTAssertEqualWithAccuracy(x, HLAB_TEST[i].a, 0.001, @"Failed test: %lu",
                               i);
    XCTAssertEqualWithAccuracy(y, HLAB_TEST[i].b, 0.001, @"Failed test: %lu",
                               i);
    XCTAssertEqualWithAccuracy(z, HLAB_TEST[i].c, 0.001, @"Failed test: %lu",
                               i);

    [testColor getLStar:&x aStar:&y bStar:&z alpha:&a];

    XCTAssertEqualWithAccuracy(x, LAB_TEST[i].a, 0.001, @"Failed test: %lu", i);
    XCTAssertEqualWithAccuracy(y, LAB_TEST[i].b, 0.001, @"Failed test: %lu", i);
    XCTAssertEqualWithAccuracy(z, LAB_TEST[i].c, 0.001, @"Failed test: %lu", i);

    [testColor getLStar:&x uStar:&y vStar:&z alpha:&a];

    XCTAssertEqualWithAccuracy(x, LUV_TEST[i].a, 0.001, @"Failed test: %lu", i);
    XCTAssertEqualWithAccuracy(y, LUV_TEST[i].b, 0.001, @"Failed test: %lu", i);
    XCTAssertEqualWithAccuracy(z, LUV_TEST[i].c, 0.001, @"Failed test: %lu", i);

    [testColor getLStar:&x cStar:&y hStarAB:&z alpha:&a];

    XCTAssertEqualWithAccuracy(x, LCH_AB_TEST[i].a, 0.001, @"Failed test: %lu", i);
    XCTAssertEqualWithAccuracy(y, LCH_AB_TEST[i].b, 0.001, @"Failed test: %lu", i);
    XCTAssertEqualWithAccuracy(z, LCH_AB_TEST[i].c, 0.001, @"Failed test: %lu", i);

    [testColor getLStar:&x cStar:&y hStarUV:&z alpha:&a];

    XCTAssertEqualWithAccuracy(x, LCH_UV_TEST[i].a, 0.001, @"Failed test: %lu", i);
    XCTAssertEqualWithAccuracy(y, LCH_UV_TEST[i].b, 0.001, @"Failed test: %lu", i);
    XCTAssertEqualWithAccuracy(z, LCH_UV_TEST[i].c, 0.001, @"Failed test: %lu", i);

    [testColor getCyan:&x magenta:&y yellow:&z black:&k alpha:&a];

    XCTAssertEqualWithAccuracy(x, CMYK_TEST[i].a, 0.001, @"Failed test: %lu", i);
    XCTAssertEqualWithAccuracy(y, CMYK_TEST[i].b, 0.001, @"Failed test: %lu", i);
    XCTAssertEqualWithAccuracy(z, CMYK_TEST[i].c, 0.001, @"Failed test: %lu", i);
    XCTAssertEqualWithAccuracy(k, CMYK_TEST[i].d, 0.001, @"Failed test: %lu", i);

    [testColor getX:&x Y:&y Z:&z alpha:&a];

    XCTAssertEqualWithAccuracy(x, XYZ_TEST[i].a, 0.001, @"Failed test: %lu", i);
    XCTAssertEqualWithAccuracy(y, XYZ_TEST[i].b, 0.001, @"Failed test: %lu", i);
    XCTAssertEqualWithAccuracy(z, XYZ_TEST[i].c, 0.001, @"Failed test: %lu", i);

    [testColor getHue:&x saturation:&y lightness:&z alpha:&a];

    XCTAssertEqualWithAccuracy(x, HSL_TEST[i].a, 0.001, @"Failed test: %lu", i);
    XCTAssertEqualWithAccuracy(y, HSL_TEST[i].b, 0.001, @"Failed test: %lu", i);
    XCTAssertEqualWithAccuracy(z, HSL_TEST[i].c, 0.001, @"Failed test: %lu", i);

    [testColor getHue:&x saturation:&y intensity:&z alpha:&a];

    XCTAssertEqualWithAccuracy(x, HSI_TEST[i].a, 0.001, @"Failed test: %lu", i);
    XCTAssertEqualWithAccuracy(y, HSI_TEST[i].b, 0.001, @"Failed test: %lu", i);
    XCTAssertEqualWithAccuracy(z, HSI_TEST[i].c, 0.001, @"Failed test: %lu", i);
  }
}

- (void)testComplements {
  UIColor *redComplement = [[UIColor redColor] getComplement];
  UIColor *greenComplement = [[UIColor greenColor] getComplement];
  UIColor *blueComplement = [[UIColor blueColor] getComplement];
  UIColor *cyanComplement = [[UIColor cyanColor] getComplement];
  UIColor *yellowComplement = [[UIColor yellowColor] getComplement];
  UIColor *magentaComplement = [[UIColor magentaColor] getComplement];
  UIColor *orangeComplement = [[UIColor orangeColor] getComplement];
  UIColor *purpleComplement = [[UIColor purpleColor] getComplement];
  UIColor *brownComplement = [[UIColor brownColor] getComplement];

  NSLog(@"Red complement: %@", redComplement);
  NSLog(@"Green complement: %@", greenComplement);
  NSLog(@"Blue complement: %@", blueComplement);
  NSLog(@"Cyan complement: %@", cyanComplement);
  NSLog(@"Yellow complement: %@", yellowComplement);
  NSLog(@"Magenta complement: %@", magentaComplement);
  NSLog(@"Orange complement: %@", orangeComplement);
  NSLog(@"Purple complement: %@", purpleComplement);
  NSLog(@"Brown complement: %@", brownComplement);
}

- (void)testLibColorspaces {
  colour_val_t distance = 0.0f;

  pixel_t *lab_1 = NULL, *lab_2 = NULL;
  size_t test_length = sizeof(ciede2000D) / sizeof(ciede2000D[0]);

  NSLog(@"Running %lu tests...", test_length);

  for (size_t i = 0; i < test_length; i++) {
    lab_1 = &LAB_TEST_SET[i * 2];
    lab_2 = &LAB_TEST_SET[(i * 2) + 1];

    CIEDE_2000(*lab_1, *lab_2, 1.0f, 1.0f, 1.0f, &distance);

    XCTAssertEqualWithAccuracy(distance, ciede2000D[i], 0.0001,
                               @"Failed test: %lu", i);

    CIE_94(*lab_1, *lab_2, LIBCOLORSPACES_GRAPHICS, &distance);

    XCTAssertEqualWithAccuracy(distance, cie_graphics_1994D[i], 0.0001,
                               @"Failed test: %lu", i);

    CIE_94(*lab_1, *lab_2, LIBCOLORSPACES_TEXTILES, &distance);

    XCTAssertEqualWithAccuracy(distance, cie_textiles_1994D[i], 0.0001,
                               @"Failed test: %lu", i);

    CMC_1984(*lab_1, *lab_2, 1.0f, 1.0f, &distance);

    XCTAssertEqualWithAccuracy(distance, cmc_1_1_1984D[i], 0.0001,
                               @"Failed test: %lu", i);

    CMC_1984(*lab_1, *lab_2, 2.0f, 1.0f, &distance);

    XCTAssertEqualWithAccuracy(distance, cmc_2_1_1984D[i], 0.0001,
                               @"Failed test: %lu", i);

    CIE_76(*lab_1, *lab_2, &distance);

    XCTAssertEqualWithAccuracy(distance, cie1976D[i], 0.0001,
                               @"Failed test: %lu", i);
  }
}

- (void)testRGBWorkSpaceMatrices {
  pixel_t rgb = {0.254902f, 0.411765f, 0.882353f, 1.0f}, xyz;
  size_t test_length = sizeof(rgb_2_xyz_working_space_matrix_test_results) /
                       sizeof(rgb_2_xyz_working_space_matrix_test_results[0]);

  for (size_t i = 0; i < test_length; i++) {
    RGB_2_XYZ(rgb, &xyz, rgb_workspace_options[i]);

    XCTAssertEqualWithAccuracy(xyz.a,
                               rgb_2_xyz_working_space_matrix_test_results[i].a,
                               0.0001, @"Failed test: %lu", i);

    XCTAssertEqualWithAccuracy(xyz.b,
                               rgb_2_xyz_working_space_matrix_test_results[i].b,
                               0.0001, @"Failed test: %lu", i);

    XCTAssertEqualWithAccuracy(xyz.c,
                               rgb_2_xyz_working_space_matrix_test_results[i].c,
                               0.0001, @"Failed test: %lu", i);
  }
}

@end
