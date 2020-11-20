/*
 * vim: filetype=c:fenc=utf-8:ts=4:et:sw=4:sts=4
 */
#include <inttypes.h>
#include <stddef.h>
#include <math.h>
#include <stdio.h>
#include <sys/mman.h>
#include <syscall.h>
#include <netinet/in.h>
#include <stdlib.h>
#include "graphics.h"
#define NUM 200
#define Max 600
extern void *tlsf_create_with_pool(void *mem, size_t bytes);
extern void *g_heap;
#define cr RGB(100, 100, 100)
#define black RGB(0, 0, 0)
#define RED RGB(255, 0, 0)
#define BLUE RGB(0, 0, 255)
#define GREEN RGB(0, 255, 0)
/**
 * GCC insists on __main
 *    http://gcc.gnu.org/onlinedocs/gccint/Collect2.html
 */
void task_foo_insertionSort(void *a)
{
    int arr[NUM];
    void srand(uint32_t seed);
    srand(time(NULL)); //生成随机数

    for (int i = 0; i < NUM; i++)
    {
        arr[i] = rand() % Max;
        line(i, Max, i, Max - arr[i], cr);
    }
    //arr[0]=*((int*)a);
    for (int i = 0; i < NUM; i++)
    {
        for (int j = i; j > 0; j--)
            if (arr[j] < arr[j - 1])
            {
                int temp;
                temp = arr[j];
                arr[j] = arr[j - 1];
                arr[j - 1] = temp;

                line(j, 0, j, Max, black);
                line(j - 1, 0, j - 1, Max, black);
                line(j, Max, j, Max - arr[j], cr);
                line(j - 1, Max, j - 1, Max - arr[j - 1], cr);
            }
            else
                break;
        ;
    }
    task_exit(0);
}

void task_foo_bubbleSort(void *a)
{
    int  temp;
    int n=NUM;
    int array[NUM];
    void srand(uint32_t seed);
    srand(time(NULL)); //生成随机数
    for (int j = 0; j < NUM; j++)
    {
        array[j] = rand() % Max;
        line(j + NUM, Max, j + NUM, Max - array[j], RED);
    }
    int swapped;
    //int newn; // 理论上,可以使用newn进行优化,但实际优化效果较差

    do{
        swapped = 0;
        for( int i = 1 ; i <n ; i ++ )
            if( array[i-1] > array[i] ){
               
                temp = array[i-1];
                array[i-1] = array[i];
                array[i] = temp;
                swapped = 1;
                line( i+ NUM, 0, i + NUM, Max, black);
                line(i - 1 + NUM , 0, i-1 + NUM , Max, black);
                line(i + NUM, Max, i + NUM, Max - array[i], RED);
                line(i - 1 + NUM, Max, i - 1 + NUM, Max - array[i-1], RED);
                
            }
        n--;
    }while(swapped);
    task_exit(0);
}

void task_foo_shellSort(void *a)
{
    int arr[NUM];
    void srand(uint32_t seed);
    srand(time(NULL)); //生成随机数
    for (int i = 0; i < NUM; i++)
    {
        arr[i] = rand() % Max;
        line(i + NUM + NUM, Max, i + NUM + NUM, Max - arr[i], BLUE);
    }
    int h = 1;
    while (h < NUM / 3)
        h = 3 * h + 1;
    // 计算 increment sequence: 1, 4, 13, 40, 121, 364, 1093...

    while (h >= 1)
    {

        // h-sort the array
        for (int i = h; i < NUM; i++)
        {

            // 对 arr[i], arr[i-h], arr[i-2*h], arr[i-3*h]... 使用插入排序
            int e = arr[i];
            int j;
            for (j = i; j >= h && e < arr[j - h]; j -= h)
            {
                arr[j] = arr[j - h];
                line(j + NUM + NUM, 0, j + NUM + NUM, Max, black);
                line(j + NUM + NUM, Max, j + NUM + NUM, Max - arr[j], BLUE);
            }
            arr[j] = e;
            line(j + NUM + NUM, 0, j + NUM + NUM, Max, black);
            line(j + NUM + NUM, Max, j + NUM + NUM, Max - arr[j], BLUE);
        }

        h /= 3;
    }
    task_exit(0);
}

int __partition(int arr[], int l, int r)
{

    int v = arr[l];

    int j = l; // arr[l+1...j] < v ; arr[j+1...i) > v
    for (int i = l + 1; i <= r; i++)
        if (arr[i] < v)
        {
            j++;
            int temp;
            temp = arr[j];
            arr[j] = arr[i];
            arr[i] = temp;
            line(j + NUM + NUM + NUM, 0, j + NUM + NUM + NUM, Max, black);
            line(i + NUM + NUM + NUM, 0, i + NUM + NUM + NUM, Max, black);
            line(j + NUM + NUM + NUM, Max, j + NUM + NUM + NUM, Max - arr[j], GREEN);
            line(i + NUM + NUM + NUM, Max, i + NUM + NUM + NUM, Max - arr[i], GREEN);
        }

    int temp;
    temp = arr[j];
    arr[j] = arr[l];
    arr[l] = temp;
    line(j + NUM + NUM + NUM, 0, j + NUM + NUM + NUM, Max, black);
    line(l + NUM + NUM + NUM, 0, l + NUM + NUM + NUM, Max, black);
    line(j + NUM + NUM + NUM, Max, j + NUM + NUM + NUM, Max - arr[j], GREEN);
    line(l + NUM + NUM + NUM, Max, l + NUM + NUM + NUM, Max - arr[l], GREEN);

    return j;
}
// 对arr[l...r]部分进行快速排序
void __quickSort(int arr[], int l, int r)
{

    if (l >= r)
        return;

    int p = __partition(arr, l, r);
    __quickSort(arr, l, p - 1);
    __quickSort(arr, p + 1, r);
}

void task_foo_quickSort(void *a)
{
    int arr[NUM];
    void srand(uint32_t seed);
    srand(time(NULL)); //生成随机数
    for (int i = 0; i < NUM; i++)
    {
        arr[i] = rand() % Max;
        line(i + NUM + NUM + NUM, Max, i + NUM + NUM + NUM, Max - arr[i], GREEN);
    }
    __quickSort(arr, 0, NUM - 1);
    task_exit(0);
}

void __main()
{
    size_t heap_size = 32 * 1024 * 1024;
    void *heap_base = mmap(NULL, heap_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    g_heap = tlsf_create_with_pool(heap_base, heap_size);
}
/**
 * 第一个运行在用户模式的线程所执行的函数
 */
void main(void *pv)
{
    init_graphic(0x143); //init_graphic(int mode)进入图形模式
    //选择排序线程
    unsigned char *stack_ins;
    unsigned int stack_size = 1024 * 1024;
    stack_ins = (unsigned char *)malloc(stack_size);
    int tid_ins;
    tid_ins = task_create(stack_ins + stack_size, &task_foo_insertionSort, (void *)0);
    //冒泡排序线程
    unsigned char *stack_bub;
    unsigned int stack_size_bub = 1024 * 1024;
    stack_bub = (unsigned char *)malloc(stack_size_bub);
    int tid_bub = task_create(stack_bub + stack_size_bub, &task_foo_bubbleSort, (void *)0);
    //shellshort
    unsigned char *stack_shell;
    unsigned int stack_size_shell = 1024 * 1024;
    stack_shell = (unsigned char *)malloc(stack_size_shell);
    task_create(stack_shell + stack_size_shell, &task_foo_shellSort, (void *)0);
    //快速排序
    unsigned char *stack_quick;
    unsigned int stack_size_quick = 1024 * 1024;
    stack_quick = (unsigned char *)malloc(stack_size_quick);
    task_create(stack_quick + stack_size_quick, &task_foo_quickSort, (void *)0);
    //等待线程退出
    task_wait(tid_ins, 0);
    task_wait(tid_bub, 0);
    exit_graphic();
    free(stack_ins);
    free(stack_bub);
    free(stack_quick);
    free(stack_shell);
    while (1)
        ;
    task_exit(0);
}
