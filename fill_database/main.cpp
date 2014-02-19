// Note the code is written for max sl.no of 100
// Temporary steps to take: after updating databse change slno. both in txt and in the program (#define)


#define Sl_no_size 7
#define Number_param 4

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE *stream_ts, *stream_db;

char buffer_ts[200]={0};
char buffer_db[200]={0};

int get_param_ts(const char param[30])
{
    int param_value;
    int i;

    char temp[10];
    char *strg_ptr;

    strg_ptr= strstr(buffer_ts, param);
    strg_ptr= strg_ptr+ strlen(param)+1;

    for(i=0; *strg_ptr!=' ';strg_ptr++,i++)
        temp[i]= *strg_ptr;
    temp[i]='\0';

    param_value=atoi(temp);

    return param_value;
}



int check_h_class(int Height_class)
{
    return (Height_class/10);
}


int getoffset()
{
    int i;
    int buff_size=500;

    char get_off[buff_size];

    if( ( stream_db = fopen( "database.txt", "r" ) )==NULL)
    {
      printf("The file database.txt to read offset_char cannot be opened\n");
      exit(1);
    }

    else
    {
        fread( get_off, sizeof(char), sizeof(get_off), stream_db );

        for(i=0;i<buff_size;i++)
        {
            if(get_off[i]=='1')
            {
                i++;
                break;
            }
        }
        fclose(stream_db);
        return i;
    }
}


void Update_db(int Height_ts,int Stride_length_ts,int Left_stride_ts,int Right_stride_ts)
{
    char buffer_db[1000]={0};

  if( ( stream_db = fopen( "database.txt", "r" ) )==NULL)
    {
      printf("The file database.txt was not opened and a new database file will be created\n");

      if( ( stream_db = fopen( "database.txt", "w" ) )==NULL)
        {
          printf("The file database.txt could not be created\n");
          exit (1);
        }

      else
        {
        printf( "The file database.txt was created\n");
        fprintf(stream_db,"Sl.No\tHeight\tStr_len\tLft_str\tRight stride\n");
        fprintf(stream_db, "1\t%d\t%d\t%d\t%d\n", Height_ts, Stride_length_ts, Left_stride_ts, Right_stride_ts);
        printf( "The file database.txt was updated\n");
        }
    }

  else
    {
      fclose(stream_db);

      if( ( stream_db = fopen( "database.txt", "a+" ) )==NULL)
        {
          printf("The file database.txt could not be opened\n");
          exit (1);
        }

      else
        {
          printf( "The file database.txt was opened\n");
          fprintf(stream_db, "1\t%d\t%d\t%d\t%d\n", Height_ts, Stride_length_ts, Left_stride_ts, Right_stride_ts);
          printf( "The file database.txt was updated\n");
        }
    }

    fclose(stream_db);
}


void Analyse_gait(int Height_ts,int Stride_length_ts,int Left_stride_ts,int Right_stride_ts,int Height_class_ts)
{
    int Height_db=0;
    int Stride_length_db=0;
    int Left_stride_db=0;
    int Right_stride_db=0;
    int db_number;                                    // Number of gaits falling in the Height_class_ts category;
    int offset;
    int offset_char;
    int i, j;
    int Sl_no[Sl_no_size];
    int Sl_no_buff[Sl_no_size]={0};                                  // This will limit the max Sl.No
    int Height_buff[Sl_no_size]={0};

    offset_char=getoffset();
    offset=(offset_char+2)*sizeof(char);

    if( ( stream_db = fopen( "database.txt", "r" ) )==NULL)
    {
      printf("The file database.txt cannot be opened\n");
      exit(1);
    }

    else
    {
      printf( "The file database.txt was opened\n");
      if(0!=fseek(stream_db,offset,SEEK_SET))
      {
        printf("\n fseek() to start of data base failed\n");
        exit(1);
      }

      for(i=0;i<Sl_no_size;i++)
      {
          int garbage_var[Sl_no_size+2];

          fscanf (stream_db, "%d\t%d\t%d\t%d\t%d\n", &Sl_no_buff[i], &Height_buff[i], &garbage_var[i], &garbage_var[i+1], &garbage_var[i+2] );

     /*     if(0!=fseek(stream_db,10,SEEK_CUR))
          {
            printf("\n fseek() to sl.no %dth data in the database failed\n", i+1);
            exit(2);
          }
      */

       //   printf("sl.no is %d\t\t and height is %d\n", Sl_no_buff[i], Height_buff[i]);
      }

    }
    for(db_number=0,i=0;i<Sl_no_size;i++)
    {
        if((Height_buff[i]/10)== Height_class_ts)
        {
            Sl_no[db_number]=Sl_no_buff[i];
            db_number++;
        }
    }


    fclose(stream_db);

    int db_array[db_number][Number_param];



    if( ( stream_db = fopen( "database.txt", "r" ) )==NULL)
    {
      printf("The file database.txt cannot be opened\n");
      exit(1);
    }

    else
    {
      printf( "The file database.txt was opened\n");
      if(0!=fseek(stream_db,offset,SEEK_SET))
      {
        printf("\n fseek() to start of data base failed\n");
        exit(1);
      }

      for(i=0,j=0;i<Sl_no_size;i++)
      {
          int a, b, c, d, e;

          fscanf (stream_db, "%d\t%d\t%d\t%d\t%d\n", &a, &b, &c, &d, &e );

          if(a==Sl_no[j])
          {
              db_array[j][0]=b;
              db_array[j][1]=c;
              db_array[j][2]=d;
              db_array[j][3]=e;
           //   printf( "Sl.no is %d, height is %d, Str_len is %d, Lft_str is %d, Right_str is %d\n", Sl_no[j], db_array[j][0], db_array[j][1], db_array[j][2], db_array[j][3] );
              j++;
          }
      }

    for(i=0;i<db_number;i++)
    {
        Height_db += db_array[i][0];
        Stride_length_db += db_array[i][1];
        Left_stride_db += db_array[i][2];
        Right_stride_db += db_array[i][3];
    }


    Height_db = Height_db/db_number;
    Stride_length_db = Stride_length_db/db_number;
    Left_stride_db = Left_stride_db/db_number;
    Right_stride_db = Right_stride_db/db_number;

    if( Stride_length_ts<(Stride_length_db-10) || Stride_length_ts>(Stride_length_db+10) )
        printf( "\nAbnormal Stride length has been detected. Normal: %d, Test Subject: %d\n", Stride_length_db, Stride_length_ts );

    if( Left_stride_ts<(Left_stride_db-1000) || Left_stride_ts>(Left_stride_db+1000) )
        printf( "Abnormal Left stride has been detected. Normal: %d, Test Subject: %d\n",  Left_stride_db, Left_stride_ts );

    if( Right_stride_ts<(Right_stride_db-1000) || Right_stride_ts>(Right_stride_db+1000) )
        printf( "Abnormal Right stride has been detected. Normal: %d, Test Subject: %d\n",  Right_stride_db, Right_stride_ts );

   /* printf("The height class is %d and db_number is %d", Height_class_ts, db_number);

      for(i=0;i<db_number;i++)
        printf("\nThe coinciding sl_no is %d\n", Sl_no[i]);
   */

    fclose(stream_db);
    }

    return;
}





int main()
{
    int Height_ts,Height_db;
    int Stride_length_ts,Stride_length_db;
    int Left_stride_ts,Left_stride_db;
    int Right_stride_ts,Right_stride_db;
    int Height_class_ts,Height_class_db;

    char option;

  if( ( stream_ts = fopen( "test_subject.txt", "r" ) )==NULL)
    {
    printf("The file test_sub.txt was not opened\n");
    exit(1);
    }
  else
    printf( "The file test_sub.txt was opened\n");

  fread( buffer_ts, sizeof(char), sizeof(buffer_ts), stream_ts );


  Height_ts= get_param_ts("Height");
  Stride_length_ts= get_param_ts("Stride length");
  Left_stride_ts= get_param_ts("Left stride");
  Right_stride_ts= get_param_ts("Right stride");

/*
printf(" the Height is %d cm\n", Height_ts);
printf(" the Stride length is %d cm\n", Stride_length_ts);
printf(" the Left stride is %d ms\n", Left_stride_ts);
printf(" the Right stride is %d ms\n", Right_stride_ts);
*/

  Height_class_ts= check_h_class(Height_ts);

  while(1)
  {
    printf("\n\nWhat do you want to do? \n");
    printf("Option 1: Update the database\n");
    printf("Option 2: Analyse Gait\n");
    printf("Option 3: Exit\n\n");

    scanf("%c", &option);

    switch (option)
    {
    case '1':
        Update_db(Height_ts, Stride_length_ts, Left_stride_ts, Right_stride_ts);
        fclose(stream_ts);
        exit(0);

    case '2':
        Analyse_gait(Height_ts, Stride_length_ts, Left_stride_ts, Right_stride_ts, Height_class_ts);
        fclose(stream_ts);
        exit(0);

    case '3':
        fclose(stream_ts);
        exit(0);

    default:
        printf("Invalid option chosen. Please choose a valid option\n");
        break;
    }
  }
fclose(stream_ts);
return 0;
}

