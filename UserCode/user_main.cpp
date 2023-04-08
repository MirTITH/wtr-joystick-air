#include "user_main.hpp"
#include "main.h"
#include "cmsis_os.h"

using namespace std;

int TestValue = 0;

__attribute__((section(".itcmram"))) void StartDefaultTask(void const *argument)
{
    (void)argument;
    while (true) {
        TestValue++;
        vTaskDelay(1);
    }
}
