#include <stdio.h>
#include <string.h>

#include "shm.h"

#define SHM_SZ 256

int main()
{
  int shmid;
  enum shm_creat_status shm_status;
  void *addr =NULL;// 映射后的首地址
  char buffer[64] = {0};

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


  // 将共享内存的数据拷贝到buffer中
  memcpy(buffer, addr, 10);

  printf("buffer : %s\n ",buffer);

  shm_dt(addr);
  // shm_del(shmid);

  return 0;
}
