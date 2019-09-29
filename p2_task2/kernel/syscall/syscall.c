#include <common.h>
#include <os/lock.h>
#include <os/sched.h>
#include <screen.h>
#include <sys/syscall.h>
#include <type.h>

void system_call_helper(int fn, int arg1, int arg2, int arg3)
{
    // syscall[fn](arg1, arg2, arg3)
}

void sys_sleep(uint32_t time)
{
    invoke_syscall(SYSCALL_SLEEP, time, IGNORE, IGNORE);
}

void sys_block(list_node_t *queue)
{
    invoke_syscall(SYSCALL_BLOCK, (ptr_t)queue, IGNORE, IGNORE);
}

void sys_unblock_one(list_node_t *queue)
{
    invoke_syscall(SYSCALL_UNBLOCK_ONE, (ptr_t)queue, IGNORE, IGNORE);
}

void sys_unblock_all(list_node_t *queue)
{
    invoke_syscall(SYSCALL_UNBLOCK_ALL, (ptr_t)queue, IGNORE, IGNORE);
}

void sys_write(char *buff)
{
    invoke_syscall(SYSCALL_WRITE, (ptr_t)buff, IGNORE, IGNORE);
}

void sys_reflush()
{
    invoke_syscall(SYSCALL_REFLUSH, IGNORE, IGNORE, IGNORE);
}

void sys_move_cursor(int x, int y)
{
    invoke_syscall(SYSCALL_CURSOR, x, y, IGNORE);
}

void mutex_lock_init(mutex_lock_t *lock)
{
    invoke_syscall(SYSCALL_MUTEX_LOCK_INIT, (ptr_t)lock, IGNORE, IGNORE);
}

void mutex_lock_acquire(mutex_lock_t *lock)
{
    invoke_syscall(SYSCALL_MUTEX_LOCK_ACQUIRE, (ptr_t)lock, IGNORE, IGNORE);
}

void mutex_lock_release(mutex_lock_t *lock)
{
    invoke_syscall(SYSCALL_MUTEX_LOCK_RELEASE, (ptr_t)lock, IGNORE, IGNORE);
}
