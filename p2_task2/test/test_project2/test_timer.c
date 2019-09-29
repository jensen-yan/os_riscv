#include <os/time.h>
#include <test2.h>
#include <os/sched.h>
#include <stdio.h>
#include <sys/syscall.h>

void timer_task(void)
{
    int count = 0;
    int print_location = 2;

    while (1)
    {
        /* call get_timer() to get time */
        uint32_t time = get_timer();
        sys_move_cursor(1, print_location);
        printf("> [TASK] This is a thread to timing! (%u/%u seconds).\n", time, time_elapsed);
    }
}
