void screen_changed (GtkWindow*, GdkScreen*, GtkWidget*);
gboolean expose(GtkWidget*, GdkEventExpose*, gpointer);
GdkPixbuf *create_pixbuf(const gchar*);
void rotate_ruler(GtkWidget*);
gboolean getXCursor();
gboolean view_popup_menu (GtkWidget*, GdkEventButton*, gpointer);
gboolean stay_on_top(GtkWidget*, GdkEvent*, gpointer);
