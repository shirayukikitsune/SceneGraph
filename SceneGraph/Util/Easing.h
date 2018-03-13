#pragma once

#include <cmath>

#include "Constants.h"

// From https://github.com/warrenm/AHEasing/blob/master/AHEasing/easing.c

namespace kitsune {
namespace scenegraph {
namespace util {

    enum struct EasingFunction
    {
        ///! Modeled after the line y = x
        Linear,

        ///! Modeled after the parabola y = x^2
        QuadraticIn,
        ///! Modeled after the parabola y = -x^2 + 2x
        QuadraticOut,
        ///! Modeled after the piecewise quadratic
        /// y = (1/2)((2x)^2)             ; [0, 0.5)
        /// y = -(1/2)((2x-1)*(2x-3) - 1) ; [0.5, 1]
        Quadratic,

        ///! Modeled after the cubic y = x^3
        CubicIn,
        ///! Modeled after the cubic y = (x - 1)^3 + 1
        CubicOut,
        ///! Modeled after the piecewise cubic
        /// y = (1/2)((2x)^3)       ; [0, 0.5)
        /// y = (1/2)((2x-2)^3 + 2) ; [0.5, 1]
        Cubic,

        ///! Modeled after the quartic x^4
        QuarticIn,
        ///! Modeled after the quartic y = 1 - (x - 1)^4
        QuarticOut,
        ///! Modeled after the piecewise quartic
        /// y = (1/2)((2x)^4)        ; [0, 0.5)
        /// y = -(1/2)((2x-2)^4 - 2) ; [0.5, 1]
        Quartic,

        ///! Modeled after the quintic y = x^5
        QuinticIn,
        ///! Modeled after the quintic y = (x - 1)^5 + 1
        QuinticOut,
        ///! Modeled after the piecewise quintic
        /// y = (1/2)((2x)^5)       ; [0, 0.5)
        /// y = (1/2)((2x-2)^5 + 2) ; [0.5, 1]
        Quintic,

        ///! Modeled after quarter-cycle of sine wave
        SineIn,
        ///! Modeled after quarter-cycle of sine wave (different phase)
        SineOut,
        ///! Modeled after half sine wave
        Sine,

        ///! Modeled after shifted quadrant IV of unit circle
        CircularIn,
        ///! Modeled after shifted quadrant II of unit circle
        CircularOut,
        ///! Modeled after the piecewise circular function
        /// y = (1/2)(1 - sqrt(1 - 4x^2))           ; [0, 0.5)
        /// y = (1/2)(sqrt(-(2x - 3)*(2x - 1)) + 1) ; [0.5, 1]
        Circular,

        ///! Modeled after the exponential function y = 2^(10(x - 1))
        ExponentialIn,
        ///! Modeled after the exponential function y = -2^(-10x) + 1
        ExponentialOut,
        ///! Modeled after the piecewise exponential
        /// y = (1/2)2^(10(2x - 1))         ; [0,0.5)
        /// y = -(1/2)*2^(-10(2x - 1))) + 1 ; [0.5,1]
        Exponential,

        ///! Modeled after the damped sine wave y = sin(13pi/2*x)*pow(2, 10 * (x - 1))
        ElasticIn,
        ///! Modeled after the damped sine wave y = sin(-13pi/2*(x + 1))*pow(2, -10x) + 1
        ElasticOut,
        ///! Modeled after the piecewise exponentially-damped sine wave:
        /// y = (1/2)*sin(13pi/2*(2*x))*pow(2, 10 * ((2*x) - 1))      ; [0,0.5)
        /// y = (1/2)*(sin(-13pi/2*((2x-1)+1))*pow(2,-10(2*x-1)) + 2) ; [0.5, 1]
        Elastic,

        ///! Modeled after the overshooting cubic y = x^3-x*sin(x*pi)
        BackIn,
        ///! Modeled after overshooting cubic y = 1-((1-x)^3-(1-x)*sin((1-x)*pi))
        BackOut,
        ///! Modeled after the piecewise overshooting cubic function:
        /// y = (1/2)*((2x)^3-(2x)*sin(2*x*pi))           ; [0, 0.5)
        /// y = (1/2)*(1-((1-x)^3-(1-x)*sin((1-x)*pi))+1) ; [0.5, 1]
        Back,

        BounceIn,
        BounceOut,
        Bounce
    };

    template <EasingFunction Function>
    class Easing
    {
    public:
        float operator()(float p) const { return 0.0f; }
    };

    template <>
    class Easing<EasingFunction::Linear>
    {
    public:
        float operator()(float p) const { return p; }
    };

    template <>
    class Easing<EasingFunction::QuadraticIn>
    {
    public:
        float operator()(float p) const { return p * p; }
    };

    template <>
    class Easing<EasingFunction::QuadraticOut>
    {
    public:
        float operator()(float p) const { return -(p * (p - 2.0f));; }
    };

    template <>
    class Easing<EasingFunction::Quadratic>
    {
    public:
        float operator()(float p) const
        {
            if (p < 0.5f) {
                return 2.0f * p * p;
            } else {
                return (-2.0f * p * p) + (4.0f * p) - 1;
            }
        }
    };

    template <>
    class Easing<EasingFunction::CubicIn>
    {
    public:
        float operator()(float p) const { return p * p * p; }
    };

    template <>
    class Easing<EasingFunction::CubicOut>
    {
    public:
        float operator()(float p) const
        {
            float f = p - 1.0f;
            return f * f * f + 1.0f;
        }
    };

    template <>
    class Easing<EasingFunction::Cubic>
    {
    public:
        float operator()(float p) const
        {
            if (p < 0.5f) {
                return 4.0f * p * p * p;
            } else {
                float f = ((2.0f * p) - 2.0f);
                return 0.5f * f * f * f + 1.0f;
            }
        }
    };

    template <>
    class Easing<EasingFunction::QuarticIn>
    {
    public:
        float operator()(float p) const { return p * p * p * p; }
    };

    template <>
    class Easing<EasingFunction::QuarticOut>
    {
    public:
        float operator()(float p) const
        {
            float f = p - 1.0f;
            return f * f * f * (1.0f - p) + 1.0f;
        }
    };

    template <>
    class Easing<EasingFunction::Quartic>
    {
    public:
        float operator()(float p) const
        {
            if (p < 0.5f) {
                return 8.0f * p * p * p * p;
            } else {
                float f = p - 1.0f;
                return -8.0f * f * f * f * f + 1.0f;
            }
        }
    };

    template <>
    class Easing<EasingFunction::QuinticIn>
    {
    public:
        float operator()(float p) const { return p * p * p * p * p; }
    };

    template <>
    class Easing<EasingFunction::QuinticOut>
    {
    public:
        float operator()(float p) const
        {
            float f = p - 1.0f;
            return f * f * f * f * f + 1.0f;
        }
    };

    template <>
    class Easing<EasingFunction::Quintic>
    {
    public:
        float operator()(float p) const
        {
            if (p < 0.5f) {
                return 16.0f * p * p * p * p * p;
            } else {
                float f = ((2.0f * p) - 2.0f);
                return  0.5f * f * f * f * f * f + 1.0f;
            }
        }
    };

    template <>
    class Easing<EasingFunction::SineIn>
    {
    public:
        float operator()(float p) const
        {
            return sinf((p - 1.0f) * PI_2) + 1.0f;
        }
    };

    template <>
    class Easing<EasingFunction::SineOut>
    {
    public:
        float operator()(float p) const
        {
            return sinf(p * PI_2);
        }
    };

    template <>
    class Easing<EasingFunction::Sine>
    {
    public:
        float operator()(float p) const
        {
            return 0.5f * (1.0f - cosf(p * PI));
        }
    };

    template <>
    class Easing<EasingFunction::CircularIn>
    {
    public:
        float operator()(float p) const
        {
            return 1.0f - sqrtf(1.0f - (p * p));
        }
    };

    template <>
    class Easing<EasingFunction::CircularOut>
    {
    public:
        float operator()(float p) const
        {
            return sqrtf((2.0f - p) * p);
        }
    };

    template <>
    class Easing<EasingFunction::Circular>
    {
    public:
        float operator()(float p) const
        {
            if (p < 0.5f)
            {
                return 0.5f * (1.0f - sqrtf(1.0f - 4.0f * (p * p)));
            }
            else
            {
                return 0.5f * (sqrtf(-((2.0f * p) - 3.0f) * ((2.0f * p) - 1.0f)) + 1.0f);
            }
        }
    };

    template <>
    class Easing<EasingFunction::ExponentialIn>
    {
    public:
        float operator()(float p) const
        {
            return (p == 0.0f) ? p : powf(2.0f, 10.0f * (p - 1.0f));
        }
    };

    template <>
    class Easing<EasingFunction::ExponentialOut>
    {
    public:
        float operator()(float p) const
        {
            return (p == 1.0f) ? p : 1.0f - pow(2.0f, -10.0f * p);
        }
    };

    template <>
    class Easing<EasingFunction::Exponential>
    {
    public:
        float operator()(float p) const
        {
            if (p == 0.0f || p == 1.0f) {
                return p;
            }

            if(p < 0.5f)
            {
                return 0.5f * pow(2.0f, (20.0f * p) - 10.0f);
            }
            else
            {
                return -0.5f * pow(2.0f, (-20.0f * p) + 10.0f) + 1.0f;
            }
        }
    };

    template <>
    class Easing<EasingFunction::ElasticIn>
    {
    public:
        float operator()(float p) const
        {
            return sinf(13.0f * PI_2 * p) * powf(2.0f, 10.0f * (p - 1.0f));
        }
    };

    template <>
    class Easing<EasingFunction::ElasticOut>
    {
    public:
        float operator()(float p) const
        {
            return sinf(-13.0f * PI_2 * (p + 1.0f)) * powf(2.0f, -10.0f * p) + 1.0f;
        }
    };

    template <>
    class Easing<EasingFunction::Elastic>
    {
    public:
        float operator()(float p) const
        {
            if (p < 0.5f)
            {
                return 0.5f * sinf(13.0f * PI_2 * (2.0f * p)) * powf(2.0f, 10.0f * ((2.0f * p) - 1.0f));
            }
            else
            {
                return 0.5f * (sinf(-13.0f * PI_2 * ((2.0f * p - 1.0f) + 1.0f)) * powf(2.0f, -10.0f * (2.0f * p - 1.0f)) + 2.0f);
            }
        }
    };

    template <>
    class Easing<EasingFunction::BackIn>
    {
    public:
        float operator()(float p) const
        {
            return p * p * p - p * sinf(p * PI);
        }
    };

    template <>
    class Easing<EasingFunction::BackOut>
    {
    public:
        float operator()(float p) const
        {
            float f = 1.0f - p;
            return 1.0f - (f * f * f - f * sinf(f * PI));
        }
    };

    template <>
    class Easing<EasingFunction::Back>
    {
    public:
        float operator()(float p) const
        {
            if (p < 0.5f)
            {
                float f = 2.0f * p;
                return 0.5f * (f * f * f - f * sinf(f * PI));
            }
            else
            {
                float f = 1.0f - (2.0f * p - 1);
                return 0.5f * (1.0f - (f * f * f - f * sinf(f * PI))) + 0.5f;
            }
        }
    };

    template <>
    class Easing<EasingFunction::BounceOut>
    {
    public:
        float operator()(float p) const
        {
            if (p < 4.0f / 11.0f)
            {
                return 121.0f * p * p / 16.0f;
            }
            else if (p < 8.0f / 11.0f)
            {
                return (363.0f / 40.0f * p * p) - (99.0f / 10.0f * p) + 17.0f / 5.0f;
            }
            else if (p < 9.0f / 10.0f)
            {
                return (4356.0f / 361.0f * p * p) - (35442.0f / 1805.0f * p) + 16061.0f / 1805.0f;
            }
            else
            {
                return (54.0f / 5.0f * p * p) - (513.0f / 25.0f * p) + 268.0f / 25.0f;
            }
        }
    };

    template <>
    class Easing<EasingFunction::BounceIn>
    {
    private:
        static Easing<EasingFunction::BounceOut> BounceOutEasing;

    public:
        float operator()(float p) const
        {
            return 1.0f - BounceOutEasing(1.0f - p);
        }
    };

    template <>
    class Easing<EasingFunction::Bounce>
    {
    private:
        static Easing<EasingFunction::BounceIn> BounceInEasing;
        static Easing<EasingFunction::BounceOut> BounceOutEasing;

    public:
        float operator()(float p) const
        {
            if (p < 0.5f)
            {
                return 0.5f * BounceInEasing(p * 2.0f);
            }
            else
            {
                return 0.5f * BounceOutEasing(p * 2.0f - 1.0f) + 0.5f;
            }
        }
    };

}
}
}
