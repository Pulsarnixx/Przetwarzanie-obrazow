#COMPILER VERSION
CC = gcc
#COMPILER FLAGS
CCFLAGS = -Iinc -std=c99 -W -pedantic
#MAKEFILE OPTIONS
APP_NAME = PrzetwObraz
EX = .c
SRC = src/
OBJ = obj/
INC = inc/

#Creating executable program

start: $(OBJ)main.o $(OBJ)opcje.o $(OBJ)obs.o $(OBJ)fun.o 
	$(CC) -std=c99 -W -pedantic $(OBJ)main.o $(OBJ)fun.o $(OBJ)obs.o $(OBJ)opcje.o -o $(APP_NAME)

obj/main.o: $(SRC)main$(EX)
	$(CC)  -c -o $(OBJ)main.o $(SRC)main$(EX)

obj/fun.o: $(SRC)fun$(EX) $(INC)fun.h $(INC)str_obr2.h
	$(CC) -c -o $(OBJ)fun.o $(SRC)fun$(EX)

obj/obs.o: $(SRC)obs$(EX) $(INC)obs.h $(INC)str_obr2.h
	$(CC) -c -o $(OBJ)obs.o $(SRC)obs$(EX)

obj/opcje.o: $(SRC)opcje$(EX) $(INC)opcje.h $(INC)str_obr2.h
	$(CC) -c -o $(OBJ)opcje.o $(SRC)opcje$(EX)



#Cleaning all .o stuff
clean: 
	rm $(OBJ)*.o $(APP_NAME)









