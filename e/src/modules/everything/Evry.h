#ifndef EVRY_H
#define EVRY_H

#include "e.h"

#define EVRY_API_VERSION 16

#define EVRY_ACTION_OTHER 0
#define EVRY_ACTION_FINISHED 1
#define EVRY_ACTION_CONTINUE 2

#define EVRY_ASYNC_UPDATE_ADD 0
#define EVRY_ASYNC_UPDATE_CLEAR 1
#define EVRY_ASYNC_UPDATE_REFRESH 2

#define EVRY_COMPLETE_NONE   0
#define EVRY_COMPLETE_INPUT  1
#define EVRY_COMPLETE_BROWSE 2

#define VIEW_MODE_NONE  -1
#define VIEW_MODE_LIST   0
#define VIEW_MODE_DETAIL 1
#define VIEW_MODE_THUMB  2

#define EVRY_PLUGIN_SUBJECT 0
#define EVRY_PLUGIN_ACTION  1
#define EVRY_PLUGIN_OBJECT  2


extern int _e_module_evry_log_dom;

#ifndef EINA_LOG_DEFAULT_COLOR
#define EINA_LOG_DEFAULT_COLOR EINA_COLOR_CYAN
#endif

#undef DBG
#undef INF
#undef WRN
#undef ERR

#define DBG(...) EINA_LOG_DOM_DBG(_e_module_evry_log_dom , __VA_ARGS__)
#define INF(...) EINA_LOG_DOM_INFO(_e_module_evry_log_dom , __VA_ARGS__)
#define WRN(...) EINA_LOG_DOM_WARN(_e_module_evry_log_dom , __VA_ARGS__)
#define ERR(...) EINA_LOG_DOM_ERR(_e_module_evry_log_dom , __VA_ARGS__)

typedef struct _Evry_Plugin		Evry_Plugin;
typedef struct _Evry_Item		Evry_Item;
typedef struct _Evry_Item_App		Evry_Item_App;
typedef struct _Evry_Item_File		Evry_Item_File;
typedef struct _Evry_Action		Evry_Action;
typedef struct _Evry_State		Evry_State;
typedef struct _Evry_View		Evry_View;
typedef struct _History			Evry_History;
typedef struct _History_Entry		History_Entry;
typedef struct _History_Types		History_Types;
typedef struct _History_Item		History_Item;
typedef struct _Config			Evry_Config;
typedef struct _Plugin_Config		Plugin_Config;
typedef struct _Plugin_Setting		Plugin_Setting;
typedef struct _Evry_Event_Item_Changed Evry_Event_Item_Changed;

typedef unsigned int Evry_Type;

#define EVRY_ITEM(_item) ((Evry_Item *)_item)
#define EVRY_ACTN(_item) ((Evry_Action *) _item)
#define EVRY_PLUGIN(_plugin) ((Evry_Plugin *) _plugin)
#define EVRY_VIEW(_view) ((Evry_View *) _view)
#define EVRY_FILE(_it) ((Evry_Item_File *) _it)

#define CHECK_TYPE(_item, _type) \
  (((Evry_Item *)_item)->type && ((Evry_Item *)_item)->type == _type)

#define CHECK_SUBTYPE(_item, _type) \
  (((Evry_Item *)_item)->subtype && ((Evry_Item *)_item)->subtype == _type)

#define IS_BROWSEABLE(_item) \
  ((Evry_Item *)_item)->browseable

#define GET_APP(_app, _item) Evry_Item_App *_app = (Evry_Item_App *) _item
#define GET_FILE(_file, _item) Evry_Item_File *_file = (Evry_Item_File *) _item
#define GET_EVRY_PLUGIN(_p, _plugin) Evry_Plugin *_p = (Evry_Plugin*) _plugin
#define GET_VIEW(_v, _view) View *_v = (View*) _view
#define GET_ACTION(_act, _item) Evry_Action *_act = (Evry_Action *) _item
#define GET_PLUGIN(_p, _plugin) Plugin *_p = (Plugin*) _plugin
#define GET_ITEM(_it, _any) Evry_Item *_it = (Evry_Item *) _any

#define EVRY_ITEM_DATA_INT_SET(_item, _data) ((Evry_Item *)_item)->data = (void*)(long) _data
#define EVRY_ITEM_DATA_INT_GET(_item) (long) ((Evry_Item *)_item)->data
#define EVRY_ITEM_ICON_SET(_item, _icon) ((Evry_Item *)_item)->icon = _icon

#define EVRY_ITEM_DETAIL_SET(_it, _detail)				\
  if (EVRY_ITEM(_it)->detail) eina_stringshare_del(EVRY_ITEM(_it)->detail); \
  EVRY_ITEM(_it)->detail = eina_stringshare_add(_detail);

#define EVRY_ITEM_LABEL_SET(_it, _label)				\
  if (EVRY_ITEM(_it)->label) eina_stringshare_del(EVRY_ITEM(_it)->label); \
  EVRY_ITEM(_it)->label = eina_stringshare_add(_label);

#define EVRY_ITEM_CONTEXT_SET(_it, _context)				\
  if (EVRY_ITEM(_it)->context) eina_stringshare_del(EVRY_ITEM(_it)->context); \
  EVRY_ITEM(_it)->context = eina_stringshare_add(_context);

#define EVRY_ITEM_NEW(_base, _plugin, _label, _icon_get, _free)	\
  (_base *) evry_item_new(EVRY_ITEM(E_NEW(_base, 1)), EVRY_PLUGIN(_plugin), \
			  _label, _icon_get, _free)

#define EVRY_ITEM_FREE(_item) evry_item_free((Evry_Item *)_item)

#define EVRY_PLUGIN_NEW(_base, _name, _icon, _item_type, _begin, _cleanup, _fetch, _free) \
  evry_plugin_new(EVRY_PLUGIN(E_NEW(_base, 1)), _name, _(_name), _icon, _item_type, \
		  _begin, _cleanup, _fetch, _free)


#define EVRY_ACTION_NEW(_name, _in1, _in2, _icon, _action, _check) \
  evry_action_new(_name, _(_name), _in1, _in2, _icon, _action, _check)


#define EVRY_PLUGIN_FREE(_p) \
  if (_p) evry_plugin_free(EVRY_PLUGIN(_p))


#define EVRY_PLUGIN_ITEMS_CLEAR(_p) { \
     Evry_Item *it;				\
     EINA_LIST_FREE(EVRY_PLUGIN(_p)->items, it) \
       it->fuzzy_match = 0; }

#define EVRY_PLUGIN_ITEMS_FREE(_p) { \
     Evry_Item *it;				\
     EINA_LIST_FREE(EVRY_PLUGIN(_p)->items, it) \
       evry_item_free(it); }


#define EVRY_PLUGIN_ITEMS_SORT(_p, _sortcb) \
  EVRY_PLUGIN(_p)->items = eina_list_sort \
    (EVRY_PLUGIN(_p)->items, eina_list_count(EVRY_PLUGIN(_p)->items), _sortcb)

#define EVRY_PLUGIN_ITEM_APPEND(_p, _item) \
  EVRY_PLUGIN(_p)->items = eina_list_append(EVRY_PLUGIN(_p)->items, EVRY_ITEM(_item))

#define EVRY_PLUGIN_ITEMS_ADD(_plugin, _items, _input, _match_detail, _set_usage) \
  evry_util_plugin_items_add(EVRY_PLUGIN(_plugin), _items, _input, _match_detail, _set_usage)

#define IF_RELEASE(x) do {						\
     if (x) {								\
	const char *__tmp; __tmp = (x); (x) = NULL; eina_stringshare_del(__tmp); \
     }									\
     (x) = NULL;							\
  } while (0)

struct _Evry_Item
{
  /* label to show for this item */
  const char *label;

  /* optional: more information to be shown */
  const char *detail;

  /* optional: for 'static' fdo icon name, otherwise use _icon_get */
  const char *icon;

  /* item can be browsed, e.g. folders */
  Eina_Bool browseable;

  /* optional: for internally use by plugins */
  void *data;

  /* optional: priority hints for sorting */
  int priority;

  /* optional: store value of fuzzy match with input */
  int fuzzy_match;

  /* optional: plugin can set id to identify
   * it in history otherwise label is used */
  const char *id;

  /* optional: context provided by item. e.g. to remember which action
   * was performed on a file with a specific mimetype */
  const char *context;

  /* is set to type of Evry_Plugin by default */
  Evry_Type type;

  /* optional */
  Evry_Type subtype;

  /* do not set! */
  Eina_List *items;

  Evas_Object *(*icon_get) (Evry_Item *it, Evas *e);
  void (*free) (Evry_Item *item);

  /* do not set by plugin! */
  Eina_Bool selected;
  Eina_Bool marked;
  Evry_Plugin *plugin;
  int ref;
  double usage;
  History_Item *hi;
};

struct _Evry_Action
{
  Evry_Item base;

  /* identifier */
  const char *name;

  struct
  {
    /* requested type for action */
    Evry_Type type;
    Evry_Type subtype;
    /* handle multiple items */
    Eina_Bool accept_list;

    /* do not set ! */
    const Evry_Item *item;
    Eina_List *items;
  } it1;

  struct
  {
    Evry_Type type;
    Evry_Type subtype;
    Eina_Bool accept_list;

    /* do not set ! */
    const Evry_Item *item;
    Eina_List *items;
  } it2;


  /* optional: this action is specific for a item 'context'.
     e.g. 'copy' for file mime-type is not, 'image viewer' is.
     default is FALSE */
  Eina_Bool remember_context;

  int  (*action)     (Evry_Action *act);

  /* optional: check whether action fits to chosen item */
  int  (*check_item) (Evry_Action *act, const Evry_Item *it);
  /* optional */
  void (*free)       (Evry_Action *act);
  /* optional: must be defined when  action is browseable */
  Eina_List *(*fetch) (Evry_Action *act);
};

struct _Evry_Item_App
{
  Evry_Action base;
  const char *file;
  Efreet_Desktop *desktop;
};

struct _Evry_Item_File
{
  Evry_Item base;
  const char *url;
  const char *path;
  const char *mime;
};

struct _Evry_Plugin
{
  Evry_Item base;

  /* identifier */
  const char *name;

  /* list of items visible for everything after fetch */
  Eina_List *items;

  /* required: get candidates matching string, fill 'items' list */
  int  (*fetch) (Evry_Plugin *p, const char *input);

  /* required: run when state is removed in which this plugin is
     active. free 'items' here */
  void (*finish) (Evry_Plugin *p);

  /* optional: plugin is added to the list of current plugins and
     queried for results when not returning NULL. The previos
     selectors item is passed, i.e. a plugin registered as action
     receives the subject, a plugin registered as object receives the
     action item. here you can check wheter the plugin can be queried,
     for given context (provided by item) */
  Evry_Plugin *(*begin) (Evry_Plugin *p, const Evry_Item *item);

  /* optional: provide a list of subitems of 'item'. this function
     must return a new instance which must be freed in 'finish' */
  Evry_Plugin *(*browse) (Evry_Plugin *p, const Evry_Item *item);

  /* optional: try to complete current item:
     return: EVRY_COMPLETE_INPUT when input was changed
     return: EVRY_COMPLETE_BROWSE to browse item */
  int  (*complete) (Evry_Plugin *p, const Evry_Item *item, char **input);

  /* optional: handle key events: return positive when key was
     handled */
  int  (*cb_key_down)  (Evry_Plugin *p, const Ecore_Event_Key *ev);

  /* optional: use this when begin returned a new instance or you
     have extended plugin struct */
  void (*free) (Evry_Plugin *p);

  /* optional: whether the plugin uses evry_async_update to add new items */
  /* default FALSE */
  Eina_Bool async_fetch;

  /* optional: request items to be remembered for usage statistic */
  /* default TRUE */
  Eina_Bool history;

  /* optional: if transient, item is removed from history on cleanup */
  /* default FALSE */
  Eina_Bool transient;

  /* optional: config path registered for the module, to show
     'configure' button in everything config */
  const char *config_path;

  /* not to be set by plugin! */
  Plugin_Config *config;
  unsigned int request;
};


struct _Evry_State
{
  char *inp; /* alloced input */

  char *input; /* pointer to input + trigger */
  /* all available plugins for current state */
  Eina_List   *plugins;

  /* currently active plugins, i.e. those that provide items */
  Eina_List   *cur_plugins;

  /* active plugin */
  Evry_Plugin *plugin;

  /* selected item */
  Evry_Item   *cur_item;

  /* marked items */
  Eina_List   *sel_items;

  Eina_Bool plugin_auto_selected;
  Eina_Bool item_auto_selected;

  /* current view instance */
  Evry_View *view;

  Eina_Bool changed;
  Eina_Bool trigger_active;

  unsigned int request;
};

struct _Evry_View
{
  Evry_View  *id;
  const char *name;
  const char *trigger;
  int active;
  Evas_Object *o_list;
  Evas_Object *o_bar;

  Evry_View *(*create) (Evry_View *view, const Evry_State *s, const Evas_Object *swallow);
  void (*destroy)      (Evry_View *view);
  int  (*cb_key_down)  (Evry_View *view, const Ecore_Event_Key *ev);
  int  (*update)       (Evry_View *view, int slide);
  void (*clear)        (Evry_View *view, int slide);

  int priority;
};

struct _Evry_Event_Item_Changed
{
  Evry_Item *item;
  int changed_selection;
  int changed_icon;
};

/* FIXME this should be exposed.
   - add functions to retrieve this stuff */
struct _Config
{
  int version;
  /* position */
  double rel_x, rel_y;
  /* size */
  int width, height;

  Eina_List *modules;

  /* generic plugin config */
  Eina_List *conf_subjects;
  Eina_List *conf_actions;
  Eina_List *conf_objects;
  Eina_List *conf_views;

  int scroll_animate;
  double scroll_speed;

  int hide_input;
  int hide_list;

  /* quick navigation mode */
  int quick_nav;

  const char *cmd_terminal;
  const char *cmd_sudo;

  /* default view mode */
  int view_mode;
  int view_zoom;

  int history_sort_mode;

  /* use up/down keys for prev/next in thumb view */
  int cycle_mode;

  unsigned char first_run;

  /* not saved data */
  Eina_List *actions;
  Eina_List *views;

  int min_w, min_h;
};

struct _Plugin_Config
{
  /* do not set! */
  const char *name;
  int enabled;

  /* request initial sort order of this plugin */
  int priority;

  /* trigger to show plugin exclusively */
  const char *trigger;

  /* only show plugin when triggered */
  int trigger_only;

  /* preffered view mode */
  int view_mode;

  /* minimum input char to start query items,
     this must be handled by plugin */
  int min_query;

  /* show items of plugin in aggregator */
  int aggregate;

  /* if not top-level the plugin is shown in aggregator
     instead of the items  */
  int top_level;

  /* Eina_Hash *settings; */

  /* do not set! */
  Evry_Plugin *plugin;
};

/* struct _Plugin_Config_Setting
 * {
 *   const char *str;
 *   double num;
 * }; */

struct _History_Entry
{
  Eina_List *items;
};

struct _History_Types
{
  Eina_Hash *types;
};

struct _History
{
  int version;
  Eina_Hash *subjects;
  Eina_Hash *actions;
  double begin;

  Eina_Bool changed;
};

struct _History_Item
{
  const char *plugin;
  const char *context;
  const char *input;
  double last_used;
  double usage;
  int count;
  int transient;
  const char *data;
};

/* evry.c */
EAPI void evry_item_select(const Evry_State *s, Evry_Item *it);
EAPI void evry_item_mark(const Evry_State *state, Evry_Item *it, Eina_Bool mark);
EAPI void evry_plugin_select(const Evry_State *s, Evry_Plugin *p);
EAPI int  evry_list_win_show(void);
EAPI void evry_list_win_hide(void);
EAPI Evry_Item *evry_item_new(Evry_Item *base, Evry_Plugin *p, const char *label,
			      Evas_Object *(*icon_get) (Evry_Item *it, Evas *e),
			      void (*cb_free) (Evry_Item *item));
EAPI void evry_item_free(Evry_Item *it);
EAPI void evry_item_ref(Evry_Item *it);

EAPI void evry_plugin_async_update(Evry_Plugin *plugin, int state);
EAPI void evry_clear_input(Evry_Plugin *p);

/* evry_util.c */
EAPI Evas_Object *evry_icon_mime_get(const char *mime, Evas *e);
EAPI Evas_Object *evry_icon_theme_get(const char *icon, Evas *e);
EAPI int  evry_fuzzy_match(const char *str, const char *match);
EAPI Eina_List *evry_fuzzy_match_sort(Eina_List *items);
EAPI int evry_util_exec_app(const Evry_Item *it_app, const Evry_Item *it_file);
EAPI char *evry_util_url_escape(const char *string, int inlength);
EAPI char *evry_util_unescape(const char *string, int length);
EAPI void evry_util_file_detail_set(Evry_Item_File *file);
EAPI Eina_Bool evry_util_module_config_check(const char *module_name, int conf, int epoch, int version);
EAPI Evas_Object *evry_util_icon_get(Evry_Item *it, Evas *e);
EAPI int evry_util_plugin_items_add(Evry_Plugin *p, Eina_List *items, const char *input, int match_detail, int set_usage);
EAPI int evry_items_sort_func(const void *data1, const void *data2);
EAPI void evry_event_item_changed(Evry_Item *it, int change_icon, int change_selected);
EAPI char *evry_util_md5_sum(const char *str);

EAPI const char *evry_file_path_get(Evry_Item_File *file);
EAPI const char *evry_file_uri_get(Evry_Item_File *file);

/* e_mod_main.c */
/* returns 1 when a new plugin config was created. in this case you can
   set defaults of p->config */
EAPI int evry_plugin_register(Evry_Plugin *p, int type, int priority);
EAPI void evry_plugin_unregister(Evry_Plugin *p);
EAPI void evry_action_register(Evry_Action *act, int priority);
EAPI void evry_action_unregister(Evry_Action *act);
EAPI void evry_view_register(Evry_View *view, int priority);
EAPI void evry_view_unregister(Evry_View *view);
EAPI Evry_Action *evry_action_find(const char *name);


EAPI void evry_history_load(void);
EAPI void evry_history_unload(void);
EAPI History_Item *evry_history_add(Eina_Hash *hist, Evry_Item *it, const char *ctxt, const char *input);
EAPI int  evry_history_item_usage_set(Eina_Hash *hist, Evry_Item *it, const char *input, const char *ctxt);
EAPI History_Types *evry_history_types_get(Eina_Hash *hist, Evry_Type type);

EAPI Evry_Plugin *evry_plugin_new(Evry_Plugin *base, const char *name, const char *label, const char *icon,
				  Evry_Type item_type,
				  Evry_Plugin *(*begin) (Evry_Plugin *p, const Evry_Item *item),
				  void (*cleanup) (Evry_Plugin *p),
				  int  (*fetch)   (Evry_Plugin *p, const char *input),
				  void (*free) (Evry_Plugin *p));

EAPI void evry_plugin_free(Evry_Plugin *p);

EAPI Evry_Action *evry_action_new(const char *name, const char *label,
				  Evry_Type type1, Evry_Type type2,
				  const char *icon,
				  int  (*action)     (Evry_Action *act),
				  int  (*check_item) (Evry_Action *act, const Evry_Item *it));

EAPI void evry_action_free(Evry_Action *act);

EAPI int evry_api_version_check(int version);

EAPI Evry_Type evry_type_register(const char *type);
EAPI const char *evry_type_get(Evry_Type type);

EAPI extern int EVRY_EVENT_ITEM_SELECT;
EAPI extern int EVRY_EVENT_ITEM_CHANGED;
EAPI extern int EVRY_EVENT_ITEMS_UPDATE;

EAPI extern Evry_Type EVRY_TYPE_NONE;
EAPI extern Evry_Type EVRY_TYPE_FILE;
EAPI extern Evry_Type EVRY_TYPE_DIR;
EAPI extern Evry_Type EVRY_TYPE_APP;
EAPI extern Evry_Type EVRY_TYPE_ACTION;
EAPI extern Evry_Type EVRY_TYPE_PLUGIN;
EAPI extern Evry_Type EVRY_TYPE_BORDER;
EAPI extern Evry_Type EVRY_TYPE_TEXT;

EAPI extern Evry_History *evry_hist;
EAPI extern Evry_Config *evry_conf;

#endif
