#include "JSON/parser.h"

namespace JSON {

    ParseError::ParseError(int line, int index)
            : std::runtime_error("") {
        std::stringstream ss;
        ss << "JSON/parse: syntax error at " << line << "/" << index;
        static_cast<std::runtime_error &>(*this) =
                std::runtime_error(ss.str());
    }


    Parser::Parser() : parseIndex(0), lineNumber(1) {}

    void Parser::reset() {
        lineNumber = 1;
        parseIndex = 0;
        while (!objectStack.empty()) {
            objectStack.pop();
        }
    }

    void Parser::clearWhitespace() {
        while (hasNext() && isspace(source.at(parseIndex))) {
            if (peek() == 10 || peek() == 12 || peek() == 13) {
                lineNumber++;
            }
            parseIndex++;
        }
    }

    void Parser::consume() {
        parseIndex++;
    }

    bool Parser::hasNext() const {
        return parseIndex < source.length();
    }

    char Parser::next() {
        if (!hasNext()) {
            // End of Stream already reached?
            throw ParseError(lineNumber, parseIndex);
        }
        // Increase the line number. This is used to print
        // the position when a parse error occurs.
        if (peek() == '\n') {
            lineNumber++;
        }

        return source.at(parseIndex++);
    }

    char Parser::peek() {
        if (!hasNext()) {
            // End of Stream already reached?
            throw ParseError(lineNumber, parseIndex);
        }
        return source.at(parseIndex);
    }

    Value &Parser::top() const {
        return *objectStack.top();
    }

    Value &Parser::store(const Value &val) {
        if (top().is(JSON_ARRAY)) {
            // Current parse position is inside an array:
            // Append the new item to the end of the array
            top().push_back(val);
            return top().asMutable<Array>().back();
        } else if (top().is(JSON_OBJECT)) {
            // Current parse position is inside an object:
            // Read the current property and store the new item
            // under this property inside the current object.
            top()[currentProperty.str().c_str()] = val;
            return top()[currentProperty.str().c_str()];
        } else {
            // Parse position is not inside an array and not
            // inside an object. Put the new item on the top
            // of the stack.
            top() = val;
            return top();
        }
    }

    bool Parser::validNumericChar(char code) const {
        return (code >= 48 && code <= 57)
               || (code == 46)     // '.'
               || (code == 45)     // '-'
               || (code == 43)     // '+'
               || (code == 69)     // 'E'
               || (code == 101);   // 'e'
    }

    bool Parser::validNumericStartingChar(char code) const {
        return (code >= 48 && code <= 57)
               || (code == 45);    // '-'
    }

    bool Parser::validHexDigit(char code) const {
        return (code >= 48 && code <= 57)
               || (code >= 65 && code <= 70)
               || (code >= 97 && code <= 102);
    }

/**
 * { ... }
 */
    void Parser::parseObject() {
        clearWhitespace();
        if (next() == '{') {
            clearWhitespace();

            // Push a new object on the stack
            objectStack.push(&store(Object{}));

            if (peek() != '}') {
                // Parse object properties
                while (hasNext()) {
                    parseProperty();
                    clearWhitespace();

                    if (hasNext() && peek() != ',') {
                        break;
                    } else {
                        // Another property to parse. Get rid
                        // of the ',' and go on...
                        consume(); // ','
                    }
                }
            }

            // End of properties
            clearWhitespace();

            if (peek() == '}') {
                consume(); // '}'
                // Done with parsing the object.
                // Pop it from the stack, since this is no longer
                // the reference object for eventual coming items.
                objectStack.pop();
                return;
            } else {
                // Objects must end with ...}
                throw ParseError(lineNumber, parseIndex);
            }

        } else {
            // Objects have to start with {...
            throw ParseError(lineNumber, parseIndex);
        }
    }

/**
 * ...:
 */
    void Parser::parseProperty() {
        // Reset currentProperty buffer
        currentProperty.str("");
        clearWhitespace();

        // Properties must start with '"'
        if (peek() == ESC_QUOTATION_MARK) {
            consume(); // '"'
            while (peek() != ESC_QUOTATION_MARK) {
                currentProperty << next();
            }

            // Properties must end with '"'
            if (next() != ESC_QUOTATION_MARK) {
                throw ParseError(lineNumber, parseIndex);
            } else {
                clearWhitespace();
                // ...": ...
                // Properties must be in the form of
                // "key": value
                if (peek() != ':') {
                    throw ParseError(lineNumber, parseIndex);
                } else {
                    consume(); // ':'
                    // Parse the value
                    // :...
                    parseValue();
                }
            }
        } else {
            throw ParseError(lineNumber, parseIndex);
        }
    }

/**
 * null
 */
    void Parser::parseNull() {
        currentString.str("");

        // Read the next four characters and test if they
        // are equal to 'null'
        for (int i = 0; i < 4; i++) {
            currentString << next();
        }

        if (currentString.str().compare("null") == 0) {
            store(Value());
        } else {
            throw ParseError(lineNumber, parseIndex);
        }
    }

/**
 * numbers
 */
    void Parser::parseNumber() {
        currentString.str("");
        while (hasNext() && validNumericChar(peek())) {
            currentString << next();
        }

        store(fromString<double>(currentString.str()));
    }

/**
 * true | false
 */
    void Parser::parseBoolean() {
        currentString.str("");
        // consume lowercase letters
        while (hasNext() && peek() >= 97 && peek() <= 122) {
            currentString << next();
        }

        bool result;
        if (currentString.str().compare("true") == 0) {
            result = true;
        } else if (currentString.str().compare("false") == 0) {
            result = false;
        } else {
            throw ParseError(lineNumber, parseIndex);
        }

        store(result);
    }

/**
 * :...
 */
    void Parser::parseValue() {
        clearWhitespace();

        // Decide the type of the value on the stream
        switch (peek()) {
            case '{':
                parseObject();
                return;
            case '"':
                parseString();
                return;
            case '[':
                parseArray();
                return;
            case 'n':
                parseNull();
                return;
            case 't':
            case 'f':
                parseBoolean();
                return;
            default:
                // If none of the former types matched always try to
                // parse a number.
                if (validNumericStartingChar(peek())) {
                    parseNumber();
                    return;
                } else {
                    throw ParseError(lineNumber, parseIndex);
                }
        }
    }

/**
 * "..."
 */
    void Parser::parseString() {
        // Reset string buffer
        currentString.str("");
        consume(); // '"'
        while (peek() != ESC_QUOTATION_MARK) {
            // String contains an escaped character?
            if (peek() == ESC_REVERSE_SOLIUDS) {
                escapeChar();
            } else {
                currentString << next();
            }
        }

        consume(); // '"'
        store(currentString.str());
    }

/**
 * \...
 */
    void Parser::escapeChar() {
        consume(); // REVERSE_SOLIDUS
        // Decide which escape character follows
        switch (peek()) {
            case ESC_BACKSPACE:
                consume();
                currentString << (char) 8;
                break;
            case ESC_HORIZONTAL_TAB:
                consume();
                currentString << (char) 9;
                break;
            case ESC_NEWLINE:
                consume();
                currentString << (char) 10;
                break;
            case ESC_FORMFEED:
                consume();
                currentString << (char) 12;
                break;
            case ESC_CARRET:
                consume();
                currentString << (char) 13;
                break;
            case ESC_QUOTATION_MARK:
            case ESC_REVERSE_SOLIUDS:
            case ESC_SOLIDUS:
                currentString << next();
                break;
            case ESC_UNICODE:
                // a \u occured
                // read the following sequence and insert it's
                // unicode representation into the string.
                readUTF8Escape();
                break;
            default:
                throw ParseError(lineNumber, parseIndex);
        }
    }

    void Parser::readUTF8Escape() {
        consume(); // u
        int codePoint = -1;
        std::string tmp(4, 32);
        std::stringstream ss;
        std::string result;
        wideString str32;

        for (unsigned int index = 0; index < tmp.length(); index++) {
            if (!hasNext() || !validHexDigit(peek())) {
                throw ParseError(lineNumber, parseIndex);
            }

            tmp[index] = next();
        }

        ss << std::hex << tmp;
        ss >> codePoint;
        str32.push_back(codePoint);
        utf8::utf32to8(str32.begin(), str32.end(), std::back_inserter(result));
        currentString << result;
    }

/**
 * [...]
 */
    void Parser::parseArray() {
        consume(); // '['

        objectStack.push(&store(Array{}));
        clearWhitespace();

        // Empty array?
        if (peek() == ']') {
            consume(); // ']'
            objectStack.pop();
            return;
        }

        while (hasNext()) {
            parseValue();
            clearWhitespace();

            if (peek() != ',') {
                break;
            } else {
                consume(); // ','
            }
        }

        if (peek() != ']') {
            throw ParseError(lineNumber, parseIndex);
        } else {
            consume(); // ']'
        }

        objectStack.pop();
    }

    void Parser::parse(Value &value, const std::string &source) {
        reset();
        if (source.length() > 0) {
            value = nullptr;
            this->source = source;
            objectStack.push(&value);
            parseValue();
            clearWhitespace();
            if (parseIndex < source.length()) {
                throw ParseError(lineNumber, parseIndex);
            }
        }
    }

    void Parser::parse(Value &value, const char *source) {
        std::string _source(source);
        parse(value, _source);
    }

} // End namespace JSON
