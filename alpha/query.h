#ifndef QUERY_H
#define QUERY_H
void startTables(db *data)
{   /*
    data->querry("DROP TABLE Utilizatori" ) ;
    data->querry("DROP TABLE Carti");
    data->querry("DROP TABLE Autori");
    data->querry("DROP TABLE Publicatii");
    data->querry("DROP TABLE TipGenuri");
    data->querry("DROP TABLE SubgenCarte");
    data->querry("DROP TABLE Cautari");
    data->querry("DROP TABLE Descarcari");
    data->querry("DROP TABLE Ratings");

    */
    data->querry("CREATE TABLE Utilizatori (username TEXT,id_user INTEGER,admin INTEGER);");
    data->querry("INSERT INTO Utilizatori VALUES('ioan.bodnar',1,1);");
    data->querry("INSERT INTO Utilizatori VALUES('cristian.hasna',2,0);");
    data->querry("INSERT INTO Utilizatori VALUES('lucian.vartolomei',3,0);");
    data->querry("INSERT INTO Utilizatori VALUES('rares.mircea',4,0);");
    data->querry("INSERT INTO Utilizatori VALUES('marian.gafitescu',5,0);");
    data->querry("INSERT INTO Utilizatori VALUES('geo.stefan',6,0);");
    data->querry("INSERT INTO Utilizatori VALUES('alex.pomohaci',4,0);");

    data->querry("CREATE TABLE Carti (ISBN TEXT PRIMARY KEY NOT NULL, titlu TEXT NOT NULL,an INTEGER NOT NULL,cale TEXT,rating FLOAT);");
    data->querry("INSERT INTO Carti VALUES ('9786069830710','Amintiri din copilarie',1888,'Amintiri.pdf',0);");
    data->querry("INSERT INTO Carti VALUES ('6068391069','Povestea lui Harap-Alb',1877,'harap-alb.pdf',0);");
    data->querry("INSERT INTO Carti VALUES ('789735967826','Luceafarul',1893,'luceafarul.pdf',0);");
    data->querry("INSERT INTO Carti VALUES ('9786066867122','Ion',1957,'ion.pdf',0);");
    data->querry("INSERT INTO Carti VALUES ('9786077417122','Cimitirul Buna-Vestire',1965,'cimitir.pdf',0); ");
    data->querry("INSERT INTO Carti VALUES ('9786066417122','Cartea cu jucarii',1959,'cartea.pdf',0);");
    data->querry("INSERT INTO Carti VALUES ('2186996417122','Poezii',1971,'NSP.pdf',0);");

    data->querry("CREATE TABLE Autori (id_autor INTEGER PRIMARY KEY NOT NULL, nume TEXT NOT NULL,nr_carti INTEGER NOT NULL);");
    data->querry("INSERT INTO Autori VALUES (1,'Mihai Eminescu',1);");
    data->querry("INSERT INTO Autori VALUES (2,'Liviu Rebreanu',1);");
    data->querry("INSERT INTO Autori VALUES (3,'Tudor Arghezii',2);");
    data->querry("INSERT INTO Autori VALUES (4,'Ion Creanga',2);");
    data->querry("INSERT INTO Autori VALUES (5,'Nichita Stanescu',1);");

    data->querry("CREATE TABLE Publicatii(ISBN TEXT PRIMARY KEY NOT NULL,id_autor INTEGER PRYMARY KEY NOT NULL);");
    data->querry("INSERT INTO Publicatii VALUES ('9786069830710',4);");
    data->querry("INSERT INTO Publicatii VALUES ('6068391069',4);");
    data->querry("INSERT INTO Publicatii VALUES ('789735967826',1);");
    data->querry("INSERT INTO Publicatii VALUES ('9786066867122',2);");
    data->querry("INSERT INTO Publicatii VALUES ('9786077417122',3);");
    data->querry("INSERT INTO Publicatii VALUES ('9786066417122',3);");
    data->querry("INSERT INTO Publicatii VALUES ('2186996417122',5);");


    data->querry("CREATE TABLE TipGenuri(id_gen TEXT NOT NULL,nume_gen TEXT NOT NULL);");
    data->querry("INSERT INTO TipGenuri VALUES('g1','Dramatic');");
    data->querry("INSERT INTO TipGenuri VALUES('g2','Epic');");
    data->querry("INSERT INTO TipGenuri VALUES('g3','Liric');");
    data->querry("INSERT INTO TipGenuri VALUES('g4','Fantastic');");

    data->querry("CREATE TABLE SubgenCarte(ISBN TEXT,id_gen TEXT,subgen TEXT)");
    data->querry("INSERT INTO SubgenCarte VALUES('9786069830710','g2','Memorii');");
    data->querry("INSERT INTO SubgenCarte VALUES('6068391069','g2','Memorii');");
    data->querry("INSERT INTO SubgenCarte VALUES('789735967826','g3','Poezie');");
    data->querry("INSERT INTO SubgenCarte VALUES('9786066867122','g1','Realism Literar');");
    data->querry("INSERT INTO SubgenCarte VALUES('9786066867122','g1','Roman Interbelic');");
    data->querry("INSERT INTO SubgenCarte VALUES('9786077417122','g2','Roman interbelic');");
    data->querry("INSERT INTO SubgenCarte VALUES('9786066417122','g2','Roman interbelic');");
    data->querry("INSERT INTO SubgenCarte VALUES('2186996417122','g3','Poezii');");

    data->querry("CREATE TABLE Cautari(id_user TEXT, tip TEXT ,nume TEXT)");

    data->querry("CREATE TABLE Descarcari (id_user INTEGER, nume TEXT)");

    data->querry("CREATE TABLE Ratings (titlu TEXT, rating FLOAT,username TEXT)");




}
#endif // QUERY_H
