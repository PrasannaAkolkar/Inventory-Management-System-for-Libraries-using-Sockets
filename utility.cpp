#include "utility.h"
#include<iostream>
#include <bits/stdc++.h>
#include<fstream>
#include<string>

#include <signal.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>

using namespace std;
int serverMPortUDP = 44000 + 102;
string delim = "||--||";

unordered_map<string,int> fileContentsMapping;

string serverName(int port){ // maps server port to the server initial
	unordered_map<int,string> portMap;
	portMap.insert(make_pair(41102, "S"));
	portMap.insert(make_pair(42102, "L"));
	portMap.insert(make_pair(43102, "H"));

	return portMap[port];

}

vector<string> splitStringOnDelimiter(string str, string delim){

    size_t pos = str.find(delim);
    vector<string> res;
    if(pos!=string::npos){
    	res.push_back(str.substr(0, pos));
    	res.push_back(str.substr(pos + delim.length()));
    }

    return res;
}

string convertCharToString(char *ch){
    string str = ch;
    return str; 
}

// string removeSpacesTrailingLeading(string str){ // referred from https://www.appsloveworld.com/cplus/100/1/how-to-trim-a-stdstring

// 	str.erase(0, str.find_first_not_of(' '));
// 	str.erase(str.find_last_not_of(' ') + 1);

// 	return str;

// }

string removeSpacesTrailingLeading(string str){

    str.erase(0, str.find_first_not_of(" \t\n\r\f\v"));
    str.erase(str.find_last_not_of(" \t\n\r\f\v") + 1);

    return str;
}

// getting the book Id and Book count
void manipulateString(string line){

	
	size_t index = line.find(",");

	if(index!=string::npos){
		string bookId = line.substr(0,index);
		string bookCount = line.substr(index+1);

		bookId = removeSpacesTrailingLeading(bookId);
		bookCount = removeSpacesTrailingLeading(bookCount);

		int bookCountIntVal = stoi(bookCount);
		fileContentsMapping[bookId] = bookCountIntVal;
	}


}

// to read from file and perform necessary manipulation
// https://stackoverflow.com/questions/13035674/how-to-read-a-file-line-by-line-or-a-whole-text-file-at-once
void processFile(string FILENAME){

	fstream file;
	file.open(FILENAME, ios::in); 

	if(file.is_open()){
		string line;
		while(getline(file,line)){
			manipulateString(line);
		}
	}
	file.close(); 
}

// update book count
int bookCodeCount(string bookCode){

	int bookCount = 0;

	for (const auto &pair : fileContentsMapping) {

		if(bookCode == pair.first){

			bookCount = pair.second;
			fileContentsMapping[bookCode]--;
		}

    }

    return bookCount;

}
// only queries book count, does not update the count
int queryBookCount(string bookCode){

	int bookCount = 0;

	for (const auto &pair : fileContentsMapping) {

		if(bookCode == pair.first){

			bookCount = pair.second;
		}

    }

    return bookCount;

}

bool doesBookExist(string bookCode){
	for (const auto &pair : fileContentsMapping) {
		if(bookCode == pair.first){
			return true;
		}
	}
	return false;
}

void initUDPSocket(string FILENAME, int serverPort){

	unordered_map<int,string> portMap;

	portMap.insert(make_pair(41102, "S"));
	portMap.insert(make_pair(42102, "L"));
	portMap.insert(make_pair(43102, "H"));


	struct sockaddr_in serverLAddress, serverMAddress;

	int newSocket = socket(AF_INET, SOCK_DGRAM, 0);

	int lengthOfServerMAddress = sizeof(serverMAddress);
	int lengthOfServerLAddress = sizeof(serverLAddress);

	serverLAddress.sin_family = AF_INET;
    serverLAddress.sin_addr.s_addr = INADDR_ANY;
    serverLAddress.sin_port = htons(serverPort);

    char buffer[1024];

    
    bind(newSocket, (const struct sockaddr*)&serverLAddress, lengthOfServerLAddress);

    cout<<"Server "<<serverName(serverPort)<<" is up and running using UDP on port "<<serverPort<<endl;

    processFile(FILENAME);

    while(1){

    	// socket code referred from https://www.geeksforgeeks.org/udp-server-client-implementation-c/
    	
    	string outputMessage = "";
    	int data = recvfrom(newSocket,buffer,1024,MSG_WAITALL,(struct sockaddr*)&serverMAddress,(socklen_t*)&lengthOfServerMAddress);
    	buffer[data] = '\0';

    	string bufferVal = convertCharToString(buffer);

    	vector<string> bufferVectorSplit = splitStringOnDelimiter(bufferVal, delim);
		
    	string isAdmin = bufferVectorSplit[1];
    	bufferVal = bufferVectorSplit[0];

    	string resp = "";
    	// if admin user, proper count of books need to be sent
    	if(isAdmin == "1"){

    		cout<<"Server "<<serverName(serverPort)<<" received an inventory status request for code "<<bufferVal<<endl;
    		outputMessage = "Server " +  (serverName(serverPort)) + " finished sending the inventory status to the Main Server using UDP on port " + to_string(serverPort);

    		if(doesBookExist(bufferVal)){
    			int bookCount = queryBookCount(bufferVal);
    			if(bookCount<=0){
    				resp = "NA" + delim + "0"; // book not available
    			}else{
    				resp = "A" + delim + to_string(bookCount); // book available
    			}
    		}else{

    			resp = "DNE" + delim + "-1"; // if the book does not exist
    		}

    	}else{

    	cout<<"Server "<<serverName(serverPort)<<" received "<<bufferVal<<" code from the Main Server"<<endl;
    	outputMessage = "Server " +  (serverName(serverPort)) + " finished sending the availability status of code " + bufferVal + " to the Main Server using UDP on port " + to_string(serverPort);
    	if(doesBookExist(bufferVal))
		{
			int bookCount = bookCodeCount(bufferVal);
			if(bookCount<=0){
				resp = "NA" + delim + "0";
			}
			else{
				resp = "A" + delim + "0";
				// resp = "A" + delim + to_string(bookCount);
			}
		}

		else{
			resp = "DNE" + delim + "-1";
		}
	}

    	sendto(newSocket,resp.c_str(),resp.length(),0,(const struct sockaddr*)&serverMAddress,lengthOfServerMAddress);
    	cout<<outputMessage<<endl;
    }
    close(newSocket);
}