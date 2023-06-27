//
// Created by 62359 on 2023/6/27.
//

#include "shm.h"

#define PATHNAME "/home"
#define PRO_ID 101

enum shm_creat_status shm_creat(size_t size, int *pshmid)
{
  key_t key;
  int shmid; // 共享内存id

  key = ftok(PATHNAME, PRO_ID);

  shmid = shmget(key, size, 0);// 获取共享内存id（判断是否已存在共享内存）
  printf("shm.c - shmid : %d\n",shmid);
  if (shmid == -1){ // 共享内存不存在
    shmid = shmget(key, size, IPC_CREAT|0644); // 创建
    if (shmid == -1){
      perror("[ERROR] shmget(): ");
      return SHM_CREAT_ERROR;
    }
    *pshmid = shmid;
    return SHM_CREAT_NEW;
  } else{
    *pshmid = shmid;
    return SHM_HAS_EXIST;
  }
}

void *shm_at(int shmid)
{
  void *addr = NULL; // 映射后首地址
  addr = shmat(shmid, NULL, 0);
  if (addr == (void*)-1){
    perror("[ERROR] shmat(): ");
    return NULL;
  }
  return addr;
}


int shm_dt(const void *shmaddr)
{
  return shmdt(shmaddr);
}

int shm_del(int shmid)
{
  return shmctl(shmid, IPC_RMID, NULL);
}

