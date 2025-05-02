#include "flag-argument.hpp"
#include "exceptions.hpp"
#include "arguments.hpp"

bool FlagArgument::value()
{
    if (this->current_value.has_value())
    {
        return this->current_value.value();
    }
    if (this->default_value.has_value())
    {
        return this->default_value.value();
    }
    throw MissingArgsException::from_switch(this->switches[0]);
}

FlagArgument::FlagArgument(Arguments *args) : Argument(args)
{
    args->flagArgs.push_back(this);
}

bool FlagArgument::checkArgs(std::deque<std::string> &args)
{
    std::string front = args.front();
    for (std::string sw : this->switches)
    {
        if (front == sw)
        {
            args.pop_front();
            if (this->default_value.has_value())
            {
                this->current_value = !this->default_value.value();
            }
            else {
                this->current_value = true;
            }
            return true;
        }
    }
    return false;
}