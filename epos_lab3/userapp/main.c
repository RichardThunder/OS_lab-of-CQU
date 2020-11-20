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
#define NUM 550
#define Max 400
extern void *tlsf_create_with_pool(void *mem, size_t bytes);
extern void *g_heap;
#define cr RGB(100, 100, 100)
#define black RGB(0, 0, 0)
#define RED RGB(255, 0, 0)
#define BLUE RGB(0, 0, 255)
#define GREEN RGB(0, 255, 0)
#define PRI_USER_MIN    0//值越大优先级越高
#define PRI_USER_MAX  120
#define NZERO 20//nice的取值范围：[-NZERO,NZERO-1],值越小，优先级越高
#define WAIT 5/(1e-6)

int tid_foo3;
struct Ctrl
{
    int tid1;
    int tid2;
};

/**
 * GCC insists on __main
 *    http://gcc.gnu.org/onlinedocs/gccint/Collect2.html
 */

void task_foo_bubbleSort1(void *a)
{
    int  temp;
    int n=NUM;
    int array[NUM];
    long nan= WAIT;
    struct timespec ts;
    ts.tv_sec=0;
    ts.tv_nsec=nan;
    void srand(uint32_t seed);
    srand(time(NULL)); //生成随机数
    for (int j = 0; j < NUM; j++)
    {
        array[j] = rand() % Max;
        line(Max-array[j], j, Max, j, RED);
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
                line( 0, i, Max, i, black);
                line(0 , i-1, Max, i-1, black);
                line(Max-array[i], i, Max, i, RED);
                line(Max-array[i-1], i-1, Max, i-1, RED);
                nanosleep(&ts,NULL);
                
            }
        n--;
    }while(swapped);
    task_exit(0);
}

void task_foo_bubbleSort2(void *a)
{
    int  temp;
    int n=NUM;
    int array[NUM];
    long nan= WAIT;
    struct timespec ts;
    ts.tv_sec=0;
    ts.tv_nsec=nan;
    void srand(uint32_t seed);
    srand(time(NULL)); //生成随机数
    for (int j = 0; j < NUM; j++)
    {
        array[j] = rand() % Max;
        line(Max, j, Max+array[j], j, BLUE);
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
                line( Max, i, Max*2, i, black);
                line(Max , i-1, Max*2 , i-1, black);
                line(Max, i, Max+array[i], i, BLUE);
                line(Max, i-1, Max+array[i-1], i-1, BLUE);
                nanosleep(&ts,NULL);
            }
        n--;
    }while(swapped);
    task_exit(0);
}

void tsk_foo3(void *pv)
{  
    struct  Ctrl *c=(struct Ctrl *)pv;
	int prio1= getpriority(c->tid1);//获取线程优先级
    int prio2= getpriority(c->tid2);
	
    for(int i=0;i<=prio1;i=i+10)  //画出nice的实体化
	line(Max-i-1,NUM+20,Max-i-1,NUM+50,RED);
    for(int i=0;i<=prio2;i=i+10)
	line(Max+1+i,NUM+20,Max+1+i,NUM+50,BLUE);
    
    
    do{
    int key = getchar();
    if(key==0x4800) {
		prio1 = prio1+2;
		if(prio1 > PRI_USER_MAX) prio1=PRI_USER_MAX;//控制priority范
	    setpriority(c->tid1,prio1);//up
	    line(Max-1-prio1,NUM+20,Max-1-prio1,NUM+50,RED);
    }
   else if(key==0x5000)  {

	    
		prio1 = prio1-2;
		if(prio1 < PRI_USER_MIN) prio1=PRI_USER_MIN;
	    setpriority(c->tid1,prio1);//down
        line(Max-(prio1+10)-1,NUM+20,Max-(prio1+10)-1,NUM+50,black);//消失涂黑
	    //for(int i=0;i<=prio1;i++)  //画出nice的实体化
	    //line(Max-i-1,NUM+20,Max-i-1,NUM+50,RED);
    }
      if(key==0x4d00)  {
		prio2 = prio2+2;
		if(prio2 > PRI_USER_MAX) prio2=PRI_USER_MAX;
	    setpriority(c->tid2,prio2); //right
	    line(Max+1+prio2,NUM+20,Max+1+prio2,NUM+50,BLUE);
    }
   else if(key==0x4b00)  {
	    
        prio2 = prio2-2;
	    if(prio2 < PRI_USER_MIN) prio2=PRI_USER_MIN;
	    setpriority(c->tid2,prio2);//left
        line(Max+1+(prio2+10),NUM+20,Max+1+(prio2+10),NUM+50,black);    
   }
   }while(1); 
   
   // printf("This is task foo with tid=%d is Over!\r\n", task_getid()); 
    task_exit(0);//不能直接return (void *)0，必须调用task_exit
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
    
    //冒泡排序线程/
    unsigned char *stack_bub1;
    unsigned char *stack_bub2;
    unsigned char *stack_foo3;
    unsigned int stack_size_bub = 1024 * 1024;
    stack_bub1 = (unsigned char *)malloc(stack_size_bub);
    stack_bub2 = (unsigned char *)malloc(stack_size_bub);
    stack_foo3=(unsigned char *)malloc(stack_size_bub);
    struct Ctrl *tsk_c=(struct Ctrl *)malloc(stack_size_bub);
    int tid_bub1 = task_create(stack_bub1 + stack_size_bub, &task_foo_bubbleSort1, (void *)0);
    int tid_bub2 = task_create(stack_bub2 + stack_size_bub, &task_foo_bubbleSort2, (void *)0);
    tsk_c->tid1=tid_bub1;
    tsk_c->tid2=tid_bub2;
    int tid_foo3 = task_create(stack_foo3 +stack_size_bub, &tsk_foo3, (void *)tsk_c); 
    
    
    
    setpriority(tid_bub1,10);//设置优先级,数值越小越优先
    setpriority(tid_bub2,10);
    setpriority(tid_foo3,0);
   

    task_wait(tid_bub1, 0);
    task_wait(tid_bub2, 0);
    
    exit_graphic();
    free(stack_bub1);
    free(stack_bub2);
    free(stack_foo3);
  
    while (1)
        ;
    task_exit(0);
}



