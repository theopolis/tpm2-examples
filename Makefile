CXX = clang++
CXXFLAGS = -Wall -g -std=c++11
LFLAGS = -L /usr/local/lib/ -ltpm2sapi -ltpm2tctisock

all: tpm2_pcrs tpm2_info

tpm2_info: tpm2.o tpm2_info.o
	$(CXX) $(CXXFLAGS) $(LFLAGS) tpm2.o tpm2_info.o -o tpm2_info

tpm2_info.o: tpm2.h tpm2_info.cpp
	$(CXX) $(CXXFLAGS) -c tpm2_info.cpp

tpm2_pcrs: tpm2.o tpm2_pcrs.o
	$(CXX) $(CXXFLAGS) $(LFLAGS) tpm2.o tpm2_pcrs.o -o tpm2_pcrs

tpm2_pcrs.o: tpm2.h tpm2_pcrs.cpp
	$(CXX) $(CXXFLAGS) -c tpm2_pcrs.cpp

tpm2.o:
	$(CXX) $(CXXFLAGS) -c tpm2.cpp

clean:
	rm -f tpm2_pcrs *.o

format:
	clang-format -i *.cpp *.h
