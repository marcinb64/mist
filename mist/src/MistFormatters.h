#ifndef MISTFORMATTERS_H_
#define MISTFORMATTERS_H_

#include <spdlog/spdlog.h>

template <> struct fmt::formatter<mist::Point2i> {
    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin()) { return ctx.end(); }

    template <typename FormatContext>
    auto format(const mist::Point2i &c, FormatContext &ctx) -> decltype(ctx.out())
    {
        return format_to(ctx.out(), "({}, {})", c.x, c.y);
    }
};

template <> struct fmt::formatter<mist::Point2f> {
    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin()) { return ctx.end(); }

    template <typename FormatContext>
    auto format(const mist::Point2f &c, FormatContext &ctx) -> decltype(ctx.out())
    {
        return format_to(ctx.out(), "({:.3f}, {:.3f})", c.x, c.y);
    }
};

template <> struct fmt::formatter<mist::Point2<double>> {
    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin()) { return ctx.end(); }

    template <typename FormatContext>
    auto format(const mist::Point2<double> &c, FormatContext &ctx) -> decltype(ctx.out())
    {
        return format_to(ctx.out(), "({:.3f}, {:.3f})", c.x, c.y);
    }
};

#endif
