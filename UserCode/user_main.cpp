#include "user_main.hpp"
#include "main.h"
#include "cmsis_os.h"

using namespace std;

void StartDefaultTask(void *argument)
{
    (void)argument;

    while (true) {
        vTaskDelay(1);
    }
}
