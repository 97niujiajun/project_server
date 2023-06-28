#include "sem.h"
#include <sys/sem.h>


#define SEM_PATHNAME "/home"
#define SEM_PRO_ID 99

/*
 *  在不同操作中传参给'semctl'
 	@param val : 设置单个信号量的值时使用，对应的操作类型是SETVAL
	@param *array : 获取或设置多个信号量的值时使用，对应的操作类型是GETALL 和 SETALL
 */
union semun{
	int val; // SETVAL
	unsigned short *array;// Array for GETALL/SETALL
};

/*
	创建信号量集合
	@param nsems : 信号量的数量
	@param values : 信号量的值
	@return :
		成功 : 0
		失败 : -1
*/
int sem_create(int nsems,unsigned short values[])
{
	int semid,ret; 
	key_t key;		
	union semun s;

	key = ftok(SEM_PATHNAME,SEM_PRO_ID);
	if (key == -1){
		perror("[ERROR] ftok() : ");
		return -1;
	}

	semid = semget(key,nsems,IPC_CREAT|0666);
	if (semid == -1){
		perror("[ERROR] semget() : ");
		return -1;
	}
	
	s.array = values;

	ret = semctl(semid,0,SETALL,s);
	if (ret == -1){
		perror("[ERROR] semctl() : ");
		return -1;	
	}
	
	return semid;
}

/*
	占用信号量资源
	@param semid : 信号量集合的id
	@param semnum : 操作的信号量编号
*/
int sem_p(int semid,int semnum)
{
	struct sembuf sops;

	sops.sem_num = semnum; 	
	sops.sem_op = -1;
	sops.sem_flg = SEM_UNDO;// 进程终止,会自动释放
	
	return semop(semid,&sops,1);
}

/*
	释放信号量资源
	@param semid : 信号量集合的id
	@param semnum : 操作的信号量编号
*/
int sem_v(int semid,int semnum)
{
	struct sembuf sops;

	sops.sem_num = semnum; 	
	sops.sem_op = 1;
	sops.sem_flg = SEM_UNDO;

	return semop(semid,&sops,1);
}

/*
	删除信号量集合
	@param semid : 信号量集合的id
*/
int sem_del(int semid)
{
	return semctl(semid,0,IPC_RMID,NULL);
}
