include ${PROJECT}.mk

all: release

%: %.mk
	@PROJECT="$@" make -f build/build.mk ${BUILD} --no-print-directory

invoke: ${DEPENDENCIES}
	@BUILD=${BUILD} PROJECT=${PROJECT} make -f build/compile.mk --no-print-directory

release: BUILD:="release"
release: invoke

debug: BUILD:="debug"
debug: invoke
