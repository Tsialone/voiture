#include <iostream>
#include <cmath>
double roundToDecimals(double value, int decimals) {
    double factor = std::pow(10, decimals);
    return std::round(value * factor) / factor;
}
