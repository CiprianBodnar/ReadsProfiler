#include "db.h"
#include<iostream>
db::db(char * filename)
{
    database=NULL;
    open(filename);
}

bool db::open(char *filename)
{
    if(sqlite3_open(filename,&database)==SQLITE_OK)
            return 1;
     return 0;
}

vector<vector<string> > db::querry(string querry)
{
   sqlite3_stmt *statement;
   vector<vector<string> > results;

   if(sqlite3_prepare_v2(database,querry.c_str(),-1,&statement,0)==SQLITE_OK)
   {
       int cols=sqlite3_column_count(statement);
       int result=0;
       while(1)
       {
           result=sqlite3_step(statement);
           if(result==SQLITE_ROW)
           {
               vector<string> values;
               for(int col=0;col<cols;col++)
                   values.push_back((char*)sqlite3_column_text(statement,col));
               results.push_back(values);
           }
           else
               break;
       }
       sqlite3_finalize(statement);
   }
   string error=sqlite3_errmsg(database);
   if(error!="not an error")
       cout<<querry<<" "<<error<<endl;
   return results;
}

void db::close()
{
    sqlite3_close(database);
}

bool db::checkIfExistusername(string username)
{
    sqlite3_stmt *statement;

      vector<string> results;

    if(sqlite3_prepare_v2(database,"SELECT username from Utilizatori",-1,&statement,0)==SQLITE_OK)
    {
      int cols=sqlite3_column_count(statement);
      int result=0;
      while(1)
      {
          result=sqlite3_step(statement);
          if(result==SQLITE_ROW)
          {

              for(int col=0;col<cols;col++)
                  results.push_back((char*)sqlite3_column_text(statement,col));
             // results.push_back((values));
          }
          else
              break;
      }
      sqlite3_finalize(statement);

    }
    else
    {
        string error=sqlite3_errmsg(database);
        if(error!="not an error")
            cout<<error<<endl;
    }

    for(vector<string> ::iterator it=results.begin();it!=results.end();it++)
       if(username==(*it))
            return 1;
    return 0;

}

bool db::checkIfisAdmin(string username)
{
    sqlite3_stmt *statement;

      vector<vector<string>> results;

    if(sqlite3_prepare_v2(database,"SELECT username,admin from Utilizatori",-1,&statement,0)==SQLITE_OK)
    {
      int cols=sqlite3_column_count(statement);
      int result=0;
      while(1)
      {
          result=sqlite3_step(statement);
          if(result==SQLITE_ROW)
          {
              vector<string> value;
              for(int col=0;col<cols;col++)
                  value.push_back((char*)sqlite3_column_text(statement,col));
              results.push_back((value));
          }
          else
              break;
      }
      sqlite3_finalize(statement);

    }
    else
    {
        string error=sqlite3_errmsg(database);
        if(error!="not an error")
            cout<<error<<endl;
    }

     for(vector<vector<string>>::iterator it=results.begin();it!=results.end();it++)
      {
          vector<string> row=(*it);

              if(username==row.at(0))
              {
                  if(row.at(1)=="1")
                      return 1;

              }
      }

    return 0;

}

vector<vector<string>> db::searchbyBooktitle(string Booktitle)
{
    sqlite3_stmt *statement;

    vector<vector<string>> results;
    string quer;

    quer="SELECT nume,titlu,an,Publicatii.ISBN,rating from Carti join Publicatii on Carti.ISBN=Publicatii.ISBN join Autori on Autori.id_autor=Publicatii.id_autor  where titlu='";
    quer=quer+Booktitle;
    quer=quer+"';";


    if(sqlite3_prepare_v2(database,quer.c_str(),-1,&statement,0)==SQLITE_OK)
   {
        int cols=sqlite3_column_count(statement);
        int result=0;
        while(1)
        {
            result=sqlite3_step(statement);
            if(result==SQLITE_ROW)
            {
                vector<string> value;
                for(int col=0;col<cols;col++)
                {
                    value.push_back((char*)sqlite3_column_text(statement,col));
                    value.push_back("-");
                }
                results.push_back((value));
            }
            else
                break;
        }
        sqlite3_finalize(statement);

    }
    else
    {
        string error=sqlite3_errmsg(database);
        if(error!="not an error")
            cout<<error<<endl;
    }
    if(results.size()==0)
    {
        vector<string> a;
        a.push_back("Nu s-a gasit nici o carte cu acest titlu.");
        results.push_back(a);

    }

    return results;

}

vector<vector<string>> db::searchbyYearofBook(string year)
{
    sqlite3_stmt *statement;

    vector<vector<string>> results;
    string quer;

    quer="SELECT nume,titlu,an,Publicatii.ISBN,rating from Carti join Publicatii on Carti.ISBN=Publicatii.ISBN join Autori on Autori.id_autor=Publicatii.id_autor where an='";
    quer=quer+year;
    quer=quer+"';";


    if(sqlite3_prepare_v2(database,quer.c_str(),-1,&statement,0)==SQLITE_OK)
   {
        int cols=sqlite3_column_count(statement);
        int result=0;
        while(1)
        {
            result=sqlite3_step(statement);
            if(result==SQLITE_ROW)
            {
                vector<string> value;
                for(int col=0;col<cols;col++)
                   { value.push_back((char*)sqlite3_column_text(statement,col));
                    value.push_back("-");
                }
                results.push_back((value));
            }
            else
                break;
        }
        sqlite3_finalize(statement);

    }
    else
    {
        string error=sqlite3_errmsg(database);
        if(error!="not an error")
            cout<<error<<endl;
    }

    if(results.size()==0)
    {
        vector<string> a;
        a.push_back("Nu s-a gasit nici o carte care sa corespunda anului dorit.");
        results.push_back(a);

    }




    return results;

}

vector<vector<string>> db::searchbyISBNofBook(string ISBN)
{
    sqlite3_stmt *statement;

    vector<vector<string>> results;
    string quer;

    quer="SELECT Autori.nume,titlu,an,Carti.ISBN,rating from Carti join Publicatii on Carti.ISBN=Publicatii.ISBN join Autori on Autori.id_autor=Publicatii.id_autor where Carti.ISBN='";
    quer=quer+ISBN;
    quer=quer+"';";


    if(sqlite3_prepare_v2(database,quer.c_str(),-1,&statement,0)==SQLITE_OK)
   {
        int cols=sqlite3_column_count(statement);
        int result=0;
        while(1)
        {
            result=sqlite3_step(statement);
            if(result==SQLITE_ROW)
            {
                vector<string> value;
                for(int col=0;col<cols;col++)
                {
                    value.push_back((char*)sqlite3_column_text(statement,col));
                    value.push_back("-");
                }
                results.push_back((value));
            }
            else
                break;
        }
        sqlite3_finalize(statement);

    }
    else
    {
        string error=sqlite3_errmsg(database);
        if(error!="not an error")
            cout<<error<<endl;
    }

    if(results.size()==0)
    {
        vector<string> a;
        a.push_back("Nu s-a gasit nici o carte dupa acest ISBN.");
        results.push_back(a);

    }
    return results;

}


vector<vector<string>> db::searchBookbyAuthor(string Author)
{
    sqlite3_stmt *statement;

    vector<vector<string>> results;
    string quer;

    quer="SELECT Autori.nume,titlu,an,Carti.ISBN,rating from Carti join Publicatii on Publicatii.ISBN=Carti.ISBN join Autori on Autori.id_autor=Publicatii.id_autor where nume='";
    quer=quer+Author;
    quer=quer+"';";


    if(sqlite3_prepare_v2(database,quer.c_str(),-1,&statement,0)==SQLITE_OK)
   {
        int cols=sqlite3_column_count(statement);
        int result=0;
        while(1)
        {
            result=sqlite3_step(statement);
            if(result==SQLITE_ROW)
            {
                vector<string> value;
                for(int col=0;col<cols;col++)
                {
                    value.push_back((char*)sqlite3_column_text(statement,col));
                    value.push_back("-");
                }
                results.push_back((value));
            }
            else
                break;
        }
        sqlite3_finalize(statement);

    }
    else
    {
        string error=sqlite3_errmsg(database);
        if(error!="not an error")
            cout<<error<<endl;
    }


    if(results.size()==0)
    {
        vector<string> a;
        a.push_back("Nu s-a gasit nici o carte dupa acest autor.");
        results.push_back(a);

    }

    return results;

}

vector<vector<string>> db::searchBookbyGen(string Gen)
{
    sqlite3_stmt *statement;

    vector<vector<string>> results;
    string quer;

    quer="SELECT Autori.nume,titlu,an,Carti.ISBN,rating from Carti join Publicatii on Publicatii.ISBN=Carti.ISBN join Autori on Autori.id_autor=Publicatii.id_autor join SubgenCarte on "
         "SubgenCarte.ISBN=Carti.ISBN join TipGenuri on TipGenuri.id_gen=SubgenCarte.id_gen where nume_gen='";
    quer=quer+Gen;
    quer=quer+"';";


    if(sqlite3_prepare_v2(database,quer.c_str(),-1,&statement,0)==SQLITE_OK)
   {
        int cols=sqlite3_column_count(statement);
        int result=0;
        while(1)
        {
            result=sqlite3_step(statement);
            if(result==SQLITE_ROW)
            {
                vector<string> value;
                for(int col=0;col<cols;col++)
                {
                    value.push_back((char*)sqlite3_column_text(statement,col));
                    value.push_back("-");
                }

                results.push_back((value));
            }
            else
                break;
        }
        sqlite3_finalize(statement);

    }
    else
    {
        string error=sqlite3_errmsg(database);
        if(error!="not an error")
            cout<<error<<endl;
    }


    if(results.size()==0)
    {
        vector<string> a;
        a.push_back("Nu s-a gasit nici o carte dupa acest gen.");
        results.push_back(a);

    }

    return results;

}

vector<vector<string>> db::searchBookbyRating(string Rating)
{
    sqlite3_stmt *statement;

    vector<vector<string>> results;
    string quer;

    quer="SELECT distinct Autori.nume,titlu,an,Carti.ISBN,rating from Carti join Publicatii on Publicatii.ISBN=Carti.ISBN join Autori on Autori.id_autor=Publicatii.id_autor join SubgenCarte on "
         "SubgenCarte.ISBN=Carti.ISBN join TipGenuri on TipGenuri.id_gen=SubgenCarte.id_gen where rating='";
    quer=quer+Rating;
    quer=quer+"';";


    if(sqlite3_prepare_v2(database,quer.c_str(),-1,&statement,0)==SQLITE_OK)
   {
        int cols=sqlite3_column_count(statement);
        int result=0;
        while(1)
        {
            result=sqlite3_step(statement);
            if(result==SQLITE_ROW)
            {
                vector<string> value;
                for(int col=0;col<cols;col++)
                   { value.push_back((char*)sqlite3_column_text(statement,col));
                    value.push_back("-");
                   }
                results.push_back((value));
            }
            else
                break;
        }
        sqlite3_finalize(statement);

    }
    else
    {
        string error=sqlite3_errmsg(database);
        if(error!="not an error")
            cout<<error<<endl;
    }
    if(results.size()==0)
    {
        vector<string> a;
        a.push_back("Nu s-a gasit nici o carte cu acest rating.");
        results.push_back(a);

    }
    return results;
}


string db::Filename(string BookTitle)
{
    sqlite3_stmt *statement;
    string quer,results;
    quer="SELECT cale FROM Carti where titlu='";
    quer=quer+BookTitle;
    quer=quer+"';";
    if(sqlite3_prepare_v2(database,quer.c_str(),-1,&statement,0)==SQLITE_OK)
   {
        //int cols=sqlite3_column_count(statement);
        int result=0;
        while(1)
        {
            result=sqlite3_step(statement);
            if(result==SQLITE_ROW)
                results=(char*)sqlite3_column_text(statement,0);

            else
                break;
        }
        sqlite3_finalize(statement);

    }
    else
    {
        string error=sqlite3_errmsg(database);
        if(error!="not an error")
            cout<<error<<endl;
    }

    if(results.size()==0)
        results="!exist";
    return results;

}

string db::getUserid(string user)
{
    sqlite3_stmt *statement;
    string quer,results;
    quer="SELECT id_user FROM Utilizatori where username='";
    quer=quer+user;
    quer=quer+"';";
    if(sqlite3_prepare_v2(database,quer.c_str(),-1,&statement,0)==SQLITE_OK)
    {

        //int cols=sqlite3_column_count(statement);
        int result=0;
        while(1)
        {
            result=sqlite3_step(statement);
            if(result==SQLITE_ROW)
                results=(char*)sqlite3_column_text(statement,0);

            else
                break;
        }
        sqlite3_finalize(statement);

    }

    return results;


}

void db::addTohistorySearch(string id_user, string tip, string what)
{

    string quer="INSERT INTO Cautari VALUES('";
    quer=quer+id_user;
    quer=quer+"','";
    quer=quer+tip;
    quer=quer+"','";
    quer=quer+what;
    quer=quer+"');";
    sqlite3_stmt *statement;
    vector<vector<string> > results;

    if(sqlite3_prepare_v2(database,quer.c_str(),-1,&statement,0)==SQLITE_OK)
    {
        int cols=sqlite3_column_count(statement);
        int result=0;
        while(1)
        {
            result=sqlite3_step(statement);
            if(result==SQLITE_ROW)
            {
                vector<string> values;
                for(int col=0;col<cols;col++)
                    values.push_back((char*)sqlite3_column_text(statement,col));
                results.push_back(values);
            }
            else
                break;
        }
        sqlite3_finalize(statement);
    }
    string error=sqlite3_errmsg(database);
    if(error!="not an error")
        cout<<quer<<" "<<error<<endl;
}

void db::addTohostoryDownload(string id_user, string titlu)
{
    string quer;
    quer="INSERT INTO Descarcari VALUES('";
    quer=quer+id_user;
    quer=quer+"','";
    quer=quer+titlu;
    quer=quer+"');";
    sqlite3_stmt *statement;
    vector<vector<string> > results;

    if(sqlite3_prepare_v2(database,quer.c_str(),-1,&statement,0)==SQLITE_OK)
    {
        int cols=sqlite3_column_count(statement);
        int result=0;
        while(1)
        {
            result=sqlite3_step(statement);
            if(result==SQLITE_ROW)
            {
                vector<string> values;
                for(int col=0;col<cols;col++)
                    values.push_back((char*)sqlite3_column_text(statement,col));
                results.push_back(values);
            }
            else
                break;
        }
        sqlite3_finalize(statement);
    }
    string error=sqlite3_errmsg(database);
    if(error!="not an error")
        cout<<quer<<" "<<error<<endl;
}

vector<vector<string>> db::printSearchTable(string user)
{
    sqlite3_stmt *statement;

    vector<vector<string>> results;
    string quer;


    quer="SELECT * FROM Cautari WHERE id_user='";
    quer=quer+user;
    quer=quer+"';";


    if(sqlite3_prepare_v2(database,quer.c_str(),-1,&statement,0)==SQLITE_OK)
   {
        int cols=sqlite3_column_count(statement);
        int result=0;
        while(1)
        {
            result=sqlite3_step(statement);
            if(result==SQLITE_ROW)
            {
                vector<string> value;
                for(int col=0;col<cols;col++)
                    value.push_back((char*)sqlite3_column_text(statement,col));
                results.push_back((value));
            }
            else
                break;
        }
        sqlite3_finalize(statement);

    }
    else
    {
        string error=sqlite3_errmsg(database);
        if(error!="not an error")
            cout<<error<<endl;
    }

    if(results.size()==0)
    {
        vector<string> a;
        a.push_back("Fara cautari pentru acest user");
        results.push_back(a);

    }

    return results;
}

vector<vector<string>> db::printDownloadTable(string user)
{
    sqlite3_stmt *statement;

    vector<vector<string>> results;
    string quer;


    quer="SELECT * FROM Descarcari WHERE id_user='";
    quer=quer+user;
    quer=quer+"';";



    if(sqlite3_prepare_v2(database,quer.c_str(),-1,&statement,0)==SQLITE_OK)
   {
        int cols=sqlite3_column_count(statement);
        int result=0;
        while(1)
        {
            result=sqlite3_step(statement);
            if(result==SQLITE_ROW)
            {
                vector<string> value;
                for(int col=0;col<cols;col++)
                    value.push_back((char*)sqlite3_column_text(statement,col));
                results.push_back((value));
            }
            else
                break;
        }
        sqlite3_finalize(statement);

    }
    else
    {
        string error=sqlite3_errmsg(database);
        if(error!="not an error")
            cout<<error<<endl;
    }

    if(results.size()==2)
    {
        vector<string> a;
        a.push_back("Nici o carte descarcata pentru acest user");
        results.push_back(a);

    }

    return results;
}

bool db::checkIfBookExists(string Bookname)
{
    sqlite3_stmt *statement;

    vector<vector<string>> results;
    string quer;

    quer="SELECT nume,titlu,an,Publicatii.ISBN,rating from Carti join Publicatii on Carti.ISBN=Publicatii.ISBN join Autori on Autori.id_autor=Publicatii.id_autor where titlu='";
    quer=quer+Bookname;
    quer=quer+"';";


    if(sqlite3_prepare_v2(database,quer.c_str(),-1,&statement,0)==SQLITE_OK)
   {
        int cols=sqlite3_column_count(statement);
        int result=0;
        while(1)
        {
            result=sqlite3_step(statement);
            if(result==SQLITE_ROW)
            {
                vector<string> value;
                for(int col=0;col<cols;col++)
                    value.push_back((char*)sqlite3_column_text(statement,col));
                results.push_back((value));
            }
            else
                break;
        }
        sqlite3_finalize(statement);

    }
    else
    {
        string error=sqlite3_errmsg(database);
        if(error!="not an error")
            cout<<error<<endl;
    }
    if(results.size()==0)
    {
        return 0;

    }

    return 1;

}

void db::addRating(string username,float rating,string book)
{
    string quer;
    quer="INSERT INTO Ratings VALUES('";
    string ratingS=to_string(rating);
    quer=quer+book;
    quer=quer+"',";
    quer=quer+ratingS;
    quer=quer+",'";
    quer=quer+username;
    quer=quer+"');";
    sqlite3_stmt *statement;
    vector<vector<string> > results;

    if(sqlite3_prepare_v2(database,quer.c_str(),-1,&statement,0)==SQLITE_OK)
    {
        int cols=sqlite3_column_count(statement);
        int result=0;
        while(1)
        {
            result=sqlite3_step(statement);
            if(result==SQLITE_ROW)
            {
                vector<string> values;
                for(int col=0;col<cols;col++)
                    values.push_back((char*)sqlite3_column_text(statement,col));
                results.push_back(values);
            }
            else
                break;
        }
        sqlite3_finalize(statement);
    }
    string error=sqlite3_errmsg(database);
    if(error!="not an error")
        cout<<quer<<" "<<error<<endl;
}
void db::printfTable()
{
    string quer="SELECT * FROM Ratings";
    sqlite3_stmt *statement;
    vector<vector<string> > results;

    if(sqlite3_prepare_v2(database,quer.c_str(),-1,&statement,0)==SQLITE_OK)
    {
        int cols=sqlite3_column_count(statement);
        int result=0;
        while(1)
        {
            result=sqlite3_step(statement);
            if(result==SQLITE_ROW)
            {
                vector<string> values;
                for(int col=0;col<cols;col++)
                    values.push_back((char*)sqlite3_column_text(statement,col));
                results.push_back(values);
            }
            else
                break;
        }
        sqlite3_finalize(statement);
    }
    string error=sqlite3_errmsg(database);
    if(error!="not an error")
        cout<<quer<<" "<<error<<endl;

    for(vector<vector<string> >::iterator it=results.begin();it!=results.end();it++)
    {
        vector<string> row =*it;

        for(vector<string>::iterator jt=row.begin();jt!=row.end();jt++)
            cout<<(*jt)<<"  ";
        cout<<endl;
    }
    cout<<endl;
}

void db::UpdateRating(float rating, string book,string tabel)
{

    string quer="UPDATE '";
    quer=quer+tabel;
    quer=quer+"' SET rating=";
    string ratingS=to_string(rating);
    quer=quer+ratingS;
    quer=quer+" where titlu='";
    quer=quer+book;
    quer=quer+"';";

    sqlite3_stmt *statement;
    vector<vector<string> > results;

    if(sqlite3_prepare_v2(database,quer.c_str(),-1,&statement,0)==SQLITE_OK)
    {
        int cols=sqlite3_column_count(statement);
        int result=0;
        while(1)
        {
            result=sqlite3_step(statement);
            if(result==SQLITE_ROW)
            {
                vector<string> values;
                for(int col=0;col<cols;col++)
                    values.push_back((char*)sqlite3_column_text(statement,col));
                results.push_back(values);
            }
            else
                break;
        }
        sqlite3_finalize(statement);
    }
    string error=sqlite3_errmsg(database);
    if(error!="not an error")
        cout<<quer<<" "<<error<<endl;
}

float db::AVGRating(string book)
{
    string quer="SELECT AVG(rating) FROM RATINGS WHERE titlu='";
    quer=quer+book;
    quer=quer+"' GROUP BY titlu;";
    sqlite3_stmt *statement;
    vector<vector<string> > results;

    if(sqlite3_prepare_v2(database,quer.c_str(),-1,&statement,0)==SQLITE_OK)
    {
        int cols=sqlite3_column_count(statement);
        int result=0;
        while(1)
        {
            result=sqlite3_step(statement);
            if(result==SQLITE_ROW)
            {
                vector<string> values;
                for(int col=0;col<cols;col++)
                    values.push_back((char*)sqlite3_column_text(statement,col));
                results.push_back(values);
            }
            else
                break;
        }
        sqlite3_finalize(statement);
    }
    string error=sqlite3_errmsg(database);
    if(error!="not an error")
        cout<<quer<<" "<<error<<endl;

    float rezultat;
    quer=results.at(0).at(0);
    rezultat=atoi(quer.c_str());
    return rezultat;
}


bool db::checkIfUserExistsInRating(string username,string bookname)
{
    string quer="SELECT username FROM Ratings where username='";
    quer=quer+username;
    quer=quer+"' and titlu='";
    quer=quer+bookname;
    quer=quer+"';";

    sqlite3_stmt *statement;
    vector<vector<string> > results;

    if(sqlite3_prepare_v2(database,quer.c_str(),-1,&statement,0)==SQLITE_OK)
    {
        int cols=sqlite3_column_count(statement);
        int result=0;
        while(1)
        {
            result=sqlite3_step(statement);
            if(result==SQLITE_ROW)
            {
                vector<string> values;
                for(int col=0;col<cols;col++)
                    values.push_back((char*)sqlite3_column_text(statement,col));
                results.push_back(values);
            }
            else
                break;
        }
        sqlite3_finalize(statement);
    }
    string error=sqlite3_errmsg(database);
    if(error!="not an error")
        cout<<quer<<" "<<error<<endl;

    if(results.size()==0)
        return 0;
    return 1;

}

vector<vector<string>> db::recommand(string user)
{
    vector<vector<string>> results;
    sqlite3_stmt *statement;
    string quer="SELECT titlu,rating FROM Carti EXCEPT SELECT nume,rating FROM Descarcari join Carti on Carti.titlu=Descarcari.nume WHERE id_user='";
    quer=quer+user;
    quer=quer+"' ORDER BY rating desc;";
    if(sqlite3_prepare_v2(database,quer.c_str(),-1,&statement,0)==SQLITE_OK)
   {
        int cols=sqlite3_column_count(statement);
        int result=0;
        while(1)
        {
            result=sqlite3_step(statement);
            if(result==SQLITE_ROW)
            {
                vector<string> value;
                for(int col=0;col<cols;col++)
                    value.push_back((char*)sqlite3_column_text(statement,col));
                results.push_back((value));
            }
            else
                break;
        }
        sqlite3_finalize(statement);

    }
    else
    {
        string error=sqlite3_errmsg(database);
        if(error!="not an error")
            cout<<error<<endl;
    }




    return results;
}

bool check(vector<vector<string>> Result)
{
    const char * a=Result.at(0).at(0).c_str();
    if(!(a[0]=='N' && a[1]=='u' && a[2]==' ' && a[3]=='s' && a[4]=='-' && a[5]=='a'))
        return 1;
    return 0;
}

vector<vector<string>> db::addGenre(vector<vector<string> > Result)
{


    if(check(Result)==1)
     {
    string quer;
    int start=0;
    for(unsigned int i=0;i<Result.size();i++)
    {

            start=0;
            quer.erase();
            quer="SELECT nume_gen,subgen FROM Carti join SubgenCarte on SubgenCarte.ISBN=Carti.ISBN join TipGenuri on SubgenCarte.id_gen=TipGenuri.id_gen where Carti.ISBN='";
            quer=quer+Result.at(i).at(6);
            quer=quer+"';";

            sqlite3_stmt *statement;
            if(sqlite3_prepare_v2(database,quer.c_str(),-1,&statement,0)==SQLITE_OK)
            {
                int cols=sqlite3_column_count(statement);
                int result=0;
                while(1)
                {
                    result=sqlite3_step(statement);
                    if(result==SQLITE_ROW)
                    {

                        for(int col=start;col<cols;col++)
                            Result.at(i).push_back((char*)sqlite3_column_text(statement,col));
                        start=1;
                    }
                    else
                        break;
                }
                sqlite3_finalize(statement);
            }
            string error=sqlite3_errmsg(database);
            if(error!="not an error")
                cout<<quer<<" "<<error<<endl;


    }

    }
    return Result;
}
