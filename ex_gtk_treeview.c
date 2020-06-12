/*
 * https://developer.gnome.org/gtk2/stable/TreeWidget.html
 * https://en.wikibooks.org/wiki/GTK%2B_By_Example/Tree_View/Tree_Models
 * 
 */

#include <gtk/gtk.h>

/* Create enum for display all columns */
enum{
  TITLE_COLUMN,
  AUTHOR_COLUMN,
  QTY_COLUMN,
  N_COLUMNS
};

void view_selected(GtkTreeSelection *sel, gpointer data);

int main(int argc, char *argv[]){
  
  gtk_init(&argc, &argv);
  
  /* 1. Create win */
  GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(win), "GtkTreeView example");
  g_signal_connect(G_OBJECT(win), "destroy", G_CALLBACK(gtk_main_quit), NULL);

  /* 2. Create store */
  GtkTreeStore *store = gtk_tree_store_new(
    N_COLUMNS,
    G_TYPE_STRING, 
    G_TYPE_STRING, 
    G_TYPE_UINT);
  
  /* 3. Create iter */
  GtkTreeIter iter1, iter2;
  
  /* 4. Append data to store referrence to &iter */
  gtk_tree_store_append(store, &iter1, NULL);
  gtk_tree_store_set(store, &iter1, 
    TITLE_COLUMN, "คัมภีร์การใช้งานไมโครคอนโทรเลอร์", 
    AUTHOR_COLUMN, "เดชฤทธิ์ มณีธรรม", 
    QTY_COLUMN, 2, -1);
  
  gtk_tree_store_append(store, &iter2, &iter1);
  gtk_tree_store_set(store, &iter2, 
    TITLE_COLUMN, "เล่ม 1: Arduino", 
    QTY_COLUMN, 1, -1);

  gtk_tree_store_append(store, &iter2, &iter1);
  gtk_tree_store_set(store, &iter2, 
    TITLE_COLUMN, "เล่ม 2: Raspberry Pi", 
    QTY_COLUMN, 1, -1);
  
  gtk_tree_store_append(store, &iter1, NULL);
  gtk_tree_store_set(store, &iter1, 
    TITLE_COLUMN, "Mastering Qt5", 
    AUTHOR_COLUMN, "Guillame Lazar", 
    QTY_COLUMN, 1, -1);

  /* 5. Creaete tree to contain store and keep tree widget to win */
  GtkWidget *tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
  gtk_container_add(GTK_CONTAINER(win), tree);
 
  /* 6. Renderer data to view */
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes(
    "ผู้แต่ง", 
    renderer, 
    "text", AUTHOR_COLUMN, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes(
    "ชื่อเรื่อง", 
    renderer, 
    "text", TITLE_COLUMN, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes(
    "จำนวน", 
    renderer, 
    "text", QTY_COLUMN, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);
  
  GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree));
  gtk_tree_selection_set_mode(GTK_TREE_SELECTION(sel), GTK_SELECTION_SINGLE);
  
  g_signal_connect(G_OBJECT(sel), "changed", G_CALLBACK(view_selected), NULL);

  /* 7. Show all widget */
  gtk_widget_show_all(win);
  gtk_main();
  
  return 0;
}

void view_selected(GtkTreeSelection *sel, gpointer data){
  GtkTreeIter iter;
  GtkTreeModel *model;
  if (gtk_tree_selection_get_selected(sel, &model, &iter)){
    const gchar *title, *author;
    const gint qty;
    
    gtk_tree_model_get(model, &iter, AUTHOR_COLUMN, &author, -1);
    gtk_tree_model_get(model, &iter, TITLE_COLUMN, &title, -1);
    gtk_tree_model_get(model, &iter, QTY_COLUMN, &qty, -1);

    g_print("Author: %s, Title: %s, Qty: %d\n", author, title, qty);
  }
}
