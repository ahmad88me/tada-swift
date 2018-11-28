HEADERS = tada_swift.h features.h 
NOMOBJS =  tada_swift.o features.o # no main objects
OBJS =  $(NOMOBJS) main.o 
TOBJS = $(NOMOBJS) tests.o
NOMSOURCES = tada_swift.cpp features.cpp # sources excluding main.cpp
SOURCES = $(NOMSOURCES) main.cpp
TSOURCES = $(NOMSOURCES) tests.cpp # test sources
CXXFLAGS = -I /usr/local/include/eigen3  -std=c++11  
CC = g++
OBJ_DIR = build
SRC_DIR = src
BIN_DIR = bin
LIBS = -lfcm -pthread -leasylogger -ltabularparser
#LIBS = -pthread
TLIBS = $(LIBS) -lgtest
TESTAPP = bin/testtadaswift
COVAPP = bin/covtadaswift
COVCLEANFILES = gcov.css snow.png ruby.png *.gcov  *.gcda *.gcno index-sort-f.html index-sort-l.html index.html \
				amber.png glass.png updown.png coverage.info emerald.png Users usr v1\

OBJS_ABS = $(patsubst %,$(OBJ_DIR)/%,$(OBJS))
TOBJS_ABS = $(patsubst %,$(OBJ_DIR)/%,$(TOBJS))
SOURCES_ABS = $(patsubst %,$(SRC_DIR)/%,$(SOURCES))
TSOURCES_ABS = $(patsubst %,$(SRC_DIR)/%,$(TSOURCES))


$(BIN_DIR)/tadaswift : $(OBJS_ABS)
	$(CC) -o $@ $? $(LIBS)

$(OBJS_ABS): $(SOURCES_ABS)
	$(CC) $(CXXFLAGS) -c $(SOURCES_ABS)
	mv *.o build/

.PHONY: clean run debug test cov cleancov  codecov  covnoclean

debug: 
	$(CC) $(CXXFLAGS) -g -c $(SOURCES_ABS)
	mv *.o build/
	$(CC) -g  -o bin/tadaswift $(OBJS_ABS) $(LIBS)
	valgrind --tool=massif bin/tadanum --time-unit=B --stacks=yes

test:
	$(CC) $(CXXFLAGS) -c $(TSOURCES_ABS)
	mv *.o build/
	$(CC) $(CXXFLAGS) -o $(TESTAPP) $(TOBJS_ABS) $(TLIBS)
	$(TESTAPP)

covnoclean:
	$(CC) $(CXXFLAGS) -c -fprofile-arcs -ftest-coverage -fPIC  $(TSOURCES_ABS)
	mv *.o build/
	$(CC) $(CXXFLAGS) -o $(COVAPP) -fprofile-arcs -ftest-coverage $(TOBJS_ABS) $(TLIBS) 
	$(COVAPP)
	lcov --directory . --capture --output-file coverage.info
	lcov --remove coverage.info '/usr/*' --output-file coverage.info
	lcov --list coverage.info

cov:
	$(MAKE) covnoclean
	$(MAKE) clean

codecov:
	$(MAKE) covnoclean
	curl -s https://codecov.io/bash > codecovpush.sh
	chmod +x codecovpush.sh
	./codecovpush.sh
	rm codecovpush.sh
	$(MAKE) clean	


cleancov:
	-rm -Rf $(COVCLEANFILES)
	
run: 
	bin/tadaswift ${ARGS}

clean:
	$(RM) -R $(BIN_DIR)/* 
	$(RM) -R $(OBJ_DIR)/* 
	$(MAKE) cleancov
