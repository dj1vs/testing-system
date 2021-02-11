#ifndef STRINGOPERATOR_H
#define STRINGOPERATOR_H

#include <QString>
class StringOperator{
public:
    QString cutArg(QString x, QString req)
    {
        //{cmd='do something';}
        int i = 1;
        while(i < x.size() - 1)
        {
            int argEnd = getArgEnd(x, i);
            if(x.mid(i, argEnd - i + 1) != req)
            {
                int argTextStart = argEnd + 3;
                int argTextEnd = getArgTextEnd(x, argTextStart);
                i = argTextEnd + 3;
                while(x[i] == ' ' || x[i] == '\n') ++i;
            }
            else
            {
                int argTextStart = argEnd + 3;
                int argTextEnd = getArgTextEnd(x, argTextStart);
                return x.mid(argTextStart, argTextEnd-argTextStart + 1);
            }
        }
        return "";
    };
    bool validatePackage(QString x)
    {
        if(x.indexOf("cmd") == -1 || x.size() < 3)
            return false;
        if(x[0] != '{' || x[x.size()-1] != '}' || x[x.size()-2] != ';')
            return false;
        for(int i = 1; i < x.size()-3; ++i)
        {
            for(;i != x.size() && x.mid(i, 2) != "='"; ++i);
            if( i == x.size())
                return false;
            for(;i != x.size() && x.mid(i, 2) != "';"; ++i);
            if( i == x.size())
                return false;
            i += 3;
        }
        int specCharacterCount = 0;
        for(int i = 1; i < x.size()-1; ++i)
            if (x[i] == ' ' || x[i] == '\n')
                ++specCharacterCount;
        if(specCharacterCount == x.size() -2)
            return false;
        return true;

    }
   QString validateUserInput(QString x)
    {
       while(x.indexOf("';") != -1 || x.indexOf("='") != -1)
       {
           while(x.indexOf("';") != -1)
           {
              int pos = x.indexOf("';");
              x.insert(pos+1, " ");
           }
           while(x.indexOf("='") != -1)
           {
              int pos = x.indexOf("='");
              x.insert(pos+1, " ");
           }
       }
       return x;
    }
private:
    int getArgTextEnd(QString x, int start)
    {
        int res = start;
        for(;x.mid(res, 2) != "';"; ++res);
        return --res;
    };
    int getArgEnd(QString x, int start)
    {
        int res = start;
        for(;x.mid(res, 2) != "='" ; ++res);
        return --res;
    }
};

#endif // STRINGOPERATOR_H
