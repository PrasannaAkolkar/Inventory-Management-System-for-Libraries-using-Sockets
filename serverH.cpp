#include "utility.h"

int serverHPort = 43000 + 102;
int mainServerPort = 44000 + 102;

string FILENAME = "history.txt";

int main(){

	initUDPSocket(FILENAME, serverHPort);
	
	return 0;
}