#pragma once

#include <QString>

class CommandOutput {

    public:
        CommandOutput(int sentExitCode, QString sentStandardout, QString sentStandarderr);
        int getExitCode();
        QString getStandardOut();
        QString getStandardErr();
    private:
        int exitCode;
        QString standardout;
        QString standarderr;
};
