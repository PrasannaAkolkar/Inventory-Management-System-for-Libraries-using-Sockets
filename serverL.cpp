#include "utility.h"

int serverLPort = 42000 + 102;
int mainServerPort = 44000 + 102;

string FILENAME = "literature.txt";

int main(){

	initUDPSocket(FILENAME, serverLPort);
	
	return 0;
}