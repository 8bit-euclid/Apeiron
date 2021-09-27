#pragma once

namespace Apeiron{

/***************************************************************************************************************************************************************
* Compile-time constants
***************************************************************************************************************************************************************/

#define COMPILE_TIME_CONST(_constant, _value) constexpr Float _constant = _value;

COMPILE_TIME_CONST(Small, Epsilon)
COMPILE_TIME_CONST(TenSmall, 10.0*Small)
COMPILE_TIME_CONST(HundredSmall, 100.0*Small)
COMPILE_TIME_CONST(TenthSmall, Small/10.0)
COMPILE_TIME_CONST(HundrethSmall, Small/100.0)
COMPILE_TIME_CONST(Zero, 0.0)
COMPILE_TIME_CONST(Half, 0.5)
COMPILE_TIME_CONST(Third, 1.0/3.0)
COMPILE_TIME_CONST(Quart, 1.0/4.0)
COMPILE_TIME_CONST(Fifth, 1.0/5.0)
COMPILE_TIME_CONST(Sixth, 1.0/6.0)
COMPILE_TIME_CONST(One, 1.0)
COMPILE_TIME_CONST(Two, 2.0)
COMPILE_TIME_CONST(Three, 3.0)
COMPILE_TIME_CONST(Four, 4.0)
COMPILE_TIME_CONST(Five, 5.0)
COMPILE_TIME_CONST(Pi, 3.14159265358979323846264338327950288419716939937510)
COMPILE_TIME_CONST(TwoPi, Two*Pi)
COMPILE_TIME_CONST(HalfPi, Half*Pi)
COMPILE_TIME_CONST(ThirdPi, Third*Pi)
COMPILE_TIME_CONST(QuartPi, Quart*Pi)
COMPILE_TIME_CONST(SixthPi, Sixth*Pi)
COMPILE_TIME_CONST(e, 2.71828182845904523536028747135266249775724709369995)

/***************************************************************************************************************************************************************
* Run-time constants
***************************************************************************************************************************************************************/

#define RUN_TIME_CONST(_constant, _value) const Float _constant = _value;

RUN_TIME_CONST(Phi, Half*(One + std::sqrt(Five)))

}//Apeiron