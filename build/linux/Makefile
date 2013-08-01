FILEPATH=../../src
SOURCES_TMP=$(shell find ${FILEPATH} -name *.cpp)
SOURCES=$(patsubst $(FILEPATH)/%,%,${SOURCES_TMP})
SOURCES_C_TMP=$(shell find ${FILEPATH} -name *.c)
SOURCES_C=$(patsubst $(FILEPATH)/%,%,${SOURCES_C_TMP})
SOURCES_CC_TMP=$(shell find ${FILEPATH} -name *.cc)
SOURCES_CC=$(patsubst $(FILEPATH)/%,%,${SOURCES_CC_TMP})
OBJS_TMP=$(patsubst %.cpp,%.o,$(SOURCES))
OBJS_CPP=$(patsubst %,objs/%,${OBJS_TMP})
OBJS_C_TMP=$(patsubst %.c,%.o,$(SOURCES_C))
OBJS_C=$(patsubst %,objs/%,${OBJS_C_TMP})
OBJS_CC_TMP=$(patsubst %.cc,%.o,$(SOURCES_CC))
OBJS_CC=$(patsubst %,objs/%,${OBJS_CC_TMP})
OBJS=${OBJS_CPP} ${OBJS_C} ${OBJS_CC}
INCLUDE=${FILEPATH}/../include

all: bin/apsis

bin/apsis: bin ${OBJS}
	gcc -o bin/apsis ${OBJS} -lm -lstdc++ -lGL -lSDL

objs/%.o: ${FILEPATH}/%.cc objs
	@mkdir -p $(dir $@)
	gcc -o $@ -c $< -I ${INCLUDE} -O3 -masm=intel

objs/%.o: ${FILEPATH}/%.c objs
	@mkdir -p $(dir $@)
	gcc -o $@ -c $< -I ${INCLUDE} -O3 -masm=intel

objs/%.o: ${FILEPATH}/%.cpp objs
	@mkdir -p $(dir $@)
	gcc -o $@ -c $< -I ${INCLUDE} -O3 -masm=intel

objs:
	mkdir -p objs

bin:
	mkdir -p bin

clean:
	rm -rf objs

realclean: veryclean

veryclean: clean
	rm -rf bin
