//
// Created by 62359 on 2023/6/27.
//

#ifndef PROJECT_SERVER__SHM_H_
#define PROJECT_SERVER__SHM_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

/*
    共享内存创建状态:
        SHM_HAS_EXIST : 共享内存已经存在
        SHM_CREAT_NEW : 共享内存为新创建
        SHM_CREAT_ERROR ： 共享内存创建错误
    用途: 用于后续环形队列区分是否为第一次创建，在第一次创建是会进行初始化
*/;
enum shm_creat_status{
  SHM_HAS_EXIST = 0,
  SHM_CREAT_NEW,
  SHM_CREAT_ERROR,
};

/*
 * 创建共享内存
 * @:内存大小
 * @:共享内存id
 * return:状态
 */
extern enum shm_creat_status shm_creat(size_t size, int *pshmid);

extern void *shm_at(int shmid); // 添加映射
extern int shm_dt(const void *shmaddr); // 解除映射
extern int shm_del(int shmid); // 删除共享内存

#endif //PROJECT_SERVER__SHM_H_
