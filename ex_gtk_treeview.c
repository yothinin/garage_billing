#include <gtk/gtk.h>

/* Create enum for display all columns */
enum{
  TITLE_COLUMN,
  AUTHOR_COLUMN,
  CHECKED_COLUMN,
  N_COLUMNS
};

int main(int argc, char *argv[]){
  
  gtk_init(&argc, &argv);
  
  /* 1. Create win */
  GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(win), "GtkTreeView example");

  /* 2. Create store */
  GtkTreeStore *store = gtk_tree_store_new(
    N_COLUMNS,
    G_TYPE_STRING, 
    G_TYPE_STRING, 
    G_TYPE_BOOLEAN);
  
  /* 3. Create iter */
  GtkTreeIter iter;
  
  /* 4. Append data to store referrence to &iter */
  gtk_tree_store_append(store, &iter, NULL);
  gtk_tree_store_set(store, &iter, 
    TITLE_COLUMN, "คัมภีร์การใช้งานไมโครคอนโทรเลอร์", 
    AUTHOR_COLUMN, "เดชฤทธิ์ มณีธรรม", 
    CHECKED_COLUMN, FALSE, -1);
  
  gtk_tree_store_append(store, &iter, NULL);
  gtk_tree_store_set(store, &iter, 
    TITLE_COLUMN, "Mastering Qt5", 
    AUTHOR_COLUMN, "Guillame Lazar", 
    CHECKED_COLUMN, FALSE, -1);

  /* 5. Creaete tree to contain store and keep tree widget to win */
  GtkWidget *tree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
  gtk_container_add(GTK_CONTAINER(win), tree);
 
  /* 6. Renderer data to view */
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes(
    "Author", 
    renderer, 
    "text", AUTHOR_COLUMN, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes(
    "Title", 
    renderer, 
    "text", TITLE_COLUMN, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

  renderer = gtk_cell_renderer_toggle_new();
  column = gtk_tree_view_column_new_with_attributes(
    "Checked out", 
    renderer, 
    "active", CHECKED_COLUMN, NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(tree), column);

  /* 7. Show all widget */
  gtk_widget_show_all(win);
  gtk_main();
  
  return 0;
}
