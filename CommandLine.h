#pragma once

namespace wwhrt {

// This file should not be modified.

constexpr const char* helpMessage =
        "bookbuilder [options] <data>\n"
        "\n"
        "Options:\n"
        "    --check         Print the state of the book after every event\n"
        "    --measure       Print the time to process the entire file\n"
        "    --breakpoint N  The event number to break on when debugging in "
        "gdb";

struct CommandLine {
    bool measure = false;
    bool check = false;
    std::string path;

    void parse(int argc, char** argv) {
        for (int i = 1; i < argc; i++) {
            std::string_view arg = argv[i];
            if (arg == "--help" || arg == "-h") {
                std::cerr << helpMessage << std::endl;
                std::exit(0);
            } else if (arg == "--measure") {
                measure = true;
            } else if (arg == "--check") {
                check = true;
            } else if (arg == "--breakpoint") {
                i++;
                if (i >= argc) {
                    std::cerr << "You must pass an argument to --breakpoint"
                              << std::endl;
                    std::exit(1);
                }
                std::string_view s = argv[i];
                uint32_t seqNum = 0;
                try {
                    seqNum = std::stoul(std::string(s));
                } catch (std::exception& e) {
                    std::cerr << s << " is not a valid number" << std::endl;
                    std::exit(1);
                }
                Reader::breakOnSeqNum = seqNum;
            } else {
                path = arg;
            }
        }
        if (path.empty()) {
            std::cerr << "You must specify a path of event data" << std::endl;
            std::exit(1);
        }
    }
};

} // namespace wwhrt
