#include "user_main.hpp"
#include "main.h"
#include "cmsis_os.h"

using namespace std;

int TestValue = 0;

__attribute__((section(".itcmram"))) int TestTask(int value)
{
    value++;
    return value;
}

__attribute__((section(".itcmram"))) void StartDefaultTask(void *argument)
{
    (void)argument;
    while (true) {
        TestValue = TestTask(TestValue);
        vTaskDelay(1);
    }
}
