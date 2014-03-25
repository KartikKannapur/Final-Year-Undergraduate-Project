#include <stdlib.h>
#include <gtk/gtk.h>

  //Global Declaration
  GtkWidget *entry1;
  GtkWidget *entry2;
  GtkWidget *entry3;
  GtkWidget *entry4;
  GtkWidget *entry5;

  GtkWidget *vbox;
  GtkWidget *window;


void button_clicked_ok(GtkWidget *widget, gpointer window)
{
    printf("%s",entry4);

}

void EnterPersonDetails(GtkWidget *widget, gpointer window)
{

  GtkWidget *table;

  GtkWidget *label1;
  GtkWidget *label2;
  GtkWidget *label3;
  GtkWidget *label4;
  GtkWidget *label5;

  GtkWidget *button_ok;

  GtkWidget *fixed;

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Enter Details");
  gtk_window_set_default_size(GTK_WINDOW(window), 450, 300);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(window), 20);
  gtk_widget_show(window);

  table = gtk_table_new(7,7, FALSE);
  gtk_container_add(GTK_CONTAINER(window), table);

  label1 = gtk_label_new("Name");
  label2 = gtk_label_new("Age");
  label3 = gtk_label_new("Sex");
  label4 = gtk_label_new("Height");
  label5 = gtk_label_new("Weight");

  gtk_table_attach(GTK_TABLE(table), label1, 0, 1, 0, 1,
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
  gtk_table_attach(GTK_TABLE(table), label2, 0, 1, 1, 2,
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
  gtk_table_attach(GTK_TABLE(table), label3, 0, 1, 2, 3,
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
  gtk_table_attach(GTK_TABLE(table), label4, 0, 1, 3, 4,
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
  gtk_table_attach(GTK_TABLE(table), label5, 0, 1, 4, 5,
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

  entry1 = gtk_entry_new();
  entry2 = gtk_entry_new();
  entry3 = gtk_entry_new();
  entry4 = gtk_entry_new();
  entry5 = gtk_entry_new();

  gtk_table_attach(GTK_TABLE(table), entry1, 1, 2, 0, 1,
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
  gtk_table_attach(GTK_TABLE(table), entry2, 1, 2, 1, 2,
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
  gtk_table_attach(GTK_TABLE(table), entry3, 1, 2, 2, 3,
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
  gtk_table_attach(GTK_TABLE(table), entry4, 1, 2, 3, 4,
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
  gtk_table_attach(GTK_TABLE(table), entry5, 1, 2, 4, 5,
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);

  gtk_widget_show(table);

  gtk_widget_show(label1);
  gtk_widget_show(label2);
  gtk_widget_show(label3);
  gtk_widget_show(label4);
  gtk_widget_show(label5);

  gtk_widget_show(entry1);
  gtk_widget_show(entry2);
  gtk_widget_show(entry3);
  gtk_widget_show(entry4);
  gtk_widget_show(entry5);

  fixed = gtk_fixed_new();

  button_ok = gtk_button_new_with_label("OK");
  gtk_widget_set_size_request(button_ok, 80, 30 );
  gtk_fixed_put(GTK_FIXED(fixed), button_ok, 50, 50);

  gtk_table_attach(GTK_TABLE(table), button_ok, 0, 1, 5, 6,
      GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);


  g_signal_connect(G_OBJECT(button_ok), "clicked",
        G_CALLBACK(button_clicked_ok), (gpointer) window);

  gtk_widget_show(window);
}

void GetPersonParameters(GtkWidget *widget, gpointer window)
{
  GtkWidget *Kinect;
  GtkWidget *Arduino;

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Parameters");
  gtk_window_set_default_size(GTK_WINDOW(window), 450, 300);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(window), 20);
  gtk_widget_show(window);

  gtk_container_add(GTK_CONTAINER(window), vbox);

  Kinect = gtk_button_new_with_label("Kinect Data");
  Arduino = gtk_button_new_with_label("Arduino & Sensors");

  gtk_box_pack_start(GTK_BOX(vbox), Kinect, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), Arduino, TRUE, TRUE, 0);


}

void AnalyzePersonGait(GtkWidget *widget, gpointer window)
{

}

void ViewPersonDatabase(GtkWidget *widget, gpointer window)
{

}

void AboutTheProject(GtkWidget *widget, gpointer window)
{
  GtkWidget *dialog = gtk_about_dialog_new();
  gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), "Gait Analysis");


  gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog),
      "\nGuide : Dr.A.Srinivas \n\n Amith Sreedhar\n Kartik Kannapur\n Lingaraju E V");

  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog),
     "Development of an ATOM based Embedded System for Gait Analysis using a Kinect Camera.");
  gtk_dialog_run(GTK_DIALOG (dialog));
  gtk_widget_destroy(dialog);
}

int main( int argc, char *argv[])
{
  GtkWidget *EnterDetails;
  GtkWidget *GetParameters;
  GtkWidget *AnalyzeGait;
  GtkWidget *ViewDatabase;
  GtkWidget *AboutProject;
  GtkWidget *Quit;

  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Gait Analysis");
  gtk_window_set_default_size(GTK_WINDOW(window), 450, 300);
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_container_set_border_width(GTK_CONTAINER(window), 5);
  gtk_widget_show(window);

  vbox = gtk_vbox_new(TRUE, 1);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  EnterDetails = gtk_button_new_with_label("Enter Details");
  GetParameters = gtk_button_new_with_label("Get Parameters");
  AnalyzeGait = gtk_button_new_with_label("Analyze Gait");
  ViewDatabase = gtk_button_new_with_label("View Database");
  AboutProject = gtk_button_new_with_label("About the Project");
  Quit = gtk_button_new_with_label("Quit");


  gtk_box_pack_start(GTK_BOX(vbox), EnterDetails, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), GetParameters, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), AnalyzeGait, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), ViewDatabase, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), AboutProject, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), Quit, TRUE, TRUE, 0);

  g_signal_connect(G_OBJECT(EnterDetails), "clicked",
        G_CALLBACK(EnterPersonDetails), (gpointer) window);

  g_signal_connect(G_OBJECT(GetParameters), "clicked",
        G_CALLBACK(GetPersonParameters), (gpointer) window);

  g_signal_connect(G_OBJECT(AnalyzeGait), "clicked",
        G_CALLBACK(AnalyzePersonGait), (gpointer) window);

  g_signal_connect(G_OBJECT(ViewDatabase), "clicked",
        G_CALLBACK(ViewPersonDatabase), (gpointer) window);

  g_signal_connect(G_OBJECT(AboutProject), "clicked",
        G_CALLBACK(AboutTheProject), (gpointer) window);

  g_signal_connect(G_OBJECT(Quit), "clicked",
        G_CALLBACK(gtk_main_quit), G_OBJECT(window));


  gtk_widget_show_all(window);
  gtk_main();

  return 0;
}
