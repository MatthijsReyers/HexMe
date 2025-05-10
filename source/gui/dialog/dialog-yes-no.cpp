#include "./dialog-yes-no.hpp"

std::vector<std::string> yesNoBtns = {
    "yes",
    "no",
    "cancel",
};

namespace gui
{
    DialogYesNo::DialogYesNo(const std::vector<std::string> t) : Dialog(t, yesNoBtns) {}
    DialogYesNo::DialogYesNo(const std::string t) : Dialog(t, yesNoBtns) {}
}
