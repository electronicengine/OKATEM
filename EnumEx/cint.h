#ifndef CINT_H
#define CINT_H

template<auto u>
struct cint
{
    using U = decltype(u);
    static constexpr auto cint_value = u;

    template<typename T>
    operator T() { return static_cast<T>(cint_value); }
};

#endif // CINT_H
