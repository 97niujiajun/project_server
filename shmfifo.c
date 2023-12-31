//
// Created by 62359 on 2023/6/27.
//
#include <string.h>
#include "shmfifo.h"

static int g_shmid; // 共享内存id

shm_fifo_t *shmfifo_init(int blocks, int blksz)
{
  int shm_sz = 0;
  enum shm_creat_status s_creat_type;
  unsigned short values[] = {0,blocks,1};//对3个信号量进行初始化

  shm_fifo_t *p_shm_fifo = malloc(sizeof(shm_fifo_t)); // 分配环形队列数据结构空间
  if (!p_shm_fifo){
    return NULL;
  }

  shm_sz = blocks * blksz + sizeof(shm_head_t); // 计算共享内存的大小

  s_creat_type = shm_creat(shm_sz, &g_shmid); // 创建共享内存

  if (s_creat_type != SHM_CREAT_ERROR){

    p_shm_fifo->p_head = (shm_head_t *)shm_at(g_shmid);// 共享内存映射,返回的首地址给p_head用。

    if (s_creat_type == SHM_CREAT_NEW){ // 如果是新创建的共享内存,则进行初始化
      p_shm_fifo->p_head->rpos = 0; // 初始化读环形队列的位置,相当于出队列的位置
      p_shm_fifo->p_head->wpos = 0; // 初始化写环形队列的位置,相当于入队列的位置
      p_shm_fifo->p_head->blocks = blocks; // 初始化 block 的数量
      p_shm_fifo->p_head->blksz = blksz;  // 初始化每个 block 的大小
      p_shm_fifo->p_head->semid = sem_create(3,values); // 创建信号量集合,包含3个信号量
    }

    //计算数据的起始位置，并保存到payload 指针中
    p_shm_fifo->p_payload = (char *)(p_shm_fifo->p_head + 1);

    return p_shm_fifo;//返回环形队列结构体
  }
  return NULL;

}

void shmfifo_destroy(shm_fifo_t *fifo)
{
  sem_del(fifo->p_head->semid); // 删除信号量集合
  shm_dt(fifo->p_head); // 解除共享内存映射
  shm_del(g_shmid); // 删除共享内存
  free(fifo); // 释放环形队列结构体对象
}

void shmfifo_put(shm_fifo_t *fifo,const void *buf)
{
  int pos = 0;

  sem_p(fifo->p_head->semid,SEM_FULL_ID);// 占用 判满 信号量
  sem_p(fifo->p_head->semid,SEM_MUTEX_ID); // 占用 互斥 信号量

  /*
      int rpos; // 出队列的位置
      int wpos; // 入队列的位置
      int blocks; // 数据块个数
      int blksz; // 数据块大小
      int semid; // 信号量集合ID
   */
  // 以下的代码，入队wpos仅做了一次变化，就是数据进入到环形队列后，就向后移动一位。 注意 pos 和 wpos的区别。
  pos = fifo->p_head->wpos * fifo->p_head->blksz;// 计算偏移量 = wpos * blksz

  memcpy(fifo->p_payload + pos,buf,fifo->p_head->blksz);// 拷贝数据到环形队列中

  fifo->p_head->wpos = (fifo->p_head->wpos + 1) % (fifo->p_head->blocks);// 更新偏移

  sem_v(fifo->p_head->semid,SEM_MUTEX_ID); // 注意先释放互斥的信号量
  sem_v(fifo->p_head->semid,SEM_EMPTY_ID);
}

void shmfifo_get(shm_fifo_t *fifo, void *buf)
{
  int pos = 0;

  sem_p(fifo->p_head->semid,SEM_EMPTY_ID);// 占用 判空 的信号量
  sem_p(fifo->p_head->semid,SEM_MUTEX_ID);

  pos = fifo->p_head->rpos * fifo->p_head->blksz;// 计算偏移量

  memcpy(buf,fifo->p_payload + pos,fifo->p_head->blksz);//从环形队列中拷贝数据
  fifo->p_head->rpos = (fifo->p_head->rpos + 1) % (fifo->p_head->blocks);// 更新偏移
  sem_v(fifo->p_head->semid,SEM_MUTEX_ID);
  sem_v(fifo->p_head->semid,SEM_FULL_ID);
}