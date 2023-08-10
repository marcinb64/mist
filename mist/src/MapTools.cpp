#include "MapTools.h"

#include <random>

using namespace mist;

DiamondSquare::DiamondSquare(Matrix<double> &output_) : output(output_), size(output.getXSize())
{
}

auto DiamondSquare::setSeed(long seed_) -> DiamondSquare &
{
    seed = seed_;
    return *this;
}

auto DiamondSquare::setRoughness(double roughness_) -> DiamondSquare &
{
    roughness = roughness_;
    return *this;
}

auto DiamondSquare::setInitialRandomness(double initRand_) -> DiamondSquare &
{
    initialRandomness = initRand_;
    return *this;
}

auto DiamondSquare::build() -> void
{
    static std::default_random_engine             rng;
    static std::uniform_real_distribution<double> randomize {-1.0, 1.0};
    rng.seed(seed);

    const auto N = size - 1;

    // Init corners
    output.at({0, 0}) = randomize(rng);
    output.at({0, N}) = randomize(rng);
    output.at({N, 0}) = randomize(rng);
    output.at({N, N}) = randomize(rng);

    int        stepSize = N;
    auto       noise = initialRandomness;
    const auto noiseMult = pow(2, -roughness); // each step decreases randomness by this factor
    do {
        // Diamond step
        for (int y = stepSize / 2; y < size; y += stepSize) {
            for (int x = stepSize / 2; x < size; x += stepSize) {
                const auto total = diamond({x, y}, stepSize / 2) + randomize(rng) * noise;
                output.at({x, y}) = std::clamp(total, -1.0, 1.0);
            }
        }

        // Square step - even rows
        for (int y = 0; y < size; y += stepSize) {
            for (int x = stepSize / 2; x <= N; x += stepSize) {
                const auto total = square({x, y}, stepSize / 2) + randomize(rng) * noise;
                output.at({x, y}) = std::clamp(total, -1.0, 1.0);
            }
        }
        // Square step - odd rows
        for (int y = stepSize / 2; y < N; y += stepSize) {
            for (int x = 0; x <= N; x += stepSize) {
                const auto total = square({x, y}, stepSize / 2) + randomize(rng) * noise;
                output.at({x, y}) = std::clamp(total, -1.0, 1.0);
            }
        }

        noise *= noiseMult;
        stepSize /= 2;

    } while (stepSize > 1);
}

auto DiamondSquare::diamond(const Point2i &p, int a) -> double
{
    return (output.at({p.x - a, p.y - a}) + output.at({p.x + a, p.y - a}) +
            output.at({p.x - a, p.y + a}) + output.at({p.x + a, p.y + a})) /
           4;
}

auto DiamondSquare::square(const Point2i &p, int a) -> double
{
    double  sum = 0;
    int     N = 0;
    Point2i src;

    auto includePoint = [&](const Point2i &x) {
        sum += output.at(x);
        ++N;
    };

    src = {p.x - a, p.y};
    if (src.x >= 0) includePoint(src);
    src = {p.x + a, p.y};
    if (src.x < size) includePoint(src);
    src = {p.x, p.y - a};
    if (src.y >= 0) includePoint(src);
    src = {p.x, p.y + a};
    if (src.y < size) includePoint(src);

    return sum / N;
}
