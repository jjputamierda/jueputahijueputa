#include "Semaforo.h"

union semun {
	int val;    			 /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux-specific) */
};

/**
@brief 
@details 
@param[in] 
@param[out] 
@pre 
@remark 
@return 
@exception 
@author 
@date 
*/

Semaforo::Semaforo(int key){
	id = semget(key, 1, IPC_CREAT|0600);
	if(id == -1){
		std::cerr << "Error en la contrucción del semáforo\n";
	}
	union semun u;
	u.val = 0;
	if(semctl(id, 0, SETVAL, u) == -1){
		std::cerr << "Error en la instrucción semctl()\n";
	}
}

/**
@brief 
@details 
@param[in] 
@param[out] 
@pre 
@remark 
@return 
@exception 
@author 
@date 
*/

Semaforo::~Semaforo(){
	if(semctl(id, 1, IPC_RMID) == -1){
		std::cerr << "Error en la destrucción del semáforo\n";
	}
}

/**
@brief 
@details 
@param[in] 
@param[out] 
@pre 
@remark 
@return 
@exception 
@author Johel Phillips B7XXXX (Pandemic Plan Remastered)
@date 14/9/20
*/

void Semaforo::wait(){
	struct sembuf z;
	z.sem_num = 0;
	z.sem_op = -1;
	z.sem_flg = 0;
	if(semop(id, &z, 1) == -1){
		std::cerr << "Error en el método wait()\n";
	}
}

/**
@brief 
@details 
@param[in] 
@param[out] 
@pre 
@remark 
@return 
@exception 
@author Johel Phillips B7XXXX (Pandemic Plan Remastered)
@date 14/9/20
*/

void Semaforo::signal(){
	struct sembuf z;
	z.sem_num = 0;
	z.sem_op = 1; 
	z.sem_flg = 0;
	if(semop(id, &z, 1) == -1){
		std::cerr << "Error en el método signal()\n";
	}
}