/*
 * https://developer.gnome.org/gtk2/stable/TreeWidget.html
 * https://en.wikibooks.org/wiki/GTK%2B_By_Example/Tree_View/Tree_Models
 * http://scentric.net/tutorial/sec-treemodel-rowref.html
 * 
 */

#include <gtk/gtk.h>

/* Create enum for display all columns */
enum{
  COL_TITLE,
  COL_AUTHOR,
  COL_QTY,
  COL_COUNTS
};

void view_dbclicked(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, gpointer data);
void view_selected(GtkTreeSelection *sel, gpointer data);

int main(int argc, char *argv[]){
  
  gtk_init(&argc, &argv);
  
  /* 1. Create win */
  GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(win), "GtkTreeView example");
  g_signal_connect(G_OBJECT(win), "destroy", G_CALLBACK(gtk_main_quit), NULL);

  /* 2. Create store */
  GtkTreeStore *store = gtk_tree_store_new(
    COL_COUNTS,
    G_TYPE_STRING, 
    G_TYPE_STRING, 
    G_TYPE_UINT);
  
  /* 3. Create iter */
  GtkTreeIter iter1, iter2;
  
  /* 4. Append data to store referrence to &iter */
  gtk_tree_store_append(store, &iter1, NULL);
  gtk_tree_store_set(store, &iter1, 
    COL_TITLE, "คัมภีร์การใช้งานไมโครคอนโทรเลอร์ (0)", 
    COL_AUTHOR, "เดชฤทธิ์ มณีธรรม", 
    COL_QTY, 2, -1);
  
  gtk_tree_store_append(store, &iter2, &iter1);
  gtk_tree_store_set(store, &iter2, 
    COL_TITLE, "เล่ม 1: Arduino (0:0)", 
    COL_QTY, 1, -1);

  gtk_tree_store_append(store, &iter2, &iter1);
  gtk_tree_store_set(store, &iter2, 
    COL_TITLE, "เล่ม 2: Raspberry Pi (0:1)", 
    COL_QTY, 1, -1);
  
  gtk_tree_store_append(store, &iter1, NULL);
  gtk_tree_store_set(store, &iter1, 
    COL_TITLE, "Mastering Qt5 (1)", 
    COL_AUTHOR, "Guillame Lazar", 
    COL_QTY, 1, -1);

  gtk_tree_store_append(store, &iter2, &iter1);
  gtk_tree_store_set(store, &iter2, 
    COL_TITLE, "Vol 1: Qt5 CookBook (1:0)", 
    COL_QTY, 1, -1);

  gtk_tree_store_append(store, &iter2, &iter1);
  gtk_tree_store_set(store, &iter2, 
    COL_TITLE, "Vol 2: Qt5 BluePrint (1:1)", 
    COL_QTY, 1, -1);

  gtk_tree_store_append(store, &iter2, &iter1);
  gtk_tree_store_set(store, &iter2, 
    COL_TITLE, "Vol 3: Qt Creater (1:2)", 
    COL_QTY, 1, -1);


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
    "text", COL_AUTHOR, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes(
    "ชื่อเรื่อง", 
    renderer, 
    "text", COL_TITLE, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes(
    "จำนวน", 
    renderer, 
    "text", COL_QTY, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);
  
  GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree));
  gtk_tree_selection_set_mode(GTK_TREE_SELECTION(sel), GTK_SELECTION_SINGLE);

  g_signal_connect(G_OBJECT(tree), "row-activated", G_CALLBACK(view_dbclicked), NULL);
  g_signal_connect(G_OBJECT(sel), "changed", G_CALLBACK(view_selected), NULL);

  /* 7. Show all widget */
  gtk_widget_show_all(win);
  gtk_main();
  
  return 0;
}

void view_selected(GtkTreeSelection *sel, gpointer data){
  GtkTreeIter iter, parent;
  GtkTreePath *sel_path;
  GtkTreeModel *model;
  if (gtk_tree_selection_get_selected(sel, &model, &iter)){
    const gchar *title, *author, *p_author;
    const gint qty = 0;
    //const gint n = gtk_tree_model_iter_n_children (model, &iter);
    parent = iter;
    p_author = "";
    if (gtk_tree_model_iter_parent(model, &parent, &iter)){
      //g_print("This iter have parent.\n");
      gtk_tree_model_get(model, &parent, COL_AUTHOR, &p_author, -1);
      //g_print("Parent author: %s\n", p_author);
    }
    
    gtk_tree_model_get(model, &iter, COL_AUTHOR, &author, -1);
    gtk_tree_model_get(model, &iter, COL_TITLE, &title, -1);
    gtk_tree_model_get(model, &iter, COL_QTY, &qty, -1);
    
    sel_path = gtk_tree_model_get_path(model, &iter);
    const gchar *path_str = gtk_tree_path_to_string(sel_path);
    const gchar *iter_str = gtk_tree_model_get_string_from_iter(model, &iter);

    g_print("%s:\n", iter_str);

    //g_print("iters: %d\n", n);
    g_print("Path: %s\n", path_str);
    g_print("\tauthor: %s, title: %s, qty: %d\n", (author == NULL)?p_author:author, title, qty);
  }
}

void view_dbclicked(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, gpointer data){
  //GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(widget));
  GtkTreeModel *model = gtk_tree_view_get_model(treeview);
  GtkTreeIter iter;
  //GtkTreePath *path = gtk_tree_model_get_path(model, &iter);
  
  
  if (gtk_tree_model_get_iter(model, &iter, path)){
    if (gtk_tree_view_row_expanded(treeview, path)){
      gtk_tree_view_collapse_row(treeview, path);
    }else{
      gtk_tree_view_expand_row(treeview, path, TRUE);
    }
    //g_print("Double Clicked\n");
  }
}
