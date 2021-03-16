#pragma once

namespace lib
{
namespace core
{

template<typename T>
struct Size
{
    Size() : w(0), h(0) { }
    Size(T wi, T hi) : w(wi), h(hi) { }

    T w = 0;
    T h = 0;
};

using Sized = Size<int>;
using Sizef = Size<float>;

} // namespace core
} // namespace lib
