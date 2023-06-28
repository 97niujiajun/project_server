//
// Created by 62359 on 2023/6/27.
//
#ifndef PROJECT_SERVER__SHMFIFO_H_
#define PROJECT_SERVER__SHMFIFO_H_

#include "sem.h"
#include "shm.h"

#define SEM_EMPTY_ID 0
#define SEM_FULL_ID 1
#define SEM_MUTEX_ID 2


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


#endif //PROJECT_SERVER__SHMFIFO_H_
