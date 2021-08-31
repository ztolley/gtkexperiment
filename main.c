#include <gtk/gtk.h>


/* Define TfeTextView Widget which is the child object of GtkTextView */
#define TFE_TYPE_TEXT_VIEW tfe_text_view_get_type ()

G_DECLARE_FINAL_TYPE (TfeTextView, tfe_text_view, TFE, TEXT_VIEW, GtkTextView)

struct _TfeTextView {
    GtkTextView parent;
    GFile *file;
};

G_DEFINE_TYPE (TfeTextView, tfe_text_view, GTK_TYPE_TEXT_VIEW);

static void tfe_text_view_init(TfeTextView *tv) {
}

static void tfe_text_view_class_init(TfeTextViewClass *class) {
}

void tfe_text_view_set_file(TfeTextView *tv, GFile *f) {
    tv->file = f;
}

GFile *tfe_text_view_get_file(TfeTextView *tv) {
    return tv->file;
}

GtkWidget *tfe_text_view_new(void) {
    return GTK_WIDGET (g_object_new(TFE_TYPE_TEXT_VIEW, NULL));
}

/* ---------- end of the definition of TfeTextView ---------- */




static void app_activate(GApplication *app, gpointer user_data) {
    g_print("You need a filename argument.\n");
}

static void app_open(GApplication *app, GFile **files, gint n_files, gchar *hint, gpointer user_data) {
    GtkWidget *win;
    GtkWidget *screen;
    GtkWidget *notebook;
    GtkNotebookPage *notebookPage;
    GtkWidget *label;

    GtkWidget *textView;
    GtkTextBuffer *textBuffer;
    char *contents;
    gsize length;
    char *fileName;
    GtkBuilder *builder;

    builder = gtk_builder_new_from_resource("/com/github/ztolley/editor/window.ui");
    win = GTK_WIDGET (gtk_builder_get_object(builder, "win"));
    gtk_window_set_application(GTK_WINDOW (win), GTK_APPLICATION (app));
    notebook = GTK_WIDGET (gtk_builder_get_object(builder, "notebook"));
    g_object_unref(builder);

    for (int i = 0; i < n_files; i++) {
        if (g_file_load_contents(files[i], NULL, &contents, &length, NULL, NULL)) {
            screen = gtk_scrolled_window_new();
            textView = tfe_text_view_new();
            textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));
            gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textView), GTK_WRAP_WORD_CHAR);
            gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(screen), textView);

            tfe_text_view_set_file(TFE_TEXT_VIEW(textView), g_file_dup(files[i]));
            gtk_text_buffer_set_text(textBuffer, contents, length);
            g_free(contents);

            fileName = g_file_get_basename(files[i]);
            label = gtk_label_new(fileName);

            gtk_notebook_append_page(GTK_NOTEBOOK(notebook), screen, label);
            notebookPage = gtk_notebook_get_page(GTK_NOTEBOOK(notebook), screen);

            g_object_set(notebookPage, "tab-expand", TRUE, NULL);
            g_free(fileName);
        } else if ((fileName = g_file_get_path(files[i])) != NULL) {
            g_print("No such file: %s\n", fileName);
            g_free(fileName);
        } else {
            g_print("No valid file is given\n");
        }
    }

    if (gtk_notebook_get_pages(GTK_NOTEBOOK(notebook)) > 0) {
        gtk_widget_show(win);
    } else {
        gtk_window_destroy(GTK_WINDOW(win));
    }
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int stat;

    app = gtk_application_new("com.github.ToshioCP.pr1", G_APPLICATION_HANDLES_OPEN);
    g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
    g_signal_connect(app, "open", G_CALLBACK(app_open), NULL);
    stat = g_application_run(G_APPLICATION (app), argc, argv);
    g_object_unref(app);
    return stat;
}

