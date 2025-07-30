#ifndef INOTIFY_WATCH_H
    #define INOTIFY_WATCH_H

    #include <gtk/gtk.h>
    #include <sys/inotify.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <glib.h>
    #include <stdio.h>

    void inotify_watch_start(GtkWidget *window, const char *filepath);

#endif