/* example:
 * 
 * http://www.compsci.hunter.cuny.edu/~sweiss/course_materials/csci493.70/lecture_notes/GTK_treeview.pdf
 * https://en.wikibooks.org/wiki/GTK%2B_By_Example/Tree_View/Tree_Models
 * https://stackoverflow.com/questions/45532565/how-to-get-the-data-of-an-activated-row-gtk-c
 * https://developer.gnome.org/gtk2/stable/TreeWidget.html
 * 
 */

#include <gtk/gtk.h>

enum{
  COL_CODE = 0, 
  COL_NAME,
  COL_PHONE,
  NUM_COLS
};

GtkWidget *entCode, *entName, *entAddr1, *entAddr2, *entPhone;
GtkWidget *btnSave, *btnCancel;

static GtkTreeModel *create_and_fill_model (void);
static GtkWidget *create_view_and_model(void);
void frm_customer(gpointer vbox, gpointer hbox);
void sensitive_default(void);
void view_clicked(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, gpointer data);
void view_selected(GtkTreeSelection *sel, gpointer data);

int main(int argc, char *argv[]){
  //GtkWidget *win;
  GtkWidget *view;
  GtkWidget *label;
  
  GtkWidget *button, *vbox, *hbox, *dlg;
  
  gtk_init(&argc, &argv);
  GtkSettings *default_settings = gtk_settings_get_default();
  g_object_set(default_settings, "gtk-button-images", TRUE, NULL);
  
  dlg = gtk_dialog_new();
  gtk_window_set_title(GTK_WINDOW(dlg), "ลูกค้า");
  gtk_window_set_position(GTK_WINDOW(dlg), GTK_WIN_POS_CENTER);
  gtk_widget_set_size_request(dlg, 600, 600);
  gtk_container_set_border_width(GTK_CONTAINER(dlg), 10);
  
  hbox = gtk_hbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dlg)->vbox), hbox);
  
  view = create_view_and_model();
  gtk_box_pack_start(GTK_BOX(hbox), view, FALSE, FALSE, 0);
  
  GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
  gtk_tree_selection_set_mode(GTK_TREE_SELECTION(sel), GTK_SELECTION_SINGLE);
  
  g_signal_connect(G_OBJECT(view), "row-activated", G_CALLBACK(view_clicked), NULL);
  g_signal_connect(G_OBJECT(sel), "changed", G_CALLBACK(view_selected), NULL);
     
  vbox = gtk_vbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(hbox), vbox, FALSE, FALSE, 0);
  
  /* entry zone */
  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

  frm_customer(vbox, hbox);

  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dlg)->vbox), hbox, FALSE, FALSE, 0);
  label = gtk_label_new("*** คลิกรายชื่อเพื่อเลือกทำรายการ");
  gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);
  /* end entry zone */

  hbox = gtk_hbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dlg)->action_area), hbox);

  button = gtk_button_new_from_stock(GTK_STOCK_QUIT);
  g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gtk_main_quit), NULL);
  gtk_box_pack_end(GTK_BOX(hbox), button, FALSE, FALSE, 0);
   
  gtk_widget_show_all(dlg);
  gtk_main();
  return 0;
}

void view_selected(GtkTreeSelection *sel, gpointer data){
  GtkTreeIter iter;
  GtkTreeModel *model;
  if (gtk_tree_selection_get_selected(sel, &model, &iter)){
    const gchar *code, *name, *phone;
    gtk_tree_model_get(model, &iter, COL_CODE, &code, -1);
    gtk_tree_model_get(model, &iter, COL_NAME, &name, -1);
    gtk_tree_model_get(model, &iter, COL_PHONE, &phone, -1);

    gtk_entry_set_text(GTK_ENTRY(entCode), code);
    gtk_entry_set_text(GTK_ENTRY(entName), name);
    gtk_entry_set_text(GTK_ENTRY(entPhone), phone);

    g_print("Selected code: %s\n", code);
  }
}

//void view_clicked(GtkWidget *widget, GtkTreePath *tree_path, gpointer user_data){
void view_clicked(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, gpointer data){
  //GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(widget));
  GtkTreeModel *model = gtk_tree_view_get_model(treeview);
  GtkTreeIter iter;
  //GtkTreePath *path = gtk_tree_model_get_path(model, &iter);
  
  
  if (gtk_tree_model_get_iter(model, &iter, path)){
    g_print("Double Clicked\n");
  }
}

void frm_customer(gpointer vbox, gpointer hbox){
  GtkWidget *label;
  label = gtk_label_new("รหัส: ");
  gtk_misc_set_alignment(GTK_MISC(label), 1.0f, 0.5f);
  gtk_widget_set_size_request(label, 100, 30);
  gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 10);
  
  entCode = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(hbox), entCode, FALSE, FALSE, 10);
  gtk_widget_set_size_request(entCode, 50, 30);
  
  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
  label = gtk_label_new("ชื่อ-นามสกุล: ");
  gtk_misc_set_alignment(GTK_MISC(label), 1.0f, 0.5f);
  gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 10);
  gtk_widget_set_size_request(label, 100, 30);
  
  entName = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(hbox), entName, FALSE, FALSE, 10);

  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
  label = gtk_label_new("ที่อยู่ 1: ");
  gtk_misc_set_alignment(GTK_MISC(label), 1.0f, 0.5f);
  gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 10);
  gtk_widget_set_size_request(label, 100, 30);

  entAddr1 = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(hbox), entAddr1, FALSE, FALSE, 10);

  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
  label = gtk_label_new("ที่อยู่ 2: ");
  gtk_misc_set_alignment(GTK_MISC(label), 1.0f, 0.5f);
  gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 10);
  gtk_widget_set_size_request(label, 100, 30);

  entAddr2 = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(hbox), entAddr2, FALSE, FALSE, 10);

  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
  label = gtk_label_new("โทรศํพท์: ");
  gtk_misc_set_alignment(GTK_MISC(label), 1.0f, 0.5f);
  gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 10);
  gtk_widget_set_size_request(label, 100, 30);

  entPhone = gtk_entry_new();
  gtk_box_pack_start(GTK_BOX(hbox), entPhone, FALSE, FALSE, 10);
  
  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

  btnCancel = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
  gtk_box_pack_end(GTK_BOX(hbox), btnCancel, FALSE, FALSE, 10);

  btnSave = gtk_button_new_from_stock(GTK_STOCK_SAVE);
  gtk_box_pack_end(GTK_BOX(hbox), btnSave, FALSE, FALSE, 0);

  sensitive_default();
}

void sensitive_default(void){
  gtk_widget_set_sensitive(entCode, TRUE);
  gtk_widget_set_sensitive(entName, FALSE);
  gtk_widget_set_sensitive(entAddr1, FALSE);
  gtk_widget_set_sensitive(entAddr2, FALSE);
  gtk_widget_set_sensitive(entPhone, FALSE);
  gtk_widget_set_sensitive(btnSave, FALSE);
  gtk_widget_set_sensitive(btnCancel, FALSE);
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
    "text", COL_CODE, NULL);
  
  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_insert_column_with_attributes(
    GTK_TREE_VIEW(view), 
    -1, 
    "ชื่อ-นามสกุล", 
    renderer, 
    "text", COL_NAME, NULL);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_insert_column_with_attributes(
    GTK_TREE_VIEW(view), 
    -1, 
    "โทรศัพท์", 
    renderer, 
    "text", COL_PHONE, NULL);
  
  model = create_and_fill_model();
  gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
  
  g_object_unref(model);
  return view;
}

static GtkTreeModel *create_and_fill_model(void){
  GtkListStore *store;
  GtkTreeIter iter;
  
  store = gtk_list_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
  
  /*Append a row and fill in some data */
  gtk_list_store_append(store, &iter);
  gtk_list_store_set(
    store, &iter, 
    COL_CODE, "001",
    COL_NAME, "เมธา เทพสายฟ้า", 
    COL_PHONE, "081-123-4567", -1);
    
  gtk_list_store_append(store, &iter);
  gtk_list_store_set(
    store, &iter, 
    COL_CODE, "002",
    COL_NAME, "สายฟ้า เทพเมธา", 
    COL_PHONE, "082-345-6789", -1);
    
  gtk_list_store_append(store, &iter);
  gtk_list_store_set(
    store, &iter, 
    COL_CODE, "003",
    COL_NAME, "เทพ สายฟ้าเมธา", 
    COL_PHONE, "083-456-7890", -1);
  
  return GTK_TREE_MODEL(store);
}
