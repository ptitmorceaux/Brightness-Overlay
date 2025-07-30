#include <sys/inotify.h>
#include <unistd.h>
#include <fcntl.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <X11/extensions/shape.h>
#include "opacity.h"
#include "inotify_watch.h"

#define MAX_OPACITY 0.90

double getFormatedOpacity();
static gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer data);
void make_window_click_through(GtkWidget *window);
void update_opacity_and_queue_draw(GtkWidget *window);
int main(int argc, char **argv);

// Limite et normalise l’opacité obtenue
double getFormatedOpacity() {
    double opacity = getOpacity();
    if (opacity > MAX_OPACITY) return MAX_OPACITY;
    if (opacity < 0.0) return 0.0;
    return opacity;
}

static gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
    cairo_set_source_rgba(cr, 0, 0, 0, getFormatedOpacity());
    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
    cairo_paint(cr);
    return FALSE;
}

void make_window_click_through(GtkWidget *window) {
    Display *display = GDK_DISPLAY_XDISPLAY(gdk_display_get_default());
    Window xwin = GDK_WINDOW_XID(gtk_widget_get_window(window));

    Region region = XCreateRegion();

    XShapeCombineRegion(display, xwin, ShapeInput, 0, 0, region, ShapeSet);

    XFlush(display);
}

// Cette fonction sera appelée par inotify quand le fichier change
void update_opacity_and_queue_draw(GtkWidget *window) {
    double opacity = getFormatedOpacity();

    if (opacity <= 0.0)
        gtk_widget_hide(window); // Masquer complètement si opacité nulle
    
    else {
        make_window_click_through(window); // Remet le click-through
        
        if (!gtk_widget_get_visible(window))
            gtk_widget_show_all(window); // Réafficher uniquement si pas déjà visible
        
        gtk_widget_queue_draw(window);     // Redessine avec la nouvelle opacité
    }
}

int main(int argc, char **argv) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
    gtk_window_set_keep_above(GTK_WINDOW(window), TRUE);
    gtk_window_set_accept_focus(GTK_WINDOW(window), FALSE);

    GdkDisplay *display = gdk_display_get_default();
    GdkMonitor *monitor = gdk_display_get_primary_monitor(display);
    GdkRectangle geometry;
    gdk_monitor_get_geometry(monitor, &geometry);

    GdkScreen *screen = gtk_window_get_screen(GTK_WINDOW(window));
    GdkVisual *visual = gdk_screen_get_rgba_visual(screen);
    if (visual)
        gtk_widget_set_visual(window, visual);

    gtk_window_set_default_size(GTK_WINDOW(window), geometry.width, geometry.height);
    gtk_window_move(GTK_WINDOW(window), 0, 0);
    gtk_window_set_type_hint(GTK_WINDOW(window), GDK_WINDOW_TYPE_HINT_DOCK);

    gtk_widget_set_app_paintable(window, TRUE);
    g_signal_connect(window, "draw", G_CALLBACK(on_draw), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    double initial_opacity = getFormatedOpacity();

    if (initial_opacity <= 0.0)
        gtk_widget_hide(window);
    
    else {
        gtk_widget_show_all(window);
        make_window_click_through(window);
    }

    // Lance le watcher qui update et redessine en cas de changement
    inotify_watch_start(window, FILE_BRIGHTNESS);

    gtk_main();

    return 0;
}