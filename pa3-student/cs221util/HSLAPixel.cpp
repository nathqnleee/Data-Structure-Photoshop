/**
 * @file HSLAPixel.cpp
 * Implementation of the HSLAPixel class for use in with the PNG library.
 *
 * @author CS 225: Data Structures
 * @version 2018r1
 */

#include "HSLAPixel.h"
#include <cmath>
#include <iostream>
using namespace std;

#define PI 3.14159265

namespace cs221util {
  HSLAPixel::HSLAPixel() {
    h = 0;
    s = 0;
    l = 1.0;
    a = 1.0;
  }

  HSLAPixel::HSLAPixel(double hue, double saturation, double luminance) {
    h = hue;
    s = saturation;
    l = luminance;
    a = 1.0;
  }

  HSLAPixel::HSLAPixel(double hue, double saturation, double luminance, double alpha) {
    h = hue;
    s = saturation;
    l = luminance;
    a = alpha;
  }

  bool HSLAPixel::operator== (HSLAPixel const & other) const {
    // thank/blame Wade for the following function
    if (fabs(a - other.a) > 0.10) {
        //cout << "a: " << fabs(a - other.a) << endl; return false; }
        return false; }
    if ( a == 0 ) { return true; }

    if (fabs(l - other.l) > 0.10) {
        //cout << "l: " << fabs(l - other.l) << endl; return false; }
        return false; }
    if (l == 0 || l == 1) { return true; }

    if (fabs(s - other.s) > 0.10) {
        //cout << "s: " << fabs(s - other.s) << endl; return false; }
        return false; }
    if (s == 0) { return true; }

    if (fabs(h - other.h) < 5.5 || fabs(h - other.h) > 354.5 ) { return true; }
    else {
        //cout << "h: " << h << "other.h: " << other.h << endl; return false; }
        return false; }
}

  bool HSLAPixel::operator!= (HSLAPixel const & other) const {
    return !(*this == other);
  }

  bool HSLAPixel::operator<  (HSLAPixel const & other) const {
      if (*this == other) { return false; }

      if (l < other.l)
          return true;
      else if (l > other.l)
          return false;
      // == lumininance

      if (s < other.s)
          return true;
      else if (s > other.s)
          return false;
      // == saturation

      if (h < other.h)
          return true;
      else if (h > other.h)
          return false;
      // == hue

      if (a < other.a)
          return true;
      else if (a > other.a)
          return false;
      // == alpha

      // same pixel
      return false;
  }

  // This ignores alpha
  double HSLAPixel::distanceTo(HSLAPixel const & other) const {
      return (sin(h * PI / 180) * s * l - sin(other.h * PI / 180) * other.s * other.l)
          * (sin(h * PI / 180) * s * l - sin(other.h * PI / 180) * other.s * other.l)
          + (cos(h * PI / 180) * s * l - cos(other.h * PI / 180) * other.s * other.l)
          * (cos(h * PI / 180) * s * l - cos(other.h * PI / 180) * other.s * other.l)
          + (l - other.l) * (l - other.l);
  }


   std::ostream & operator<<(std::ostream & out, HSLAPixel const & pixel) {
    out << "(" << pixel.h << ", " << pixel.s << ", " << pixel.l << (pixel.a != 1 ? ", " + std::to_string(pixel.a) : "") << ")";

    return out;
  }


}
