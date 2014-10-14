#include <iostream>
#include <gtk/gtk.h>

GdkPixbuf *create_pixbuf(const gchar * filename)
{
   GdkPixbuf *pixbuf;
   GError *error = NULL;
   pixbuf = gdk_pixbuf_new_from_file(filename, &error);
   if(!pixbuf) {
      fprintf(stderr, "%s\n", error->message);
      g_error_free(error);
   }
   return pixbuf;
}

gint count = 0;
char buf[256];

void increase(GtkWidget *widget, gpointer label)
{
  count++;

  sprintf(buf, "%d", count);
  gtk_label_set_text(GTK_LABEL(label), buf);
}

void decrease(GtkWidget *widget, gpointer label)
{
  count--;

  sprintf(buf, "%d", count);
  gtk_label_set_text(GTK_LABEL(label), buf);
}

int main(int argc, char* argv[])
{
	GtkWidget *label;
	GtkWidget *window;
	GtkWidget *frame;
	GtkWidget *plus;
	GtkWidget *minus;
	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Flight Deck");
	gtk_window_set_default_size(GTK_WINDOW(window), 230, 150);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_icon(GTK_WINDOW(window), create_pixbuf("f450.png"));
	frame = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window), frame);
	plus = gtk_button_new_with_label("+");
	gtk_widget_set_size_request(plus, 80, 35);
	gtk_fixed_put(GTK_FIXED(frame), plus, 50, 20);
	minus = gtk_button_new_with_label("-");
	gtk_widget_set_size_request(minus, 80, 35);
	gtk_fixed_put(GTK_FIXED(frame), minus, 50, 80);
	label = gtk_label_new("0");
	gtk_fixed_put(GTK_FIXED(frame), label, 190, 58);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(plus, "clicked", G_CALLBACK(increase), label);
	g_signal_connect(minus, "clicked", G_CALLBACK(decrease), label);
	g_signal_connect_swapped(G_OBJECT(window), "destroy",G_CALLBACK(gtk_main_quit), NULL);

	gtk_widget_show_all(window);
	gtk_main();

	return 0;
}
