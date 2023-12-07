#include "ServoHandler.h"

namespace ServoHandler
{
    Servo servo;

    bool setup()
    {
        servo.attach(SERVO_MOTOR_PIN);
        servo.write(SERVO_MOTOR_CLOSED_ANGLE);
        Serial.println("Servo motor setup");
        return true;
    }

    void open()
    {
        servo.write(SERVO_MOTOR_OPEN_ANGLE);
    }

    void close()
    {
        servo.write(SERVO_MOTOR_CLOSED_ANGLE);
    }
} // namespace ServoHandler
