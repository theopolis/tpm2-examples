CXX = clang++
CXXFLAGS = -Wall -g -std=c++11
LFLAGS = -L /usr/local/lib/ -ltpm2sapi -ltpm2tctisock

all: tpm2_pcrs

tpm2_pcrs: tpm2_pcrs.o
	$(CXX) $(CXXFLAGS) $(LFLAGS) tpm2_pcrs.o -o tpm2_pcrs

tpm2_pcrs.o: tpm2.h tpm2_pcrs.cpp
	$(CXX) $(CXXFLAGS) -c tpm2_pcrs.cpp

clean:
	rm -f tpm2_pcrs *.o

format:
	clang-format -i *.cpp
