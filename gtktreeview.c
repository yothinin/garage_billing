#include <gtk/gtk.h>

enum{
  COL_NAME = 0, 
  COL_AGE, 
  NUM_COLS
};

static GtkTreeModel *create_and_fill_model (void);
static GtkWidget *create_view_and_model(void);

int main(int argc, char *argv[]){
  //GtkWidget *win;
  GtkWidget *view;
  
  GtkWidget *button , *hbox, *dlg;
  
  gtk_init(&argc, &argv);
  
  dlg = gtk_dialog_new();
  gtk_window_set_title(GTK_WINDOW(dlg), "ลูกค้า");
  gtk_window_set_position(GTK_WINDOW(dlg), GTK_WIN_POS_CENTER);
  gtk_widget_set_size_request(dlg, 400, 300);
  
  hbox = gtk_hbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dlg)->vbox), hbox);
  
  //win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  //g_signal_connect(win, "delete_event", gtk_main_quit, NULL);
  
  view = create_view_and_model();
  //gtk_container_add(GTK_CONTAINER(win), view);
  gtk_box_pack_start(GTK_BOX(hbox), view, FALSE, FALSE, 0);
  
  button = gtk_button_new_from_stock(GTK_STOCK_QUIT);
  g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gtk_main_quit), NULL);
  gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dlg)->action_area), button);
  
  gtk_widget_show_all(dlg);
  gtk_main();
  return 0;
}

static GtkWidget *create_view_and_model(void){
  GtkCellRenderer *renderer;
  GtkTreeModel *model;
  GtkWidget *view;
  
  view = gtk_tree_view_new();
  
  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_insert_column_with_attributes(
    GTK_TREE_VIEW(view), 
    -1, 
    "รหัส", 
    renderer, 
    "text", COL_NAME, NULL);
  
  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_insert_column_with_attributes(
    GTK_TREE_VIEW(view), 
    -1, 
    "ชื่อ-นามสกุล", 
    renderer, 
    "text", COL_AGE, NULL);
  
  model = create_and_fill_model();
  gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
  
  g_object_unref(model);
  return view;
}

static GtkTreeModel *create_and_fill_model(void){
  GtkListStore *store;
  GtkTreeIter iter;
  
  store = gtk_list_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_UINT);
  
  /*Append a row and fill in some data */
  gtk_list_store_append(store, &iter);
  gtk_list_store_set(
    store, &iter, 
    COL_NAME, "เมธา เทพสายฟ้า", 
    COL_AGE, 20, -1);
    
  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, 
  COL_NAME, "โยธา ลีลาหาญ", 
  COL_AGE, 18, -1);
  
  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter, 
  COL_NAME, "สุริยา วงศ์เทพบุตร", 
  COL_AGE, 25, -1);
  
  return GTK_TREE_MODEL(store);
}
