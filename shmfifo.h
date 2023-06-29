//
// Created by 62359 on 2023/6/27.
//
#ifndef PROJECT_SERVER__SHMFIFO_H_
#define PROJECT_SERVER__SHMFIFO_H_

#include "sem.h"
#include "shm.h"

// 这里的0，1，2是values[]中的下标
// unsigned short values[] = {0,blocks,1};//对3个信号量进行初始化
#define SEM_EMPTY_ID 0 // 判空
#define SEM_FULL_ID 1  // 判满
#define SEM_MUTEX_ID 2 // 互斥


// 头节点
typedef struct shm_head{
  int rpos; // 出队列的位置
  int wpos; // 入队列的位置
  int blocks; // 数据块个数
  int blksz; // 数据块大小
  int semid; // 信号量集合ID
}shm_head_t;

typedef struct shm_fifo{
  shm_head_t *p_head; // 指向共享内存 head 区域
  char *p_payload; // 指向共享内存 data 区域
}shm_fifo_t;

// 创建环形队列，初始化
extern shm_fifo_t *shmfifo_init(int blocks, int blksz);

// 销毁环形队列
extern void shmfifo_destroy(shm_fifo_t *fifo);

// 环形队列写数据,*fifo指向结构体变量的指针，const buffer地址。
extern void shmfifo_put(shm_fifo_t *fifo,const void *buf);

// 环形队列读数据
extern void shmfifo_get(shm_fifo_t *fifo, void *buf);

#endif //PROJECT_SERVER__SHMFIFO_H_
