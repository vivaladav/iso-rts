#include "core/event/ApplicationEventListener.h"

#include "EventDispatcher.h"

namespace lib
{
namespace core
{

ApplicationEventListener::~ApplicationEventListener()
{
    if(mDispatcher)
        mDispatcher->RemoveApplicationListener(this);
}

void ApplicationEventListener::OnApplicationQuit(ApplicationEvent &) { }

} // namespace core
} // namespace lib
