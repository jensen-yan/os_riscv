#include <os/mm.h>

uint32_t memCurr;   //一个全局变量，保存栈的个数，默认为0

//输入栈个数，返回分配空间底部的指针
ptr_t allocPage(int numPage)
{
    ptr_t ret = (ptr_t) FREEMEM + (memCurr*PAGE_SIZE);
    //ptr_t ret = (ptr_t) INIT_KERNEL_STACK + (memCurr*PAGE_SIZE);
    //改成从最开始空闲处分配，和head.s匹配
    memCurr += numPage;
    return ret;
}
