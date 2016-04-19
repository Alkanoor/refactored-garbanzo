OBJ_DIR = obj
SRC_DIR1 = .
SRC_DIR2 = src
SRC = $(wildcard $(SRC_DIR2)/*.cpp) $(wildcard $(SRC_DIR1)/*.cpp)
OBJ := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SRC)))
VPATH=.:src


RELEASE_DIR = bin
EXEC = $(RELEASE_DIR)/createDataset

OUT = data

PYTHON = python


CC = g++
CFLAGS = -Wall -Werror -O1 -std=c++11 -g
LDFLAGS =


all: createDir $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

createDir:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(RELEASE_DIR)
	@mkdir -p $(OUT)

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(RELEASE_DIR)
	@rm -rf $(OUT)

convert:
	$(RELEASE_DIR)/createDataset

python:
	python $(PYTHON)/final.py

find_best_parameters:
	python $(PYTHON)/find_best_parameters.py

