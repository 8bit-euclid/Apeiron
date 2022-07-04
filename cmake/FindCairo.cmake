# - Try to find Cairo
# Once done, this will define
#
#  Cairo_FOUND - system has Cairo
#  Cairo_INCLUDE_DIRS - the Cairo include directories
#  Cairo_LIBRARIES - link these to use Cairo

include(LibFindMacros)

# Freetype is not necessarily required on all platforms...
if(NOT WIN32)
    set(cairo_needs_freetype true)
endif(NOT WIN32)

# Dependencies
if(cairo_needs_freetype)
    libfind_package(Cairo Freetype)
endif(cairo_needs_freetype)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(Cairo_PKGCONF cairo)

# Include dir
find_path(Cairo_INCLUDE_DIR
        NAMES cairo.h
        HINTS ${Cairo_PKGCONF_INCLUDE_DIRS}
        PATH_SUFFIXES cairo
        )

# Finally the library itself
find_library(Cairo_LIBRARY
        NAMES cairo
        HINTS ${Cairo_PKGCONF_LIBRARY_DIRS}
        )

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(Cairo_PROCESS_INCLUDES Cairo_INCLUDE_DIR)
set(Cairo_PROCESS_LIBS Cairo_LIBRARY)
if(cairo_needs_freetype)
    list(APPEND Cairo_PROCESS_INCLUDES Freetype_INCLUDE_DIR)
    list(APPEND Cairo_PROCESS_LIBS Freetype_LIBRARY)
endif(cairo_needs_freetype)
libfind_process(Cairo)