#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <unistd.h>

 //Global Declaration

 //Format
 //GtkWidget *entry;
 GtkWidget *entry1;
 GtkWidget *entry2;
 GtkWidget *entry3;
 GtkWidget *entry4;
 GtkWidget *entry5;
 GtkWidget *entry6;

 //GtkWidget *label;
 GtkWidget *label1;
 GtkWidget *label2;
 GtkWidget *label3;
 GtkWidget *label4;
 GtkWidget *label5;
 GtkWidget *label6;

 GtkWidget *vbox;
 GtkWidget *window;
 GtkWidget *cancel_button_widget;
 GtkWidget *ok_button_widget;

 GtkWidget *hbox;

 //Open the Database file - View Database
 FILE *stream;

void cancel_event(GtkWidget *widget, gpointer data)
{
   main();

}

void save_event(GtkWidget *widget, gpointer data)
{
   //Format
   //const char *text = gtk_entry_get_text(GTK_ENTRY(entry));
   const char *text1 = gtk_entry_get_text(GTK_ENTRY(entry1));
   const char *text2 = gtk_entry_get_text(GTK_ENTRY(entry2));
   const char *text3 = gtk_entry_get_text(GTK_ENTRY(entry3));
   const char *text4 = gtk_entry_get_text(GTK_ENTRY(entry4));
   const char *text5 = gtk_entry_get_text(GTK_ENTRY(entry5));
   const char *text6 = gtk_entry_get_text(GTK_ENTRY(entry6));

   float text_weight = atof(text5);
   float text_height = atof(text6);
   float bmi = (text_weight/(text_height*text_height));

   printf("First Name: %s\n", text1);
   printf("Last Name: %s\n", text2);
   printf("Age: %s\n", text3);
   printf("Sex: %s\n", text4);
   printf("Weight (in kg):%s\n", text5);
   printf("Height (in m): %s\n", text6);
   printf("Body Mass Index (BMI): %f\n\n",bmi);
}

void EnterPersonDetails(GtkWidget *widget, gpointer window)
{
 GtkWidget *Save;
 GtkWidget *Cancel;

 window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
 gtk_window_set_title(GTK_WINDOW(window), "Enter Details");
 gtk_window_set_default_size(GTK_WINDOW(window), 450, 300);
 gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
 gtk_container_set_border_width(GTK_CONTAINER(window), 20);
 gtk_widget_show(window);

 //Format
 //label = gtk_label_new(" ");
 label1 = gtk_label_new("First Name :");
 label2 = gtk_label_new("Last Name :");
 label3 = gtk_label_new("Age :");
 label4 = gtk_label_new("Sex :");
 label5 = gtk_label_new("Weight (in kg):");
 label6 = gtk_label_new("Height (in m):");

 //Format
 //entry = gtk_entry_new();
 entry1 = gtk_entry_new();
 entry2 = gtk_entry_new();
 entry3 = gtk_entry_new();
 entry4 = gtk_entry_new();
 entry5 = gtk_entry_new();
 entry6 = gtk_entry_new();
 hbox = gtk_vbox_new(FALSE,2);

 Save = gtk_button_new_with_label("Save");
 Cancel = gtk_button_new_with_label("Cancel");
 g_signal_connect(G_OBJECT(Save), "clicked", G_CALLBACK(save_event),NULL);
 g_signal_connect(G_OBJECT(Cancel), "clicked", G_CALLBACK(cancel_event),NULL);

 //Format
 //gtk_box_pack_start(GTK_BOX(hbox), entry, TRUE, TRUE, 0);
 gtk_box_pack_start(GTK_BOX(hbox), label1, FALSE, FALSE, 0);
 gtk_box_pack_start(GTK_BOX(hbox), entry1, FALSE, FALSE, 0);
 gtk_box_pack_start(GTK_BOX(hbox), label2, FALSE, FALSE, 0);
 gtk_box_pack_start(GTK_BOX(hbox), entry2, FALSE, FALSE, 0);
 gtk_box_pack_start(GTK_BOX(hbox), label3, FALSE, FALSE, 0);
 gtk_box_pack_start(GTK_BOX(hbox), entry3, FALSE, FALSE, 0);
 gtk_box_pack_start(GTK_BOX(hbox), label4, FALSE, FALSE, 0);
 gtk_box_pack_start(GTK_BOX(hbox), entry4, FALSE, FALSE, 0);
 gtk_box_pack_start(GTK_BOX(hbox), label5, FALSE, FALSE, 0);
 gtk_box_pack_start(GTK_BOX(hbox), entry5, FALSE, FALSE, 0);
 gtk_box_pack_start(GTK_BOX(hbox), label6, FALSE, FALSE, 0);
 gtk_box_pack_start(GTK_BOX(hbox), entry6, FALSE, FALSE, 0);

 gtk_box_pack_start(GTK_BOX(hbox), Save, FALSE, FALSE, 5);
 gtk_box_pack_start(GTK_BOX(hbox), Cancel, FALSE, FALSE, 0);
 gtk_container_add(GTK_CONTAINER(window), hbox);
 gtk_widget_show_all(window);


}


void GetPersonParameters(GtkWidget *widget, gpointer window)
{
 //Run Arduino & Kinect Code
 //Desktop/Project-Final-Year/Skeleton/bin/Debug/

   system("gnome-terminal --working-directory=/home/punith/Desktop/Project-Final-Year/Skeleton/bin/Debug/ -x sh -c './Skeleton; exec bash'");

}

void LearnPersonSVM(GtkWidget *widget, gpointer window)
{
   //Learn SVM
   system("gnome-terminal --working-directory=/home/punith/Desktop/svmlight/ -x sh -c './svm_learn example2/train.dat example2/model; exec bash'");
}

void ClassifyPersonSVM(GtkWidget *widget, gpointer window)
{
   //Classify SVM
   system("gnome-terminal --working-directory=/home/punith/Desktop/svmlight/ -x sh -c './svm_classify example2/test.dat example2/model example2/predictions; exec bash'");
}

void AnalyzePersonGait(GtkWidget *widget, gpointer window)
{
   //SVM
   GtkWidget *Learn;
   GtkWidget *Classify;

   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title(GTK_WINDOW(window), "Analyze Gait - Support Vector Machine");
   gtk_window_set_default_size(GTK_WINDOW(window), 450, 400);
   gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
   gtk_container_set_border_width(GTK_CONTAINER(window), 5);
   gtk_widget_show(window);

   vbox = gtk_vbox_new(TRUE, 1);
   gtk_container_add(GTK_CONTAINER(window), vbox);

   Learn = gtk_button_new_with_label("Learn");
   Classify = gtk_button_new_with_label("Classify");

   gtk_box_pack_start(GTK_BOX(vbox), Learn, TRUE, TRUE, 0);
   gtk_box_pack_start(GTK_BOX(vbox), Classify, TRUE, TRUE, 0);

   g_signal_connect(G_OBJECT(Learn), "clicked",
       G_CALLBACK(LearnPersonSVM), (gpointer) window);

   g_signal_connect(G_OBJECT(Classify), "clicked",
       G_CALLBACK(ClassifyPersonSVM), (gpointer) window);

   gtk_widget_show_all(window);
}

//OpenNI - User Tracker Code
void SkeletonPersonTracking(GtkWidget *widget, gpointer window)
{
   system("gnome-terminal --working-directory=/home/punith/Desktop/Project-Final-Year/openni/Platform/Linux/Bin/x64-Release/ -x sh -c './Sample-NiUserTracker; exec bash'");
}


//Hip, Knee & Elbow Angles
void HipPersonAngleRight(GtkWidget *widget, gpointer window)
{
   //Hip Angle
   system("gnome-terminal --working-directory=/home/punith/Desktop/GraphicalAnalysis -x sh -c 'gnuplot -persist hip_right_gnuplot.sh; exec bash'");
}
void HipPersonAngleLeft(GtkWidget *widget, gpointer window)
{
   //Hip Angle
   system("gnome-terminal --working-directory=/home/punith/Desktop/GraphicalAnalysis -x sh -c 'gnuplot -persist hip_left_gnuplot.sh; exec bash'");
}
//----------//
void KneePersonAngleRight(GtkWidget *widget, gpointer window)
{
   //Knee Angle
   system("gnome-terminal --working-directory=/home/punith/Desktop/GraphicalAnalysis -x sh -c 'gnuplot -persist knee_right_gnuplot.sh; exec bash'");
}
void KneePersonAngleLeft(GtkWidget *widget, gpointer window)
{
   //Knee Angle
   system("gnome-terminal --working-directory=/home/punith/Desktop/GraphicalAnalysis -x sh -c 'gnuplot -persist knee_left_gnuplot.sh; exec bash'");
}
//----------//
void ElbowPersonAngleRight(GtkWidget *widget, gpointer window)
{
   //Elbow Angle
   system("gnome-terminal --working-directory=/home/punith/Desktop/GraphicalAnalysis -x sh -c 'gnuplot -persist elbow_right_gnuplot.sh; exec bash'");
}
void ElbowPersonAngleLeft(GtkWidget *widget, gpointer window)
{
   //Elbow Angle
   system("gnome-terminal --working-directory=/home/punith/Desktop/GraphicalAnalysis -x sh -c 'gnuplot -persist elbow_left_gnuplot.sh; exec bash'");
}

void GraphicalPersonAnalysis(GtkWidget *widget, gpointer window)
{
   //Graphical Analysis
   GtkWidget *HipAngleRight;
   GtkWidget *HipAngleLeft;

   GtkWidget *KneeAngleRight;
   GtkWidget *KneeAngleLeft;

   GtkWidget *ElbowAngleRight;
   GtkWidget *ElbowAngleLeft;

   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title(GTK_WINDOW(window), "Graphical Analysis");
   gtk_window_set_default_size(GTK_WINDOW(window), 450, 400);
   gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
   gtk_container_set_border_width(GTK_CONTAINER(window), 5);
   gtk_widget_show(window);

   vbox = gtk_vbox_new(TRUE, 1);
   gtk_container_add(GTK_CONTAINER(window), vbox);

   HipAngleRight = gtk_button_new_with_label("Hip Angle - Right vs % Gait Cycle");
   HipAngleLeft = gtk_button_new_with_label("Hip Angle - Left vs % Gait Cycle");

   KneeAngleRight = gtk_button_new_with_label("Knee Angle - Right vs % Gait Cycle");
   KneeAngleLeft = gtk_button_new_with_label("Knee Angle - Left vs % Gait Cycle");

   ElbowAngleRight = gtk_button_new_with_label("Elbow Angle - Right vs % Gait Cycle");
   ElbowAngleLeft = gtk_button_new_with_label("Elbow Angle - Left vs % Gait Cycle");

   gtk_box_pack_start(GTK_BOX(vbox), HipAngleRight,TRUE, TRUE, 0);
   gtk_box_pack_start(GTK_BOX(vbox), HipAngleLeft, TRUE, TRUE, 0);

   gtk_box_pack_start(GTK_BOX(vbox), KneeAngleRight, TRUE, TRUE, 0);
   gtk_box_pack_start(GTK_BOX(vbox), KneeAngleLeft, TRUE, TRUE, 0);

   gtk_box_pack_start(GTK_BOX(vbox), ElbowAngleRight, TRUE, TRUE, 0);
   gtk_box_pack_start(GTK_BOX(vbox), ElbowAngleLeft, TRUE, TRUE, 0);

   g_signal_connect(G_OBJECT(HipAngleRight), "clicked",
       G_CALLBACK(HipPersonAngleRight), (gpointer) window);
   g_signal_connect(G_OBJECT(HipAngleLeft), "clicked",
       G_CALLBACK(HipPersonAngleLeft), (gpointer) window);

   g_signal_connect(G_OBJECT(KneeAngleRight), "clicked",
       G_CALLBACK(KneePersonAngleRight), (gpointer) window);
   g_signal_connect(G_OBJECT(KneeAngleLeft), "clicked",
       G_CALLBACK(KneePersonAngleLeft), (gpointer) window);


   g_signal_connect(G_OBJECT(ElbowAngleRight), "clicked",
       G_CALLBACK(ElbowPersonAngleRight), (gpointer) window);
   g_signal_connect(G_OBJECT(ElbowAngleLeft), "clicked",
       G_CALLBACK(ElbowPersonAngleLeft), (gpointer) window);

   gtk_widget_show_all(window);
}


void ViewPersonDatabase(GtkWidget *widget, gpointer window)
{

   if( (stream = fopen( "/home/punith/Desktop/Project-Final-Year/Extracting_points/Joint_xyzpos.txt", "r" )) == NULL )
     printf( "The Database has not been opened\n\n" );

   else
   {
         printf( "The Database has been opened\n\n" );
         system("gedit /home/punith/Desktop/Project-Final-Year/Extracting_points/Joint_xyzpos.txt");
         fclose(stream);
   }
}

void AboutTheProject(GtkWidget *widget, gpointer window)
{
 GtkWidget *dialog = gtk_about_dialog_new();
 gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), "Gait Analysis");


 gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog),
     "\nGuide : Dr.A.Srinivas \n\n Amith Sreedhar\n Kartik Kannapur\n Lingaraju E V");

 gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog),
    "Development of an Embedded System for Gait Analysis using a Kinect Camera.");
 gtk_dialog_run(GTK_DIALOG (dialog));
 gtk_widget_destroy(dialog);
}

//Main
int main( int argc, char *argv[])
{
 GtkWidget *EnterDetails;
 GtkWidget *GetParameters;
 GtkWidget *AnalyzeGait;
 GtkWidget *SkeletonTracking;
 GtkWidget *GraphicalAnalysis;
 GtkWidget *ViewDatabase;
 GtkWidget *AboutProject;
 GtkWidget *Quit;

 gtk_init(&argc, &argv);

 window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
 gtk_window_set_title(GTK_WINDOW(window), "Gait Analysis");
 gtk_window_set_default_size(GTK_WINDOW(window), 450, 400);
 gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
 gtk_container_set_border_width(GTK_CONTAINER(window), 5);
 gtk_widget_show(window);

 vbox = gtk_vbox_new(TRUE, 1);
 gtk_container_add(GTK_CONTAINER(window), vbox);

 EnterDetails = gtk_button_new_with_label("Enter Details");
 GetParameters = gtk_button_new_with_label("Get Parameters");
 SkeletonTracking = gtk_button_new_with_label("Skeleton Tracking");
 GraphicalAnalysis = gtk_button_new_with_label("Graphical Analysis");
 AnalyzeGait = gtk_button_new_with_label("Analyze Gait");
 ViewDatabase = gtk_button_new_with_label("View Database");
 AboutProject = gtk_button_new_with_label("About the Project");
 Quit = gtk_button_new_with_label("Quit");


 gtk_box_pack_start(GTK_BOX(vbox), EnterDetails, TRUE, TRUE, 0);
 gtk_box_pack_start(GTK_BOX(vbox), GetParameters, TRUE, TRUE, 0);
 gtk_box_pack_start(GTK_BOX(vbox), SkeletonTracking, TRUE, TRUE, 0);
 gtk_box_pack_start(GTK_BOX(vbox), GraphicalAnalysis, TRUE, TRUE, 0);
 gtk_box_pack_start(GTK_BOX(vbox), AnalyzeGait, TRUE, TRUE, 0);
 gtk_box_pack_start(GTK_BOX(vbox), ViewDatabase, TRUE, TRUE, 0);
 gtk_box_pack_start(GTK_BOX(vbox), AboutProject, TRUE, TRUE, 0);
 gtk_box_pack_start(GTK_BOX(vbox), Quit, TRUE, TRUE, 0);

 g_signal_connect(G_OBJECT(EnterDetails), "clicked",
       G_CALLBACK(EnterPersonDetails), (gpointer) window);

 g_signal_connect(G_OBJECT(GetParameters), "clicked",
       G_CALLBACK(GetPersonParameters), (gpointer) window);


 g_signal_connect(G_OBJECT(SkeletonTracking), "clicked",
       G_CALLBACK(SkeletonPersonTracking), (gpointer) window);

 g_signal_connect(G_OBJECT(GraphicalAnalysis), "clicked",
       G_CALLBACK(GraphicalPersonAnalysis), (gpointer) window);

 g_signal_connect(G_OBJECT(ViewDatabase), "clicked",
       G_CALLBACK(ViewPersonDatabase), (gpointer) window);

 g_signal_connect(G_OBJECT(AnalyzeGait), "clicked",
       G_CALLBACK(AnalyzePersonGait), (gpointer) window);

 g_signal_connect(G_OBJECT(AboutProject), "clicked",
       G_CALLBACK(AboutTheProject), (gpointer) window);

 g_signal_connect(G_OBJECT(Quit), "clicked",
       G_CALLBACK(gtk_main_quit), G_OBJECT(window));


 gtk_widget_show_all(window);
 gtk_main();

 return 0;
}
