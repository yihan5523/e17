#include <Elementary.h>
#include "elm_priv.h"

/**
 * @defgroup Clock Clock
 *
 * It's a widget to show clock with animation. The update of time is
 * shown in an animation like the flip of a sheet.
 *
 */

typedef struct _Widget_Data Widget_Data;

struct _Widget_Data
{
   Evas_Object *clk;
   double interval;
   Eina_Bool seconds : 1;
   Eina_Bool am_pm : 1;
   Eina_Bool edit : 1;
   Elm_Clock_Digedit digedit;
   int hrs, min, sec;
   Evas_Object *digit[6];
   Evas_Object *ampm;
   Evas_Object *sel_obj;
   Ecore_Timer *ticker, *spin;
   struct
     {
        int hrs, min, sec;
        char ampm;
        Eina_Bool seconds : 1;
        Eina_Bool am_pm : 1;
        Eina_Bool edit : 1;
	Elm_Clock_Digedit digedit;
     } cur;
};

static const char *widtype = NULL;
static void _del_hook(Evas_Object *obj);
static void _theme_hook(Evas_Object *obj);
static Eina_Bool _ticker(void *data);
static Eina_Bool _signal_clock_val_up(void *data);
static Eina_Bool _signal_clock_val_down(void *data);
static void _time_update(Evas_Object *obj);

static void
_del_hook(Evas_Object *obj)
{
   Widget_Data *wd = elm_widget_data_get(obj);
   if (!wd) return;
   int i;
   for (i = 0; i < 6; i++)
     {
	if (wd->digit[i]) evas_object_del(wd->digit[i]);
     }
   if (wd->ampm) evas_object_del(wd->ampm);
   if (wd->ticker) ecore_timer_del(wd->ticker);
   if (wd->spin) ecore_timer_del(wd->spin);
   free(wd);
}

static void
_theme_hook(Evas_Object *obj)
{
   Widget_Data *wd = elm_widget_data_get(obj);
   if (!wd) return;
   if (elm_widget_focus_get(obj))
     edje_object_signal_emit(wd->clk, "elm,action,focus", "elm");
   else
     edje_object_signal_emit(wd->clk, "elm,action,unfocus", "elm");
   wd->cur.am_pm = !wd->cur.am_pm; /* hack - force update */
   _time_update(obj);
}

static void
_on_focus_hook(void *data __UNUSED__, Evas_Object *obj)
{
   Widget_Data *wd = elm_widget_data_get(obj);
   if (!wd) return;
   if (elm_widget_focus_get(obj))
     edje_object_signal_emit(wd->clk, "elm,action,focus", "elm");
   else
     edje_object_signal_emit(wd->clk, "elm,action,unfocus", "elm");
}

static void
_signal_emit_hook(Evas_Object *obj, const char *emission, const char *source)
{
   Widget_Data *wd = elm_widget_data_get(obj);
   int i;
   if (!wd) return;
   edje_object_signal_emit(wd->clk, emission, source);
   for (i = 0; i < 6; i++)
     {
	if (wd->digit[i])
	  edje_object_signal_emit(wd->digit[i], emission, source);
     }
}

static void
_signal_callback_add_hook(Evas_Object *obj, const char *emission, const char *source, void (*func_cb) (void *data, Evas_Object *o, const char *emission, const char *source), void *data)
{
   Widget_Data *wd = elm_widget_data_get(obj);
   int i;
   if (!wd) return;
   edje_object_signal_callback_add(wd->clk, emission, source, func_cb, data);
   for (i = 0; i < 6; i++)
     {
	if (wd->digit[i])
	  edje_object_signal_callback_add(wd->digit[i], emission, source,
		func_cb, data);
     }
}

static void *
_signal_callback_del_hook(Evas_Object *obj, const char *emission, const char *source, void (*func_cb) (void *data, Evas_Object *o, const char *emission, const char *source))
{
   Widget_Data *wd = elm_widget_data_get(obj);
   int i;
   if (!wd) return NULL;
   for (i = 0; i < 6; i++)
     {
	edje_object_signal_callback_del(wd->digit[i], emission, source,
	      func_cb);
     }
   return edje_object_signal_callback_del(wd->clk, emission, source, func_cb);
}

static Eina_Bool
_ticker(void *data)
{
   Widget_Data *wd = elm_widget_data_get(data);
   double t;
   struct timeval timev;
   struct tm *tm;
   time_t tt;
   if (!wd) return ECORE_CALLBACK_CANCEL;
   gettimeofday(&timev, NULL);
   t = ((double)(1000000 - timev.tv_usec)) / 1000000.0;
   wd->ticker = ecore_timer_add(t, _ticker, data);
   if (!wd->edit)
     {
	tt = (time_t)(timev.tv_sec);
	tzset();
	tm = localtime(&tt);
	if (tm)
	  {
	     wd->hrs = tm->tm_hour;
	     wd->min = tm->tm_min;
	     wd->sec = tm->tm_sec;
	     _time_update(data);
	  }
     }
   return ECORE_CALLBACK_CANCEL;
}

static Eina_Bool
_signal_clock_val_up(void *data)
{
   Widget_Data *wd = elm_widget_data_get(data);
   if (!wd) goto clock_val_up_exit_on_error;
   if (!wd->edit) goto clock_val_up_cancel;
   if (!wd->sel_obj) goto clock_val_up_cancel;
   if (wd->sel_obj == wd->digit[0])
     {
	wd->hrs = wd->hrs + 10;
	if (wd->hrs >= 24) wd->hrs -= 24;
     }
   if (wd->sel_obj == wd->digit[1])
     {
	wd->hrs = wd->hrs + 1;
	if (wd->hrs >= 24) wd->hrs -= 24;
     }
   if (wd->sel_obj == wd->digit[2])
     {
	wd->min = wd->min + 10;
	if (wd->min >= 60) wd->min -= 60;
     }
   if (wd->sel_obj == wd->digit[3])
     {
	wd->min = wd->min + 1;
	if (wd->min >= 60) wd->min -= 60;
     }
   if (wd->sel_obj == wd->digit[4])
     {
	wd->sec = wd->sec + 10;
	if (wd->sec >= 60) wd->sec -= 60;
     }
   if (wd->sel_obj == wd->digit[5])
     {
	wd->sec = wd->sec + 1;
	if (wd->sec >= 60) wd->sec -= 60;
     }
   if (wd->sel_obj == wd->ampm)
     {
	wd->hrs = wd->hrs + 12;
	if (wd->hrs > 23) wd->hrs -= 24;
     }
   wd->interval = wd->interval / 1.05;
   ecore_timer_interval_set(wd->spin, wd->interval);
   _time_update(data);
   evas_object_smart_callback_call(data, "changed", NULL);
   return ECORE_CALLBACK_RENEW;
clock_val_up_cancel:
   wd->spin = NULL;
clock_val_up_exit_on_error:
   return ECORE_CALLBACK_CANCEL;
}

static Eina_Bool
_signal_clock_val_down(void *data)
{
   Widget_Data *wd = elm_widget_data_get(data);
   if (!wd) goto clock_val_up_exit_on_error;
   if (!wd->edit) goto clock_val_down_cancel;
   if (!wd->sel_obj) goto clock_val_down_cancel;
   if (wd->sel_obj == wd->digit[0])
     {
	wd->hrs = wd->hrs - 10;
	if (wd->hrs < 0) wd->hrs += 24;
     }
   if (wd->sel_obj == wd->digit[1])
     {
	wd->hrs = wd->hrs - 1;
	if (wd->hrs < 0) wd->hrs += 24;
     }
   if (wd->sel_obj == wd->digit[2])
     {
	wd->min = wd->min - 10;
	if (wd->min < 0) wd->min += 60;
     }
   if (wd->sel_obj == wd->digit[3])
     {
	wd->min = wd->min - 1;
	if (wd->min < 0) wd->min += 60;
     }
   if (wd->sel_obj == wd->digit[4])
     {
	wd->sec = wd->sec - 10;
	if (wd->sec < 0) wd->sec += 60;
     }
   if (wd->sel_obj == wd->digit[5])
     {
	wd->sec = wd->sec - 1;
	if (wd->sec < 0) wd->sec += 60;
     }
   if (wd->sel_obj == wd->ampm)
     {
	wd->hrs = wd->hrs - 12;
	if (wd->hrs < 0) wd->hrs += 24;
     }
   wd->interval = wd->interval / 1.05;
   ecore_timer_interval_set(wd->spin, wd->interval);
   _time_update(data);
   evas_object_smart_callback_call(data, "changed", NULL);
   return ECORE_CALLBACK_RENEW;
clock_val_down_cancel:
   wd->spin = NULL;
clock_val_down_exit_on_error:
   return ECORE_CALLBACK_CANCEL;
}

static void
_signal_clock_val_up_start(void *data, Evas_Object *obj, const char *emission __UNUSED__, const char *source __UNUSED__)
{
   Widget_Data *wd = elm_widget_data_get(data);
   if (!wd) return;
   wd->interval = 0.85;
   wd->sel_obj = obj;
   if (wd->spin) ecore_timer_del(wd->spin);
   wd->spin = ecore_timer_add(wd->interval, _signal_clock_val_up, data);
   _signal_clock_val_up(data);
}

static void
_signal_clock_val_down_start(void *data, Evas_Object *obj, const char *emission __UNUSED__, const char *source __UNUSED__)
{
   Widget_Data *wd = elm_widget_data_get(data);
   if (!wd) return;
   wd->interval = 0.85;
   wd->sel_obj = obj;
   if (wd->spin) ecore_timer_del(wd->spin);
   wd->spin = ecore_timer_add(wd->interval, _signal_clock_val_down, data);
   _signal_clock_val_down(data);
}

static void
_signal_clock_val_change_stop(void *data, Evas_Object *obj __UNUSED__, const char *emission __UNUSED__, const char *source __UNUSED__)
{
   Widget_Data *wd = elm_widget_data_get(data);
   if (!wd) return;
   if (wd->spin) ecore_timer_del(wd->spin);
   wd->spin = NULL;
   wd->sel_obj = NULL;
}

static void
_time_update(Evas_Object *obj)
{
   Widget_Data *wd = elm_widget_data_get(obj);
   Edje_Message_Int msg;
   int ampm = 0;
   const char *style = elm_widget_style_get(obj);
   if (!wd) return;
   if ((wd->cur.seconds != wd->seconds) || (wd->cur.am_pm != wd->am_pm) ||
       (wd->cur.edit != wd->edit) || (wd->cur.digedit != wd->digedit))
     {
	int i;
	Evas_Coord mw, mh;

	for (i = 0; i < 6; i++)
	  {
	     if (wd->digit[i])
	       {
		  evas_object_del(wd->digit[i]);
		  wd->digit[i] = NULL;
	       }
	  }
	if (wd->ampm)
	  {
	     evas_object_del(wd->ampm);
	     wd->ampm = NULL;
	  }

	if ((wd->seconds) && (wd->am_pm))
	  _elm_theme_object_set(obj, wd->clk, "clock", "base-all", style);
	else if (wd->seconds)
	  _elm_theme_object_set(obj, wd->clk, "clock", "base-seconds", style);
	else if (wd->am_pm)
	  _elm_theme_object_set(obj, wd->clk, "clock", "base-am_pm", style);
	else
	  _elm_theme_object_set(obj, wd->clk, "clock", "base", style);
	edje_object_scale_set(wd->clk, elm_widget_scale_get(obj) * 
                              _elm_config->scale);

	for (i = 0; i < 6; i++)
	  {
	     char buf[16];

	     if ((!wd->seconds) && (i >= 4)) break;
	     wd->digit[i] = edje_object_add(evas_object_evas_get(wd->clk));
	     _elm_theme_object_set(obj, wd->digit[i], "clock", "flipdigit", style);
	     edje_object_scale_set(wd->digit[i], elm_widget_scale_get(obj) * 
                                   _elm_config->scale);
	     if (wd->edit && (wd->digedit & (1 << i)))
	       edje_object_signal_emit(wd->digit[i], "elm,state,edit,on", "elm");
	     edje_object_signal_callback_add(wd->digit[i], "elm,action,up,start",
					     "", _signal_clock_val_up_start, obj);
	     edje_object_signal_callback_add(wd->digit[i], "elm,action,up,stop",
					     "", _signal_clock_val_change_stop, obj);
	     edje_object_signal_callback_add(wd->digit[i], "elm,action,down,start",
					     "", _signal_clock_val_down_start, obj);
	     edje_object_signal_callback_add(wd->digit[i], "elm,action,down,stop",
					     "", _signal_clock_val_change_stop, obj);
	     mw = mh = -1;
	     elm_coords_finger_size_adjust(1, &mw, 2, &mh);
	     edje_object_size_min_restricted_calc(wd->digit[i], &mw, &mh, mw, mh);
	     elm_coords_finger_size_adjust(1, &mw, 2, &mh);
	     edje_extern_object_min_size_set(wd->digit[i], mw, mh);
	     snprintf(buf, sizeof(buf), "d%i", i);
	     edje_object_part_swallow(wd->clk , buf, wd->digit[i]);
	     evas_object_show(wd->digit[i]);
	  }
	if (wd->am_pm)
	  {
	     wd->ampm = edje_object_add(evas_object_evas_get(wd->clk));
	     _elm_theme_object_set(obj, wd->ampm, "clock", "flipampm", style);
	     edje_object_scale_set(wd->ampm, elm_widget_scale_get(obj) * 
                                   _elm_config->scale);
	     if (wd->edit)
	       edje_object_signal_emit(wd->ampm, "elm,state,edit,on", "elm");
	     edje_object_signal_callback_add(wd->ampm, "elm,action,up,start",
					     "", _signal_clock_val_up_start, obj);
	     edje_object_signal_callback_add(wd->ampm, "elm,action,up,stop",
					     "", _signal_clock_val_change_stop, obj);
	     edje_object_signal_callback_add(wd->ampm, "elm,action,down,start",
					     "", _signal_clock_val_down_start, obj);
	     edje_object_signal_callback_add(wd->ampm, "elm,action,down,stop",
					     "", _signal_clock_val_change_stop, obj);
	     mw = mh = -1;
	     elm_coords_finger_size_adjust(1, &mw, 2, &mh);
	     edje_object_size_min_restricted_calc(wd->ampm, &mw, &mh, mw, mh);
	     elm_coords_finger_size_adjust(1, &mw, 2, &mh);
	     edje_extern_object_min_size_set(wd->ampm, mw, mh);
	     edje_object_part_swallow(wd->clk , "ampm", wd->ampm);
	     evas_object_show(wd->ampm);
	  }

	edje_object_size_min_calc(wd->clk, &mw, &mh);
	evas_object_size_hint_min_set(obj, mw, mh);

	wd->cur.hrs = 0;
	wd->cur.min = 0;
	wd->cur.sec = 0;
	wd->cur.ampm = -1;
	wd->cur.seconds = wd->seconds;
	wd->cur.am_pm = wd->am_pm;
	wd->cur.edit = wd->edit;
	wd->cur.digedit = wd->digedit;
     }
   if (wd->hrs != wd->cur.hrs)
     {
	int hrs;
	int d1, d2, dc1, dc2;

	hrs = wd->hrs;
	if (wd->am_pm)
	  {
	     if (hrs >= 12)
	       {
		  if (hrs > 12) hrs -= 12;
		  ampm = 1;
	       }
	     else if (hrs == 0) hrs = 12;
	  }
	d1 = hrs / 10;
	d2 = hrs % 10;
	dc1 = wd->cur.hrs / 10;
	dc2 = wd->cur.hrs % 10;
	if (d1 != dc1)
	  {
	     msg.val = d1;
	     edje_object_message_send(wd->digit[0], EDJE_MESSAGE_INT, 1, &msg);
	  }
	if (d2 != dc2)
	  {
	     msg.val = d2;
	     edje_object_message_send(wd->digit[1], EDJE_MESSAGE_INT, 1, &msg);
	  }
	wd->cur.hrs = hrs;
     }
   if (wd->min != wd->cur.min)
     {
	int d1, d2, dc1, dc2;

	d1 = wd->min / 10;
	d2 = wd->min % 10;
	dc1 = wd->cur.min / 10;
	dc2 = wd->cur.min % 10;
	if (d1 != dc1)
	  {
	     msg.val = d1;
	     edje_object_message_send(wd->digit[2], EDJE_MESSAGE_INT, 1, &msg);
	  }
	if (d2 != dc2)
	  {
	     msg.val = d2;
	     edje_object_message_send(wd->digit[3], EDJE_MESSAGE_INT, 1, &msg);
	  }
	wd->cur.min = wd->min;
     }
   if (wd->seconds)
     {
	if (wd->sec != wd->cur.sec)
	  {
	     int d1, d2, dc1, dc2;

	     d1 = wd->sec / 10;
	     d2 = wd->sec % 10;
	     dc1 = wd->cur.sec / 10;
	     dc2 = wd->cur.sec % 10;
	     if (d1 != dc1)
	       {
		  msg.val = d1;
		  edje_object_message_send(wd->digit[4], EDJE_MESSAGE_INT, 1, &msg);
	       }
	     if (d2 != dc2)
	       {
		  msg.val = d2;
		  edje_object_message_send(wd->digit[5], EDJE_MESSAGE_INT, 1, &msg);
	       }
	     wd->cur.sec = wd->sec;
	  }
     }
   else
     wd->cur.sec = -1;

   if (wd->am_pm)
     {
	if (wd->hrs >= 12) ampm = 1;
	if (ampm != wd->cur.ampm)
	  {
	     if (wd->cur.ampm != ampm)
	       {
		  msg.val = ampm;
		  edje_object_message_send(wd->ampm, EDJE_MESSAGE_INT, 1, &msg);
	       }
	     wd->cur.ampm = ampm;
	  }
     }
   else
     wd->cur.ampm = -1;
}

/**
 * Add a new clock to the parent
 *
 * @param parent The parent object
 *
 * This function inserts a clock widget on a given canvas to show a
 * animated clock.
 *
 * @ingroup Clock
 */
EAPI Evas_Object *
elm_clock_add(Evas_Object *parent)
{
   Evas_Object *obj;
   Evas *e;
   Widget_Data *wd;

   wd = ELM_NEW(Widget_Data);
   e = evas_object_evas_get(parent);
   obj = elm_widget_add(e);
   ELM_SET_WIDTYPE(widtype, "clock");
   elm_widget_type_set(obj, "clock");
   elm_widget_sub_object_add(parent, obj);
   elm_widget_data_set(obj, wd);
   elm_widget_del_hook_set(obj, _del_hook);
   elm_widget_theme_hook_set(obj, _theme_hook);
   elm_widget_on_focus_hook_set(obj, _on_focus_hook, NULL);
   elm_widget_signal_emit_hook_set(obj, _signal_emit_hook);
   elm_widget_signal_callback_add_hook_set(obj, _signal_callback_add_hook);
   elm_widget_signal_callback_del_hook_set(obj, _signal_callback_del_hook);

   wd->clk = edje_object_add(e);
   elm_widget_resize_object_set(obj, wd->clk);

   wd->cur.ampm = -1;
   wd->cur.seconds = EINA_TRUE;
   wd->cur.am_pm = EINA_TRUE;
   wd->cur.edit = EINA_TRUE;
   wd->cur.digedit = ELM_CLOCK_NONE;

   _time_update(obj);
   _ticker(obj);

   return obj;
}

/**
 * Set the clock time
 *
 * @param obj The clock object
 * @param hrs The hours to set
 * @param min The minutes to set
 * @param sec The secondes to set
 *
 * This function updates the time that is showed by the clock widget
 *
 * @ingroup Clock
 */
EAPI void
elm_clock_time_set(Evas_Object *obj, int hrs, int min, int sec)
{
   ELM_CHECK_WIDTYPE(obj, widtype);
   Widget_Data *wd = elm_widget_data_get(obj);
   if (!wd) return;
   wd->hrs = hrs;
   wd->min = min;
   wd->sec = sec;
   _time_update(obj);
}

/**
 * Get clock time
 *
 * @param obj The clock object
 * @param hrs Pointer to the variable to get the hour of this clock
 * object
 * @param min Pointer to the variable to get the minute of this clock
 * object
 * @param sec Pointer to the variable to get the second of this clock
 * object
 *
 * This function gets the time set of the clock widget and returns it
 * on the variables passed as the arguments to function
 *
 * @ingroup Clock
 */
EAPI void
elm_clock_time_get(const Evas_Object *obj, int *hrs, int *min, int *sec)
{
   ELM_CHECK_WIDTYPE(obj, widtype);
   Widget_Data *wd = elm_widget_data_get(obj);
   if (!wd) return;
   if (hrs) *hrs = wd->hrs;
   if (min) *min = wd->min;
   if (sec) *sec = wd->sec;
}

/**
 * Set if the clock settings can be edited
 *
 * @param obj The clock object
 * @param edit Bool option for edited (1 = yes, 0 = no)
 *
 * This function sets if the clock settings can be edited or not.
 * By default or if digit_edit option was previously set to ELM_CLOCK_NONE,
 * all digits are editable. To choose what digits to make editable
 * use elm_clock_digit_edit_set().
 *
 * @ingroup Clock
 */
EAPI void
elm_clock_edit_set(Evas_Object *obj, Eina_Bool edit)
{
   ELM_CHECK_WIDTYPE(obj, widtype);
   Widget_Data *wd = elm_widget_data_get(obj);
   if (!wd) return;
   wd->edit = edit;
   if (edit && (wd->digedit == ELM_CLOCK_NONE))
     elm_clock_digit_edit_set(obj, ELM_CLOCK_ALL);
   else
     _time_update(obj);
}

/**
 * Get if the clock settings can be edited
 *
 * @param obj The clock object
 * @return Bool option for edited (1 = yes, 0 = no)
 *
 * This function gets if the clock settings can be edited or not.
 *
 * @ingroup Clock
 */
EAPI Eina_Bool
elm_clock_edit_get(const Evas_Object *obj)
{
   ELM_CHECK_WIDTYPE(obj, widtype) EINA_FALSE;
   Widget_Data *wd = elm_widget_data_get(obj);
   if (!wd) return EINA_FALSE;
   return wd->edit;
}

/**
 * Set what digits of the clock are editable
 *
 * @param obj The clock object
 * @param digedit Bit mask indicating the digits to edit
 *
 * If the digedit param is ELM_CLOCK_NONE, editing will be disabled.
 *
 * @ingroup Clock
 */
EAPI void
elm_clock_digit_edit_set(Evas_Object *obj, Elm_Clock_Digedit digedit)
{
   ELM_CHECK_WIDTYPE(obj, widtype);
   Widget_Data *wd = elm_widget_data_get(obj);
   if (!wd) return;
   wd->digedit = digedit;
   if (digedit == ELM_CLOCK_NONE)
     elm_clock_edit_set(obj, EINA_FALSE);
   else
     _time_update(obj);
}

/**
 * Get what digits of the clock are editable
 *
 * @param obj The clock object
 * @return Bit mask indicating the digits.
 *
 * @ingroup Clock
 */
EAPI Elm_Clock_Digedit
elm_clock_digit_edit_get(const Evas_Object *obj)
{
   ELM_CHECK_WIDTYPE(obj, widtype) 0;
   Widget_Data *wd = elm_widget_data_get(obj);
   if (!wd) return 0;
   return wd->digedit;
}

/**
 * Set if the clock shows hours in military or am/pm mode
 *
 * @param obj The clock object
 * @param am_pm Bool option for the hours mode
 * (1 = am/pm, 0 = military)
 *
 * This function sets the clock to show hours in military or am/pm
 * mode. Some countries like Brazil the military mode (00-24h-format)
 * is used in opposition to the USA where the am/pm mode is more
 * common used.
 *
 * @ingroup Clock
 */
EAPI void
elm_clock_show_am_pm_set(Evas_Object *obj, Eina_Bool am_pm)
{
   ELM_CHECK_WIDTYPE(obj, widtype);
   Widget_Data *wd = elm_widget_data_get(obj);
   if (!wd) return;
   wd->am_pm = am_pm;
   _time_update(obj);
}

/**
 * Get if the clock shows hours in military or am/pm mode
 *
 * @param obj The clock object
 * @return Bool option for the hours mode
 * (1 = am/pm, 0 = military)
 *
 * This function gets if the clock show hours in military or am/pm
 * mode. Some countries like Brazil the military mode (00-24h-format)
 * is used in opposition to the USA where the am/pm mode is more
 * common used.
 *
 * @ingroup Clock
 */
EAPI Eina_Bool
elm_clock_show_am_pm_get(const Evas_Object *obj)
{
   ELM_CHECK_WIDTYPE(obj, widtype) EINA_FALSE;
   Widget_Data *wd = elm_widget_data_get(obj);
   if (!wd) return EINA_FALSE;
   return wd->am_pm;
}

/**
 * Set if the clock shows hour with the seconds
 *
 * @param obj The clock object
 * @param seconds Bool option for the show seconds
 * (1 = show seconds, 0 = not show seconds)
 *
 * This function sets the clock to show or not to show the elapsed
 * seconds.
 *
 * @ingroup Clock
 */
EAPI void
elm_clock_show_seconds_set(Evas_Object *obj, Eina_Bool seconds)
{
   ELM_CHECK_WIDTYPE(obj, widtype);
   Widget_Data *wd = elm_widget_data_get(obj);
   if (!wd) return;
   wd->seconds = seconds;
   _time_update(obj);
}

/**
 * Get if the clock shows hour with the seconds
 *
 * @param obj The clock object
 * @return Bool option for the show seconds
 * (1 = show seconds, 0 = not show seconds)
 *
 * This function gets if the clock show or not show the elapsed
 * seconds.
 *
 * @ingroup Clock
 */
EAPI Eina_Bool
elm_clock_show_seconds_get(const Evas_Object *obj)
{
   ELM_CHECK_WIDTYPE(obj, widtype) EINA_FALSE;
   Widget_Data *wd = elm_widget_data_get(obj);
   if (!wd) return EINA_FALSE;
   return wd->seconds;
}
