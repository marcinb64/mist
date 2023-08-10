#include "Point.h"

#include "Noise.h"
#include <chrono>
#include <iostream>

using namespace mist;

auto timeNoise() -> long
{
    using T = double;

    PerlinNoise2 perlin;
    OctaveNoise2 octaves(perlin);
    octaves.setNumOctaves(2);

    static constexpr auto N = 2048;
    T                     out = 0;

    auto t0 = std::chrono::system_clock::now();

    for (int i = 0; i < N * N; ++i)
        out += octaves.sample({0.4, 1.3});

    auto t1 = std::chrono::system_clock::now();
    auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
    std::cout << "Time: " << dt.count() << "ms\n";

    return dt.count();
}

int main()
{
    try {
        constexpr auto N = 5;

        long total = 0;
        for (int i = 0; i < 5; ++i)
            total += timeNoise();

        std::cout << "\n *** Avg: " << (total / N) << "ms\n";
    }

    catch (std::exception &e) {
        std::cerr << "Fatal: " << e.what() << "\n";
    }

    return 0;
}
