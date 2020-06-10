/*
 * https://www.gnu.org/software/guile-gnome/docs/gtk/html/GtkTreeModel.html
 * http://scentric.net/tutorial/sec-treemodel-rowref.html
 * https://cpp.hotexamples.com/examples/-/-/GTK_ICON_VIEW/cpp-gtk_icon_view-function-examples.html
 * Get a GtkListStore selected example: 
 * https://ubuntuforums.org/showthread.php?t=1208655
 */

#include <gtk/gtk.h>
#include <assert.h>

GtkWidget *window;
GtkListStore *list_store;

enum{
  COL_DISPLAY_NAME,
  COL_PIXBUF, 
  NUM_COLS
};

enum{
  STRING_COLUMN,
  INT_COLUMN,
  N_COLUMNS
};

GtkTreeModel *init_model(void);
void icon_clicked(GtkWidget *widget, GtkTreePath *tree_path, gpointer user_data);
static gboolean exitprog(void);
static gboolean delete_event(GtkWidget *window, GdkEvent *event, gpointer parent);

int main(int argc, char *argv[]){
  GtkWidget *icon_view;
  GtkWidget *sw;
  
  gtk_init(&argc, &argv);
  
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  
  gtk_window_set_title(GTK_WINDOW(window), "ระบบพิมพ์ใบเสร็จ");
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

  gtk_window_set_icon_from_file(GTK_WINDOW(window), "./resources/bulb.png", 0);
  
  sw = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(window), sw);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw), 
    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(sw), 
    GTK_SHADOW_IN);
    
  icon_view = gtk_icon_view_new_with_model(init_model());
  gtk_container_add(GTK_CONTAINER(sw), icon_view);
  
  gtk_icon_view_set_text_column(GTK_ICON_VIEW(icon_view), 
    COL_DISPLAY_NAME);
  gtk_icon_view_set_pixbuf_column(GTK_ICON_VIEW(icon_view), COL_PIXBUF);
  gtk_icon_view_set_selection_mode(GTK_ICON_VIEW(icon_view), 
    GTK_SELECTION_SINGLE);
    
  g_signal_connect(G_OBJECT(icon_view), 
      "selection_changed", G_CALLBACK(icon_clicked), list_store);
  
  g_signal_connect(GTK_OBJECT(window), 
      "delete_event", G_CALLBACK(delete_event),(gpointer) window);
  
  gtk_widget_show_all(window);
  gtk_main();
  
  return 0;
}

GtkTreeModel *init_model(void){
  GdkPixbuf *p1, *p2, *p3, *p4, *p5;
  GtkTreeIter iter;
  GError *err = NULL;  
  
  p1 = gdk_pixbuf_new_from_file("./resources/customer.png", &err);
  p2 = gdk_pixbuf_new_from_file("./resources/parts.png", &err);
  p3 = gdk_pixbuf_new_from_file("./resources/invoice.png", &err);
  p4 = gdk_pixbuf_new_from_file("./resources/settings.png", &err);
  p5 = gdk_pixbuf_new_from_file("./resources/exit.png", &err);
  
  assert(err == NULL);
  
  list_store = gtk_list_store_new(NUM_COLS, G_TYPE_STRING, GDK_TYPE_PIXBUF);
  
  gtk_list_store_append(list_store, &iter);
  gtk_list_store_set(list_store, &iter, COL_DISPLAY_NAME, "ลูกค้า", COL_PIXBUF, p1, -1);
  gtk_list_store_append(list_store, &iter);
  gtk_list_store_set(list_store, &iter, COL_DISPLAY_NAME, "สต็อค", COL_PIXBUF, p2, -1);
  gtk_list_store_append(list_store, &iter);
  gtk_list_store_set(list_store, &iter, COL_DISPLAY_NAME, "ออกบิล", COL_PIXBUF, p3, -1);
  gtk_list_store_append(list_store, &iter);
  gtk_list_store_set(list_store, &iter, COL_DISPLAY_NAME, "ตั้งค่า", COL_PIXBUF, p4, -1);
  gtk_list_store_append(list_store, &iter);
  gtk_list_store_set(list_store, &iter, COL_DISPLAY_NAME, "เลิกใช้งาน", COL_PIXBUF, p5, -1);
  
  g_object_unref(p1);
  g_object_unref(p2);
  g_object_unref(p3);
  g_object_unref(p4);
  g_object_unref(p5);
  
  return GTK_TREE_MODEL(list_store);
}

void icon_clicked(GtkWidget *widget, GtkTreePath *tree_path, gpointer user_data){
  GtkTreeModel *model = gtk_icon_view_get_model(GTK_ICON_VIEW(widget));
  GtkTreeIter iter;
 
  GList *sel = gtk_icon_view_get_selected_items(GTK_ICON_VIEW(widget));
   
  if (sel != NULL){
    GtkTreePath *path;
    path = sel->data;

    gtk_tree_model_get_iter(model, &iter, path);
    if (gtk_tree_model_get_iter(model, &iter, path)){
      gchar *str;
      gtk_tree_model_get(model, &iter, 0, &str, -1);
      gint *idx = gtk_tree_path_get_indices(path);
      g_print("%d -> %s\n", idx[0], str);
      
      switch (idx[0]){
        case 4:
          exitprog();
          break;
        default:
          break;
      }
    }
  }      
}

static gboolean
exitprog(void){
  
  gtk_widget_hide(window);
  
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(window), 
              GTK_DIALOG_DESTROY_WITH_PARENT, 
              GTK_MESSAGE_QUESTION, 
              GTK_BUTTONS_YES_NO, 
              "คุณต้องการออกจากโปรแกรมใช่หรือไม่?");
  gtk_window_set_title(GTK_WINDOW(dialog), "Question");
  int result = gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
  if (result == GTK_RESPONSE_YES){
    gtk_main_quit();
    return FALSE;
  }else{
    gtk_widget_show_all(window);
    return TRUE;
  }
}

static gboolean 
delete_event(GtkWidget *window, GdkEvent *event, gpointer parent){
  GtkWidget *dialog;
  dialog = gtk_dialog_new_with_buttons("Exit program", 
              GTK_WINDOW(parent), GTK_DIALOG_MODAL, 
              GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT, 
              GTK_STOCK_OK, GTK_RESPONSE_ACCEPT, NULL);
 
  GtkWidget *label = gtk_label_new(
        "กรุณายืนยัน, ต้องการออกจากโปรแกรมใช่หรือไม่");
  GtkWidget *image = gtk_image_new_from_stock(
        GTK_STOCK_DIALOG_QUESTION, GTK_ICON_SIZE_LARGE_TOOLBAR);
  GtkWidget *hbox = gtk_hbox_new(FALSE, 5);
 
  gtk_container_set_border_width(GTK_CONTAINER(hbox), 10);
  gtk_box_pack_start(GTK_BOX(hbox), image, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
 
  gtk_box_pack_start(
        GTK_BOX(GTK_DIALOG(dialog)->vbox), hbox, FALSE, FALSE, 0);
 
  gtk_container_set_border_width(GTK_CONTAINER(dialog), 10);
  gtk_widget_show_all(dialog);
 
  int result = gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
 
  if (result == GTK_RESPONSE_ACCEPT){
    gtk_main_quit();
    return FALSE;
  }else{
    return TRUE;
  }
}
