#pragma once

namespace lib
{
namespace core
{

template<typename T>
struct Rect
{
    Rect() : x(0), y(0), w(0), h(0) { }
    Rect(T xi, T yi, T wi, T hi) : x(xi), y(yi), w(wi), h(hi) { }

    T x = 0;
    T y = 0;
    T w = 0;
    T h = 0;
};

using Rectd = Rect<int>;
using Rectf = Rect<float>;

} // namespace core
} // namespace lib
