#pragma once

#include "argument.hpp"
#include "exceptions.hpp"

#include <string>

class IntArgument : public Argument<int64_t>
{
    private:
        IntArgument(Arguments* args);

        std::optional<int64_t> min_value;
        std::optional<int64_t> max_value;
        
        std::string test;

    protected:
        /**
         * Sets the range and default value of the argument in this order: {min, max, default}. 
         * Note that is you provide more than 3 values they are ignored.
         */
        template <typename... int64_t> 
        
        void setRange(int64_t... val);

        void setValue(int64_t value);

    public:
        template <fixed_string... ss> 
        static IntArgument create(Arguments* args, const char* desc);
        template <fixed_string... ss, typename... int64_t> 
        static IntArgument create(Arguments* args, int64_t... val);
        template <fixed_string... ss, typename... int64_t>  
        static IntArgument create(Arguments* args, const char* desc, int64_t... val);

        bool checkArgs(std::deque<std::string>& args);
        int64_t value();
};


template <fixed_string... ss> 
IntArgument IntArgument::create(Arguments* args, const char* desc) {
    auto arg = IntArgument(args);
    arg.addSwitches<ss...>();
    arg.description = desc;
    return args;
}

template <fixed_string... ss, typename... int64_t> 
IntArgument IntArgument::create(Arguments* args, int64_t... val) {
    auto arg = IntArgument(args);
    arg.addSwitches<ss...>();
    arg.setRange(val...);
    return args;
}

template <fixed_string... ss, typename... int64_t> 
IntArgument IntArgument::create(Arguments* args, const char* desc, int64_t... val) {
    auto arg = IntArgument(args);
    arg.description = desc;
    arg.addSwitches<ss...>();
    arg.setRange(val...);
    return args;
}

template <typename... int64_t> 
void IntArgument::setRange(int64_t... val) {
    int i = 0;
    for(const auto p : { val... }) {
        switch (i)
        {
            case 0: this->min_value = p; break;
            case 1: this->max_value = p; break;
            case 2: this->default_value = p; break;
            default: return;
        }
        i++;
    }
}
