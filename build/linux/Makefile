FILEPATH=../../src
LIBS=m stdc++ GL SDL dl
INCLUDE=${FILEPATH}/../include
CC=gcc
LD=${CC}
CPPC=${CC}

SOURCES_CPP_TMP=$(shell find ${FILEPATH} -name *.cpp)
SOURCES_CPP=$(patsubst $(FILEPATH)/%,%,${SOURCES_CPP_TMP})
SOURCES_C_TMP=$(shell find ${FILEPATH} -name *.c)
SOURCES_C=$(patsubst $(FILEPATH)/%,%,${SOURCES_C_TMP})
SOURCES_CC_TMP=$(shell find ${FILEPATH} -name *.cc)
SOURCES_CC=$(patsubst $(FILEPATH)/%,%,${SOURCES_CC_TMP})
SOURCES=${SOURCES_CPP_TMP} ${SOURCES_C_TMP} ${SOURCES_CC_TMP}

OBJS_TMP=$(patsubst %.cpp,%.o,$(SOURCES_CPP))
OBJS_CPP=$(patsubst %,objs/%,${OBJS_TMP})
OBJS_C_TMP=$(patsubst %.c,%.o,$(SOURCES_C))
OBJS_C=$(patsubst %,objs/%,${OBJS_C_TMP})
OBJS_CC_TMP=$(patsubst %.cc,%.o,$(SOURCES_CC))
OBJS_CC=$(patsubst %,objs/%,${OBJS_CC_TMP})
OBJS=${OBJS_CPP} ${OBJS_C} ${OBJS_CC}

LIBS_CMD=$(patsubst %,-l%,$(LIBS))
INCLUDE_CMD=$(patsubst %,-I%,$(INCLUDE))

#DEADCODE_ELIM_FLAGS=-fdata-sections -ffunction-sections
DEADCODE_ELIM_FLAGS=

CPPFLAGS=-std=c++11 -O3 -masm=intel ${DEADCODE_ELIM_FLAGS}
CFLAGS=-O3 -masm=intel ${DEADCODE_ELIM_FLAGS}

all: bin/apsis

JSON_SOURCES_TMP=$(shell find ${FILEPATH}/../support/json -name *.cpp)
JSON_SOURCES=$(patsubst $(FILEPATH)/%,%,${JSON_SOURCES_TMP})
JSON_OBJS_TMP=$(patsubst %.cpp,%.o,$(JSON_SOURCES))
JSON_OBJS=$(patsubst %,objs/%,$(JSON_OBJS_TMP))
json.a: ${JSON_OBJS}
	ar rcs $@ $^

GLEW_SOURCES_TMP=$(shell find ${FILEPATH}/../support/glew -name *.c)
GLEW_SOURCES=$(patsubst $(FILEPATH)/%,%,${GLEW_SOURCES_TMP})
GLEW_OBJS_TMP=$(patsubst %.c,%.o,$(GLEW_SOURCES))
GLEW_OBJS=$(patsubst %,objs/%,$(GLEW_OBJS_TMP))
glew.a: ${GLEW_OBJS}
	ar rcs $@ $^

CLEW_SOURCES_TMP=$(shell find ${FILEPATH}/../support/clew -name *.c)
CLEW_SOURCES=$(patsubst $(FILEPATH)/%,%,${CLEW_SOURCES_TMP})
CLEW_OBJS_TMP=$(patsubst %.c,%.o,$(CLEW_SOURCES))
CLEW_OBJS=$(patsubst %,objs/%,$(CLEW_OBJS_TMP))
clew.a: ${CLEW_OBJS}
	ar rcs $@ $^

SOIL_SOURCES_TMP=$(shell find ${FILEPATH}/../support/soil -name *.c)
SOIL_SOURCES=$(patsubst $(FILEPATH)/%,%,${SOIL_SOURCES_TMP})
SOIL_OBJS_TMP=$(patsubst %.c,%.o,$(SOIL_SOURCES))
SOIL_OBJS=$(patsubst %,objs/%,$(SOIL_OBJS_TMP))
soil.a: ${SOIL_OBJS}
	ar rcs $@ $^

SDL_MIXER_SOURCES_TMP=$(shell find ${FILEPATH}/../support/SDL_mixer -name *.c)
SDL_MIXER_SOURCES=$(patsubst $(FILEPATH)/%,%,${SDL_MIXER_SOURCES_TMP})
SDL_MIXER_OBJS_TMP=$(patsubst %.c,%.o,$(SDL_MIXER_SOURCES))
SDL_MIXER_OBJS=$(patsubst %,objs/%,$(SDL_MIXER_OBJS_TMP))
sdl_mixer.a: ${SDL_MIXER_OBJS}
	ar rcs $@ $^

OGG_SOURCES_TMP=$(shell find ${FILEPATH}/../support/ogg -name *.c)
OGG_SOURCES=$(patsubst $(FILEPATH)/%,%,${OGG_SOURCES_TMP})
OGG_OBJS_TMP=$(patsubst %.c,%.o,$(OGG_SOURCES))
OGG_OBJS=$(patsubst %,objs/%,$(OGG_OBJS_TMP))
ogg.a: ${OGG_OBJS}
	ar rcs $@ $^

VORBIS_SOURCES_TMP=$(shell find ${FILEPATH}/../support/vorbis -name *.c)
VORBIS_SOURCES=$(patsubst $(FILEPATH)/%,%,${VORBIS_SOURCES_TMP})
VORBIS_OBJS_TMP=$(patsubst %.c,%.o,$(VORBIS_SOURCES))
VORBIS_OBJS=$(patsubst %,objs/%,$(VORBIS_OBJS_TMP))
vorbis.a: ${VORBIS_OBJS}
	ar rcs $@ $^

freetype.a:
	make -f ft_make

bin/apsis: bin json.a glew.a clew.a soil.a sdl_mixer.a ogg.a vorbis.a freetype.a ${OBJS}
	${LD} -o bin/apsis ${OBJS} support/ogg/*.o glew.a clew.a json.a soil.a sdl_mixer.a freetype.a vorbis.a ${LIBS_CMD} -Wl,--gc-sections -Wl,-s

objs/%.o: ${FILEPATH}/%.cc objs
	@mkdir -p $(dir $@)
	${CC} -o $@ -c $< -I $(dir $<) ${INCLUDE_CMD} ${CFLAGS}

objs/%.o: ${FILEPATH}/%.c objs
	@mkdir -p $(dir $@)
	${CC} -o $@ -c $< -I $(dir $<) ${INCLUDE_CMD} ${CFLAGS}

objs/%.o: ${FILEPATH}/%.cpp objs
	@mkdir -p $(dir $@)
	${CPPC} -o $@ -c $< ${INCLUDE_CMD} ${CPPFLAGS}

objs:
	mkdir -p objs

bin:
	mkdir -p bin

ctags:
	ctags ${SOURCES}

docs: ctags

clean:
	rm -rf objs
	rm -rf tags

realclean: veryclean

veryclean: clean
	rm -rf bin
