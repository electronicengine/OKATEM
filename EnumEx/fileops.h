#ifndef FILEOPS_H
#define FILEOPS_H

#include <iostream>
#include <string>
#include <fstream>

class FileOps
{
public:
    FileOps();

    static std::string readFile();
    static int writeFile(const std::string& Data);

};

#endif // FILEOPS_H
