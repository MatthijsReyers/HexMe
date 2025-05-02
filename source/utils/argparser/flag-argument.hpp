#pragma once

#include "argument.hpp"
#include <vector>

class FlagArgument : public Argument<bool>
{
    private:
        FlagArgument(Arguments* args);

    public:
        template <fixed_string... ss> 
        static FlagArgument create(Arguments* args, bool val);
        template <fixed_string... ss> 
        static FlagArgument create(Arguments* args, const char* desc);
        template <fixed_string... ss> 
        static FlagArgument create(Arguments* args, const char* desc, bool val);

        bool checkArgs(std::deque<std::string>& args);
        bool value();
};

template <fixed_string... ss> 
FlagArgument FlagArgument::create(Arguments* args, bool val) {
    auto flag = FlagArgument(args);
    flag.default_value = val;
    flag.addSwitches<ss...>();
    return flag;
}

template <fixed_string... ss>
FlagArgument FlagArgument::create(Arguments* args, const char* desc) {
    auto flag = FlagArgument(args);
    flag.description = desc;
    flag.addSwitches<ss...>();
    return flag;
}

template <fixed_string... ss>
FlagArgument FlagArgument::create(Arguments* args, const char* desc, bool val) {
    auto flag = FlagArgument(args);
    flag.default_value = val;
    flag.description = desc;
    flag.addSwitches<ss...>();
    return flag;
}
