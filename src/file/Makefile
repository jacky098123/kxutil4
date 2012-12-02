CC			=	g++ -ggdb -Wall
INCLUDE		=	
CXXFILES	=	file.cpp  

OBJ_DIR		=	.obj
SOBJ_DIR	=	.shared

MKDIRS		=	$(OBJ_DIR) $(SOBJ_DIR)

OFILES   	= 	$(CXXFILES:%.cpp=$(OBJ_DIR)/%.o)
SOFILES	 	= 	$(CXXFILES:%.cpp=$(SOBJ_DIR)/%.o)

all: libfile.so libfile.a

.SUFFIXES: .o .cpp
.cpp.o:
	$(CC) -c $(INCLUDE) $< -o $@
$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CC) -c $(INCLUDE) $< -o $@
$(SOBJ_DIR)/%.o: %.cpp | $(SOBJ_DIR)
	$(CC) -fPIC -c $(INCLUDE) $< -o $@

libfile.a: $(OFILES) 
	ar rcv $@ $(OFILES)
libfile.so: $(SOFILES)
	$(CC) -shared -o $@ $^

$(sort $(MKDIRS)):
	@mkdir -p $@

.PHONY: clean
clean:
	rm -f *.o
	rm -f *.a
	rm -f *.so
	rm -rf $(MKDIRS)
