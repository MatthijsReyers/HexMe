#include "./dialog-okay.hpp"

namespace gui
{
    DialogOkay::DialogOkay(const std::vector<std::string> t) : Dialog(t, "OK") {}
    DialogOkay::DialogOkay(const std::string t) : Dialog(t, "OK") {}
}
