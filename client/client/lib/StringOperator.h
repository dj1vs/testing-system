#ifndef STRINGOPERATOR_H
#define STRINGOPERATOR_H

#include <QString>
class StringOperator{
public:
    QString cutArg(QString x, QString req)
    {
        if(req == "cmd")
        {
            // {cmd=' ... '; ... }
            int cmdStart = 6;
            int cmdLen = 1;
            for(int i = cmdStart+1; ; ++i)
            {
                if(x[i] == '\'' && x[i+1] == ';')
                    break;
                cmdLen++;
            }
            return x.mid(cmdStart, cmdLen);

        }
        return "";
    };
    bool validatePackage();
    bool validateInput();
};

#endif // STRINGOPERATOR_H
