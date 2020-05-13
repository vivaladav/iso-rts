#include "DummyListener.h"

#include "core/Application.h"

#include <cassert>

namespace lib
{
namespace core
{

DummyListener::DummyListener(Application * app) : mApp(app)
{
    assert(app);
}

void DummyListener::OnApplicationQuit()
{
    mApp->Exit();
}

} // namespace core
} // namespace lib
