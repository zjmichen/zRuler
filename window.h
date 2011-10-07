void screenChanged (GtkWindow*, GdkScreen*, GtkWidget*);
gboolean expose(GtkWidget*, GdkEventExpose*, gpointer);
GdkPixbuf *createPixbuf(const gchar*);
void rotateRuler(GtkWidget*);
gboolean getXCursor();
gboolean viewPopupMenu (GtkWidget*, GdkEventButton*, gpointer);
void viewAboutDialog(GtkWidget*, gpointer);
gboolean stayOnTop(GtkWidget*, GdkEvent*, gpointer);
gboolean create_new_ruler(GtkWidget*, GdkEvent*, gpointer);
