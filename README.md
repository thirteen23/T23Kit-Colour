#T23Kit-Colour
UIColor Category with various colorspace transformations and ∆-E methods.
_______________
##Discussion

Currently there are three pieces to this library:

1. UIColor conversions to non-standard color spaces
2. UIColor neighbourhood color generations
3. UIColor distance (∆-E) calculations

###Color Conversions

Below are the currently supported colorspace conversions:
*   CIE XYZ
*   Hunter 1948 (L, a, b)
*   CIE 1976 (L\*, a\*, b\*)