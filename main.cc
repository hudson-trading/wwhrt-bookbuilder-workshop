#include <limits>
#include <iostream>
#include <string_view>
#include <chrono>
#include <iomanip>

#include "BookBuilder.h"

#include "Reader.h"
#include "Measuring.h"
#include "Printing.h"
#include "CommandLine.h"

// The main function for our book builder program. Type ./bookbuilder --help
// to see options. Please see BookBuilder.{h,cc} and Allocator.h for
// class implementations to be modifed in this workshop.
int main(int argc, char** argv) {
    wwhrt::CommandLine args;
    args.parse(argc, argv);

    wwhrt::Reader reader{args.path.data()};

    // BookBuilder is the code that YOU will be modifying.
    wwhrt::BookBuilder bb;
    reader.addSubscriber(&bb);

    // These classes are for verifying your implementation. Do not modify.
    wwhrt::EventPrinter eventPrinter;
    wwhrt::BookPrinter bookPrinter(&bb);
    wwhrt::BookMeasurer bookMeasurer(&bb);
    if (args.check) {
        reader.addSubscriber(&eventPrinter);
        reader.addSubscriber(&bookPrinter);
    } else {
        reader.addSubscriber(&bookMeasurer);
    }

    if (args.measure) {
        auto before = std::chrono::high_resolution_clock::now();
        reader.run();
        auto after = std::chrono::high_resolution_clock::now();
        auto diff = after - before;
        std::cout << "Time: " << std::fixed << std::setprecision(9)
                  << std::chrono::nanoseconds(diff).count() / 1.0e9
                  << std::endl;
    } else {
        reader.run();
    }
    return 0;
}
