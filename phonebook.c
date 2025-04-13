#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_CONTACTS 1000

typedef struct {
    char name[100];
    char phone[20];
    char email[100];
    char date[20];
    char dept_code[10];  // Added to store department code
} Contact;

Contact all_contacts[MAX_CONTACTS];
int total_contacts = 0;

GtkWidget *entry_name, *entry_phone, *entry_email, *combo_dept, *search_entry;
GtkWidget *treeview;
GtkListStore *store;

const char *departments[] = {
    "Computer Science and Engineering", 
    "Department of Fashion Design", 
    "School of Law", 
    "School of Agriculture", 
    "Department of Pharmacy", 
    "Department of English", 
    "Computer Application",
    "Management", 
    NULL
};

const char *dept_abbr[] = {"CSE", "FSN", "LAW", "AGR", "PHR", "ENG", "CA", "MBA", NULL};

int load_contacts(Contact contacts[], int max);

void save_contact_to_file(Contact *c, const char *dept_code) {
    FILE *fp = fopen("faculty_contact.txt", "a");
    if (fp) {
        fprintf(fp, "%s:%s|%s|%s|%s\n", dept_code, c->name, c->phone, c->email, c->date);
        fclose(fp);
    }
}

void refresh_treeview() {
    gtk_list_store_clear(store);
    GtkTreeIter iter;
    for (int i = 0; i < total_contacts; i++) {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter, 
            0, all_contacts[i].name, 
            1, all_contacts[i].phone, 
            2, all_contacts[i].email, 
            3, all_contacts[i].date, 
            -1);
    }
}

void show_all_contacts(GtkWidget *widget, gpointer data) {
    total_contacts = load_contacts(all_contacts, MAX_CONTACTS);
    refresh_treeview();
}

void add_contact(GtkWidget *widget, gpointer data) {
    const char *name = gtk_entry_get_text(GTK_ENTRY(entry_name));
    const char *phone = gtk_entry_get_text(GTK_ENTRY(entry_phone));
    const char *email = gtk_entry_get_text(GTK_ENTRY(entry_email));
    const char *dept_full = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo_dept));

    if (!name || !phone || !email || !dept_full || strlen(name) == 0) return;

    const char *dept_code = dept_abbr[gtk_combo_box_get_active(GTK_COMBO_BOX(combo_dept))];

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    Contact c;
    strncpy(c.name, name, sizeof(c.name));
    strncpy(c.phone, phone, 20);
    strncpy(c.email, email, 100);
    strncpy(c.dept_code, dept_code, sizeof(c.dept_code));
    strftime(c.date, sizeof(c.date), "%Y-%m-%d", t);

    all_contacts[total_contacts++] = c;
    save_contact_to_file(&c, dept_code);

    GtkTreeIter iter;
    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter, 
        0, name, 
        1, phone, 
        2, email, 
        3, c.date, 
        -1);
}

int load_contacts(Contact contacts[], int max) {
    FILE *fp = fopen("faculty_contact.txt", "r");
    if (!fp) return 0;
    
    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), fp) && count < max) {
        char *dept_code = strtok(line, ":");
        char *name = strtok(NULL, "|");
        char *phone = strtok(NULL, "|");
        char *email = strtok(NULL, "|");
        char *date = strtok(NULL, "\n");

        if (name && phone && email && date) {
            strncpy(contacts[count].name, name, sizeof(contacts[count].name));
            strncpy(contacts[count].phone, phone, sizeof(contacts[count].phone));
            strncpy(contacts[count].email, email, sizeof(contacts[count].email));
            strncpy(contacts[count].date, date, sizeof(contacts[count].date));
            strncpy(contacts[count].dept_code, dept_code, sizeof(contacts[count].dept_code));
            count++;
        }
    }
    fclose(fp);
    return count;
}

void delete_selected(GtkWidget *widget, gpointer data) {
    GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    GtkTreeModel *model;
    GtkTreeIter iter;

    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        char *name, *phone, *email, *date;
        gtk_tree_model_get(model, &iter, 0, &name, 1, &phone, 2, &email, 3, &date, -1);

        // Find and remove from memory array
        int index = -1;
        for (int i = 0; i < total_contacts; i++) {
            if (strcmp(all_contacts[i].name, name) == 0 &&
                strcmp(all_contacts[i].phone, phone) == 0 &&
                strcmp(all_contacts[i].email, email) == 0) {
                index = i;
                break;
            }
        }

        if (index != -1) {
            // Rewrite the entire file while skipping the deleted contact
            FILE *fp = fopen("faculty_contact.txt", "w");
            if (fp) {
                for (int i = 0; i < total_contacts; i++) {
                    if (i != index) {
                        fprintf(fp, "%s:%s|%s|%s|%s\n", 
                            all_contacts[i].dept_code,
                            all_contacts[i].name, 
                            all_contacts[i].phone, 
                            all_contacts[i].email, 
                            all_contacts[i].date);
                    }
                }
                fclose(fp);
            }

            // Update memory array
            for (int i = index; i < total_contacts - 1; i++) {
                all_contacts[i] = all_contacts[i + 1];
            }
            total_contacts--;

            // Update view
            refresh_treeview();
        }
    }
}

void search_contacts(GtkEntry *entry, gpointer user_data) {
    const char *search_text = gtk_entry_get_text(entry);
    GtkTreeIter iter;
    gboolean valid;

    gtk_list_store_clear(store);

    for (int i = 0; i < total_contacts; i++) {
        if (strlen(search_text) == 0 ||
            strstr(all_contacts[i].name, search_text) ||
            strstr(all_contacts[i].phone, search_text) ||
            strstr(all_contacts[i].email, search_text)) {
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, 
                0, all_contacts[i].name, 
                1, all_contacts[i].phone, 
                2, all_contacts[i].email, 
                3, all_contacts[i].date, 
                -1);
        }
    }
}

void sort_by_name(GtkWidget *widget, gpointer data) {
    // Simple bubble sort for demonstration
    for (int i = 0; i < total_contacts - 1; i++) {
        for (int j = 0; j < total_contacts - i - 1; j++) {
            if (strcasecmp(all_contacts[j].name, all_contacts[j+1].name) > 0) {
                // Swap contacts
                Contact temp = all_contacts[j];
                all_contacts[j] = all_contacts[j+1];
                all_contacts[j+1] = temp;
            }
        }
    }
    
    // Rewrite the file with sorted contacts
    FILE *fp = fopen("faculty_contact.txt", "w");
    if (fp) {
        for (int i = 0; i < total_contacts; i++) {
            fprintf(fp, "%s:%s|%s|%s|%s\n", 
                all_contacts[i].dept_code,
                all_contacts[i].name, 
                all_contacts[i].phone, 
                all_contacts[i].email, 
                all_contacts[i].date);
        }
        fclose(fp);
    }
    
    refresh_treeview();
}

GtkWidget* create_treeview() {
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    store = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    treeview = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Name", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Phone", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Email", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    column = gtk_tree_view_column_new_with_attributes("Date", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    return treeview;
}

void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Contact Manager");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 400);

    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Input fields
    entry_name = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_name), "Name");
    gtk_grid_attach(GTK_GRID(grid), entry_name, 0, 0, 1, 1);

    entry_phone = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_phone), "Phone");
    gtk_grid_attach(GTK_GRID(grid), entry_phone, 1, 0, 1, 1);

    entry_email = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_email), "Email");
    gtk_grid_attach(GTK_GRID(grid), entry_email, 2, 0, 1, 1);

    combo_dept = gtk_combo_box_text_new();
    for (int i = 0; departments[i]; i++)
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_dept), departments[i]);
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_dept), 0);
    gtk_grid_attach(GTK_GRID(grid), combo_dept, 3, 0, 1, 1);

    // Buttons
    GtkWidget *btn_add = gtk_button_new_with_label("Add Contact");
    g_signal_connect(btn_add, "clicked", G_CALLBACK(add_contact), NULL);
    gtk_grid_attach(GTK_GRID(grid), btn_add, 4, 0, 1, 1);

    GtkWidget *btn_delete = gtk_button_new_with_label("Delete Selected");
    g_signal_connect(btn_delete, "clicked", G_CALLBACK(delete_selected), NULL);
    gtk_grid_attach(GTK_GRID(grid), btn_delete, 5, 0, 1, 1);

    GtkWidget *btn_show = gtk_button_new_with_label("Show All");
    g_signal_connect(btn_show, "clicked", G_CALLBACK(show_all_contacts), NULL);
    gtk_grid_attach(GTK_GRID(grid), btn_show, 6, 0, 1, 1);

    // Search and filter
    search_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(search_entry), "Search...");
    gtk_grid_attach(GTK_GRID(grid), search_entry, 0, 1, 5, 1);
    g_signal_connect(search_entry, "changed", G_CALLBACK(search_contacts), NULL);

    GtkWidget *btn_sort_name = gtk_button_new_with_label("Sort by Name");
    g_signal_connect(btn_sort_name, "clicked", G_CALLBACK(sort_by_name), NULL);
    gtk_grid_attach(GTK_GRID(grid), btn_sort_name, 5, 1, 2, 1);

    // TreeView with scroll
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_vexpand(scrolled, TRUE);
    gtk_grid_attach(GTK_GRID(grid), scrolled, 0, 2, 7, 1);

    treeview = create_treeview();
    gtk_container_add(GTK_CONTAINER(scrolled), treeview);

    // Load initial data
    total_contacts = load_contacts(all_contacts, MAX_CONTACTS);
    refresh_treeview();

    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.example.ContactManager", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}