#ifndef db_H
#define db_H
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<sqlite3.h>
#include<string.h>
#include<vector>
using namespace std;


class db
{

private:
    sqlite3 *database;

public:
    db(char * filename);
    ~db();
    void close();
    bool open(char *filename);
    vector<vector<string> > querry (string querry);

    bool checkIfExistusername(string username);
    bool checkIfisAdmin(string username);
    vector<vector<string>> searchbyBooktitle(string Booktitle);
    vector<vector<string>> searchbyYearofBook(string year);
    vector<vector<string>> searchbyISBNofBook(string ISBN);
    vector<vector<string>> searchBookbyAuthor(string Author);
    vector<vector<string>> searchBookbyGen(string Gen);
    vector<vector<string>> searchBookbyRating(string Rating);
    string Filename(string BookTitle);
    string getUserid(string user);
    void addTohistorySearch(string id_user,string tip,string what);
    void addTohostoryDownload(string id_user,string titlu);
    vector<vector<string>> printSearchTable(string user);
    vector<vector<string>> printDownloadTable(string user);
    bool checkIfBookExists(string Bookname);
    void addRating(string username,float rating,string book);
    //rating
    void UpdateRating(float rating,string book,string tabel);
    float AVGRating(string book);

    bool checkIfUserExistsInRating(string username,string bookname);
    //recommend
    vector<vector<string>> recommand(string user);

    void printfTable();

    vector<vector<string>> addGenre(vector<vector<string>> Result);

};

#endif // db_H
