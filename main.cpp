//задача: создать секцию в мегобайт общей памяти и каждый байт заполнить числом 42
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <iostream>
#define MemorySize 1948576
#define _BYTE_ 1048576

int main()
{

	key_t shmkey = ftok("file", 1);
	if(-1 == shmkey)
	{
		std::cout << "Ошибка получения ключа SYS5\n";
		return -1;
	}
	//создание сектора памяти
	int shmid = shmget(shmkey, MemorySize, IPC_CREAT | IPC_EXCL | 0777);
	if(-1 == shmid)
	{
		if(errno == EEXIST)
		{
			std::cout << "Общая память уже существует удалите ее\n";
			return -3;
		}
		else
		std::cout << "Ошибка создания сектора общей памяти\n";
		return -2;
	}
	//подключение к секторц памяти
//	void *shmptr = nullptr;
	char *res = (char *)shmat(shmid, NULL, 0);
	if((void *)-1 == (void *)res)
	{
		std::cout << "Ошибка подключения к сегменту\n";
		return -4;
	}
	//char *s =(char *)shmptr;
	for(int i =0; i < _BYTE_; i++)
	{
		res[i] = 42;
	}
	for(int i = 0; i < 5; i++)
	{
		std::cout << res[i] << std::endl;
	}
	// отключаемся от памяти
	if(-1 == shmdt((void *)res))
	{
		std::cout << "Ошибка отключения от памяти\n";
		return -5;
	}
	//удаление памяти
	if(-1 == shmctl(shmid, IPC_RMID, NULL))
	{
		std::cout << "Ошибка удаления сегмента\n";
		return -6;
	}
	return 0;
}
