#include "Serializable.h"

#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

class Jugador: public Serializable
{
public:
    Jugador(const char * _n, int16_t _x, int16_t _y):x(_x),y(_y)
    {
        strncpy(name, _n, 80);
    };

    virtual ~Jugador(){};

    void to_bin()
    {
	int32_t total = 80 * sizeof(char) + 2 * sizeof(int16_t);
	alloc_data(total);
	char * idx = _data + sizeof(int32_t);
	memcpy(idx, (void*)name, 80);
	idx += 80;
	memcpy(idx, &x, sizeof(int16_t));
	idx += sizeof(int16_t);
	memcpy(idx, &y, sizeof(int16_t));
	idx += sizeof(int16_t);
	
    }

    int from_bin(char * data)
    {

	char * idx = data + sizeof(int32_t);
	memcpy(name, idx, 80);
	idx += 80;
	memcpy(&x, idx, 2);
	idx += sizeof(int16_t);//2 por el tam de int16_t (2bytes)
	memcpy(&y, idx, 2);
	idx += sizeof(int16_t);

	/*if(success)
	 return 0;

	else
	 return -1;*/

	return 0;
    }

public:
    char name[80];

    int16_t x;
    int16_t y;
};

int main(int argc, char **argv)
{
	//////////////////////////////////////////////////////////Crear
	Jugador one("ready_player_one", 15, 300);

	int fd = open("PRUEBA", O_CREAT | O_WRONLY);

	if(fd == -1){
		std::cout << "Error al abrir el archivo" << "\n";
		return 0;
	}
	
	one.to_bin();
	ssize_t w = write(fd, one.data(), one.size());

	if(w == -1) {
	
		std::cout << "Error al escribir" << "\n";
		return 0;
	}

	close(fd);
	
	

	//////////////////////////////////////////////////////////Leer

	Jugador playerCopy(" ", 0, 0);

	int fdr = open("PRUEBA", O_RDONLY);
	if(fdr == -1){
		std::cout << "Error al abrir el archivo" << "\n";
		return 0;
	}

	//tamaño
	int32_t tam;
	read(fdr, &tam, sizeof(int32_t));

	lseek(fdr, 0, SEEK_SET);

	//data aux
	char* aux = (char*)malloc(tam);
	read(fdr, aux, tam);

	playerCopy.from_bin(buff);
	
	close(fdr);
	
	std::cout << playerCopy.name << " " << playerCopy.x << std::endl;

	return 0;

}

