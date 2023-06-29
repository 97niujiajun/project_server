/* 一，测试共享内存是否成功
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
*/


#include <stdio.h>
#include <string.h>

#include "shmfifo.h"

typedef struct person{
  int age;
  char name[32];
}person_t;

// 服务器负责从环形队列中读取数据
int main(void)
{
  person_t person;
  shm_fifo_t *fifo = shmfifo_init(3, sizeof(person_t)); // 初始化环形队列，三个数据

  for (;;) {
    shmfifo_get(fifo, &person);
    printf("name = %s, age = %d\n", person.name,person.age);
  }
  return 0;
}












