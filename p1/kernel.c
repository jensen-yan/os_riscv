#include <sbi.h>

#define VERSION_BUF 50

int version = 1;
char buf[VERSION_BUF];

//int value = 2019;

int main(void)
{
    // call BIOS putstr to print message "Hello OS!"
    // TODO:
    char msg[] = "Hello OS!\n\r";
    sbi_console_putstr(msg);

    // DON't MODIFY THIS
    char ver_str[] = "Version: ";
    int i;
    for (i = 0; i < sizeof(ver_str) - 1; ++i) {
        buf[i] = ver_str[i];
    }
    buf[i] = '0'+version; // version must between 0 and 9
    buf[i+1] = '\n';
    buf[i+2] = '\r';
    // buf should be "Version: 1\n\r"

    // output buf
    // TODO:
    sbi_console_putstr(buf);
    // loop forever
    while(1);

    return 0;
}
