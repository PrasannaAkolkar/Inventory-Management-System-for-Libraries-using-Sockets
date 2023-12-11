#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <vector>

using namespace std;

vector<string> splitStringOnDelimiter(string str, string delim);
string convertCharToString(char *ch);
string removeSpacesTrailingLeading(string str);
void manipulateString(string line);
void processFile(string FILENAME);
int bookCodeCount(string bookCode);
int queryBookCount(string bookCode);
bool doesBookExist(string bookCode);
void initUDPSocket(string FILENAME, int serverPort);

#endif
