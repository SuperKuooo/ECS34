CXX=g++

CUR_DIR = $(shell pwd)
INC_DIR = ./include
SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin
TESTBIN_DIR = ./testbin
LIB_DIR = ./lib
LIBCSV_DIR = ./libcsv-3.0.3

INCLUDE         += -I $(INC_DIR)
CFLAGS          +=  -Wall
XMLFLAG			= -lexpat
LDFLAGS         += 
TESTLDFLAGS     += -lgtest_main -lgtest  -lpthread
CPPFLAGS        += -std=c++14 
PROJ_NAME       = proj5
TESTROUTER_NAME = testrouter
LIBCSV_NAME     = libcsv.a
#MAIN_OBJ        = $(OBJ_DIR)/main.o
MAIN_OBJ        = $(OBJ_DIR)/speedtest.o

TESTROUTER_OBJ = $(OBJ_DIR)/testrouter.o

PROJ_OBJS     = $(OBJ_DIR)/CSVReader.o \
		$(OBJ_DIR)/CSVWriter.o \
		$(OBJ_DIR)/XMLReader.o \
		$(OBJ_DIR)/XMLWriter.o \
		$(OBJ_DIR)/MapRouter.o \
        $(OBJ_DIR)/StringUtils.o

all: $(LIB_DIR)/$(LIBCSV_NAME)
$(LIB_DIR)/$(LIBCSV_NAME): $(LIBCSV_DIR)/libcsv.la
	cd $(LIBCSV_DIR); make install; cd ..

$(LIBCSV_DIR)/MakeFile:
	cd $(LIBCSV_DIR); ./configure --prefix=$(CUR_DIR); cd ..

$(LIBCSV_DIR)/libcsv.la: $(LIBCSV_DIR)/MakeFile
	cd $(LIBCSV_DIR); make;cd ..

#all: directories testrouter $(BIN_DIR)/$(PROJ_NAME)
all: directories $(BIN_DIR)/$(PROJ_NAME)


testrouter: $(TESTBIN_DIR)/$(TESTROUTER_NAME)
	$(TESTBIN_DIR)/$(TESTROUTER_NAME)

$(BIN_DIR)/$(PROJ_NAME): $(PROJ_OBJS) $(MAIN_OBJ)
	$(CXX) $(MAIN_OBJ) $(PROJ_OBJS) $(LIB_DIR)/$(LIBCSV_NAME) $(XMLFLAG) -o $(BIN_DIR)/$(PROJ_NAME) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(LDFLAGS) 

$(TESTBIN_DIR)/$(TESTROUTER_NAME): $(PROJ_OBJS) $(TESTROUTER_OBJ)
	$(CXX) $(PROJ_OBJS) $(TESTROUTER_OBJ) $(LIB_DIR)/$(LIBCSV_NAME) $(XMLFLAG) -o $(TESTBIN_DIR)/$(TESTROUTER_NAME) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(TESTLDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(DEFINES) $(INCLUDE) -c $< -o $@
	
.PHONY: directories
directories:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)
	mkdir -p $(TESTBIN_DIR)
	
clean::
	-rm $(OBJ_DIR)/*.o $(INC_DIR)/*.*~ $(SRC_DIR)/*.*~
	cd $(LIBCSV_DIR);make clean; cd..
	
.PHONY: clean
