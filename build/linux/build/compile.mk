# This Makefile will compile and link all files within the given SOURCE_PATH and
# produce either a BINARY or LIBRARY respective of the given BUILD mode.
#
# Inputs to the Makefile:
#
# Project:
#   PROJECT         -- load inputs from "$PROJECT.mk"
#
# Mode:
#   BUILD           -- either "debug" or "release"
#
# Files:
#   SOURCE_PATH     -- path to all files
#   HEADER_PATH     -- path to include files
#   EXCLUDE         -- list of files to not compile
#   DO_NOT_OPTIMIZE -- list of files to not optimize
#   SOURCES         -- list of files to compile (otherwise, rakes SOURCE_PATH)
#
# Output: (one of the following two)
#   BINARY          -- the name of the binary
#   LIBRARY         -- the name of the library
#
# Tools:
#   C_COMPILER      -- the C compiler
#   CPP_COMPILER    -- the C++ compiler
#   LINKER          -- the linker
#
# Options:
#   LIBS            -- list of libraries to link in
#   LINK            -- list of object files to link in
#   INCLUDE         -- list of include paths to compile with
#   DEFINE          -- list of defines to toggle
#   C_FLAGS         -- any extra C compiler flags
#   CPP_FLAGS       -- any extra C++ compiler flags
#   LINKER_FLAGS    -- any extra linker flags

# Load variables from given project file
ifdef PROJECT
include $(PROJECT).mk
endif

ifdef CONFIG
include $(CONFIG).mk
endif

# Default tools
ifndef C_COMPILER
C_COMPILER=gcc
endif
ifndef LINKER
LINKER=gcc
endif
ifndef CPP_COMPILER
CPP_COMPILER=gcc
endif

# Default include path is off of the source path
ifndef HEADER_PATH
HEADER_PATH=${SOURCE_PATH}/../include
endif

# Add extra linkage for built libraries
ifdef DEPENDENCIES
DEPS_LINK=$(foreach var, $(DEPENDENCIES),$(shell grep '^LIBRARY' $(var).mk))
DEPS_LINK:=$(patsubst LIBRARY=%,lib/${BUILD}/%.a,$(DEPS_LINK))
endif

# Debug mode flags
DEBUG_FLAGS=-g -DDEBUG_THROW_GL_ERRORS
DEBUG_LD_FLAGS=-rdynamic

# For file size compacting
DEADCODE_ELIM_FLAGS=-fdata-sections -ffunction-sections

# Release mode flags
RELEASE_LD_FLAGS=-Wl,--gc-sections -Wl,-s
RELEASE_FLAGS=${DEADCODE_ELIM_FLAGS}

# Optimization flags
UNOPTIMIZATION_FLAGS=-O0
OPTIMIZATION_FLAGS=-O3

ifdef BINARY
TYPE="executable"
NAME=${BINARY}
BINARY_FILE=${BINARY}
OUTPUT=bin/${BUILD}/${BINARY_FILE}
else
TYPE="library"
NAME=${LIBRARY}
LIBRARY_FILE=${LIBRARY}.a
OUTPUT=lib/${BUILD}/${LIBRARY_FILE}
endif

# Generate list of sources
ifndef SOURCES
SOURCES_CPP_TMP=$(shell find ${SOURCE_PATH} -name *.cpp)
SOURCES_CPP=$(patsubst $(SOURCE_PATH)/%,%,${SOURCES_CPP_TMP})
SOURCES_C_TMP=$(shell find ${SOURCE_PATH} -name *.c)
SOURCES_C=$(patsubst $(SOURCE_PATH)/%,%,${SOURCES_C_TMP})
SOURCES_CC_TMP=$(shell find ${SOURCE_PATH} -name *.cc)
SOURCES_CC=$(patsubst $(SOURCE_PATH)/%,%,${SOURCES_CC_TMP})

SOURCES:=${SOURCES_CPP} ${SOURCES_CC} ${SOURCES_C}
endif

# Exclude specified files
SOURCES:=${filter-out $(EXCLUDE),${SOURCES}}

# Exclude non-optimized files
SOURCES:=${filter-out $(DO_NOT_OPTIMIZE),${SOURCES}}

# Generate object list
OBJS=${SOURCES}
OBJS:=$(patsubst %.cpp,%.o,$(OBJS))
OBJS:=$(patsubst %.cc,%.o,$(OBJS))
OBJS:=$(patsubst %.c,%.o,$(OBJS))
OBJS:=$(patsubst %,objs/${BUILD}/${NAME}/%,${OBJS})

# Generate Do-Not-Optimize list
OBJS_NO_OPT_TMP=$(patsubst %.cc,%.o,$(DO_NOT_OPTIMIZE))
OBJS_NO_OPT_TMP:=$(patsubst %.cpp,%.o,$(OBJS_NO_OPT_TMP))
OBJS_NO_OPT_TMP:=$(patsubst %.c,%.o,$(OBJS_NO_OPT_TMP))
OBJS_NO_OPT=$(patsubst %,objs/${BUILD}/${NAME}/%,${OBJS_NO_OPT_TMP})

# Generate compiler commands
LIBS_CMD=$(patsubst %,-l%,$(LIBS))
INCLUDE_CMD=$(patsubst %,-I%,$(HEADER_PATH)) $(patsubst %,-I%,$(INCLUDE))
DEFINE_CMD=$(patsubst %,-D%,$(DEFINE))

# Determine compile flags
ifeq "${BUILD}" "debug"
CFLAGS_FINAL=${C_FLAGS} ${DEFINE_CMD} -DDEBUG_MODE ${DEBUG_FLAGS}
CPPFLAGS_FINAL=${CPP_FLAGS} ${DEFINE_CMD} -DDEBUG_MODE ${DEBUG_FLAGS}
else
CFLAGS_FINAL=${C_FLAGS} ${DEFINE_CMD} -DRELEASE_MODE ${RELEASE_FLAGS}
CPPFLAGS_FINAL=${CPP_FLAGS} ${DEFINE_CMD} -DRELEASE_MODE ${RELEASE_FLAGS}
endif

all: header ${OUTPUT} footer

header:
	@echo Building ${NAME} \(${BUILD}\)

footer:
	@echo

optimized_objs: OPT_FLAGS:=${OPTIMIZATION_FLAGS}
optimized_objs: ${OBJS}

unoptimized_objs: OPT_FLAGS:=${UNOPTIMIZATION_FLAGS}
unoptimized_objs: ${OBJS_NO_OPT}

# Build the binary executable
ifdef BINARY
bin/${BUILD}/${BINARY_FILE}: optimized_objs unoptimized_objs
	@mkdir -p bin/${BUILD}
	@echo " >> Linking $@"
	@${LINKER} -o $@ ${LINK} ${OBJS} ${OBJS_NO_OPT} ${DEPS_LINK} ${LIBS_CMD}
endif

# build the library
ifdef LIBRARY
lib/${BUILD}/${LIBRARY_FILE}: optimized_objs unoptimized_objs
	@mkdir -p lib/${BUILD}
	@echo " >> Archiving $@"
	@ar rcs $@ ${OBJS} ${OBJS_NO_OPT} ${LINK}
endif

# CC file compilation
objs/${BUILD}/${NAME}/%.o: ${SOURCE_PATH}/%.cc
	@mkdir -p $(dir $@)
	@echo " -- $(patsubst $(SOURCE_PATH)/%,%,$<)"
ifeq "${BUILD}" "debug"
	@${CPP_COMPILER} -o $@ -c $< ${INCLUDE_CMD} ${CPPFLAGS_FINAL} ${UNOPTIMIZATION_FLAGS}
else
	@${CPP_COMPILER} -o $@ -c $< ${INCLUDE_CMD} ${CPPFLAGS_FINAL} ${OPT_FLAGS}
endif

# C file compilation
objs/${BUILD}/${NAME}/%.o: ${SOURCE_PATH}/%.c
	@mkdir -p $(dir $@)
	@echo " -- $(patsubst $(SOURCE_PATH)/%,%,$<)"
ifeq "${BUILD}" "debug"
	@${C_COMPILER} -o $@ -c $< -I $(dir $<) ${INCLUDE_CMD} ${CFLAGS_FINAL} ${UNOPTIMIZATION_FLAGS}
else
	@${C_COMPILER} -o $@ -c $< -I $(dir $<) ${INCLUDE_CMD} ${CFLAGS_FINAL} ${OPT_FLAGS}
endif

# C++ file compilation
objs/${BUILD}/${NAME}/%.o: ${SOURCE_PATH}/%.cpp
	@mkdir -p $(dir $@)
	@echo " -- $(patsubst $(SOURCE_PATH)/%,%,$<)"
ifeq "${BUILD}" "debug"
	@${CPP_COMPILER} -o $@ -c $< ${INCLUDE_CMD} ${CPPFLAGS_FINAL} ${UNOPTIMIZATION_FLAGS}
else
	@${CPP_COMPILER} -o $@ -c $< ${INCLUDE_CMD} ${CPPFLAGS_FINAL} ${OPT_FLAGS}
endif
