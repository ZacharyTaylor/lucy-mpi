OBJS = lucy.o omp_main.o files_io.o
CXX = mpicxx
DEBUG = -g
CXXFLAGS= -Wall -c $(DEBUG) -O2 -L/usr/X11R6/lib -lm -lpthread -lX11 -fopenmp
LFLAGS = -Wall $(DEBUG) -lm -lX11 -fopenmp

lucy-omp : $(OBJS)
	$(CXX) $(LFLAGS) $(OBJS) -o lucy-omp

lucy.o: lucy.cpp 
	$(CXX) $(CXXFLAGS) lucy.cpp

omp_main.o: omp_main.cpp 
	$(CXX) $(CXXFLAGS) omp_main.cpp

files_io.o: files_io.cpp 
	$(CXX) $(CXXFLAGS) files_io.cpp

clean:
	\rm *.o lucy-omp
