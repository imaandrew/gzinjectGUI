#include "commandOutput.h"

CommandOutput::CommandOutput(int sentExitCode, QString sentStandardout, QString sentStandarderr) {
    exitCode = sentExitCode;
    standardout = sentStandardout;
    standarderr = sentStandarderr;
}

int CommandOutput::getExitCode() {
    return exitCode;
}

QString CommandOutput::getStandardOut() {
    return standardout;
}

QString CommandOutput::getStandardErr() {
    return standarderr;
}
