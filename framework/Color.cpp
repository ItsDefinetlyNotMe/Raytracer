#include "Color.hpp"
bool operator==(Color const& c1, Color const& c2){
return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
}
