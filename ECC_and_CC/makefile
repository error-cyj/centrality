.PHONY: clean all
CFLAGS = -Wall  -O3 -w -g 
targets = TD_ECC
sources = TD_ECC.cpp int_undirected_graph.cpp SelEle.cpp graph.cpp
objects = TD_ECC.o int_undirected_graph.o SelEle.o graph.o

all: $(targets)
	@echo "Done!"

$(targets): $(objects)
	g++ $(CFLAGS) $(objects) -o $@

%.o: %.cpp
	g++ $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(targets)
 


# TD: TD.o SelEle.o int_undirected_graph.o graph.o 
# 	g++ -o TD TD.o int_undirected_graph.o graph.o SelEle.o
# TD.o: TD.h TD.cpp 
# 	g++ -std=c++11 -c TD.cpp

# BC: BC.o int_undirected_graph.o graph.o 
# 	g++ -o BC BC.o int_undirected_graph.o graph.o 
# BC.o: BC.h BC.cpp
# 	g++ -std=c++11 -c BC.cpp


# SelEle.o: SelEle.cpp SelEle.h 
# 	g++ -std=c++11 -c SelEle.cpp
# int_undirected_graph.o: int_undirected_graph.cpp int_undirected_graph.h
# 	g++ -std=c++11 -c int_undirected_graph.cpp
# graph.o: graph.cpp graph.h
# 	g++ -std=c++11 -c graph.cpp
# clean:
# 	rm -f *.o
	