#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <time.h>

int main (){


	time_t tiempo;
	localtime(&tiempo);

	char a[100] = "Estamos en el año ";
	char dia[100] = "Hoy es el dia ";
	char s[100];
	strftime (s,100,'%w',&tiempo);
	std::cout<<a<<s<<dia<<std::endl;
	return 1;
}
