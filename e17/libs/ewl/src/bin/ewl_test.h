#include "Ewl.h"
#include <ewl-config.h>

#if HAVE___ATTRIBUTE__
#define __UNUSED__ __attribute__((unused))
#else
#define __UNUSED__
#endif

#define BUTTONS 18

typedef struct _test_set test_set;
struct _test_set
{
	char *name;
	Ewl_Callback_Function func;
	char *filename;
};

void __close_main_window                (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_border_test_window        (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_box_test_window           (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_button_test_window        (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_colorpicker_test_window   (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_combo_test_window         (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_dialog_test_window        (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_fileselector_test_window  (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_filedialog_test_window    (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_filedialog_multi_test_window (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_floater_test_window       (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_entry_test_window         (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_image_test_window         (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_imenu_test_window         (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_menu_test_window          (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_menubar_test_window       (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_notebook_test_window      (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_paned_test_window         (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_password_test_window      (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_progressbar_test_window   (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_scrollpane_test_window    (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_seeker_test_window        (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_selectionbar_test_window  (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_selectionbook_test_window (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_spinner_test_window       (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_statusbar_test_window     (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_table_test_window         (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_textarea_test_window      (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_theme_test_window         (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_tooltip_test_window       (Ewl_Widget * w, void *ev_data, void *user_data);
void __create_tree_test_window          (Ewl_Widget * w, void *ev_data, void *user_data);

