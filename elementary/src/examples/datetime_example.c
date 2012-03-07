//Compile with:
//gcc -g `pkg-config --cflags --libs elementary` datetime_example.c -o datetime_example

#include <Elementary.h>
#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#else
# define __UNUSED__
#endif

static void
_on_done(void *data __UNUSED__,
        Evas_Object *obj __UNUSED__,
        void *event_info __UNUSED__)
{
   elm_exit();
}

int
elm_main(int argc __UNUSED__, char *argv[] __UNUSED__)
{
   Evas_Object *win, *bg, *bx, *datetime;

   win = elm_win_add(NULL, "Datetime", ELM_WIN_BASIC);
   elm_win_title_set(win, "Datetime");
   evas_object_smart_callback_add(win, "delete,request", _on_done, NULL);

   bg = elm_bg_add(win);
   elm_win_resize_object_add(win, bg);
   evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_show(bg);

   bx = elm_box_add(win);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx);
   elm_box_horizontal_set(bx, EINA_FALSE);
   evas_object_show(bx);
   evas_object_size_hint_min_set(bx, 360, 200);

   //datetime showing only DATE
   datetime = elm_datetime_add(bx);
   evas_object_size_hint_weight_set(datetime, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(datetime, EVAS_HINT_FILL, 0.5);
   elm_datetime_field_visible_set(datetime, ELM_DATETIME_HOUR, EINA_FALSE);
   elm_datetime_field_visible_set(datetime, ELM_DATETIME_MINUTE, EINA_FALSE);
   elm_datetime_field_visible_set(datetime, ELM_DATETIME_AMPM, EINA_FALSE);
   elm_box_pack_end(bx, datetime);
   evas_object_show(datetime);

   //datetime showing only TIME
   datetime = elm_datetime_add(bx);
   evas_object_size_hint_weight_set(datetime, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(datetime, EVAS_HINT_FILL, 0.5);
   elm_datetime_field_visible_set(datetime, ELM_DATETIME_YEAR, EINA_FALSE);
   elm_datetime_field_visible_set(datetime, ELM_DATETIME_MONTH, EINA_FALSE);
   elm_datetime_field_visible_set(datetime, ELM_DATETIME_DATE, EINA_FALSE);
   elm_box_pack_end(bx, datetime);
   evas_object_show(datetime);

   //datetime showing both DATE and TIME
   datetime = elm_datetime_add(bx);
   evas_object_size_hint_weight_set(datetime, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(datetime, EVAS_HINT_FILL, 0.5);
   elm_box_pack_end(bx, datetime);
   evas_object_show(datetime);

   evas_object_show(win);

   elm_run();

   return 0;
}
ELM_MAIN();