EE450

Name: Prasanna Akolkar
Student ID: 5509636102

Title: Library Management System
Programming Language: C++

Code Tested On: UTM Machine (Due to MAC)

Project Description:

Compulsory Section:

Main Components: 1 main server (serverM), 3 backend servers (serverS, serverL, serverH), and 1 client.
Connection: serverM and client are connected via TCP protocol. Each backend server is connected with serverM via UDP protocol.

Functionality:

User can login from the client terminal.
Username and password are encrypted as per the requirements document.
Authenticated users (without admin rights) can query books using the book code.
Authentication failure leads to a prompt for re-authentication on the client terminal.
User can check the availability of a book by entering the book code on the terminal.


Optional Section (For extra credit)
Admin Access: Users logging in with admin credentials (username: admin, password: Admin) can check the count of queried book codes for inventory management.
Restriction: Only admin users can access book count; others receive only availability status.
I have also answered queries on piazza for extra credit

How to Run the Code:

Execute make all to generate executable files for all .cpp files.
Run serverM, followed by serverS, serverH, and serverL (in any order), and then the client.
Example Order:
./serverM
./serverS
./serverH
./serverL
./client
Ensure serverM is operational before running the client. 
If you want to run for different user, close the client and serverM terminals and start them again.


Code Files:

serverM.cpp: Main server file handling client authentication and book query requests.
serverS.cpp: Backend server for the Science department, handling requests for books starting with 'S'.
serverL.cpp: Backend server for the Literature department, handling requests for books starting with 'L'.
serverH.cpp: Backend server for the History department, handling requests for books starting with 'H'.
client.cpp: User interface for inputs and sending data to the main server.
utility.cpp: Common functions used across backend servers.
utility.h: Header file for utility.cpp containing function prototypes.

Format of Messages Exchanged:
Delimiter: Messages are delimited by “||--||”.
Example: Username "Hello" and password "World" sent as "Hello||--||World". The server splits the message using this delimiter for processing.


Idiosyncrasy of the Project:
No noticeable idiosyncrasy of the project (works according to the requirements of the project)



Reused Code:
The code for setting up sockets has been borrowed from various sources like Beej, Geeks For Geeks. This is mainly the boilerplate code which is used for setting up connections. (
Code block for trimming strings has been borrowed from https://www.appsloveworld.com/cplus/100/1/how-to-trim-a-stdstring
Code block for reading contents from file has been referred from https://stackoverflow.com/questions/13035674/how-to-read-a-file-line-by-line-or-a-whole-text-file-at-once
