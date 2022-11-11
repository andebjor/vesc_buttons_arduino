#include "VescUart.h"

static constexpr int pin_start = A4;
static constexpr int pin_stop = A5;

static int current = 0;
static bool is_parked = true;

auto setup_io_pins() -> void
{
    pinMode(pin_start, INPUT);
    pinMode(pin_stop, INPUT);

    pinMode(LED_BUILTIN, OUTPUT);
}

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

void setup()
{
    setup_io_pins();

    Serial.begin(115200);
    while (!Serial) {};

    park();
}

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

void loop()
{
    while (1)
    {
        delay(10);

        auto do_start = digitalRead(pin_start);

        if (do_start)
        {
            run_demo();
        }
    }
}
