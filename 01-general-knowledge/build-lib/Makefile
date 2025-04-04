

CUR_DIR := .
INC_DIR := $(CUR_DIR)/inc
SRC_DIR := $(CUR_DIR)/src
OBJ_DIR := $(CUR_DIR)/obj
BIN_DIR := $(CUR_DIR)/bin
LIB_DIR := $(CUR_DIR)/lib
LIB_STATIC_DIR := $(LIB_DIR)/static
LIB_SHARED_DIR := $(LIB_DIR)/shared




#create shared lib
create_obj_shared: # flag -I: determine .h directory
	gcc -c $(SRC_DIR)/main.c -o $(OBJ_DIR)/main.o -I $(INC_DIR)
	gcc -c -fPIC $(SRC_DIR)/mymath.c -o $(OBJ_DIR)/mymath.o -I $(INC_DIR)

create_shared_lib: # libmymath.so --> "lib" + "mymath" (name libs) + ".so"
	gcc -shared $(OBJ_DIR)/mymath.o -o $(LIB_SHARED_DIR)/libmymath.so

link_shared_lib: # flag -L: thu muc chua cac thu vien ; flag -l: chi dinh thu vien cu the
	gcc  $(OBJ_DIR)/main.o -L$(LIB_SHARED_DIR) -lmymath -o $(BIN_DIR)/main-use-shared-library
	
all_shared: create_obj_shared create_shared_lib link_shared_lib
	@echo "created shared bibrary"




#create static lib
create_obj_static: # flag -I: determine .h directory
	gcc -c $(SRC_DIR)/main.c -o $(OBJ_DIR)/main.o -I $(INC_DIR)
	gcc -c $(SRC_DIR)/mymath.c -o $(OBJ_DIR)/mymath.o -I $(INC_DIR)

create_static_lib: # libmymath.a --> "lib" + "mymath" (name libs) + ".a"
	ar rcs $(LIB_STATIC_DIR)/libmymath.a $(OBJ_DIR)/mymath.o

link_static_lib: # flag -L: thu muc chua cac thu vien ; flag -l: chi dinh thu vien cu the
	gcc $(OBJ_DIR)/main.o -L$(LIB_STATIC_DIR) -lmymath -o $(BIN_DIR)/main-use-static-library

all_static: create_obj_static create_static_lib link_static_lib
	@echo "created static bibrary"


clean:
	rm -rf $(BIN_DIR)/main-use-shared-library $(BIN_DIR)/main-use-static-library $(OBJ_DIR)/*.o $(LIB_SHARED_DIR)/libmymath.so $(LIB_STATIC_DIR)/libmymath.a
