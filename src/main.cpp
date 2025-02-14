#include "Types.hpp"
#include "Position.hpp"
#include "NNUE.hpp"
#include "Tests.hpp"
#include "Zobrist.hpp"
#include "Search.hpp"
#include "UCI.hpp"
#include "Thread.hpp"
#include "syzygy/syzygy.hpp"
#include <chrono>
#include <sstream>
#include <memory>

int main(int argc, char** argv)
{
    Bitboards::init_bitboards();
    Zobrist::build_rnd_hashes();
    NNUE::init();
    Syzygy::init();
    UCI::init();

    // Handle passed arguments
    std::stringstream ss;
    if (argc > 1)
        for (int i = 1; i < argc; i++)
            ss << argv[i] << ' ';

    UCI::main_loop(ss.str());

    NNUE::clean();
}
