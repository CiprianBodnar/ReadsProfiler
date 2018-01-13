#ifndef HEADER_H
#define HEADER_H

void writeToServer( int length,char auxComand[50],int sd)
{
    if(write(sd,&length,sizeof(int))<=0)
      {
             perror ("[client]Eroare la write() spre server.\n");
          //   return errno;
      }


 if (write (sd,auxComand,length) <= 0)
   {
     perror ("[client]Eroare la write() spre server.\n");
    // return errno;
   }
}



#endif // HEADER_H
