FILEPATH=../../src
LIBS=m stdc++ GL SDL dl
INCLUDE=${FILEPATH}/../include
CC=gcc
LD=${CC}
CPPC=${CC}

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

LIBS_CMD=$(patsubst %,-l%,$(LIBS))

DEADCODE_ELIM_FLAGS=-fdata-sections -ffunction-sections

CFLAGS=-O3 -masm=intel ${DEADCODE_ELIM_FLAGS}

all: bin/apsis

bin/apsis: bin ${OBJS}
	${LD} -o bin/apsis ${OBJS} ${LIBS_CMD} -Wl,--gc-sections -Wl,-s

objs/%.o: ${FILEPATH}/%.cc objs
	@mkdir -p $(dir $@)
	${CC} -o $@ -c $< -I ${INCLUDE} ${CFLAGS}

objs/%.o: ${FILEPATH}/%.c objs
	@mkdir -p $(dir $@)
	${CC} -o $@ -c $< -I ${INCLUDE} ${CFLAGS}

objs/%.o: ${FILEPATH}/%.cpp objs
	@mkdir -p $(dir $@)
	${CPPC} -o $@ -c $< -I ${INCLUDE} ${CFLAGS}

objs:
	mkdir -p objs

bin:
	mkdir -p bin

clean:
	rm -rf objs

realclean: veryclean

veryclean: clean
	rm -rf bin
