#include <gtk/gtk.h>

static GtkWidget *list;

static void QuitCallback(GtkWidget *widget, GtkWidget *dialog_window){
  gtk_main_quit();
  exit(0);
}

static void AddCallback(GtkWidget *widget, GtkWidget *dialog_window){
  char *texts[5];
  int i;
  
  for (i = 0; i < 5; i++){
    texts[0] = "36123-A";
    texts[1] = "Wood Chisel Set";
    texts[2] = "9.99";
    texts[3] = "1";
    texts[4] = "9.99";
    gtk_clist_append(GTK_CLIST(list), texts);
  }
}

int main(int argc, char *argv[]){
  GdkColor color;
  GtkStyle *style;
  
  GtkWidget *button , *hbox, *dialog_window;
  char *texts[5];
  
  static char *titles[] = {
    "Part #", 
    "Description", 
    "Unit Price", 
    "Quantity", 
    "Total", 
  };
    
  gtk_init(&argc, &argv);
  dialog_window = gtk_dialog_new();
  gtk_window_position(GTK_WINDOW(dialog_window), GTK_WIN_POS_CENTER);
  
  hbox = gtk_hbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog_window)->vbox), hbox);
  
  list = gtk_clist_new_with_titles(5, titles);
 
  color.red = color.green = 0xff;
  color.blue = 0;
  
  
  style = gtk_style_copy(GTK_WIDGET(list)->style);  
  style->fg[GTK_STATE_ACTIVE] = color;
  
  texts[0] = "36123-A"; texts[1] = "Wood Chisel Set";
  texts[2] = "9.99"; texts[3] = "1"; texts[4] = "9.99";
  gtk_clist_append(GTK_CLIST(list), texts);
  
  texts[0] = "45661-A"; texts[1] = "Work Gloves";
  texts[2] = "5.99"; texts[3] = "1", texts[4] = "5.99";
  gtk_clist_append(GTK_CLIST(list), texts);
  
  gtk_box_pack_start(GTK_BOX(hbox), list, FALSE, FALSE, 0);
  
  button = gtk_button_new_with_label("Quit");
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog_window)->action_area), button, FALSE, FALSE, 0);
  g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(QuitCallback), dialog_window);
  
  button = gtk_button_new_with_label("Add");
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog_window)->action_area), button, FALSE, FALSE, 0);
  g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(AddCallback), dialog_window);
  
  gtk_widget_show_all(dialog_window);
  gtk_main();
  
  return 0;
}
  
  
