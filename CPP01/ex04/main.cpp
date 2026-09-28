#include <iostream>
#include <fstream>
#include <string>

static std::string replaceAll(const std::string& line,
                              const std::string& s1,
                              const std::string& s2)
{
    std::string result;
    std::size_t pos = 0;
    std::size_t found;

    while (true)
    {
        found = line.find(s1, pos);
        if (found == std::string::npos)
        {
            result.append(line.substr(pos));
            break;
        }
        result.append(line.substr(pos, found - pos));
        result.append(s2);
        pos = found + s1.length();
    }
    return result;
}

int main(int argc, char** argv)
{
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0]
                  << " <filename> <s1> <s2>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::string s1 = argv[2];
    std::string s2 = argv[3];

    if (s1.empty())
    {
        std::cerr << "Error: s1 must not be empty." << std::endl;
        return 1;
    }

    std::ifstream in(filename.c_str());
    if (!in)
    {
        std::cerr << "Error: cannot open input file: "
                  << filename << std::endl;
        return 1;
    }

    std::string outname = filename + ".replace";
    std::ofstream out(outname.c_str());
    if (!out)
    {
        std::cerr << "Error: cannot open output file: "
                  << outname << std::endl;
        return 1;
    }

    std::string line;
    bool firstLine = true;

    while (std::getline(in, line))
    {
        std::string replaced = replaceAll(line, s1, s2);

        if (!firstLine)
            out << std::endl;
        out << replaced;
        firstLine = false;
    }

    return 0;
}
