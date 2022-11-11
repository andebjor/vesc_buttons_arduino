#include "VescUart.h"

static constexpr int pin_left_hard = 7;
static constexpr int pin_left_soft = 6;
static constexpr int pin_right_soft = 5;
static constexpr int pin_right_hard = 4;

#if 0
static int current = 0;
static bool is_parked = true;
#endif

auto setup_io_pins() -> void
{
    pinMode(pin_left_hard, INPUT_PULLUP);
    pinMode(pin_left_soft, INPUT_PULLUP);
    pinMode(pin_right_soft, INPUT_PULLUP);
    pinMode(pin_right_hard, INPUT_PULLUP);

    pinMode(LED_BUILTIN, OUTPUT);
}

#if 0
auto write_command() -> void
{
    if (current >= 0)
    {
        VescUartSetCurrent(current);
    }
    else
    {
        VescUartSetCurrentBrake(-current);
    }
}

auto park() -> void
{
    is_parked = true;

    current = 0;

    write_command();
    digitalWrite(LED_BUILTIN, LOW);
}
#endif

void setup()
{
    setup_io_pins();

    Serial.begin(115200);
    while (!Serial) {};

#if 0
    park();
#endif
}

#if 0
auto detect_stop() -> bool
{
    return digitalRead(pin_stop);
}

#define detect_stop() \
    if (detect_stop()) { \
        park(); \
        return true; \
    }

auto execute(int duration) -> bool
{
    write_command();
    for (int i=0; i<duration; ++i)
    {
        detect_stop();
        delay(100);
        write_command();
    }

    return false;
}

auto rotate_right(int force, int duration) -> bool
{
    current = force;

    return execute(duration);
}

auto rotate_left(int force, int duration) -> bool
{
    current = -force;

    return execute(duration);
}

auto run_demo() -> bool
{
    digitalWrite(LED_BUILTIN, HIGH);

    delay(500);
    detect_stop();


    if (rotate_right(3, 5*10))
    {
        return true;
    }
    if (rotate_left(3, 5*10))
    {
        return true;
    }
#if 0
    if (rotate_left(1, 20))
    {
        return true;
    }
    if (rotate_right(4, 15))
    {
        return true;
    }
    if (rotate_right(0, 15))
    {
        return true;
    }


    if (rotate_left(4, 5))
    {
        return true;
    }
    if (rotate_left(1, 20))
    {
        return true;
    }
    if (rotate_right(4, 15))
    {
        return true;
    }
    if (rotate_right(0, 15))
    {
        return true;
    }
#endif

    // stop rotation
    park();
    delay(1000);

    digitalWrite(LED_BUILTIN, LOW);
    return false;
}
#endif

auto is_active(int pin) -> bool
{
    return !digitalRead(pin);
}

void loop()
{
    while (1)
    {
        digitalWrite(LED_BUILTIN, LOW);
        delay(10);

        {
            auto left_soft = is_active(pin_left_soft);
            if (left_soft)
            {
                digitalWrite(LED_BUILTIN, HIGH);
                Serial.println("Left soft");
                continue;
            }
        }

        {
            auto right_soft = is_active(pin_right_soft);
            if (right_soft)
            {
                digitalWrite(LED_BUILTIN, HIGH);
                Serial.println("Right soft");
                continue;
            }
        }

        {
            auto left_hard = is_active(pin_left_hard);
            if (left_hard)
            {
                digitalWrite(LED_BUILTIN, HIGH);
                Serial.println("Left hard");
                continue;
            }
        }

        {
            auto right_hard = is_active(pin_right_hard);
            if (right_hard)
            {
                digitalWrite(LED_BUILTIN, HIGH);
                Serial.println("Right hard");
                continue;
            }
        }
    }
}
