#ifndef SEMAFORO_H
#define SEMAFORO_H
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <iostream>

class Semaforo{
	private:
		int id;

	public:
		Semaforo(int key);
		~Semaforo();
		void signal();
		void wait();
};
#endif