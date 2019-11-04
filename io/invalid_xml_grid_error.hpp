#ifndef IO__INVALID_XML_GRID_ERROR
#define IO__INVALID_XML_GRID_ERROR

#include <stdexcept>
#include <string>

namespace Picross
{
    class InvalidXMLGridError : public std::exception
    {
        private:    // Attributes
            std::string _message;

        public:     // Public methods
            InvalidXMLGridError(const char* message);
            InvalidXMLGridError(std::string message);
            const char* what() const noexcept;
    };
}

#endif//IO__INVALID_XML_GRID_ERROR