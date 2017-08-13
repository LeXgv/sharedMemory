//создать общую память через посикс(отражение виртуального файла)
#include <iostream>
#include <errno.h>
#include<unistd.h>
#include<sys/mman.h>
#include<sys/types.h>
#include <fcntl.h>
#define _MBYTE_ 1048576
int main()
{
	shm_unlink("/test.shm");
	int sfd = shm_open("/test.shm", O_CREAT | O_RDWR | O_EXCL, 0777);
	if(sfd == -1)
	{
		if(errno == EEXIST)
		{
			std::cout << "Виртуальный файл существует\n";
			return -2;
		}
		std::cout << "Ошибка создания виртуального файла\n";
	}
	if(-1 == ftruncate(sfd, _MBYTE_))
	{
		std::cout << "Ошибка изменения размера\n";
		return -1;
	}
	char *ptr = (char *)mmap(NULL, _MBYTE_, PROT_READ | PROT_WRITE, MAP_SHARED, sfd, 0);	
	if(MAP_FAILED == ptr)
	{
		std::cout << "Ошибка создания отображения\n";
	}

	for(int i = 0; i < _MBYTE_; i++)
	{
		ptr[i] = 13;
	}
	for(int i = 0; i < 5; i++)
	{
		std::cout << ptr[i] << std::endl;	
	}
	return 0;
}
