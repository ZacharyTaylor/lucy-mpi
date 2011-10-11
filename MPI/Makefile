OBJS = lucy.o send_receive.o mpi_main.o files_io.o master_slave.o
CXX = mpicxx
DEBUG = -g
CXXFLAGS= -Wall -c $(DEBUG) -O2 -L/usr/X11R6/lib -lm -lpthread -lX11
LFLAGS = -Wall $(DEBUG) -lm -lX11

lucy-mpi : $(OBJS)
	$(CXX) $(LFLAGS) $(OBJS) -o lucy-mpi

lucy.o: lucy.cpp 
	$(CXX) $(CXXFLAGS) lucy.cpp

send_receive.o: send_receive.cpp 
	$(CXX) $(CXXFLAGS) send_receive.cpp

mpi_main.o: mpi_main.cpp 
	$(CXX) $(CXXFLAGS) mpi_main.cpp

files_io.o: files_io.cpp 
	$(CXX) $(CXXFLAGS) files_io.cpp

master_slave.o: master_slave.cpp 
	$(CXX) $(CXXFLAGS) master_slave.cpp

clean:
	\rm *.o lucy-mpi
