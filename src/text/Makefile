CC			=	g++ -ggdb -Wall
INCLUDE		=	
CXXFILES	=	text.cpp 

OBJ_DIR		=	.obj
SOBJ_DIR	=	.shared
TOBJ_DIR	=	.testobj

MKDIRS		=	$(OBJ_DIR) $(SOBJ_DIR) $(TOBJ_DIR)

OFILES   	= 	$(CXXFILES:%.cpp=$(OBJ_DIR)/%.o)
SOFILES	 	= 	$(CXXFILES:%.cpp=$(SOBJ_DIR)/%.o)
TOFILES	 	= 	$(CXXFILES:%.cpp=$(TOBJ_DIR)/%.o)

all: libtext.so libtext.a

.SUFFIXES: .o .cpp
.cpp.o:
	$(CC) -c $(INCLUDE) $< -o $@
$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CC) -c $(INCLUDE) $< -o $@
$(TOBJ_DIR)/%.o: %.cpp | $(TOBJ_DIR)
	$(CC) -c $(INCLUDE) $< -o $@
$(SOBJ_DIR)/%.o: %.cpp | $(SOBJ_DIR)
	$(CC) -fPIC -c $(INCLUDE) $< -o $@

libtext.a: $(OFILES) 
	ar rcv $@ $(OFILES)
libtext.so: $(SOFILES)
	$(CC) -shared -o $@ $^

$(TOBJ_DIR)/wordlist_find.o: wordlist_find.cpp | $(TOBJ_DIR)
	$(CC) -D TEST_WORD_LIST_FIND -c $(INCLUDE) $< -o $@

debug_wordlistfind: $(TOBJ_DIR)/wordlist_find.o
	$(CC) $(INCLUDE) $^ -o $@

$(sort $(MKDIRS)):
	mkdir -p $@

.PHONY: clean
clean:
	rm -f *.o
	rm -f *.a
	rm -f *.so
	rm -f debug_*
	rm -rf $(MKDIRS)
