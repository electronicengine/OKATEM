#include "fileops.h"

std::string FileOps::readFile()
{
    std::string str;
    std::ifstream infile;
    infile.open("yusuf.txt");
    infile >> str;

    return str;

}

int FileOps::writeFile(const std::string &Data)
{

    std::ofstream outfile;
    outfile.open("yusuf.txt");
    outfile << Data;

    return 0;
}
