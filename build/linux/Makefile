PROJECTS=apsis
CONFIG=config

%: %.mk
	@PROJECT="$@" CONFIG="${CONFIG}" make -f build/build.mk ${BUILD} --no-print-directory

all: release

release: BUILD:="release"
release: ${PROJECTS}

debug: BUILD:="debug"
debug: ${PROJECTS}
