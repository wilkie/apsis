PROJECTS=apsis
CONFIG=config

all: release

%: %.mk
	+@PROJECT="$@" CONFIG="${CONFIG}" make -f build/build.mk ${BUILD} --no-print-directory

release: BUILD:="release"
release: ${PROJECTS}

debug: BUILD:="debug"
debug: ${PROJECTS}

js: BUILD:="js"
js: ${PROJECTS}
