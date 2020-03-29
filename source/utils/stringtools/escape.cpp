#include "escape.h"

namespace utils
{
    namespace stringtools
    {
        std::string escape(const std::string raw)
        {
            std::string output;
            for (std::size_t i = 0; i < raw.length(); i++)
                if (raw[i] == '\\') {
                    i++;
                    switch (raw[i])
                    {
                        case 'a': output += "\x07"; break;
                        case 'b': output += "\x08"; break;
                        case 'e': output += "\x1B"; break;
                        case 'f': output += "\x0C"; break;
                        case 'n': output += "\x0A"; break;
                        case 'r': output += "\x0D"; break;
                        case 't': output += "\x09"; break;
                        case 'v': output += "\x0B"; break;
                        case '?': output += "\x3F"; break;
                        case '"': output += "\x22"; break;
                        case '\'':output += "\x27"; break;
                        case '\\':output += "\\"; break;

                        case 'x':
                            std::string hexString = raw.substr(i+1, 2);
                            char num = std::stoi(hexString,nullptr,16);
                            output += num;
                            i += 2;
                            break;
                    }
                }
                else output += raw[i];
            return output;
        }
    }
}