all : system_d

system_d : core.cpp main.cpp interconnect.cpp  vp.cpp loader.cpp memory.cpp
	g++ -std=c++11 -DSC_INCLUDE_FX -w -g -I ${SYSTEMC}/include -L${SYSTEMC}/lib-linux64 -lsystemc -o system_d core.cpp main.cpp interconnect.cpp vp.cpp loader.cpp memory.cpp

.PHONY: clean
clean:
	rm system_*
