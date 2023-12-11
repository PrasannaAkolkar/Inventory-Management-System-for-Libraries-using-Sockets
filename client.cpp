#include <cstring>
#include <iostream>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<string>
#include<bits/stdc++.h>
// #include <netinet/in.h>

using namespace std;

int serverMPort = 45000 + 102;

int clientPort = 0;

int clientSock=0;

bool Admin = false;

string username, password;
string unencryptedUsername, unencryptedPassword;

string delim = "||--||";

// encrypt the username and password according to the rules
string encryptUsernameAndPassword(string username){

    string str = "";
    
    for(auto c:username){
        if(c>='a' && c<='z'){
            char ch = 'a' + (c-'a'+5)%26;
            str+=ch;
        }
        else if(c>='A' && c<='Z'){
            char ch = 'A' + (c-'A'+5)%26;
            str+=ch;
        }
        else if(c>='0' && c<='9'){
            char ch = '0' + (c-'0'+5)%10;
            str+=ch;
        }
        else{
            str+=c;
        }
    }
    return str;
}

// checks if the user is authenticated or not
bool takeUserInputAndCheckIfAuthenticated(int sock){
    username = "";
    password = "";
    cout<<"Please enter the username"<<endl;
    getline(cin, username);
    cout<<"Please enter the password"<<endl;
    getline(cin, password);

    unencryptedUsername = username;
    unencryptedPassword = password;

    // cout<<"Entered username is - "<<username<<endl;
    // cout<<"Entered password is - "<<password<<endl;

    username = encryptUsernameAndPassword(username);
    password = encryptUsernameAndPassword(password);

    string concatenatedUsernamePassword = username+delim+password;

    send(sock, concatenatedUsernamePassword.c_str(), concatenatedUsernamePassword.length(), 0);
    cout<<unencryptedUsername<<" sent an authentication request to the Main Server"<<endl;
    char buffer[1024] = {0};
    read(sock, buffer, 1024);

    if(buffer[0] == '3')
    {
        cout<<unencryptedUsername<<" received the result of authentication from Main Server using TCP over port "<<clientPort<<" Authentication is successful "<<endl;
        Admin = false;
        return true;
    }
    else if(buffer[0] == '1')
    {
        cout<<unencryptedUsername<<" received the result of authentication from Main Server using TCP over port "<<clientPort<<" Authentication failed: Username not found "<<endl;

        return false;
    }
    else if(buffer[0] == '2')
    {
        cout<<unencryptedUsername<<" received the result of authentication from Main Server using TCP over port "<<clientPort<<" Authentication failed: Password does not match "<<endl;

        return false;
    }
    else if(buffer[0] == '4')
    {
        cout<<unencryptedUsername<<" received the result of authentication from Main Server using TCP over port "<<clientPort<<" Authentication is successful "<<endl;
        Admin = true;
        return true;
    }
    cout<<"Some unexpected error, user could not be authenticated"<<endl;
    return false;

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
// checks the data returned from the main server
void processBookRequest(string str, string bookCode){

    if(Admin){

        vector<string> bookReq = splitStringOnDelimiter(str,delim);
        string availability = bookReq[0];
        string count = bookReq[1];

        if(availability == "DNE"){
            cout<<"Not able to find the book-code "<<bookCode<<" in the system"<<endl;
        }else{
            cout<<"Total number of book "<<bookCode <<" available = "<<count<<endl;
        }
    }else{

        string availability = str;

        if(availability == "A"){
            cout<<"The requested book "<<bookCode<<" is available in the library"<<endl;
        }else if(availability == "NA"){
            cout<<"The requested book "<<bookCode<<" is NOT available in the library"<<endl;
        }else{
            cout<<"Not able to find the book-code "<<bookCode<<" in the system"<<endl;
        }
    }
}

void requestBook(int clientSock){

    string bookCode;
    do{
        cout<<"Please enter the book code to query:"<<endl;
        getline(cin, bookCode);
    }while(bookCode.empty());
    char buffer[1024] = {0};

    send(clientSock, bookCode.c_str(), bookCode.length(), 0);
    if(!Admin)
        cout<<unencryptedUsername<<" sent the request to the Main Server"<<endl;
    else
        cout<<unencryptedUsername<<" sent the request to the Main Server with Admin Rights"<<endl;
    memset(buffer, 0, 1024);
    read(clientSock, buffer, 1024);
    cout<<"Response received from the Main Server on TCP Port: "<<clientPort<<endl;
    // cout << "Server: " << buffer << endl;
    processBookRequest(string(buffer),bookCode);

}

void getClientSocket(int clientSock){

    struct sockaddr_in local_address;
    socklen_t address_length = sizeof(local_address);

    if(getsockname(clientSock, (struct sockaddr*)&local_address, &address_length) == 0) 
        clientPort = (int) ntohs(local_address.sin_port);
    else 
        perror("Failure in getsockname");
}

int main(){

    // code for sockets referred from https://www.geeksforgeeks.org/socket-programming-cc/

	struct sockaddr_in serverMAddress;

	if ((clientSock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		cout<<"Socket could not be created";
        return -1;
    }
    serverMAddress.sin_family=AF_INET;
    serverMAddress.sin_port=htons(serverMPort);

    if(inet_pton(AF_INET, "127.0.0.1", &serverMAddress.sin_addr) <= 0) {
    	cout<<"Address invalid";
        return -1;
    }

    if (connect(clientSock, (struct sockaddr *)&serverMAddress, sizeof(serverMAddress)) < 0) {
    	cout<<"Connection Failed";
        return -1;
    }

    getClientSocket(clientSock);

    cout<<"Client is up and running"<<endl;

    int authenticated = false;

    // continue until the client is authenticated
    do{
        authenticated = takeUserInputAndCheckIfAuthenticated(clientSock);

    }while(!authenticated);

    // keep asking for book code until terminated by the user from the terminal
    while(1){
        requestBook(clientSock);
    }

     close(clientSock);
}

