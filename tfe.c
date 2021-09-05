#include <gtk/gtk.h>
#include "tfetextview.h"

static void
app_activate(GApplication *app, gpointer user_data) {
    g_print("You need a filename argument.\n");
}

static void
app_open(GApplication *app, GFile **files, gint n_files, gchar *hint, gpointer user_data) {
    GtkWidget *win;
    GtkWidget *nb;
    GtkWidget *lab;
    GtkNotebookPage *nbp;
    GtkWidget *scr;
    GtkWidget *tv;
    GtkTextBuffer *tb;
    char *contents;
    gsize length;
    char *filename;
    int i;
    GtkBuilder *build;

    build = gtk_builder_new_from_resource("/com/github/ztolley/editor/window.ui");
    win = GTK_WIDGET(gtk_builder_get_object(build, "win"));
    gtk_window_set_application(GTK_WINDOW(win), GTK_APPLICATION(app));
    nb = GTK_WIDGET(gtk_builder_get_object(build, "nb"));
    g_object_unref(build);
    for (i = 0; i < n_files; i++) {
        if (g_file_load_contents(files[i], NULL, &contents, &length, NULL, NULL)) {
            scr = gtk_scrolled_window_new();
            tv = tfe_text_view_new();
            tb = gtk_text_view_get_buffer(GTK_TEXT_VIEW(tv));
            gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(tv), GTK_WRAP_WORD_CHAR);
            gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scr), tv);

            tfe_text_view_set_file(TFE_TEXT_VIEW(tv), g_file_dup(files[i]));
            gtk_text_buffer_set_text(tb, contents, length);
            g_free(contents);
            filename = g_file_get_basename(files[i]);
            lab = gtk_label_new(filename);
            gtk_notebook_append_page(GTK_NOTEBOOK(nb), scr, lab);
            nbp = gtk_notebook_get_page(GTK_NOTEBOOK(nb), scr);
            g_object_set(nbp, "tab-expand", TRUE, NULL);
            g_free(filename);
        } else if ((filename = g_file_get_path(files[i])) != NULL) {
            g_print("No such file: %s.\n", filename);
            g_free(filename);
        } else
            g_print("No valid file is given\n");
    }
    if (gtk_notebook_get_n_pages(GTK_NOTEBOOK(nb)) > 0) {
        gtk_widget_show(win);
    } else
        gtk_window_destroy(GTK_WINDOW(win));
}

int
main(int argc, char **argv) {
    GtkApplication *app;
    int stat;

    app = gtk_application_new("com.github.ToshioCP.tfe", G_APPLICATION_HANDLES_OPEN);
    g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
    g_signal_connect(app, "open", G_CALLBACK(app_open), NULL);
    stat = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return stat;
}