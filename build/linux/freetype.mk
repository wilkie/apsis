# Source location
SOURCE_PATH=../../support/freetype
HEADER_PATH=../../include

# Freetype is WEIRD. It includes C files for some
# damned reason. Breaks everything.
SOURCES+=autofit/autofit.c
SOURCES+=base/basepic.c
SOURCES+=base/ftadvanc.c
SOURCES+=base/ftapi.c
SOURCES+=base/ftbase.c
SOURCES+=base/ftbbox.c
SOURCES+=base/ftbdf.c
SOURCES+=base/ftbitmap.c
SOURCES+=base/ftcalc.c
SOURCES+=base/ftcid.c
SOURCES+=base/ftdbgmem.c
SOURCES+=base/ftdebug.c
SOURCES+=base/ftfstype.c
SOURCES+=base/ftgasp.c
SOURCES+=base/ftgloadr.c
SOURCES+=base/ftglyph.c
SOURCES+=base/ftgxval.c
SOURCES+=base/ftinit.c
SOURCES+=base/ftlcdfil.c
SOURCES+=base/ftmac.c
SOURCES+=base/ftmm.c
SOURCES+=base/ftobjs.c
SOURCES+=base/ftotval.c
SOURCES+=base/ftoutln.c
SOURCES+=base/ftpatent.c
SOURCES+=base/ftpfr.c
SOURCES+=base/ftpic.c
SOURCES+=base/ftrfork.c
SOURCES+=base/ftsnames.c
SOURCES+=base/ftstream.c
SOURCES+=base/ftstroke.c
SOURCES+=base/ftsynth.c
SOURCES+=base/ftsystem.c
SOURCES+=base/fttrigon.c
SOURCES+=base/fttype1.c
SOURCES+=base/ftutil.c
SOURCES+=base/ftwinfnt.c
SOURCES+=base/ftxf86.c
SOURCES+=base/md5.c
SOURCES+=type42/type42.c
SOURCES+=type1/type1.c
SOURCES+=truetype/truetype.c
SOURCES+=smooth/smooth.c
SOURCES+=raster/raster.c
SOURCES+=sfnt/sfnt.c
SOURCES+=psnames/psnames.c
SOURCES+=gxvalid/gxvalid.c
SOURCES+=gzip/ftgzip.c
SOURCES+=otvalid/otvalid.c
SOURCES+=pcf/pcf.c
SOURCES+=psaux/psaux.c
SOURCES+=pfr/pfr.c
SOURCES+=pshinter/pshinter.c
SOURCES+=bdf/bdf.c
SOURCES+=cache/ftcache.c
SOURCES+=cff/cff.c
SOURCES+=cid/type1cid.c
SOURCES+=lzw/ftlzw.c
SOURCES+=winfonts/winfnt.c

# Define some things
DEFINE=FT2_BUILD_LIBRARY

# Output
LIBRARY=freetype

