#include <iostream>

namespace bfprt {

    template<class Iter>
    using DerefType = decltype(*std::declval<Iter>());

    template<class Iter, class Comp = std::less<DerefType<Iter>>>
    inline Iter inplace_partition(Iter begin, Iter end, Iter pivot, Comp comp = {}) {
        auto i = begin, j = std::prev(end);
        std::swap(*pivot, *i);
        pivot = i++;
        while (i < j) {
            while (comp(*i, *pivot)) i++;
            while (!comp(*j, *pivot)) j--;
            if (i < j) std::swap(*i, *j);
        }
        while (comp(*pivot, *i)) { i--; }
        std::swap(*pivot, *i);
        return i;
    }

    template<class Iter, class Comp = std::less<DerefType<Iter>>>
    inline void insertion_sort(Iter begin, Iter end, Comp comp = {}) {
        for (auto i = begin + 1; i < end; ++i) {
            for (auto j = begin; j < i; ++j) {
                if (!comp(*j, *i)) {
                    auto t = std::move(*i);
                    std::move(j, i, j + 1);
                    *j = std::move(t);
                    break;
                }
            }
        }
    }

    static inline constexpr size_t GROUP_SIZE = 5;

    template<class Iter, class Comp = std::less<DerefType<Iter>>>
    inline Iter kth_element(Iter begin, Iter end, size_t index, Comp comp = {});

    template<class Iter, class Comp = std::less<DerefType<Iter>>>
    inline Iter median_of_median(Iter begin, Iter end, Comp comp = {}) {
        if (end - begin <= GROUP_SIZE) {
            insertion_sort(begin, end, comp);
            return begin + (end - begin) / 2;
        }
        Iter cursor = begin;
        for (auto i = begin; i < end;) {
            auto range = std::min<size_t>(GROUP_SIZE, end - i);
            insertion_sort(i, i + range, comp);
            std::swap(*cursor++, *(i + range / 2));
            i += range;
        }
        // now, from [begin, cursor)
        return kth_element(begin, cursor, (cursor - begin) / 2, comp);
    }

    template<class Iter, class Comp>
    inline Iter kth_element(Iter begin, Iter end, size_t index, Comp comp) {
        if (end - begin <= GROUP_SIZE) {
            insertion_sort(begin, end, comp);
            return begin + index;
        }
        auto med_of_med = median_of_median(begin, end, comp);
        auto iter = inplace_partition(begin, end, med_of_med, comp);
        if (iter - begin == index) {
            return iter;
        } else if (iter - begin > index) {
            return kth_element(begin, iter, index, comp);
        } else {
            return kth_element(iter + 1, end, index - (iter - begin) - 1, comp);
        }
    }
}

int main() {
    std::string a = "99175426300";
    auto x = bfprt::kth_element(a.begin(), a.end(), 7);
    std::cout << *x << std::endl;
    return 0;
}
