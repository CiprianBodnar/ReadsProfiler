
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "db.h"
#include "query.h"
#include <iostream>
#include <vector>
#include "header.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include<netinet/in.h>
using namespace std;

#define BZEROCONST 1024
#define PORT 2907
extern int errno;




typedef struct thData{
    int idThread;
    int cl;
}thData;

static void *treat(void *);
void clientStringRespond(string text, thData tdL);
string extractfromComand(vector<string> comand,int start);
void sendVectortoClient(vector<vector<string> > result,thData tdL);
vector<vector<string> > intermediar( string a);
void startTables(db *data);

int main ()
{

  struct sockaddr_in server;
  struct sockaddr_in from;
  int sd,i=0;
  pthread_t th[100];


  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server]Eroare la socket().\n");
      return errno;
    }

  int on=1;
  setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));


  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    server.sin_port = htons (PORT);

  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server]Eroare la bind().\n");
      return errno;
    }

  if (listen (sd, 2) == -1)
    {
      perror ("[server]Eroare la listen().\n");
      return errno;
    }

  while (1)
    {
      int client;
      thData * td;
     socklen_t length = sizeof (from);

      printf ("[server]Asteptam la portul %d\n",PORT);
      fflush (stdout);


      if ( (client = accept (sd, (struct sockaddr *) &from, &length)) < 0)
        {
      perror ("[server]Eroare la accept().\n");
      continue;
         }


    td=(struct thData*)malloc(sizeof(struct thData));
    td->idThread=i++;
    td->cl=client;

    pthread_create(&th[i], NULL, &treat, td);

    }
};
static void *treat(void * arg)
{


        char  comand[50];
        int length=0;
        db *data;
        data=new db("Database.sqlite");
       // startTables(data);
          bool chestie=0;

        struct thData tdL;
        tdL= *((struct thData*)arg);
        printf ("[thread]- %d - Asteptam mesajul...\n", tdL.idThread);
        string username;
        fflush (stdout);
        pthread_detach(pthread_self());


        while(1)
        {

           if(chestie==1)
               break;

        int descriptor=read(tdL.cl,&length,sizeof(int));

        if(descriptor<=0)
        {
            perror("Eroare prima citire, clientul a inchis condexiunea\n");
            break;

        }

        bzero(comand,BZEROCONST);
        descriptor=read(tdL.cl,&comand,length);

        if(descriptor<=0)
        {
            perror("Eroare la a doua citire, clientul a inchis conexiunea");
            break;
        }


        char * secv;
        vector<string> fullcomand;
        secv=strtok(comand," ");
        while(secv!=NULL)
        {

            fullcomand.push_back(secv);
            secv=strtok(NULL," ");
        }



        if(fullcomand.at(0)=="login" and fullcomand.size()==2)
        {


            if(data->checkIfExistusername(fullcomand.at(1))==1)
               {
                bool loginpass=1;
                clientStringRespond("Login successful!",tdL);
                clientStringRespond(fullcomand.at(1),tdL);
                username=fullcomand.at(1);

                while(loginpass==1)
                {
                    fflush (stdout);
                    bzero(comand,BZEROCONST);
                    int d=read(tdL.cl,&length,sizeof(int));
                    if(d<=0)
                    {
                        chestie=1;
                        printf("[Server]Eroare la read in login!\n");
                        close ((intptr_t)arg);
                        data->close();
                        loginpass=0;
                        break;
                    }
                    d=read(tdL.cl,&comand,length);
                    if(d<=0)
                    {
                        printf("[Server]Eroare la read la login!\n");
                        close ((intptr_t)arg);
                        data->close();

                        break;
                    }


                    for(vector<string>::iterator it=fullcomand.end();it!=fullcomand.begin();it--)
                        fullcomand.pop_back();
                    secv=strtok(comand," ");
                    while(secv!=NULL)
                    {

                        fullcomand.push_back(secv);
                        secv=strtok(NULL," ");
                    }
                    int bit;
                    if(fullcomand.at(0)=="logout")
                        loginpass=0;
                    else
                        if(fullcomand.at(0)=="search" and fullcomand.size()>=3)
                        {      bit=1;
                               if(write(tdL.cl,&bit,sizeof(int))<=0)
                                   perror   ("[server]Eroare la trimitere bit!");
                               if(fullcomand.at(1)=="autor")
                               {
                                   string  extractAuthor=extractfromComand(fullcomand,2,0);
                                   vector<vector<string>> result=data->searchBookbyAuthor(extractAuthor);
                                   result=data->addGenre(result);
                                   sendVectortoClient(result,tdL);
                                   data->addTohistorySearch(data->getUserid(username),fullcomand.at(1),extractAuthor);
                               }
                               else
                                   if(fullcomand.at(1)=="titlu")
                                   {
                                       string extractTitle=extractfromComand(fullcomand,2,0);
                                       vector<vector<string>> result=data->searchbyBooktitle(extractTitle);
                                       result=data->addGenre(result);
                                       sendVectortoClient(result,tdL);
                                       data->addTohistorySearch(data->getUserid(username),fullcomand.at(1),extractTitle);

                                   }
                                    else
                                       if(fullcomand.at(1)=="an")
                                       {
                                           string extractYear=extractfromComand(fullcomand,2,0);
                                           vector<vector<string>> result=data->searchbyYearofBook(extractYear);
                                           result=data->addGenre(result);
                                           sendVectortoClient(result,tdL);
                                           data->addTohistorySearch(data->getUserid(username),fullcomand.at(1),extractYear);


                                       }
                                        else
                                           if(fullcomand.at(1)=="ISBN")
                                           {
                                               string extractISBN=extractfromComand(fullcomand,2,0);
                                               vector<vector<string>> result=data->searchbyISBNofBook(extractISBN);
                                               result=data->addGenre(result);
                                               sendVectortoClient(result,tdL);
                                               data->addTohistorySearch(data->getUserid(username),fullcomand.at(1),extractISBN);
                                           }
                                            else
                                               if(fullcomand.at(1)=="gen")
                                               {
                                                   string extractgen=extractfromComand(fullcomand,2,0);
                                                   vector<vector<string>> result=data->searchBookbyGen(extractgen);
                                                   result=data->addGenre(result);
                                                   sendVectortoClient(result,tdL);
                                                   data->addTohistorySearch(data->getUserid(username),fullcomand.at(1),extractgen);

                                               }
                                                else
                                                   if(fullcomand.at(1)=="rating")
                                                   {
                                                       string extractRating=extractfromComand(fullcomand,2,0);
                                                       vector<vector<string>> result=data->searchBookbyRating(extractRating);
                                                       result=data->addGenre(result);
                                                       sendVectortoClient(result,tdL);
                                                       data->addTohistorySearch(data->getUserid(username),fullcomand.at(1),extractRating);
                                                   }
                                                    else
                                                   {
                                                       vector<vector<string>> c=intermediar("Eroare de sintaxa! Sintaxa este search 'autor|gen|rating|titlu|ISBN|an' input");
                                                       sendVectortoClient(c,tdL);
                                                   }

                        }//end search
                        else
                            if(fullcomand.at(0)=="download" and fullcomand.size()>=2)
                            {

                                bit=2;
                                if(write(tdL.cl,&bit,sizeof(int))<=0)
                                    perror   ("[server]Eroare la trimitere bit!");
                                string supposeName=extractfromComand(fullcomand,1,0);
                                string filename=data->Filename(supposeName);

                                int filenameSize=filename.size();
                                write(tdL.cl,&filenameSize,sizeof(int));
                                write(tdL.cl,filename.c_str(),filenameSize);
                                cout<<filename;

                                if(filename!="!exist")
                                {
                                FILE *fd;
                                fd=fopen(filename.c_str(),"rb");
                                char buff[128];
                                while(1)
                                {

                                    int nread=fread(&buff,1,128,fd);
                                    if(nread>0)
                                        write(tdL.cl,&buff,nread);
                                    if(nread<1)
                                        break;
                                }
                                string title=extractfromComand(fullcomand,1,0);
                                data->addTohostoryDownload(data->getUserid(username),title);
                                }

                            }
                            else
                                if(fullcomand.at(0)=="history" and fullcomand.size()>=2)
                                {
                                    bit=3;
                                    if(write(tdL.cl,&bit,sizeof(int))<=0)
                                        perror   ("[server]Eroare la trimitere bit!");

                                    if(fullcomand.at(1)=="search")
                                    {
                                        vector<vector<string>> result;
                                        result=data->printSearchTable(data->getUserid(username));
                                        sendVectortoClient(result,tdL);
                                    }
                                    else

                                    if(fullcomand.at(1)=="download")
                                    {
                                         vector<vector<string>> result;
                                         result=data->printDownloadTable(data->getUserid(username));
                                         sendVectortoClient(result,tdL);
                                    }
                                    else
                                    {
                                        vector<vector<string>> result;
                                        vector<string>mess2;
                                        string mess="Eroare la sintaxa history! Sintaxa este: history search|download";
                                        mess2.push_back(mess);
                                        result.push_back(mess2);
                                        sendVectortoClient(result,tdL);

                                    }

                                }
                                else
                                    if(fullcomand.at(0)=="help" and fullcomand.size()==1)
                                    {
                                        bit=4;
                                        if(write(tdL.cl,&bit,sizeof(int))<=0)
                                            perror   ("[server]Eroare la trimitere bit!");
                                    }
                                    else
                                        if(fullcomand.at(0)=="add" and fullcomand.at(1)=="rating" and fullcomand.at(2)=="to" and fullcomand.size()>=5)
                                        {
                                            bit=5;
                                            if(write(tdL.cl,&bit,sizeof(int))<=0)
                                                perror   ("[server]Eroare la trimitere bit!");

                                            string rating=extractfromComand(fullcomand,fullcomand.size()-1,0);
                                            float floatRating=atoi(rating.c_str());


                                            if(floatRating>=0 and floatRating<=10)
                                            {

                                                string bookName=extractfromComand(fullcomand,3,1);
                                                if(data->checkIfBookExists(bookName)==1)
                                                {

                                                    if(data->checkIfUserExistsInRating(username,bookName)==1)
                                                       data->UpdateRating(floatRating,bookName,"Ratings");
                                                       else
                                                        data->addRating(username,floatRating,bookName);

                                                    data->UpdateRating(data->AVGRating(bookName),bookName,"Carti");
                                                    clientStringRespond("Rating adaugat cu succes!",tdL);

                                                }
                                                else
                                                    clientStringRespond("Numele cartii nu exista!",tdL);
                                            }




                                            else
                                               clientStringRespond("Nota trebuie sa fie intre 0-10",tdL);




                                        }
                                        else
                                            if(fullcomand.at(0)=="recommend" and fullcomand.size()==1)
                                            {
                                                   bit=6;
                                                   if(write(tdL.cl,&bit,sizeof(int))<=0)
                                                       perror   ("[server]Eroare la trimitere bit!");
                                                    vector<vector<string>> result=data->recommand(data->getUserid(username));
                                                    sendVectortoClient(result,tdL);

                                            }

                                            else
                                   {   bit=1;
                                       if(write(tdL.cl,&bit,sizeof(int))<=0)
                                            perror   ("[server]Eroare la trimitere bit!");
                                         vector<vector<string>> c=intermediar("Comanda nerecunscuta!");
                                        sendVectortoClient(c,tdL);
                                    }
                     }


               }
            else
                clientStringRespond("Acest username nu exista! Reincercati!",tdL);
        }
        else
            if(fullcomand.at(0)=="quit" and fullcomand.size()==1)
            {
                clientStringRespond("Ai iesit cu succes!",tdL);
                data->close();
                close(tdL.cl);
                break;
               // quit=1;

            }
            else
            clientStringRespond("Comanda gresita! sintaxa este: login 'username'",tdL);

        }
       close ((intptr_t)arg);


        return(NULL);

};

vector<vector<string> > intermediar( string a)
{
    vector<string> b;
    b.push_back(a);
    vector<vector<string>> c;
    c.push_back(b);
    return c;
}

void clientStringRespond(string text ,thData tdL)
{
    char comand[50];
    int length;
    strcpy(comand,text.c_str());
    length=strlen(comand);


    if(write(tdL.cl,&length,sizeof(int))<=0)
        perror ("[client]Eroare la write() spre server.\n");

    if (write (tdL.cl,&comand,length) <= 0)
        perror ("[client]Eroare la write() spre server.\n");

}


void sendVectortoClient(vector<vector<string> > result,thData tdL)
{
    int linii=result.size();
    if(write(tdL.cl,&linii,sizeof(int))<=0)
        perror("[server]Eroare la trimitere!");
    for(vector<vector<string> >::iterator it=result.begin();it<result.end();it++)
    {
        vector<string> row =*it;
        int col=(*it).size();
        if(write(tdL.cl,&col,sizeof(int))<=0)
            perror("[server]Eroare la trimitere!");

         for(vector<string>::iterator jt=row.begin();jt<row.end();jt++)
         {

             int size=(*jt).size();
             if(write(tdL.cl,&size,sizeof(int))<=0)
                 perror("[server]Eroare la trimitere!");
             if(write(tdL.cl,(*jt).c_str(),size)<=0)
                 perror("[server]Eroare la trimitere!");

         }

    }
}


