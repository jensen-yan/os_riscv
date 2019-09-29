#include <os/list.h>
#include <os/mm.h>
#include <os/lock.h>
#include <os/sched.h>
#include <os/time.h>
#include <screen.h>
#include <stdio.h>
#include <assert.h>

pcb_t pcb[NUM_MAX_TASK];
const ptr_t pid0_stack = INIT_KERNEL_STACK; //第一个系统栈
pcb_t pid0_pcb = {
    .pid = 0,
    .kernel_sp = (ptr_t)pid0_stack,
    .user_sp = (ptr_t)pid0_stack};
//这是kernel的pid，要使用的！！！S

LIST_HEAD(ready_queue);
//我们使用的list双向链表,其实已经默认初始化成一个双向链表了。

/* current running task PCB */
pcb_t *current_running;

/* global process id */
pid_t process_id = 1;
//正在执行的进程号

static void check_sleeping() {}

void do_scheduler(void)
{
    // TODO schedule
    // Modify the current_running pointer.
    // switch_to current_running if needed
    if(current_running == NULL)
    {
        current_running = dequeue(&ready_queue);
        current_running->status = TASK_RUNNING;
        process_id = current_running->pid;
        switch_to(&pid0_pcb, current_running);
    }
    else if(current_running->status == TASK_RUNNING){
    pcb_t *next_running = dequeue(&ready_queue);
    //取出下一个pcb
    pcb_t *current_running_tmp = current_running;
    current_running->status = TASK_READY;
    next_running->status = TASK_RUNNING;
    //list_del(&current_running->list);
    enqueue(&ready_queue, current_running);
    //从队列头部删除，插入到队列尾部
    current_running = next_running; //指向当前运行态
    process_id = current_running->pid;
    switch_to(current_running_tmp, next_running);
    //之后进入新线程
    }
    else if(current_running->status == TASK_BLOCKED)
    {
        pcb_t *next_running = dequeue(&ready_queue);
        pcb_t *current_running_tmp = current_running;
        next_running->status = TASK_RUNNING;
        current_running = next_running; //指向当前运行态
        process_id = current_running->pid;
        switch_to(current_running_tmp, next_running);
        //之后进入新线程
    }

}

void do_sleep(uint32_t sleep_time)
{
    // TODO sleep(seconds)
}

void do_block(pcb_t *pcb_node, list_head *queue)
{
    // block the pcb task into the block queue
    //list_add_tail(pcb_node, queue);
    enqueue(queue, pcb_node);
    current_running->status = TASK_BLOCKED;
    do_scheduler(); //切换到下一个进程
}

void do_unblock(list_head *queue)
{
    // unblock the `pcb` from the block queue
    pcb_t *unblock_task = dequeue(queue);
    unblock_task->status = TASK_READY;
    enqueue(&ready_queue, unblock_task);
}

void enqueue(list_head* queue, pcb_t* item)
{
    list_add_tail(&item->list, queue);
}

pcb_t* dequeue(list_head* queue)
{
    pcb_t* tmp = list_entry(queue->next, pcb_t, list);
    list_del(queue->next);
    return tmp;
    //pcb_t *next_running = list_entry(current_running->list.next, pcb_t, list);
}