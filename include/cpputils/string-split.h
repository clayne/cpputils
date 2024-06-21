#pragma once
#include <utility>
#include <iterator>
#include <algorithm>
#include <string_view>

/*
 *   S  is a std::string, or std::string_view
 */
template<typename S>
struct stringsplitter {
    S _str;
    S _sep;

/*
    // separator, which splits according to items in, and not in the set.
    struct set {
        const S& sep;

        set(const S& sep)
            : sep(sep)
        {
        }
        auto findnext(S::const_iterator first, S::const_iterator last)
        {
        }
        auto findend(S::const_iterator first, S::const_iterator last)
        {
        }

    };
    // separator, which splits according to substrings.
    struct string {
        const S& sep;

        string(const S& sep)
            : sep(sep)
        {
        }
        auto findnext(S::const_iterator first, S::const_iterator last)
        {
        }
        auto findend(S::const_iterator first, S::const_iterator last)
        {
        }


    };
*/
    struct iter {
        typename S::const_iterator p;
        typename S::const_iterator q;
        typename S::const_iterator last;
        const S& sep;

        // TODO: change interface to 'sep' such that 'sep' decides whether to do
        // a find_first_of or a search.
        // and to treat sequences of separators as one, or as separating empty elements.
        iter(typename S::const_iterator first, typename S::const_iterator last, const S& sep)
            : p(first), q(first), last(last), sep(sep)
        {
        }
        // updates 'q', to point to the first 'sep'
        S operator*()
        {
            //q = std::find_first_of(p, last, std::begin(sep), std::end(sep));
            q = std::find_if(p, last, [this](auto c) { return std::find(std::begin(sep), std::end(sep), c) != std::end(sep); });

            //q = std::search(p, last, std::begin(sep), std::end(sep));

            // workaround for lack of iterator variant of stringview
            if constexpr (std::is_same_v<S, std::string_view>) {
                return S(&*p, q-p);
            }
            else {
                return S(p, q);
            }
        }
        // updates 'p', to point to the first 'sep'
        iter& operator++()
        {
            p = std::find_if(q, last, [this](auto c) { return std::find(std::begin(sep), std::end(sep), c) == std::end(sep); });
            //p = q + std::size(ssep)
            return *this;
        }
        friend bool operator!=(const iter& lhs, const iter& rhs)
        {
            return lhs.p!=rhs.p || lhs.last!=rhs.last;
        }
    };

    // TODO: support two different string types
    stringsplitter(const S& str, const S& sep)
        : _str(str), _sep(sep)
    {
    }
    iter begin() const
    {
        return iter(std::begin(_str), std::end(_str), _sep);
    }
    iter end() const
    {
        return iter(std::end(_str), std::end(_str), _sep);
    }
};
