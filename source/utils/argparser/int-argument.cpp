#include "int-argument.hpp"
#include "exceptions.hpp"
#include "arguments.hpp"

IntArgument::IntArgument(Arguments* args) : Argument(args) {
    args->intArgs.push_back(this);
}

int64_t IntArgument::value()
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

bool IntArgument::checkArgs(std::deque<std::string> &args)
{
    std::string front = args.front();
    for (std::string sw : this->switches)
    {
        if (front == sw) {
            args.pop_front();

            if (args.size() == 0) {
                std::stringstream ss;
                ss << "Expected integer after \"" << sw << "\" but found nothing.";
                throw InvalidArgsException(ss.str());
            }

            try {
                auto value = std::stoll(args.front());
                args.pop_front();
                this->setValue(value);
            }
            catch ( const std::exception& e )
            {
                std::stringstream ss;
                ss << "Expected valid integer but found \"" << args.front() << "\".";
                throw InvalidArgsException(ss.str());
            }
            return true;
        }
    }
    return false;
}

void IntArgument::setValue(int64_t value) {
    if (this->min_value) {
        value = std::max(this->min_value.value(), value);
    }
    if (this->max_value) {
        value = std::min(this->max_value.value(), value);
    }
    this->current_value = value;
}
