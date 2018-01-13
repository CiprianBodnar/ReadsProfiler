
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <vector>
#include "header.h"
#include <fstream>
#include <fcntl.h>
#include <sys/stat.h>
using namespace std;
#define BZEROCONST 1024

extern int errno;
int port;
void writeToServer( int length,char auxComand[50],int sd);
void recvAndprintResult(vector<vector<string> > result);
int main (int argc, char *argv[])
{
  int sd,loginSucc=0;
  struct sockaddr_in server;
  char comand[50],auxComand[50];
  if (argc != 3)
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  port = atoi (argv[2]);

  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }


  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_port = htons (port);

  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }
    vector<string> vectOfComand;
    vectOfComand.push_back("a");

    int quit=0;

    while(quit==0)
    {
        bzero(comand,BZEROCONST);
        if(loginSucc==0)
              printf ("[client]Logati-va: ");



        for(vector<string>::iterator it=vectOfComand.end();it!=vectOfComand.begin();it--)
            vectOfComand.pop_back();
        fgets(comand, sizeof(comand), stdin);
        strcpy(auxComand,comand);
        char *secv ;
        secv =strtok(comand," ");
        while (secv!=NULL)
        {
             vectOfComand.push_back(secv);
             secv=strtok(NULL," ");

        }
        if(loginSucc==0)
        {
            int length=strlen(auxComand)-1;

            writeToServer(length,auxComand,sd); //write to server

            bzero(comand,BZEROCONST);
            length=0;

            if(read(sd,&length,sizeof(int))<=0)
            {
                 perror   ("[client]Eroare la citire.\n");
                 break;
                 return errno;
            }


            if(read(sd,comand,length)<=0)
            {
                perror("[client]Eroare la citire.\n");
                break;
                return errno;
            }
        }

    if(strcmp(comand,"Login successful!")==0)
        loginSucc=1;

    if(strcmp(comand,"Ai iesit cu succes!")==0)
    {quit=1; break;}

    if(loginSucc==1)
    {
        cout<<comand<<endl;
        char username[50];
        int usernameLength;
        bzero(comand,BZEROCONST);
       if(read(sd,&usernameLength,sizeof(int))<=0)
        {
            perror   ("[client]Eroare la citire.\n");
            return errno;
        }


        if(read(sd,username,usernameLength)<=0)
        {
            perror("[client]Eroare la citire.\n");
            return errno;
        }


          do
        {

               for(vector<string>::iterator it=vectOfComand.end();it!=vectOfComand.begin();it--)
                   vectOfComand.pop_back();
               printf("\e[32m");
               printf("%s:",username);
               printf("\e[39m");

               fgets(comand, sizeof(comand), stdin);
               cout<<endl;

               comand[strlen(comand)-1 ]='\0';
               strcpy(auxComand,comand);
               int length=strlen(auxComand);
               char *secv ;
               secv =strtok(comand," ");
               while (secv!=NULL)
               {
                    vectOfComand.push_back(secv);
                    secv=strtok(NULL," ");

               }
              if(vectOfComand.at(0)=="logout")
                {
                  loginSucc=0;
                    writeToServer(length,auxComand,sd);
                }
              else
                {

                  writeToServer(length,auxComand,sd);

                  int bit;
                  if(read(sd,&bit,sizeof(int))<=0)
                  {
                      perror("[client]Eroare la citire bit");
                      break;
                  }

                  if(bit==1)
                  {
                  int linii=0;
                  if(read(sd,&linii,sizeof(int))<=0)
                      perror("[client]Eroare la citire");

                  cout<<"_______________________________________________________________"<<endl<<endl;
                  vector<vector<string> > result;
                  for(int i=0;i<linii;i++)
                  {
                      int col;
                      vector<string> sentence;
                      if(read(sd,&col,sizeof(int))<=0)
                          perror("[client]Eroare la citire");
                      for(int j=0;j<col;j++)
                      {
                          int size;
                          char chapter[50];
                          bzero(chapter,BZEROCONST);
                          string a;
                          if(read(sd,&size,sizeof(int))<=0)
                              perror("[client]Eroare la citire");
                          if(read(sd,&chapter,size)<=0)
                              perror(("[client]Eroare la citire"));

                         a=chapter;
                         sentence.push_back(a);

                      }
                      result.push_back(sentence);
                  }
                  recvAndprintResult(result);
                  cout<<"_______________________________________________________________"<<endl<<endl;
                    }
                  if(bit==2)
                     {
                      char filename[20]={'\n'};
                      int bytesReceived;
                      char recvBuff[128];
                      int filenameSize;


                      read(sd,&filenameSize,sizeof(int));
                      read(sd,&filename,filenameSize);

                      if(strcmp(filename,"!exist"))
                      {
                      int fd=open(filename, O_WRONLY|O_CREAT,0666);


                       while((bytesReceived=read(sd,&recvBuff,128))>0)
                       {
                          if(write(fd,&recvBuff,128)<=0)
                              cout<<"eroare";
                          if(bytesReceived<128)
                              break;

                       }
                        cout<<"OK!"<<endl;



                      }
                      else
                          cout<<"Aceasta carte nu exista"<<endl;

                     }
                  if(bit==3)
                  {
                      cout<<"_______________________________________________________________"<<endl<<endl;
                      int linii=0;
                      if(read(sd,&linii,sizeof(int))<=0)
                          perror("[client]Eroare la citire");

                      vector<vector<string> > result;
                      for(int i=0;i<linii;i++)
                      {
                          int col;
                          vector<string> sentence;
                          if(read(sd,&col,sizeof(int))<=0)
                              perror("[client]Eroare la citire");
                          for(int j=0;j<col;j++)
                          {
                              int size;
                              char chapter[50];
                              bzero(chapter,BZEROCONST);
                              string a;
                              if(read(sd,&size,sizeof(int))<=0)
                                  perror("[client]Eroare la citire");
                              if(read(sd,&chapter,size)<=0)
                                  perror(("[client]Eroare la citire"));

                             a=chapter;
                             sentence.push_back(a);

                          }
                          result.push_back(sentence);
                      }
                      recvAndprintResult(result);
                      cout<<"_______________________________________________________________"<<endl<<endl;
                  }
                  if(bit==4)
                  {
                      cout<<endl<<endl;
                      cout<<"_____________________________________________________________"<<endl<<endl;
                      cout<<"   "<<"search an|titlu|ISBN|autor|gen 'input"<<endl<<endl;
                      cout<<"   "<<"download titlu_carte"<<endl<<endl;
                      cout<<"   "<<"history search|download"<<endl<<endl;
                      cout<<"   "<<"add rating to titlu_carte nota"<<endl<<endl;
                      cout<<"   "<<"recommend"<<endl<<endl;
                      cout<<"   "<<"logout"<<endl<<endl;
                      cout<<"_____________________________________________________________"<<endl<<endl;

                  }
                  if(bit==5)
                  {
                      int lungime;
                      char  comanda[150];
                       bzero(comanda,BZEROCONST);
                      if(read(sd,&lungime,sizeof(int))<=0)
                      {
                           perror   ("[client]Eroare la citire.\n");
                           return errno;
                      }


                      if(read(sd,&comanda,lungime)<=0)
                      {
                          perror("[client]Eroare la citire.\n");
                          return errno;
                      }

                      cout<<comanda<<endl;
                  }
                  if(bit==6)
                  {
                      cout<<"_______________________________________________________________"<<endl<<endl;
                      int linii=0;
                      if(read(sd,&linii,sizeof(int))<=0)
                          perror("[client]Eroare la citire");

                      vector<vector<string> > result;
                      for(int i=0;i<linii;i++)
                      {
                          int col;
                          vector<string> sentence;
                          if(read(sd,&col,sizeof(int))<=0)
                              perror("[client]Eroare la citire");
                          for(int j=0;j<col;j++)
                          {
                              int size;
                              char chapter[50];
                              bzero(chapter,BZEROCONST);
                              string a;
                              if(read(sd,&size,sizeof(int))<=0)
                                  perror("[client]Eroare la citire");
                              if(read(sd,&chapter,size)<=0)
                                  perror(("[client]Eroare la citire"));

                             a=chapter;
                             sentence.push_back(a);

                          }
                          result.push_back(sentence);
                      }
                      recvAndprintResult(result);
                      cout<<"_______________________________________________________________"<<endl<<endl;
                  }
                  if(bit==7)
                  {
                      char review[50];
                      fgets(review,sizeof(review),stdin);
                      writeToServer(strlen(review),review,sd);

                  }
              }

        }while(loginSucc!=0);

    }
    else
        cout<<comand<<endl;
  }

  close (sd);
}


void recvAndprintResult(vector<vector<string> > result)
{


    for(vector<vector<string> >::iterator it=result.begin();it!=result.end();it++)
    {
        vector<string> row =*it;

        for(vector<string>::iterator jt=row.begin();jt!=row.end();jt++)
            cout<<(*jt)<<"  ";
        cout<<endl;
    }
    cout<<endl;

}
