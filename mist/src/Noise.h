#ifndef NOISE2_H_
#define NOISE2_H_

#include "Matrix.h"
#include "Point.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <random>

namespace mist
{

auto setPerlinSeed(long seed) -> void;
auto perlinHash(int value) -> int;

class Noise2
{
public:
    virtual ~Noise2() = default;
    virtual auto sample(const Point2d &) -> double = 0;
};

class PerlinNoise2 : public Noise2
{
public:
    static auto setSeed(long seed) -> void { setPerlinSeed(seed); }

    PerlinNoise2(double zOffset_ = 0) : zOffset(zOffset_) {}

    double sample(const Point2d &p) override
    {
        double x = p.x;
        double y = p.y;
        double z = zOffset;

        // Find unit curve that contains the point
        const int X = static_cast<int>(std::floor(x)) & 255;
        const int Y = static_cast<int>(std::floor(y)) & 255;
        const int Z = static_cast<int>(std::floor(z)) & 255;
        // Find relative x,y,z of the point in the unit cube
        x -= std::floor(x);
        y -= std::floor(y);
        z -= std::floor(z);
        // Compute fade curves for each of x,y,z
        const double u = fade(x);
        const double v = fade(y);
        const double w = fade(z);

        // Hash coordinates of cube corners
        const int A = perlinHash(X) + Y;
        const int AA = perlinHash(A) + Z;
        const int AB = perlinHash(A + 1) + Z;
        const int B = perlinHash(X + 1) + Y;
        const int BA = perlinHash(B) + Z;
        const int BB = perlinHash(B + 1) + Z;

        // Add blended results from 8 corners of the cube
        // clang-format off
        return lerp(
            w,
            lerp(v,
                lerp(u, grad(perlinHash(AA), x, y, z), grad(perlinHash(BA), x - 1, y, z)),
                lerp(u, grad(perlinHash(AB), x, y - 1, z), grad(perlinHash(BB), x - 1, y - 1, z))
                ), 
            lerp(v,
                lerp(u, grad(perlinHash(AA + 1), x, y, z - 1), grad(perlinHash(BA + 1), x - 1, y, z - 1)),
                lerp(u, grad(perlinHash(AB + 1), x, y - 1, z - 1), grad(perlinHash(BB + 1), x - 1, y - 1, z - 1))
                )
        );
        // clang-format on
    }

private:
    double zOffset;

    static double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }

    static double lerp(double t, double a, double b) { return a + t * (b - a); }

    static double grad(int perlinHash, double x, double y, double z)
    {
        const int  h = perlinHash & 15;
        const auto u = h < 8 ? x : y;
        const auto v = h < 4 ? y : h == 12 || h == 14 ? x : z;
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }
};

/* -------------------------------------------------------------------------- */

class OctaveNoise2 : public Noise2
{

public:
    OctaveNoise2(Noise2 &noise_) : noise(noise_) {}

    auto setNumOctaves(int numOctaves_) -> OctaveNoise2 &
    {
        numOctaves = numOctaves_;
        return *this;
    }

    auto setRoughness(double roughness_) -> OctaveNoise2 &
    {
        roughness = roughness_;
        return *this;
    }

    auto setFrequencyMultiplier(double frequencyMultiplier_) -> OctaveNoise2 &
    {
        frequencyMultiplier = frequencyMultiplier_;
        return *this;
    }

    auto sample(const Point2<double> &p) -> double override
    {
        const double G = std::pow(static_cast<double>(2), -roughness);
        double       total = 0;
        double       freq = 1.0;
        double       amplitude = 1.0;
        for (auto i = 0; i < numOctaves; ++i) {
            total += amplitude * noise.sample({p.x * freq, p.y * freq});
            freq *= frequencyMultiplier;
            amplitude *= G;
        }
        return total;
    }

private:
    Noise2 &noise;
    int     numOctaves {5};
    double  roughness {0.75};
    double  frequencyMultiplier {2};
};

/* -------------------------------------------------------------------------- */

class DomainWarpedNoise2 : public Noise2
{
public:
    DomainWarpedNoise2(Noise2 &noise_) : noise(noise_) {}

    auto sample(const Point2d &p) -> double override
    {
        const Point2d q {noise.sample(p), noise.sample(p + offset)};

        return noise.sample(q);
    }

    auto setOffset(const Point2d &offset_) -> DomainWarpedNoise2 &
    {
        offset = offset_;
        return *this;
    }

private:
    Noise2 &noise;
    Point2d offset {3.2, 1.3};
};

/* -------------------------------------------------------------------------- */

template <typename T = double> class NoiseTextureBuilder2
{
public:
    NoiseTextureBuilder2(Matrix<T> &texture_, Noise2 &noise_) : texture(texture_), noise(noise_) {}

    NoiseTextureBuilder2 &setNoiseScale(double scale)
    {
        noiseScale = scale;
        return *this;
    }

    NoiseTextureBuilder2 &setXScale(double scale)
    {
        xScale = scale;
        return *this;
    }

    NoiseTextureBuilder2 &setYScale(double scale)
    {
        yScale = scale;
        return *this;
    }

    auto build()
    {
        texture.generate([&](const Point2i &p) {
            Point2d pRel {static_cast<double>(p.x) / static_cast<double>(texture.getXSize()),
                          static_cast<double>(p.y) / static_cast<double>(texture.getYSize())};
            pRel.x *= xScale;
            pRel.y *= yScale;
            return std::clamp(static_cast<T>(noise.sample(pRel) * noiseScale), static_cast<T>(-1.0),
                              static_cast<T>(1.0));
        });
    }

private:
    Matrix<T> &texture;
    Noise2    &noise;

    double noiseScale {1};
    double xScale {1};
    double yScale {1};
};

} // namespace mist

#endif