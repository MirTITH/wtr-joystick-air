#include "user_test.hpp"
#include "cmsis_os.h"

int TestValue = 0;

void TestThreadEntry(void *argument)
{
    (void)argument;

    while (1) {
        TestValue++;
        vTaskDelay(10);
    }
}
