CC=g++ -ggdb -Wall

ifndef INSTALL_DIR
INSTALL_DIR = $(PWD)/..
endif

ifdef DEEPLY
INSTALL_LIB=$(INSTALL_DIR)/lib
INSTALL_INC=$(INSTALL_DIR)/include/kxutil4
else
INSTALL_LIB=$(INSTALL_DIR)/lib
INSTALL_INC=$(INSTALL_DIR)/include
endif

SUBDIRS = db log sys file text encoding

init: libkxutil4.a initlib

libkxutil4.a: subdirs
	-rm -f *.a
	for sub in $(SUBDIRS); do \
		ar rcv $@ $$sub/.obj/*.o ; \
	done

libkxutil4.so: subdirs
	$(CC) -shared -o $@ */.shared/*.o

subdirs:
	for sub in $(SUBDIRS); do \
		make -C $$sub; \
	done

initlib: libkxutil4.a
	install -d ../include ../lib
	install -m 644 $^ ../lib
	for sub in $(SUBDIRS); do \
		install -d ../include/$$sub ; \
		install -m 644 $$sub/*hpp ../include/$$sub ; \
	done


clean:
	for sub in $(SUBDIRS); do \
		make -C $$sub clean ; \
		rm $$sub/tmp -rf ; \
	done
	rm -rf ../include
	rm -rf ../lib
	rm -f *.a *.so


bak:
	rm -f *.so *.a */*.o */*.a */*.so
	rm -rf */.obj/* */.shared/*
	rm -rf ../include
	rm -rf ../lib

install: 
	rm -rf ${INSTALL_LIB}
	rm -rf ${INSTALL_INC}
	mkdir -p ${INSTALL_LIB}
	mkdir -p ${INSTALL_INC}
	cp ../lib/*.a ${INSTALL_LIB}
	cp -rf ../include/* $(INSTALL_INC)
