PROJECTS=apsis

%: %.mk
	@PROJECT="$@" make -f build/build.mk ${BUILD} --no-print-directory

all: release

release: BUILD:="release"
release: ${PROJECTS}

debug: BUILD:="debug"
debug: ${PROJECTS}
