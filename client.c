/* 一
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
*/


#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "shmfifo.h"

typedef struct person{
  int age;
  char name[32];
}person_t;

// 创建两个子进程向服务器发送数据

int main(void)
{
  int i;
  pid_t cpid;
  shm_fifo_t *fifo = shmfifo_init(3,sizeof(person_t));
  person_t  person;

  cpid = fork();

  if (cpid == -1){
    perror("[ERROR]: fork()");
    exit(EXIT_FAILURE);
  }else if (cpid == 0){
    for (i = 0;i < 10;i++){
      strcpy(person.name,"lisi");
      person.age = 20;
      shmfifo_put(fifo,&person);
      sleep(1);
    }
    exit(EXIT_SUCCESS);
  }else if (cpid > 0){
    cpid = fork();
    if (cpid == -1){
      perror("[ERROR]: fork()");
      exit(EXIT_FAILURE);

    }else if (cpid == 0){
      for (i = 0;i < 10;i++){
        strcpy(person.name,"zhangsan");
        person.age = 30;
        shmfifo_put(fifo,&person);
        sleep(2);
      }
      exit(EXIT_SUCCESS);
    }else if(cpid > 0){
      wait(NULL);
      wait(NULL);
    }
  }

  return 0;
}


