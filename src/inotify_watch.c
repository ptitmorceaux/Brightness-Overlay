#include "inotify_watch.h"

// Callback interne, déclenché à chaque changement du fichier
static gboolean inotify_callback(GIOChannel *source, GIOCondition condition, gpointer data) {
    GtkWidget *window = GTK_WIDGET(data);
    int fd = g_io_channel_unix_get_fd(source);

    char buffer[1024];
    int length = read(fd, buffer, sizeof(buffer));
    if (length < 0) {
        perror("inotify read");
        return TRUE; // continuer à écouter
    }

    // Ici, on appelle ta fonction de refresh
    extern void update_opacity_and_queue_draw(GtkWidget *window);
    update_opacity_and_queue_draw(window);

    return TRUE; // continuer à écouter
}

void inotify_watch_start(GtkWidget *window, const char *filepath) {
    int inotify_fd = inotify_init1(IN_NONBLOCK);
    if (inotify_fd < 0) {
        perror("inotify_init1");
        return;
    }

    int wd = inotify_add_watch(inotify_fd, filepath, IN_MODIFY);
    if (wd < 0) {
        perror("inotify_add_watch");
        close(inotify_fd);
        return;
    }

    GIOChannel *channel = g_io_channel_unix_new(inotify_fd);
    g_io_add_watch(channel, G_IO_IN, inotify_callback, window);
    g_io_channel_unref(channel);
}
