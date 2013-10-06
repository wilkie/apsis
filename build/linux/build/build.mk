include ${PROJECT}.mk

ifdef CONFIG
include ${CONFIG}.mk
endif

all: release

%: %.mk
	@PROJECT="$@" CONFIG="${CONFIG}" make -f build/build.mk ${BUILD} --no-print-directory

invoke: ${DEPENDENCIES}
	@BUILD=${BUILD} PROJECT=${PROJECT} CONFIG="${CONFIG}" make -f build/compile.mk --no-print-directory

release: BUILD:="release"
release: invoke

debug: BUILD:="debug"
debug: invoke
