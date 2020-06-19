#pragma once

#include "Renderable.h"

namespace lib
{
namespace graphic
{

class DummyRenderable : public Renderable
{
public:
    void Render() override;
};

} // namespace graphic
} // namespace lib
