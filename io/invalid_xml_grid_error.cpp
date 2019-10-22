#include "invalid_xml_grid_error.hpp"

namespace Picross
{
    InvalidXMLGridError::InvalidXMLGridError(const char* message) :
        _message(message)
    {

    }

    InvalidXMLGridError::InvalidXMLGridError(std::string message) :
        _message(message)
    {

    }
    
    const char* InvalidXMLGridError::what() const noexcept
    {
        return _message.c_str();
    }
}