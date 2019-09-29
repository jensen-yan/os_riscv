#include <assert.h>
#include <elf.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IMAGE_FILE "./image"
#define ARGS "[--extended] [--vm] <bootblock> <executable-file> ..."

#define SECTOR_SIZE 512
#define BOOT_LOADER_SIG_OFFSET 0x1fe
#define OS_SIZE_LOC (BOOT_LOADER_SIG_OFFSET - 2)
#define BOOT_LOADER_SIG_1 0x55
#define BOOT_LOADER_SIG_2 0xaa
#define BOOT_MEM_LOC 0x7c00
#define OS_MEM_LOC 0x10201000

/* structure to store command line options */
static struct
{
    int vm;
    int extended;
} options;

/* prototypes of local functions */
static void create_image(int nfiles, char *files[]);
static void error(char *fmt, ...);
static void read_ehdr(Elf64_Ehdr *ehdr, FILE *fp);
static void read_phdr(Elf64_Phdr *phdr, FILE *fp, int ph,
                      Elf64_Ehdr ehdr);
static void write_segment(Elf64_Ehdr ehdr, Elf64_Phdr phdr, FILE *fp,
                          FILE *img, int *nbytes, int *first);
static void write_os_size(int nbytes, FILE *img);

int main(int argc, char **argv)
{
    char *progname = argv[0];

    /* process command line options */
    options.vm = 0;
    options.extended = 0;
    while ((argc > 1) && (argv[1][0] == '-') && (argv[1][1] == '-'))
    {
        char *option = &argv[1][2];

        if (strcmp(option, "vm") == 0)
        {
            options.vm = 1;
        }
        else if (strcmp(option, "extended") == 0)
        {
            options.extended = 1;
        }
        else
        {
            error("%s: invalid option\nusage: %s %s\n", progname,
                  progname, ARGS);
        }
        argc--;
        argv++;
    }
    if (options.vm == 1)
    {
        error("%s: option --vm not implemented\n", progname);
    }
    if (argc < 3)
    {
        /* at least 3 args (createimage bootblock kernel) */
        error("usage: %s %s\n", progname, ARGS);
    }
    create_image(argc - 1, argv + 1);
    return 0;
}

Elf64_Ehdr ehdr;

//制作image，nfiles是elf文件数，files是文件名的数组
static void create_image(int nfiles, char *files[])
{
    //Elf64_Ehdr *ehdr = NULL;
    if (nfiles != 2)
    {
        printf("please input two files\n");
    }

    FILE *bootblock, *kernel, *image;
    if ((bootblock = fopen("bootblock", "rb")) == NULL)
        printf("cannot open bootblock\n");
    if ((kernel = fopen("kernel", "rb")) == NULL)
        printf("cannot open kernel\n");
    if ((image = fopen("image", "wb")) == NULL)
        printf("cannot open image\n"); //打开3个文件

    Elf64_Ehdr *b_ehdr, *k_ehdr;
    read_ehdr(b_ehdr, bootblock);
    read_ehdr(k_ehdr, kernel);

    Elf64_Phdr *b_phdr, *k_phdr;
    read_phdr(b_phdr, bootblock, 1, *b_ehdr);
    read_phdr(k_phdr, kernel, 1, *k_ehdr);

    int first = 0;
    int nbytes = 0; //写到image的偏移量
    write_segment(*b_ehdr, *b_phdr, bootblock, image, &nbytes, &first);
    write_segment(*k_ehdr, *k_phdr, kernel, image, &nbytes, &first);

    write_os_size(nbytes, image);

    fclose(bootblock);
    fclose(kernel);
    fclose(image);
}

//读取fp文件的ELF文件头结构
static void read_ehdr(Elf64_Ehdr *ehdr, FILE *fp)
{
    ehdr = (Elf64_Ehdr*)malloc(sizeof(Elf64_Ehdr));
    fread(ehdr, sizeof(Elf64_Ehdr), 1, fp);
}

//读取第一个程序头结构，ph是程序头编号
static void read_phdr(Elf64_Phdr *phdr, FILE *fp, int ph,
                      Elf64_Ehdr ehdr)
{
    phdr = (Elf64_Phdr*)malloc(sizeof(Elf64_Phdr));
    fseek(fp, (long)ehdr.e_phoff, SEEK_SET); //偏移到program header处
    fread(phdr, sizeof(Elf64_Phdr), 1, fp); //读取program header
}

static void write_segment(Elf64_Ehdr ehdr, Elf64_Phdr phdr, FILE *fp,
                          FILE *img, int *nbytes, int *first)
{
    char buffer[1024];                          //存放要写入image的内容
    size_t filesize = phdr.p_filesz;            //程序段大小
    size_t offset = phdr.p_offset;              //程序段偏离起始地址的大小，为0x1000
    fseek(fp, (long)offset, SEEK_SET);          //fp重新定位到文件起点偏离0x1000处
    fread(buffer, filesize, 1, fp);             //把程序段写入buffer中
    fseek(img, (long)(*first * 512), SEEK_SET); //如果写过bootblock了，要从image的第二个扇区开始写
    fwrite(buffer, filesize, 1, img);           //写入image中
    *nbytes = filesize;
    *first += 1;
}

//将kernel段的大小，写入OS——SIZE——LOC中
static void write_os_size(int nbytes, FILE *img)
{
    fseek(img, (long)(0x200 - 0x4), SEEK_SET);
    char others[4] = {0x01, 0x00, 0x55, 0xaa}; //将kernel的扇区数写到image中
    int kernel_size = (nbytes + 511) / 512;    //kernel占据的扇区数
    others[0] = (char)kernel_size;
    fwrite(others, 4, 1, img);
}

/* print an error message and exit */
static void error(char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    if (errno != 0)
    {
        perror(NULL);
    }
    exit(EXIT_FAILURE);
}
