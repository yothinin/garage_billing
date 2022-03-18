#include <mysql/mysql.h>
#include <gtk/gtk.h>

GtkWidget *window;						/* main window */
GtkWidget *vBoxMain, *hbox;				/* box */
GtkWidget *halign;						/* align */
GtkWidget *button, 						/* button */
	      *btnDropDB, 
	      *btnDropTB,
	      *btnStruct, 
          *btnBrowse;

GtkWidget *frame;						/* frame */
GtkWidget *label;
GtkWidget *cmbDatabases, *cmbTables;	/* ComboBox */
GtkWidget *entry, *entUser, *entPass;	/* entry */

GtkListStore *lsDatabases, *lsTables;	/* List Store */
GtkCellRenderer *rend;					/* renderer */

GdkColor red, green, blue, white;		/* color */

GtkTreeIter iterDb, iterTb;

MYSQL *conn;

/* Display Widget */
void dspBtnTable();
void dspComboDB();
void dspConnectDB();
void disableWidget();
/* MySQL Connect */
void dbConnect(GtkWidget *widget, gpointer *user_data);
void finish_with_error(MYSQL *CONN);
/* List Databases and Tables */
void lstDatabases();
void lstTables(GtkWidget *widget, gpointer *userdata);
/* Manipulation database */
void selTable(GtkWidget *widget, gpointer *userdata);
void dropDatabase();
void dropTable();
void browseTable();
//gint setupTreeView(GtkWidget *treeview);

void finish_with_error(MYSQL *conn){
  fprintf(stderr, "%s\n", mysql_error(conn));
  //mysql_close(conn);
  //exit(1);
}

void disableWidget(){
  gtk_widget_set_sensitive(GTK_WIDGET(cmbDatabases), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(cmbTables), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(btnDropDB), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(btnDropTB), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(btnStruct), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(btnBrowse), FALSE);
}

void lstDatabases(){
  const gchar *sql = g_strdup_printf("SHOW DATABASES");
  if (mysql_query(conn, sql)){
    finish_with_error(conn);
  }
  MYSQL_RES *result = mysql_store_result(conn);
  if (result == NULL){
    finish_with_error(conn);
  }

  int i;
  int pos=0;
  gtk_list_store_clear(lsDatabases);
  int num_fields = mysql_num_fields(result);
  MYSQL_ROW row;
  while ((row = mysql_fetch_row(result))){
  for (i = 0; i < num_fields; i++){
    gtk_list_store_insert_with_values(lsDatabases, &iterDb, pos++, 0, row[i], -1);
    g_print("%s ", row[i]);
  }
  printf("\n");
  }
  mysql_free_result(result);
  gtk_widget_set_sensitive(GTK_WIDGET(cmbTables), TRUE);
}

void lstTables(GtkWidget *widget, gpointer *userdata){
  g_print("lstTables()...\n");
  if (gtk_combo_box_get_active(GTK_COMBO_BOX(widget)) >= 0){
    const gchar *u = gtk_entry_get_text(GTK_ENTRY(entUser));
    const gchar *p = gtk_entry_get_text(GTK_ENTRY(entPass));
    const gchar *db = gtk_combo_box_get_active_text(GTK_COMBO_BOX(widget));
    //g_print("Connect: %s, %s, %s\n", u, p, db);

    if (!db)
      g_print("db is null\n");
    else
      g_print("db not null\n");

    if (db != NULL){
      conn = mysql_init(NULL);
      if (mysql_real_connect(conn, "localhost", u, p, db, 0, NULL, 0)){
        const gchar *sql = g_strdup_printf("SHOW TABLES");
        g_print(sql);
        MYSQL_RES *result = mysql_list_tables(conn, NULL);
        if (result != NULL){
          finish_with_error(conn);
          int i;
          int num_fields = mysql_num_fields(result);
          MYSQL_ROW row;

          int pos=0;
          gtk_list_store_clear(GTK_LIST_STORE(lsTables));
          while ((row = mysql_fetch_row(result))){
            for (i = 0; i < num_fields; i++){
              gtk_list_store_insert_with_values(lsTables, NULL, pos++, 0, row[i], -1);
              g_print("%s ", row[i]);
            }
            printf("\n");
          }
          mysql_free_result(result);
          //g_free(sql);
          gtk_widget_set_sensitive(GTK_WIDGET(btnDropDB), TRUE);
          gtk_widget_set_sensitive(GTK_WIDGET(btnDropTB), FALSE);
          gtk_widget_set_sensitive(GTK_WIDGET(btnStruct), FALSE);
          gtk_widget_set_sensitive(GTK_WIDGET(btnBrowse), FALSE);
        }else
          g_print("result is null\n");
      }else
        gtk_widget_set_sensitive(GTK_WIDGET(btnDropDB), FALSE);
    }
  } // new
}

void selTable(GtkWidget *widget, gpointer *userdata){
  const gchar *tb = gtk_combo_box_get_active_text(GTK_COMBO_BOX(widget));
  if (tb){
    g_print("cmbTables isn't NULL\n");
    gtk_widget_set_sensitive(GTK_WIDGET(btnDropTB), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(btnDropTB), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(btnStruct), TRUE);
    gtk_widget_set_sensitive(GTK_WIDGET(btnBrowse), TRUE);
  }else{
    g_print("cmbTables is NULL\n");
    gtk_widget_set_sensitive(GTK_WIDGET(btnDropTB), FALSE);
  }
}

void dbConnect(GtkWidget *widget, gpointer *user_data){
  const gchar *u = gtk_entry_get_text(GTK_ENTRY(entUser));
  const gchar *p = gtk_entry_get_text(GTK_ENTRY(entPass));
  const gchar *btnLabel = gtk_button_get_label(GTK_BUTTON(widget));

  if (btnLabel[0] == 'C'){
    conn = mysql_init(NULL);
    if (mysql_real_connect(conn, "localhost", u, p, NULL, 0, NULL, 0)){
      gtk_button_set_label(GTK_BUTTON(widget), "Disconnect");
      gtk_entry_set_editable(GTK_ENTRY(entUser), FALSE);
      gtk_entry_set_editable(GTK_ENTRY(entPass), FALSE);
      gdk_color_parse("red", &red);
      gtk_widget_modify_bg(GTK_WIDGET(entUser), GTK_STATE_NORMAL, &red);
      gtk_widget_modify_bg(GTK_WIDGET(entPass), GTK_STATE_NORMAL, &red);
      g_print("Mysql connected!\n");
			
      /* List databases to cmbDatabases */
      gtk_widget_set_sensitive(GTK_WIDGET(cmbDatabases), TRUE);
      gtk_list_store_clear(GTK_LIST_STORE(lsDatabases));
      lstDatabases();
			
    }else{
      finish_with_error(conn);
    }
  }else{
    gtk_entry_set_editable(GTK_ENTRY(entUser), TRUE);
    gtk_entry_set_editable(GTK_ENTRY(entPass), TRUE);
    gtk_entry_set_text(GTK_ENTRY(entUser), "");
    gtk_entry_set_text(GTK_ENTRY(entPass), "");
    //gdk_color_parse("white", &color);
    gtk_widget_modify_bg(GTK_WIDGET(entUser), GTK_STATE_NORMAL, &white);
    gtk_widget_modify_bg(GTK_WIDGET(entPass), GTK_STATE_NORMAL, &white);
    gtk_button_set_label(GTK_BUTTON(widget), "Connect");
    g_print("Disconnect\n");
    //mysql_close(conn);
		
    gtk_list_store_clear(GTK_LIST_STORE(lsDatabases));
    gtk_list_store_clear(GTK_LIST_STORE(lsTables));

    disableWidget();
  }
}

void dropDatabase(){
  const gchar *db = gtk_combo_box_get_active_text(GTK_COMBO_BOX(cmbDatabases));
  const gchar *str = g_strdup_printf("กรุณายืนยันการลบฐานข้อมูล: %s", db);

  g_print("\ndropDatabase: %s\n", db);
  
  GtkWidget *dlg = gtk_dialog_new_with_buttons(
      "Drop Database?", GTK_WINDOW(window), 
      GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, 
      "ยกเลิก", 0, "ยืนยัน", 1, NULL);

  GtkWidget *lbl = gtk_label_new(str);
  gtk_container_add(
    GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dlg))), lbl);
  gtk_container_set_border_width(GTK_CONTAINER(dlg), 10);
  //gdk_color_parse("red", &color);
  gtk_widget_modify_fg(GTK_WIDGET(lbl), GTK_STATE_NORMAL, &red);
  gtk_widget_show(lbl);

  int result = gtk_dialog_run(GTK_DIALOG(dlg));
  gtk_widget_destroy(dlg);
  g_print("\nReturn code = %d\n", result);
  if (result){
    const gchar *sql = g_strdup_printf("DROP DATABASE %s", db);
    mysql_query(conn, sql);
    int n = gtk_combo_box_get_active_iter(GTK_COMBO_BOX(cmbDatabases), &iterDb);
    g_print("%d\n", n);
    gtk_list_store_remove(GTK_LIST_STORE(lsDatabases), &iterDb);
  }
}

void dropTable(){
  const gchar *db = gtk_combo_box_get_active_text(GTK_COMBO_BOX(cmbDatabases));
  const gchar *tb = gtk_combo_box_get_active_text(GTK_COMBO_BOX(cmbTables));
  const gchar *str = g_strdup_printf("กรุณายืนยันลบ ตาราง: %s", tb);
	
  g_print("\ndropTables: %s\n", tb);
  GtkWidget *dlg = gtk_dialog_new_with_buttons(
      "Drop Table?", GTK_WINDOW(window), 
      GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT, 
      "ยกเลิก", 0, "ยืนยัน", 1, NULL);
  
  GtkWidget *lbl = gtk_label_new(str);
  gtk_container_add(
      GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dlg))), lbl);
  gtk_container_set_border_width(GTK_CONTAINER(dlg), 10);
  //gdk_color_parse("red", &color);
  gtk_widget_modify_fg(GTK_WIDGET(lbl), GTK_STATE_NORMAL, &red);
  gtk_widget_show(lbl);

  int result = gtk_dialog_run(GTK_DIALOG(dlg));
  gtk_widget_destroy(dlg);
  g_print("\nReturn code = %d\n", result);
  if (result){
    const gchar *sql = g_strdup_printf("DROP TABLES %s.%s", db, tb);
    mysql_query(conn, sql);
	
    int n = gtk_combo_box_get_active_iter(GTK_COMBO_BOX(cmbTables), &iterTb);
    g_print("%d\n", n);
    gtk_list_store_remove(GTK_LIST_STORE(lsTables), &iterTb);
		
    gtk_widget_set_sensitive(GTK_WIDGET(btnStruct), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(btnBrowse), FALSE);
  }
}

void dspBtnTable(){
  hbox = gtk_hbox_new(FALSE, 0);
  frame = gtk_frame_new(" จัดการฐานข้อมูลและตาราง ");
  gtk_box_pack_start(GTK_BOX(vBoxMain), frame, FALSE, FALSE, 0);
  gtk_container_add(GTK_CONTAINER(frame), hbox);


  btnDropDB = gtk_button_new_with_label("Drop DB");
  gtk_widget_set_sensitive(GTK_WIDGET(btnDropDB), FALSE);
  gtk_box_pack_start(GTK_BOX(hbox), btnDropDB, FALSE, FALSE, 5);
  g_signal_connect(
      G_OBJECT(btnDropDB), "clicked", G_CALLBACK(dropDatabase), NULL);

  btnDropTB = gtk_button_new_with_label("Drop Table");
  gtk_widget_set_sensitive(GTK_WIDGET(btnDropTB), FALSE);
  gtk_box_pack_start(GTK_BOX(hbox), btnDropTB, FALSE, FALSE, 5);
  g_signal_connect(G_OBJECT(btnDropTB), "clicked", G_CALLBACK(dropTable), NULL);

  btnBrowse = gtk_button_new_with_label("แสดงข้อมูล");
  gtk_widget_set_sensitive(GTK_WIDGET(btnBrowse), FALSE);
  gtk_box_pack_end(GTK_BOX(hbox), btnBrowse, FALSE, FALSE, 5);
  g_signal_connect(btnBrowse, "clicked", G_CALLBACK(browseTable), NULL);

  btnStruct = gtk_button_new_with_label("โครงสร้าง");
  gtk_widget_set_sensitive(GTK_WIDGET(btnStruct), FALSE);
  gtk_box_pack_end(GTK_BOX(hbox), btnStruct, FALSE, FALSE, 5);

  label = gtk_label_new(NULL);
  const gchar 
      *lbl = "<b>*** โปรดระวัง ***</b>\nDrop DB - ลบฐานข้อมูลที่เลือก\nDrop Table - ลบตารางที่เลือก\n";
  gtk_label_set_markup(GTK_LABEL(label), lbl);
  gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_LEFT);
  //gdk_color_parse("red", &color);
  gtk_widget_modify_fg(GTK_WIDGET(label), GTK_STATE_NORMAL, &red);

  halign = gtk_alignment_new(0, 0, 0, 0);
  gtk_container_add(GTK_CONTAINER(vBoxMain), halign);
  gtk_container_add(GTK_CONTAINER(halign), label);
}

void dspComboDB(){
  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vBoxMain), hbox, FALSE, FALSE, 0);

  frame = gtk_frame_new(" ฐานข้อมูลและตาราง ");
  gtk_box_pack_start(GTK_BOX(hbox), frame, FALSE, FALSE, 0);

  hbox = gtk_hbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(frame), hbox);

  /* ListStore */
  //int pos = 0;
  lsDatabases = gtk_list_store_new(1, G_TYPE_STRING);

  /* Combobox for contain databases name */
  label = gtk_label_new_with_mnemonic("ฐานข้อมูล: ");
  gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 5);

  cmbDatabases = gtk_combo_box_new_with_model(GTK_TREE_MODEL(lsDatabases));
  rend = gtk_cell_renderer_text_new();
  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(cmbDatabases), rend, FALSE);
  gtk_cell_layout_add_attribute(GTK_CELL_LAYOUT(cmbDatabases), rend, "text", 0);
  gtk_widget_set_size_request(GTK_WIDGET(cmbDatabases), 150, 30);
  gtk_box_pack_start(GTK_BOX(hbox), cmbDatabases, FALSE, FALSE, 5);
  g_signal_connect(cmbDatabases, "changed", G_CALLBACK(lstTables), NULL);

  /* Combobox for contain tables name */
  label = gtk_label_new_with_mnemonic("ตาราง: ");
  gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 5);

  lsTables = gtk_list_store_new(1, G_TYPE_STRING);
  cmbTables = gtk_combo_box_new_with_model(GTK_TREE_MODEL(lsTables));
  rend = gtk_cell_renderer_text_new();
  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(cmbTables), rend, FALSE);
  gtk_cell_layout_add_attribute(GTK_CELL_LAYOUT(cmbTables), rend, "text", 0);
  gtk_widget_set_size_request(GTK_WIDGET(cmbTables), 150, 30);
  gtk_box_pack_start(GTK_BOX(hbox), cmbTables, FALSE, FALSE, 5);
  g_signal_connect(cmbTables, "changed", G_CALLBACK(selTable), NULL);
}

void dspConnectDB()
{
  /* Connect DB */
  frame = gtk_frame_new(" เชื่อมต่อฐานข้อมูล ");
  gtk_box_pack_start(GTK_BOX(vBoxMain), frame, FALSE, FALSE, 0);

  hbox = gtk_hbox_new(FALSE, 0);					/* hbox for user */
  gtk_container_add(GTK_CONTAINER(frame), hbox);

  /* User */
  label = gtk_label_new_with_mnemonic("ชื่อ: ");		
  gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 5);
  entUser = gtk_entry_new_with_max_length(45);
  gtk_entry_set_width_chars(GTK_ENTRY(entUser), 8);
  gtk_box_pack_start(GTK_BOX(hbox), entUser, FALSE, FALSE, 5);

  /* Password */
  label = gtk_label_new("รหัสผ่าน: ");
  gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 5);
  entPass = gtk_entry_new_with_max_length(45);
  gtk_entry_set_width_chars(GTK_ENTRY(entPass), 16);
  gtk_entry_set_visibility(GTK_ENTRY(entPass), FALSE);
  gtk_box_pack_start(GTK_BOX(hbox), entPass, FALSE, FALSE, 5);

  /* Connect */
  button = gtk_button_new_with_label("Connect");
  gtk_box_pack_start(GTK_BOX(hbox), button, FALSE, FALSE, 5);
  gtk_widget_set_size_request(GTK_WIDGET(button), 85, 30);
  g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(dbConnect), NULL);
}

void browseTable()
{
  GtkWidget *win, *treeview, *scrolled_win;
  GtkListStore *store;
  GtkTreeIter iter;
  GtkCellRenderer *rend;
  GtkTreeViewColumn *col;

  g_print("btnBrowse clicked\nbrowseTable() \n");

  const gchar *tb = gtk_combo_box_get_active_text(GTK_COMBO_BOX(cmbTables));
  const gchar *sql = g_strdup_printf("SELECT * FROM %s", tb);
  g_print("%s\n", sql);

  win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(win), g_strdup_printf("Browse: %s", tb));
  gtk_container_set_border_width(GTK_CONTAINER(win), 10);
  gtk_widget_set_size_request(win, 400, 200);

  treeview = gtk_tree_view_new();

  mysql_query(conn, sql);

  MYSQL_RES *result = mysql_store_result(conn);
  int num_fields = mysql_num_fields(result);

  store = gtk_list_store_new(num_fields, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);


  MYSQL_ROW row;
  MYSQL_FIELD *field;

  int j;
  for (j = 0; j < num_fields; j++){
    field = mysql_fetch_field(result);
    rend = gtk_cell_renderer_text_new();
    col = gtk_tree_view_column_new_with_attributes(field->name, rend, "text", j, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);
    g_print("%s\n", field->name);
  }
  g_print("\n");

  while ((row = mysql_fetch_row(result))){
    int j = 0;
    gtk_list_store_append(store, &iter);
    for (j = 0; j < num_fields; j++){
      g_print("%s\t", row[j]?row[j]:"NULL");
      gtk_list_store_set(store, &iter, j, row[j]?row[j]:"NULL", -1);
    }
    g_print("\n");
  }
	
  gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
  g_object_unref(store);

  scrolled_win = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_win), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

  gtk_container_add(GTK_CONTAINER(scrolled_win), treeview);
  gtk_container_add(GTK_CONTAINER(win), scrolled_win);

  gtk_widget_show_all(win);
}

int main(int argc, char **argv){
  gtk_init(&argc, &argv);

  //gtk_widget_modify_fg(GTK_WIDGET(lbl), GTK_STATE_NORMAL, &color);
  gdk_color_parse("red", &red);
  gdk_color_parse("green", &green);
  gdk_color_parse("blue", &blue);
  gdk_color_parse("white", &white);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "MySQL DB.1");
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);
  //gtk_widget_set_size_request(GTK_WIDGET(window), 400, 300);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

	
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

  /* Add vBoxMain to contain all hbox */
  vBoxMain = gtk_vbox_new(FALSE, 5);
  gtk_container_add(GTK_CONTAINER(window), vBoxMain);

  /* add widget here */
  /* ----------------------- */

  dspConnectDB();
  dspComboDB();
  dspBtnTable();

  /* ----------------------- */
  /* Add Exit button at the end of main window */
  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_end(GTK_BOX(vBoxMain), hbox, FALSE, FALSE, 0);

  button = gtk_button_new_from_stock(GTK_STOCK_QUIT);
  g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_set_size_request(GTK_WIDGET(button), 80, 30);

  // gtk_container_add(GTK_CONTAINER(hbox), button); 
  gtk_box_pack_end(GTK_BOX(hbox), button, FALSE, FALSE, 3);

  /* Display main window */
  gtk_widget_show_all(window);

  gtk_main();
  return 0;
}
