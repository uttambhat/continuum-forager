CC = g++
CFLAGS = -O2 -std=c++11 -lglut -lGLU -lGL

INC_DIR = include
LIB_DIR = lib
OBJ_DIR = build
SRC_DIR = src
BIN_DIR = bin

all: $(BIN_DIR)/main2.out

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.C $(INC_DIR)/resource_map.h $(INC_DIR)/forager_population.h $(INC_DIR)/generic/output_functions.h $(INC_DIR)/foraging_output_functions.h
	$(CC) -c -o $@ $< $(CFLAGS)
	
$(OBJ_DIR)/generic/output_functions.o: $(SRC_DIR)/generic/output_functions.C $(INC_DIR)/generic/output_functions.h
	$(CC) -c -o $@ $< $(CFLAGS)

$(OBJ_DIR)/foraging_output_functions.o: $(SRC_DIR)/foraging_output_functions.C $(INC_DIR)/foraging_output_functions.h $(INC_DIR)/forager_population.h
	$(CC) -c -o $@ $< $(CFLAGS)
	
$(OBJ_DIR)/forager_population.o: $(SRC_DIR)/forager_population.C $(INC_DIR)/forager_population.h $(INC_DIR)/forager.h
	$(CC) -c -o $@ $< $(CFLAGS)
	
$(OBJ_DIR)/forager.o: $(SRC_DIR)/forager.C $(INC_DIR)/forager.h $(INC_DIR)/resource_map.h
	$(CC) -c -o $@ $< $(CFLAGS)
	
$(OBJ_DIR)/resource_map.o: $(SRC_DIR)/resource_map.C $(INC_DIR)/resource_map.h
	$(CC) -c -o $@ $< $(CFLAGS)

$(BIN_DIR)/main2.out: $(OBJ_DIR)/main.o $(OBJ_DIR)/forager_population.o $(OBJ_DIR)/resource_map.o  $(OBJ_DIR)/forager.o $(OBJ_DIR)/generic/output_functions.o $(OBJ_DIR)/foraging_output_functions.o
	$(CC) -o $@ $^ $(CFLAGS)

	
clean:
	rm $(OBJ_DIR)/*.o
	
.PHONY: clean



