void screenChanged (GtkWindow*, GdkScreen*, GtkWidget*);
gboolean expose(GtkWidget*, GdkEventExpose*, gpointer);
GdkPixbuf *createPixbuf(const gchar*);
void rotateRuler(GtkWidget*);
gboolean getXCursor();
gboolean viewPopupMenu (GtkWidget*, GdkEventButton*, gpointer);
gboolean stayOnTop(GtkWidget*, GdkEvent*, gpointer);
