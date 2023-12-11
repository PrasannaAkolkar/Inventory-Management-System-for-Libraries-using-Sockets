#include <cstring>
#include <iostream>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include<fstream>
#include<bits/stdc++.h>
#include <sstream>
#include <typeinfo>


using namespace std;
#define PORT 45102

int serverMPortTCP = 45000 + 102;
int serverMPortUDP = 44000 + 102;
int serverSUDPPort = 41000 + 102;
int serverHUDPPort = 43000 + 102;
int serverLUDPPort = 42000 + 102;

int tcpSock, new_socket;

string delim = "||--||";

string isAdmin = "0";
// string bookCode = "";

// response string generation depending upon the availability and user rights
string decideClientResponseForBookCode(string bookCode, string bookCodeRespAvailability, string bookCodeRespCount){
    
    string response = "";

    if(isAdmin == "1"){
        if(bookCodeRespAvailability == "A"){

            cout<<"Number of books "<<bookCode<<" available is "<<bookCodeRespCount<<endl;
            response = bookCodeRespAvailability + delim +bookCodeRespCount;

        }else if(bookCodeRespAvailability == "NA"){

            cout<<"Number of books "<<bookCode<<" available is "<<bookCodeRespCount<<endl;
            response = bookCodeRespAvailability + delim + bookCodeRespCount;

        }else if(bookCodeRespAvailability == "DNE"){

            cout<<"Did not find "<<bookCode<<" in the book code list"<<endl;
            response = bookCodeRespAvailability + delim + bookCodeRespCount;

        }
    }else{
        if(bookCodeRespAvailability == "A"){

            cout<<"Book "<<bookCode<<" is available "<<endl;
            response = bookCodeRespAvailability;

        }else if(bookCodeRespAvailability == "NA"){

            cout<<"Book "<<bookCode<<" is not available "<<endl;
            response = bookCodeRespAvailability;

        }else if(bookCodeRespAvailability == "DNE"){

            // cout<<"Inside non admin"<<" and book code is "<<bookCode<<endl;

            cout<<"Did not find "<<bookCode<<" in the book code list"<<endl;
            response = bookCodeRespAvailability;

        }
    }
    return response;
}
unordered_map<string,string> fileContentsMapping;

string serverName(int port){
    unordered_map<int,string> portMap;
    portMap.insert(make_pair(41102, "S"));
    portMap.insert(make_pair(42102, "L"));
    portMap.insert(make_pair(43102, "H"));

    return portMap[port];

}

// https://www.appsloveworld.com/cplus/100/1/how-to-trim-a-stdstring
string removeSpacesTrailingLeading(string str){

    str.erase(0, str.find_first_not_of(" \t\n\r\f\v"));
    str.erase(str.find_last_not_of(" \t\n\r\f\v") + 1);

    return str;
}

bool checkIsAdmin(string username, string password){
    if(username == "firns" && password == "Firns")
        return true;
    return false;
}

void manipulateString(string line){

    
    size_t index = line.find(",");

    if(index!=string::npos){
        string username = line.substr(0,index);
        string password = line.substr(index+1);

        username = removeSpacesTrailingLeading(username);
        password = removeSpacesTrailingLeading(password);

        fileContentsMapping[username] = password;
    }

}

void loadMemberFile(){

    fstream file;
    file.open("member.txt", ios::in); 

    if(file.is_open()){
        string line;
        while(getline(file,line)){
            manipulateString(line);
        }
    }
    file.close(); 
}

bool checkUsername(string username){

    for (const auto &pair : fileContentsMapping) {

        if(pair.first == username){

            return true;
       }
    }
    return false;
}

bool checkPassword(string password){

    for (const auto &pair : fileContentsMapping) {

        if(pair.second == password){

            return true;
       }
    }
    return false;
}
// decides which server should the redirection be done
int redirectToServer(string bookCode){
    if(bookCode[0] == 'L'){
        return serverLUDPPort;
    }
    else if(bookCode[0] == 'S'){
        return serverSUDPPort;
    }
    else if(bookCode[0] == 'H'){
        return serverHUDPPort;
    }
    else return 0;
}

char * convertStringToChar(string str){
    return &str[0];
}

string convertCharToString(char *ch){
    string str = ch;
    return str; 
}


vector<string> splitStringOnDelimeter(string str){

    size_t pos = str.find(delim);
    vector<string> res;
    if(pos!=string::npos){
        res.push_back(str.substr(0, pos));
        res.push_back(str.substr(pos + delim.length()));
    }

    return res;
}
// receives the username and password from the user and performs authentication and sends authentication result to the client
int tcpReceiveUsernamePassword(int clientSocket, int flag, const char* authResponse, char buffer[]){

    do{

        memset(buffer, 0, 1024);
        int valRead = read(clientSocket, buffer, 1024);
        // cout<<"Doing val read"<<valRead<<endl;
        

        if(valRead == 0){
            return 0;
        }

        
        cout << "Main Server received the username and password from the client using TCP over port "<<serverMPortTCP<<endl;



        vector<string> splitUsernamePasswordArray = splitStringOnDelimeter(convertCharToString(buffer));

        string username = splitUsernamePasswordArray[0];
        string password = splitUsernamePasswordArray[1];

        // cout<<"Received username and password "<<username<<"  "<<password<<endl;

        bool isUsernameTrue = checkUsername(username);
        bool isPasswordTrue = checkPassword(password);
    
        if(!isUsernameTrue && !checkIsAdmin(username, password)){
        

            flag = 1;
            authResponse = "1";
            cout<<username<<" is not registered. Send a reply to the client\n";

        }else if(!isPasswordTrue && !checkIsAdmin(username, password)){
        
            
            flag = 2;
            authResponse = "2";
            cout<<"Password "<<password<<" does not match the username. Send a reply to the client.\n";
        }
        else if(isUsernameTrue && isPasswordTrue){

            flag = 3;
            authResponse = "3";
            cout<<"Password "<<password<<" matches the username. Send a reply to the client.\n";
        }
        
        if(checkIsAdmin(username, password)){ // send authenticated as well as admin

            flag = 3;
            authResponse = "4";

            // cout<<"Admin Rights to the user"<<endl;
            isAdmin = "1";
        }

        cout<<"\n";
        send(clientSocket, authResponse, strlen(authResponse), 0);
    }while(flag!=3);

    return 1;
    
}


int furtherOperations(int clientSocket){
    
    int flag = 0;
    const char* authResponse = "0";
    char buffer[1024] = {0};

    int x = tcpReceiveUsernamePassword(clientSocket, flag, authResponse, buffer);
    // if the client disconnected absurdly
    if(x==0){
        // cout<<"X is "<<x<<endl;
        return x;
    }

    struct sockaddr_in receiverUDPServerAddr, serverMAddress;
    int udpSocket = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&serverMAddress, 0, sizeof(serverMAddress));

    serverMAddress.sin_family = AF_INET;
    serverMAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverMAddress.sin_port = htons(serverMPortUDP); 

    socklen_t serverMAddressLen = sizeof(serverMAddress);

    if (bind(udpSocket, (struct sockaddr*)&serverMAddress,serverMAddressLen) < 0)
        cout << "Binding of UDP Socket Failed"<<endl;

    memset(&receiverUDPServerAddr, 0, sizeof(receiverUDPServerAddr));

    receiverUDPServerAddr.sin_family = AF_INET;
    receiverUDPServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    socklen_t udpServerLen = sizeof(receiverUDPServerAddr);

    while (1) {

        memset(buffer, 0, 1024);
        string bookCodeRespAvailability = "";
        string bookCodeRespCount = "";

        long valread = read(clientSocket, buffer, 1024);
        // if the client disconnected absurdly
        if (valread == 0) {
            x = 0;
            return x;
        }

        cout<<"Main Server received the book request from client using TCP over port "<<serverMPortTCP<<endl;

        int redirectPort = 0;
        string bookCode = "";

        if(sizeof(buffer)>0){
            redirectPort = redirectToServer(convertCharToString(buffer));

            string temp = convertCharToString(buffer);
            bookCode = temp;
            temp = temp + delim + isAdmin;
            
            // cout<<"Must be redirected to server - " <<redirectPort<<endl;
            if(redirectPort != 0){

                cout<<"Found "<<buffer<<" located at server "<<serverName(redirectPort)<<". Send to server "<<serverName(redirectPort)<<endl;
                
                receiverUDPServerAddr.sin_port = htons(redirectPort);
                // string temp = convertCharToString(buffer);
                // bookCode = temp;
                // temp = temp + delim + isAdmin;
                
                if (temp.size() < sizeof(buffer)) {
                    strncpy(buffer, temp.c_str(), sizeof(buffer) - 1);
                    buffer[sizeof(buffer) - 1] = '\0';
                }
               
                sendto(udpSocket, buffer, strlen(buffer), 0, (const struct sockaddr*)&receiverUDPServerAddr, udpServerLen);
                
                memset(buffer,0,sizeof(buffer));
                recvfrom(udpSocket, buffer, 1024, 0, (struct sockaddr*)&receiverUDPServerAddr, &udpServerLen);
                string response = string(buffer) + " in serverUdp";
                
                vector<string> bookCodeResp = splitStringOnDelimeter(convertCharToString(buffer));
                bookCodeRespAvailability = bookCodeResp[0];
                bookCodeRespCount = bookCodeResp[1];

                cout<<"Main Server received from server "<<serverName(redirectPort)<<" the book status result using UDP over port "<<serverMPortUDP<<endl;

                response = decideClientResponseForBookCode(bookCode, bookCodeRespAvailability, bookCodeRespCount);

                // cout<<"Number of books "<<bookCode<<" available is "<<bookCodeRespCount<<endl;
                send(clientSocket, response.c_str(), response.length(), 0);
                cout<<"Main server sent the book status to client"<<endl;
            }
            else{

                // cout<<"inside else Did not find "<<bookCode<<" in the book code list"<<endl;
                string response = "";


                response = decideClientResponseForBookCode(bookCode, "DNE", "0");
                send(clientSocket, response.c_str(), response.length(), 0);
                cout<<"Main server sent the book status to client"<<endl;
            }

        }
        else{
            cout<<"Empty buffer"<<endl;
            string response = "Book Code Not found";
            response = decideClientResponseForBookCode(bookCode, bookCodeRespAvailability, bookCodeRespCount);
            send(clientSocket, response.c_str(), response.length(), 0);
            cout<<"Main server sent the book status to client"<<endl;

        }

    }

    close(udpSocket);
    close(clientSocket);

    return x;

}

int main() {

    // code for sockets borrowed/referred from Geeks For Geeks 
    //https://www.geeksforgeeks.org/socket-programming-cc/
    struct sockaddr_in serverMAddress;
    socklen_t serverMAddressLen = sizeof(serverMAddress);

    if ((tcpSock = socket(AF_INET,SOCK_STREAM,0)) == 0) {
        cout<<"Socket creation failed"<<endl;
        exit(EXIT_FAILURE);
    }
    int option = 1;
    if (setsockopt(tcpSock, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &option, sizeof(option))) {
        cout<<"setsockopt failed"<<endl;
        exit(EXIT_FAILURE);
    }

    serverMAddress.sin_family = AF_INET;
    serverMAddress.sin_addr.s_addr = INADDR_ANY;
    serverMAddress.sin_port = htons(serverMPortTCP);

    if (bind(tcpSock, (struct sockaddr *)&serverMAddress, sizeof(serverMAddress))<0) {
        cout<<"Binding of the tcp socket failed"<<endl;
        exit(EXIT_FAILURE);
    }

    if (listen(tcpSock, 1) < 0)
        exit(EXIT_FAILURE);
    

    cout <<"Main Server is up and running."<< endl;
    loadMemberFile();
    cout <<"Main Server loaded the member list."<< endl;



    while(1) {
        // cout<<"Waiting for new connection"<<endl;
        if ((new_socket = accept(tcpSock, (struct sockaddr *)&serverMAddress, &serverMAddressLen))<0)
            continue;

        // cout<<"Socket in main server with client - "<<new_socket<<endl;

        int x = furtherOperations(new_socket);
        if(x==0)
            continue;
    }
    
    return 0;
}
