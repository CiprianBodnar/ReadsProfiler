#ifndef HEADER_H
#define HEADER_H

string extractfromComand(vector<string> comand,int start,int stop)
{
    string result;
    vector<string>::iterator it;
    it=comand.begin()+start;
    result=result+(*it);
    for(it=comand.begin()+start+1;it!=comand.end()-stop;it++)
    {
        result=result+" ";
        result=result+(*it);
    }

    return result;

}




#endif // HEADER_H
