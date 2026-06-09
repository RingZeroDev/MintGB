#pragma once

template<auto Dst, auto Src>
void ld() {
    auto value = get<Src>();
    set<Dst>(value);
}

template<auto Addr>
void jp() {
    pc = get<Addr>();
}

template<Condition Cond, auto Addr>
void jp() {
    if (test<Cond>()) {
        pc = get<Addr>();
    }
}



