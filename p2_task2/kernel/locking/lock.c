#include <os/lock.h>
#include <os/sched.h>
#include <sys/syscall.h>

#include <atomic.h>

void spin_lock_init(spin_lock_t *lock)
{
    lock->status = UNLOCKED;
}

int spin_lock_try_acquire(spin_lock_t *lock)
{
   // return atomic_swap_d(1, &lock->status);
}

void spin_lock_acquire(spin_lock_t *lock)
{
    //while(atomic_swap_d(1, &lock->status) == 1)
    //    ;
}

void spin_lock_release(spin_lock_t *lock)
{
    lock->status = UNLOCKED;
}

void do_mutex_lock_init(mutex_lock_t *lock)
{
    init_list_head(&lock->block_queue);
    lock->lock.status = UNLOCKED;
}

void do_mutex_lock_acquire(mutex_lock_t *lock)
{
    if(lock->lock.status == LOCKED)
    {
        do_block(current_running, &lock->block_queue);
        //有任务占着锁了，本任务进入阻塞队列中
    }
    else
    {
        lock->lock.status = LOCKED;
        //没人占用锁，自己占用了
    }
    
}

void do_mutex_lock_release(mutex_lock_t *lock)
{
    if(list_empty(&lock->block_queue))
    {
        lock->lock.status = UNLOCKED;
        //阻塞队列空的，把锁解开
    }
    else
    {
        do_unblock(&lock->block_queue);
        //从阻塞队列中拿出任务，放入ready队列中
    }
}
