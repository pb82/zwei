#ifndef PRINTER_H
#define PRINTER_H

#include <sstream>
#include "value.h"

namespace JSON {
    static unsigned int default_indent = 4;

    class Printer {
    public:
        void print(const Value val, std::ostringstream &out);

        std::string print(const Value val);

        virtual ~Printer() {}

    protected:
        void dispatchType(const Value &val, std::ostringstream &out);

        void printNumber(const Value &val, std::ostringstream &out);

        void printBoolean(const Value &val, std::ostringstream &out);

        void printString(const Value &val, std::ostringstream &out);

        void printBinary(const Value &val, std::ostringstream &out);

        /**
          * Allow to overwrite the formatting of Object and
          * Array for Pretty Printing.
          */
        virtual void printObject(const Value &val, std::ostringstream &out);

        virtual void printArray(const Value &val, std::ostringstream &out);
    };

    class PrettyPrinter : public Printer {
    public:
        PrettyPrinter(unsigned int indent = default_indent)
                : indentDepth(indent),
                  currentIndent(0) {}

    private:
        void printArray(const Value &value, std::ostringstream &out);

        void printObject(const Value &value, std::ostringstream &out);

        void printIndent(std::ostringstream &out);

        unsigned int indentDepth;
        unsigned int currentIndent;
    };
}

#endif // PRINTER_H
