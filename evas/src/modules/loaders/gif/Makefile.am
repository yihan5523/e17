
MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/lib \
-I$(top_srcdir)/src/lib/include \
@FREETYPE_CFLAGS@ \
@PIXMAN_CFLAGS@ \
@FRIBIDI_CFLAGS@ \
@evas_image_loader_gif_cflags@ \
@EVAS_GENERAL_CFLAGS@

if BUILD_LOADER_GIF
if !EVAS_STATIC_BUILD_GIF

pkgdir = $(libdir)/evas/modules/loaders/gif/$(MODULE_ARCH)
pkg_LTLIBRARIES = module.la

module_la_SOURCES = evas_image_load_gif.c

module_la_LIBADD = @EVAS_GENERAL_LIBS@ @evas_image_loader_gif_libs@ $(top_builddir)/src/lib/libevas.la
module_la_LDFLAGS = -no-undefined @lt_enable_auto_import@ -module -avoid-version
module_la_LIBTOOLFLAGS = --tag=disable-static

else

noinst_LTLIBRARIES = libevas_loader_gif.la

libevas_loader_gif_la_SOURCES = evas_image_load_gif.c
libevas_loader_gif_la_LIBADD = @evas_image_loader_gif_libs@

endif
endif
