#pragma once

#include <Servo.h>

#include "macros.h"

namespace ServoHandler
{
    extern Servo servo;

    bool setup();
    void open();
    void close();
} // namespace ServoHandler