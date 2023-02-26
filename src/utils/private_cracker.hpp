#pragma once

namespace utils{

template<size_t N>
struct reader {
    friend auto counted_flag(reader<N>);
};


template<size_t N>
struct setter {
    friend auto counted_flag(reader<N>) {}

    static constexpr size_t n = N;
};

template<
        auto Tag = []{},
        size_t NextVal = 0
>
[[nodiscard]]
consteval auto next_impl() {
    constexpr bool counted_past_value = requires(reader<NextVal> r) {
        counted_flag(r);
    };

    if constexpr (counted_past_value) {
        return next_impl<Tag, NextVal + 1>();
    }
    else {
        setter<NextVal> s;
        return s.n;
    }
}

template<
        auto Tag = []{},
        size_t NextVal = 0
>
[[nodiscard]]
consteval auto cur_impl() {
    constexpr bool counted_past_value = requires(reader<NextVal> r) {
        counted_flag(r);
    };

    if constexpr (counted_past_value) {
        return cur_impl<Tag, NextVal + 1>();
    }
    else if constexpr (NextVal > 0){
        return NextVal - 1;
    }
    else {
        []<bool flag = false>(){ static_assert(flag, "Counter not exist"); }();
    }
}

template<auto Tag = [](){}>
constexpr auto IncCounter(){
    return next_impl<Tag>();
}

template<auto Tag = [](){}>
constexpr auto CurCounter(){
    return cur_impl<Tag>();
}

template <typename T, typename T::type member_pointer>
struct DefinitionInserter {
    friend constexpr typename T::type impl_GetPrivateMember(T) {
        return member_pointer;
    }
};

template<typename Keys, Keys key, typename Method>
struct ADLKey {
    using type = Method;
    friend constexpr type impl_GetPrivateMember(ADLKey);
};


#define CreateGetter(Class, member, Member, name) \
template struct ::utils::DefinitionInserter<::utils::ADLKey<size_t, ::utils::IncCounter(), Member>, &Class::member>; \
decltype(auto) crack_get##_##name(){ \
    static constexpr auto num = ::utils::CurCounter(); \
    return impl_GetPrivateMember(::utils::ADLKey<size_t, num, Member>{}); \
}                                                 \


#define CreateInvoker(Class, member, Member, name) \
CreateGetter(Class, member, Member, name)\
template <class Obj, typename... Args> \
decltype(auto) crack_use##_##name(Obj&& obj, Args... args){ \
    static constexpr auto num = ::utils::CurCounter(); \
    auto ptr = crack_get##_##name(); \
    static constexpr auto len = sizeof...(args); \
    if constexpr (len > 0){ \
        (obj.*ptr)(std::forward<Args>(args)...); \
    } \
    else return  (obj.*ptr)(); \
}

}