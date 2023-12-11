# Library Management System

## Author Information
- **Name:** Prasanna Akolkar
- **Student ID:** 5509636102

## Project Overview
A Library Management System developed in C++ for managing book queries and user authentication in a library setting. The system comprises a main server, three backend servers, and a client, employing TCP and UDP protocols for communication.

### Environment
- **Programming Language:** C++
- **Tested On:** UTM Machine (Due to MAC compatibility)

## Components
- **Main Server (serverM)**
- **Backend Servers:**
  - Science Department (serverS)
  - Literature Department (serverL)
  - History Department (serverH)
- **Client**

## Communication
- **Client - Main Server:** TCP Protocol
- **Backend Servers - Main Server:** UDP Protocol

## Functionality
- **User Login:** Secure login from client terminal.
- **Encryption:** Username and password are encrypted as per requirements.
- **Book Queries:** Authenticated users can query books using book codes.
- **Authentication Failure:** Prompts re-authentication on the client terminal.
- **Book Availability Check:** Users can check the availability of books.

## Optional Features (Extra Credit)
- **Admin Access:** Special credentials for admin access.
- **Inventory Management:** Admins can check the count of queried book codes.
- **Restrictions:** Only admins can access book count data.

## How to Run
1. Execute `make all` to generate executable files.
2. Run the servers in the following order (or any):
   - `./serverM`
   - `./serverS`
   - `./serverH`
   - `./serverL`
   - `./client`
3. Ensure `serverM` is operational before running the client. 
4. To run for a different user, close the client and `serverM` terminals and start them again.

## Code Files
- `serverM.cpp`: Main server file handling client authentication and book query requests.
- `serverS.cpp`: Backend server for the Science department.
- `serverL.cpp`: Backend server for the Literature department.
- `serverH.cpp`: Backend server for the History department.
- `client.cpp`: User interface for inputting data and communicating with the main server.
- `utility.cpp`: Common functions used across backend servers.
- `utility.h`: Header file for `utility.cpp` with function prototypes.

## Message Format
- **Delimiter:** Messages are delimited by `||--||`.
- **Example:** Username "Hello" and password "World" sent as `Hello||--||World`. The server splits the message using this delimiter for processing.

## Idiosyncrasy of the Project
No noticeable idiosyncrasies - the project works according to the requirements.

## Reused Code
- Socket setup code sourced from Beej's Guide and Geeks For Geeks.
- String trimming function from [Appsloveworld](https://www.appsloveworld.com/cplus/100/1/how-to-trim-a-stdstring).
- File reading code referenced from [Stack Overflow](https://stackoverflow.com/questions/13035674/how-to-read-a-file-line-by-line-or-a-whole-text-file-at-once).
