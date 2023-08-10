#ifndef VALUE_H_
#define VALUE_H_

#include <functional>
#include <list>

namespace mist
{

class Owner;

class ValueBase
{
public:
    virtual ~ValueBase() = default;
    virtual auto stopWatching(const Owner *owner) -> void = 0;
};

class Detachable
{
public:
    virtual ~Detachable() = default;
    virtual auto detach() -> void = 0;
    virtual auto getValue() -> ValueBase * = 0;
};

class Owner
{
public:
    ~Owner()
    {
        std::list<Detachable *> copy {watchers};
        for (auto i : copy) {
            auto v = i->getValue();
            if (v) v->stopWatching(this);
        }
    }

    auto add(Detachable *d) { watchers.emplace_back(d); }

    auto remove(Detachable *d) { std::erase(watchers, d); }

private:
    std::list<Detachable *> watchers;
};

template <typename T> class Watcher : public Detachable
{
public:
    using Callback = std::function<void(const T &)>;

    Watcher(Owner *owner_, const Callback &callback_) : owner(owner_), callback(callback_) {};

    auto operator==(const Owner *o) const -> bool { return owner == o; }

    auto attachTo(ValueBase *v) -> void
    {
        watchedValue = v;
        owner->add(this);
    }

    auto detach() -> void override
    {
        watchedValue = nullptr;
        owner->remove(this);
    }

    virtual auto getValue() -> ValueBase * override { return watchedValue; }

    auto notify(const T &v) const -> void { callback(v); }

private:
    Owner                         *owner;
    std::function<void(const T &)> callback;
    ValueBase                     *watchedValue = nullptr;
};

template <typename T> class Value : public ValueBase
{
public:
    using Callback = std::function<void(const T &)>;

    Value() = default;
    Value(const Value &other) = default;
    Value &operator=(const Value &other) = default;
    Value(Value &&other) = default;
    Value &operator=(Value &&other) = default;

    explicit Value(const T &value_) noexcept : value(value_) {}

    virtual ~Value()
    {
        for (auto &i : watchers)
            i.detach();
    }

    [[nodiscard]] auto get() const noexcept -> T { return value; }
                       operator T() const noexcept { return value; }

    auto operator=(const T &v) -> Value &
    {
        value = v;
        notifyAll();
        return *this;
    }

    auto watch(Owner *owner, const Callback &c) -> void
    {
        watchers.emplace_back(Watcher<T>(owner, c));
        watchers.back().attachTo(this);
        c(value);
    }

    auto stopWatching(const Owner *owner) -> void override
    {
        for (auto &i : watchers) {
            if (i == owner) i.detach();
        }
        std::erase(watchers, owner);
    }

private:
    T                     value;
    std::list<Watcher<T>> watchers;

    auto notifyAll() -> void
    {
        for (const auto &i : watchers) {
            i.notify(value);
        }
    }
};

} // namespace mist

#endif
