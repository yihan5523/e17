#include "geist.h"
#include "geist_list.h"
#include "geist_object.h"
#include "layers.h"
#include "geist_imlib.h"

#ifndef GEIST_GTK_H
#define GEIST_GTK_H

typedef struct __geist_document geist_document;

struct __geist_document
{
   char *name;
   geist_list *layers;
   int w;
   int h;
   Imlib_Image im;
   Pixmap pmap;
};

geist_document *geist_document_new(int w, int h);
void geist_document_free(geist_document *l);
void geist_document_render(geist_document * document);

#endif
