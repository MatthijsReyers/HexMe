#pragma once

class Arguments;

#include <algorithm>
#include <optional>
#include <string>
#include <vector>
#include <memory>
#include <deque>

template <auto N>
struct fixed_string {
    constexpr fixed_string(const char (&str)[N]) { 
        std::copy_n(str, N, value);
    }
    char value[N];
};

template <typename T>
class Argument
{
    protected:
        Arguments* arguments;
        Argument(Arguments* args) : arguments(args) {}

    public:
        std::vector<std::string> switches;
        std::optional<std::string> description;
        std::optional<T> default_value;
        std::optional<T> current_value;

        /**
         * Takes a stack of argument strings and checks if the top value represents a switch for
         * this argument. Note that if the argument takes a value this function may also pop the
         * next value of the stack.
         */
        bool checkArgs(std::deque<std::string>& args);

        /**
         * Current value of this argument, note that if the user has not provided one this method
         * will raise an exception.
         */
        T value();

        template <fixed_string... ss> 
        void addSwitches() {
            std::vector<const char*> sss = {ss.value...};
            for (auto s : sss) {
                std::string s_copied;
                s_copied.append(s);
                this->switches.push_back(s_copied);
            }
        }
};
