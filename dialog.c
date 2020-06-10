#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

static void print_text (GtkWidget *widget, gint response_id, GtkWidget *entry)
{
  if (response_id == GTK_RESPONSE_ACCEPT){
    const gchar *entry_text;
    entry_text = gtk_entry_get_text (GTK_ENTRY (entry));
    printf("Entry contents: %s\n", entry_text);
    g_print("Accept entry.\n");
  }
  
  if (response_id == GTK_RESPONSE_DELETE_EVENT)
    g_print("User clicked close, no print!\n");
    
  gtk_widget_destroy(widget); // This will close the dialog
}

static void open_dialog (GtkWidget *widget, gpointer window)
{
    //GtkWidget *window = data;
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *label;
    GtkWidget *vbox;
    GtkWidget *hbox;
    
    //GtkWidget *button;
    static GtkWidget *textbox;
 
    dialog = gtk_dialog_new_with_buttons ("Get Text",
                                          GTK_WINDOW(window),
                                          GTK_DIALOG_MODAL,
                                          GTK_STOCK_SAVE,
                                          GTK_RESPONSE_ACCEPT,
                                          GTK_STOCK_CANCEL,
                                          GTK_RESPONSE_CANCEL,
                                          NULL);

    gtk_widget_set_size_request(dialog, 250, 100);

    content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
    
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add (GTK_CONTAINER (content_area), vbox);

    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
    
    label = gtk_label_new("Value: ");
    gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 5);
    
    textbox = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(hbox), textbox, FALSE, FALSE, 5);
    
    //g_signal_connect(G_OBJECT(textbox), "activate", G_CALLBACK(print_text), textbox);
    g_signal_connect(G_OBJECT(dialog), "response", G_CALLBACK(print_text), textbox);

    gtk_widget_show_all (dialog);
    
    gtk_widget_destroy (widget);
}

int main (int argc, char **argv)
{
    GtkWidget *win;
    GtkWidget *button;
    GtkWidget *vbox;
    GtkWidget *hbox;
    
    gtk_init(&argc, &argv);
    
    win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(win), "Test Diaglog");
    gtk_container_set_border_width(GTK_CONTAINER(win), 10);
    gtk_widget_set_size_request(win, 300, 200);
    
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(win), vbox);
    hbox = gtk_hbox_new(FALSE, 0);
    gtk_box_pack_end(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
    
    button = gtk_button_new_with_label("Print Text");
    gtk_box_pack_end(GTK_BOX(hbox), button, FALSE, FALSE, 0);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(open_dialog), win);
    
    gtk_widget_show_all(win);
    
    gtk_main();
    
    return 0;
}
