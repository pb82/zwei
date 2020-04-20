#ifndef PARSER_H
#define PARSER_H

#include <stack>
#include <ctype.h>
#include <stdint.h>
#include <exception>

#include "utf8.h"
#include "value.h"

namespace JSON {
    // JSON escape characters that may appear
    // after a reverse solidus
    const char ESC_REVERSE_SOLIUDS  = 92;
    const char ESC_QUOTATION_MARK   = 34;
    const char ESC_SOLIDUS          = 47;
    const char ESC_BACKSPACE        = 98;
    const char ESC_FORMFEED         = 102;
    const char ESC_NEWLINE          = 110;
    const char ESC_CARRET           = 114;
    const char ESC_HORIZONTAL_TAB   = 116;
    const char ESC_UNICODE          = 117;
    
    // Use 32 bit characters for unicode strings
    typedef std::basic_string<int32_t> wideString;

	// Used to display parse errors with the line they appeared in
	class ParseError : public std::runtime_error {
	public:
		ParseError(int line, int index);
	};

    // Represents a JSON parser
    class Parser {
        public:
			Parser();

            void parse(Value& object, const std::string& source);
            void parse(Value& object, const char * source);

        private:
			void reset();
        
            // Increment the parse index until a non-whitespace character
            // is encountered.
			void clearWhitespace();

            // Increase the parse index but ignore the current character
			void consume();

            // End of Stream reached?
			bool hasNext() const;
                
            // Return a reference to the current character in the
            // stream and increase the index.
			char next();

            // Return a reference to the current character in the
            // stream without increasing the index.
			char peek();

            // Return a reference to the top item on the stack.
            // The top item usually holds a reference to the 
            // current array / object at the parse position.
			Value& top() const;

            // Store a parsed value and return a reference
            // to it
			Value& store(const Value& val);

            // Tests if a character is allowed for numeric expressions.
			bool validNumericChar(char code) const;

            // Tests if a character is allowed to introduce a numeric
            // expression.
			bool validNumericStartingChar(char code) const;

            // Valid digit within an \u2360 unicode escape?
			bool validHexDigit(char code) const;

            void parseObject();
            void parseProperty();
            void parseValue();
            void parseString();
            void parseArray();
            void parseBoolean();
            void parseNumber();
            void parseNull();
            void escapeChar();
			void readUTF8Escape();
                        
            unsigned int parseIndex;
            unsigned int lineNumber;

            std::string source;
            std::ostringstream currentProperty;
            std::ostringstream currentString;
            
            // Since std::stack<Value&> is not possible use
            // a pointer here
            std::stack<Value *> objectStack;
    };
} // End namespace JSON

#endif // PARSER_H
