CXX=clang
CFLAGS= -c -Iinc -Wall -std=c++0x
LFLAGS= -lstdc++ -lm

SRC_DIR=src
SRC_FILES=  SingleItemStreams.cpp       \
            FStreams.cpp                \
            BufferedStreams.cpp         \
            MMappedStreams.cpp			\
            DWayMergeSorter.cpp			\
            QuickSort.cpp				\
            ExternalMergeSort.cpp		\
            HeapSort.cpp				\
            main.cpp					\
            tests/streamtests.cpp
                    
SRC=$(addprefix $(SRC_DIR)/,$(SRC_FILES))

OBJ_DIR=bin
OBJ_FILES=$(SRC_FILES:.cpp=.o);
OBJ=$(addprefix $(OBJ_DIR)/,$(OBJ_FILES))

EXEC=main

all : $(EXEC)

$(EXEC) : $(OBJ)
	@echo linking...
	@$(CXX) $(LFLAGS) $^ -o $(EXEC)


$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	@echo compiling $<...
	@$(CXX) $(CFLAGS) $< -o $@

compile: $(OBJ)

run: all
	@./$(EXEC)

clean:
	@echo removing binaries
	@rm -f $(OBJ_DIR)/*.o
	@echo removing executable
	@rm -f $(EXEC)
