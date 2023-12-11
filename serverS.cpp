#include "utility.h"

int serverSPort = 41000 + 102;
int mainServerPort = 44000 + 102;

string FILENAME = "science.txt";

int main(){

	initUDPSocket(FILENAME, serverSPort);
	
	return 0;
}