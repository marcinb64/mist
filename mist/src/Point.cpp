#include "Point.h"

using namespace mist;

namespace mist
{

std::ostream &operator<<(std::ostream &os, const Point2i &p)
{
    os << '(' << p.x << ',' << p.y << ')';
    return os;
}

} // namespace mist
