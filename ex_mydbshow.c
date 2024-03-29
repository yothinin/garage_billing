/*
 * Example from book, MySQL Developer's Library and MySQL Building User Interface.
 * Compile: gcc -g -o ex_mydbshow ex_mydbshow.c $(mysql_config --libs) $(mysql_config --include)
*/
#include <my_global.h>
#include <my_sys.h>
#include <mysql.h>
#include <m_string.h>
#include <my_getopt.h>

static char *opt_host_name = NULL; /* server host (default=localhost) */
static char *opt_user_name = NULL; /* username (default=login name) */
static char *opt_password = NULL; /* password (default=none) */
static unsigned int opt_port_num = 0; /* port number (use built-in value) */
static char *opt_socket_name = NULL; /* socket name (use built-in value) */

static char *opt_db_name = NULL;
static unsigned int opt_flags = 0;
static int ask_password = 0;

static MYSQL *conn;

static const char *client_groups[] = { "client", NULL };

static struct my_option my_opts[] = {/* option information structures */
  {"help", '?', "Display this help and exit", NULL, NULL, NULL, GET_NO_ARG, NO_ARG, 0, 0, 0, 0, 0,0},
  {"host", 'h', "Host to connect to", (uchar **) &opt_host_name, NULL, NULL, GET_STR, REQUIRED_ARG, 0, 0, 0, 0, 0, 0},
  {"password", 'p', "Password", (uchar **) &opt_password, NULL, NULL, GET_STR, OPT_ARG, 0, 0, 0, 0, 0, 0},
  {"port", 'P', "Port number", (uchar **) &opt_port_num, NULL, NULL, GET_UINT, REQUIRED_ARG, 0, 0, 0, 0, 0, 0},
  {"socket", 'S', "Socket path", (uchar **) &opt_socket_name, NULL, NULL, GET_STR, REQUIRED_ARG, 0, 0, 0, 0, 0, 0},
  {"user", 'u', "User name", (uchar **) &opt_user_name, NULL, NULL, GET_STR, REQUIRED_ARG, 0, 0, 0, 0, 0, 0},
  { NULL, 0, NULL, NULL, NULL, NULL, GET_NO_ARG, NO_ARG, 0, 0, 0, 0,0, 0 }
};

static void print_error(MYSQL *conn, char *message){
  fprintf(stderr, "%s\n", message);
  if (conn != NULL){
    fprintf(stderr, "Error %u (%s): %s\n", 
      mysql_errno(conn), mysql_sqlstate(conn), mysql_error(conn));
  }
}

static my_bool get_one_option (int optid, const struct my_option *opt, char *argument){
  switch (optid){
    case '?':
      my_print_help (my_opts); /* print help message */
      exit (0);
    case 'p':
      if (!argument)
        ask_password = 1;
      else{
        opt_password = strdup(argument);
        if (opt_password == NULL){
          print_error(NULL, "could not allocate password buffer");
          exit(1);
        }
        while(*argument)
          *argument++ = 'X';
        ask_password = 0;
      }
      break;
  }
  return (0);
}

int main (int argc, char *argv[]){
  int i;
  int opt_err;

  MY_INIT(argv[0]);
  load_defaults("my", client_groups, &argc, &argv);

  if ((opt_err = handle_options(&argc, &argv, my_opts, get_one_option)))
    exit (opt_err);

  if (ask_password)
    opt_password = get_tty_password(NULL);

  if (argc > 0){
    opt_db_name = argv[0];
    --argc; ++argv;
  }

  if (mysql_library_init(0, NULL, NULL)){
    print_error(NULL, "mysql_library_init() failed");
    exit(1);
  }

  conn = mysql_init(NULL);
  if (conn == NULL){
    print_error(NULL, "mysql_init() failed (probable out of memory)");
    exit(1);
  }

  if (mysql_real_connect(conn, opt_host_name, opt_user_name, opt_password, 
      opt_db_name, opt_port_num, opt_socket_name, opt_flags) == NULL){
    print_error(conn, "mysql_real_connect() failed");
    mysql_close(conn);
    exit(1);
  }

  MYSQL_RES *res_set;
  MYSQL_ROW row;

  if (mysql_query(conn, "show databases") != 0)
    print_error(conn, "mysql_query() failed");
  else{
    int i;
    res_set = mysql_store_result(conn);
    if (res_set == NULL)
      print_error(conn, "mysql_store_result() failed");
    else{
      //process_result_set(conn, res_set);
      while ((row = mysql_fetch_row(res_set)) != NULL){
        for (i = 0; i < mysql_num_fields(res_set); ++i){
          if (i > 0)
            fputc('\t', stdout);
          printf("%s", row[i]!=NULL?row[i]:"NULL");
        }
        fputc('\n', stdout);
      }
    }

    mysql_free_result(res_set);
  }
  
  /* disconnect from server and terminat client library. */
  mysql_close(conn);
  mysql_library_end();

  exit (0);
}
