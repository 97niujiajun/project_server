//
// Created by 62359 on 2023/6/27.
//
#include <stdio.h>
#include <string.h>

#include "shm.h"

#define SHM_SZ 256

int main()
{
  int shmid;
  enum shm_creat_status shm_status;
  void *addr =NULL;// 映射后的首地址

  shm_status = shm_creat(SHM_SZ, &shmid);

  if(shm_status == SHM_CREAT_NEW){
    printf("shared memory creat new. \n ");
  }else if(shm_status == SHM_HAS_EXIST){
    printf("share memory has exist.\n");
  }


  addr = shm_at(shmid);

  printf("shmid : %d\n",shmid);

  if (addr == NULL){
    printf("shm at failed.\n");
    return -1;
  }

  // 设置共享内存的前10个字节为‘A’
  memset(addr,'A', 10);

  shm_dt(addr);
  // shm_del(shmid); 服务器那边会删除


  return 0;
}
