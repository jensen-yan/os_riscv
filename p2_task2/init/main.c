/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *            Copyright (C) 2018 Institute of Computing Technology, CAS
 *               Author : Han Shukai (email : hanshukai@ict.ac.cn)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *         The kernel's entry, where most of the initialization work is done.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * */

#include <os/irq.h>
#include <test.h>
#include <stdio.h>
#include <os/sched.h>
#include <screen.h>
#include <common.h>
#include <sys/syscall.h>
#include <os/mm.h>

#include <csr.h>

extern void ret_from_exception();
extern void printk_task1(void);
extern void __global_pointer$();

static void init_pcb_stack(ptr_t kernel_stack, ptr_t user_stack,
						   ptr_t entry_point, pcb_t *pcb)
{
	/* you should provide a fake stack for each pcb */
	regs_context_t *pt_regs = (regs_context_t *)(kernel_stack - sizeof(regs_context_t));
	//regs_context_t *pt_regs_user = (regs_context_t *)(user_stack - sizeof(regs_context_t));
	//pt_regs_user->regs[2] = kernel_stack;	//用户态的sp初始化为内核栈起始
	//应该在pcb的32个context的起始位置
	/* TODO: */
	for (size_t i = 0; i < 32; i++)
	{
		pt_regs->regs[i] = 0;
	}
	//其实全局变量都默认初始化为0吧
	//pt_regs->regs[1] = (reg_t)&ret_from_exception; //ra
	pt_regs->regs[2] = user_stack;			//sp指针，之后应该指向任务的用户栈
	pt_regs->regs[3] = (reg_t)__global_pointer$;	//gp指针
	//pt_regs->regs[4] = entry_point;		//tp指针
	pt_regs->sbadaddr = 0;
	pt_regs->scause = 0;
	pt_regs->sepc = entry_point;		//通过sret跳转到任务入口，ra没有作用
	pt_regs->sstatus = 0; 
	if(pcb->type == KERNEL_PROCESS  || KERNEL_THREAD)
		pt_regs->sstatus = SR_SPP | SR_SPIE;
	else
		pt_regs->sstatus = SR_SPP;
	
	// set sp to simulate return from switch_to
	ptr_t new_ksp = kernel_stack - sizeof(regs_context_t) - sizeof(switchto_context_t);
	pcb->kernel_sp = new_ksp;		//这里的kernel——sp已经是减过了，switch_to直接用就好了！！！

	switchto_context_t *st_regs = (switchto_context_t *)new_ksp;
	/* TODO: */
	for (size_t i = 0; i < 14; i++)
	{
		st_regs->regs[i] = 0;
	}
	st_regs->regs[0] = (reg_t)&ret_from_exception;		//ra
	st_regs->regs[1] = kernel_stack;					//sp

}

static void init_pcb()
{
	//struct task_info **task_group = sched1_tasks; //实验一要初始话的3个内核线程
	//struct task_info **task_group_2 = lock_tasks;
	// init all of your pcb and add them into ready_queue
	// TODO:
	init_list_head(&ready_queue);				 //初始化双向链表
	//list_add_tail(&pid0_pcb.list, &ready_queue); //把pid0 进程加进去
	struct task_info *task = NULL;
	for (size_t i = 1; i <= num_sched1_tasks + num_lock_tasks; i++)
	{
		if(i <= num_sched1_tasks)
			task = sched1_tasks[i - 1];
		else
			task = lock_tasks[i - num_sched1_tasks -1];
		//定义一个任务
		pcb[i].kernel_sp = allocPage(1);
		pcb[i].user_sp = allocPage(1);
		init_pcb_stack(pcb[i].kernel_sp, pcb[i].user_sp, task->entry_point, &pcb[i]);
		//初始化pcb的内核栈以及用户栈
		//list_add_tail(&pcb[i].list, &ready_queue); 
		//添加进ready_queue，先进先出，插到后面去。
		enqueue(&ready_queue, &pcb[i]);
		pcb[i].pid = i;
		pcb[i].status = TASK_READY;
		pcb[i].type = task->type; //其实是kernel thread
	}
	//分别得到pcb1，pcb2, pcb3这三个进程，注意pcb0 是kernel进程！
	// remember to initialize `current_running`
	// TODO:
	//pid0_pcb.status = TASK_RUNNING;
	//current_running = &pid0_pcb; //初始化kernel进程先运行
	//pcb[1].status = TASK_RUNNING;
	//current_running = &pcb[1];
	current_running = NULL;
}

// jump from bootloader.
// The beginning of everything >_< ~~~~~~~~~~~~~~
int main(void)
{
	// Close the cache, no longer refresh the cache
	// when making the exception vector entry copy

	// init Process Control Block (-_-!)
	init_pcb();
	printk("> [INIT] PCB initialization succeeded.\n\r");

	// init screen (QAQ)
	init_screen();
	printk("> [INIT] SCREEN initialization succeeded.\n\r");

	//test();

	while (1)
	{
		// (QAQQQQQQQQQQQ)
		// If you do non-preemptive scheduling, you need to use it to surrender control
		do_scheduler();
	};
	return 0;
}
