//
//  CCInjectMouseMove-emscripten.cpp
//

#include "platform/CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_EMSCRIPTEN)

#include "CCInjectMouseMove-emscripten.h"
#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"


namespace
{
	const float	INTERVAL = 0.125f;
}

NS_CC_BEGIN


InjectMouseMove::InjectMouseMove()
:	Node()
,	_event(EventMouse::MouseEventType::MOUSE_MOVE)
{
	// dirty hack to be able to schedule without having to be added to the scene
	_running = true;
}

InjectMouseMove::~InjectMouseMove()
{
}


void	InjectMouseMove::updatePosition(float designX, float designY)
{
	_event.setCursorPosition(designX, designY);

	// Always unschedule first, as we want the injection to start INTERVAL time after the last actual mouse move
	unscheduleAllCallbacks();

	schedule(CC_SCHEDULE_SELECTOR(InjectMouseMove::_inject), INTERVAL);
}

void	InjectMouseMove::pauseInject()
{
	unscheduleAllCallbacks();
}


void	InjectMouseMove::_inject(float)
{
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&_event);
}

NS_CC_END


#endif // #if (CC_TARGET_PLATFORM == CC_PLATFORM_EMSCRIPTEN)
