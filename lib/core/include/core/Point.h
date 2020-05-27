#pragma once

namespace lib
{
namespace core
{

template<typename T>
struct Point
{
    Point(T xi, T yi) : x(xi), y(yi) { }

    T x = 0;
    T y = 0;
};

using Point2D = Point<int>;
using Pointf2D = Point<float>;

} // namespace core
} // namespace lib
