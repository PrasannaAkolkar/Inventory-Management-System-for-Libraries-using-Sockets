CXX = g++
CXXFLAGS = -Wall

all: serverH serverL serverS serverM client

serverM: serverM.cpp
	$(CXX) $(CXXFLAGS) -o serverM serverM.cpp

serverH: serverH.cpp
	$(CXX) $(CXXFLAGS) -o serverH utility.cpp serverH.cpp

serverL: serverL.cpp
	$(CXX) $(CXXFLAGS) -o serverL utility.cpp serverL.cpp

serverS: serverS.cpp
	$(CXX) $(CXXFLAGS) -o serverS utility.cpp serverS.cpp

client: client.cpp
	$(CXX) $(CXXFLAGS) -o client client.cpp

clean:
	rm -f serverH serverL serverS serverM client
