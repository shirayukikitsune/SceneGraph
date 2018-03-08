#include "Easing.h"

using namespace kitsune::scenegraph::util;

Easing<EasingFunction::BounceOut> Easing<EasingFunction::BounceIn>::BounceOutEasing;
Easing<EasingFunction::BounceIn> Easing<EasingFunction::Bounce>::BounceInEasing;
Easing<EasingFunction::BounceOut> Easing<EasingFunction::Bounce>::BounceOutEasing;
