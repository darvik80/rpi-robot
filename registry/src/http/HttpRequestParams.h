//
// Created by Ivan Kishchenko on 23/04/2023.
//

#pragma once

#include <vector>
#include <string>
#include <list>
#include <functional>
#include <iostream>
#include <iterator>
#include <cstddef>
#include <optional>
#include <memory>
#include <variant>

#include <boost/url.hpp>
#include <nlohmann/json.hpp>

class Filter {
public:
    enum class Operation {
        op_unknown,
        op_like,
        op_nlike,
        op_in,
        op_eq,
        op_gt,
        op_gte,
        op_lt,
        op_lte,
        op_me,
        op_nin,
        op_null
    };

    enum class Type {
        type_string,
        type_double,
        type_long,
        type_vector_string,
        type_vector_number,
    };

    struct Field {
        std::string name;
        Operation operation{Operation::op_unknown};
        Type type;
        std::variant<std::monostate, std::string, long, double, std::vector<std::string>, std::vector<double>> value;

        template<class T>
        static Field of(std::string_view name, Operation op, const T &val) {
            Field res{.name=name.data(), .operation = op};
            if constexpr (std::is_integral<T>::value) {
                res.type = Type::type_long;
                res.value = static_cast<long>(val);
            } else if constexpr (std::is_floating_point<T>::value) {
                res.type = Type::type_double;
                res.value = static_cast<double>(val);
            }

            return res;
        }

        template<std::size_t N>
        static Field of(std::string_view name, Operation op, const char (&str)[N]) {
            Field res{
                    .name=name.data(),
                    .operation = op,
                    .type = Type::type_string,
                    .value = std::string(str, N)
            };

            return res;
        }

        static Field of(std::string_view name, Operation op, const std::string &val) {
            Field res{
                    .name=name.data(),
                    .operation = op,
                    .type = Type::type_string,
                    .value = val
            };

            return res;
        }

        template<class T>
        static Field of(std::string_view name, Operation op, const std::vector<T> &val) {
            switch (op) {
                case Operation::op_nlike:
                    op = Operation::op_nin;
                    break;
                case Operation::op_like:
                case Operation::op_eq:
                default:
                    op = Operation::op_in;
                    break;
            }

            Field res{.name=name.data(), .operation = op};
            if constexpr (std::is_same<std::string, T>::value) {
                res.type = Type::type_vector_string;
                res.value = val;
            } else if constexpr (std::is_arithmetic_v<T>) {
                res.type = Type::type_vector_number;
                auto &value = res.value.emplace<std::vector<double>>();
                for (const auto &v: val) {
                    value.emplace_back(v);
                }
            }

            return res;
        }
    private:
        template<typename T>
        static std::string
        join(const std::vector<T> &v, const std::string &delim, const std::function<std::string(const T &)> &trans) {
            std::string ret;
            auto it = std::begin(v);
            if (it != std::end(v)) {
                ret = trans(v);
                while (++it != std::end(v)) ret += delim + trans(*it);
            }
            return ret;
        }
    };

    struct Iterator {
        using iterator_category = const std::forward_iterator_tag;
        using difference_type = const std::ptrdiff_t;
        using value_type = const Field;
        using pointer = const Field *;  // or also value_type*
        using reference = const Field &;  // or also value_type&

        explicit Iterator(pointer ptr) : _ptr(ptr) {}

        reference operator*() const { return *_ptr; }

        pointer operator->() { return _ptr; }

        // Prefix increment
        Iterator &operator++() {
            _ptr++;
            return *this;
        }

        // Postfix increment
        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator==(const Iterator &a, const Iterator &b) { return a._ptr == b._ptr; };

        friend bool operator!=(const Iterator &a, const Iterator &b) { return a._ptr != b._ptr; };
    private:

        pointer _ptr;
    };

    [[nodiscard]] Iterator begin() const { return Iterator{&(*_fields.begin())}; };

    [[nodiscard]] Iterator end() const { return Iterator{&(*_fields.end())}; };

    template<class T>
    Filter &add(std::string_view name, Operation op, const T &val) {
        _fields.push_back(Field::of(name, op, val));
        return *this;
    }

    template<class T>
    Filter &add(std::string_view name, const T &val) {
        add(name, Operation::op_eq, val);
        return *this;
    }

    static Filter create() {
        return {};
    }

private:
    std::vector<Field> _fields;
public:
    bool contains(std::string_view name) const {
        return std::find_if(_fields.begin(), _fields.end(), [name](auto& item) {
            return name == item.name;
        }) != _fields.end();
    }
};

struct PageRequest {
    long offset{0};
    int size{10};
    bool hasCount{false};

    static PageRequest of(long offset, int size, bool count = true) {
        return PageRequest{
                .offset = offset,
                .size = size,
                .hasCount = count,
        };
    }
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PageRequest, hasCount, offset, size)

template<typename T>
struct Page {
    typedef std::shared_ptr<Page> Ptr;
    std::list<T> data;
    std::size_t total{0};
};

class HttpRequestParams {
    Filter _filter;
    PageRequest _page{.offset = 0, .size = 10, .hasCount=true};
public:
    explicit HttpRequestParams(boost::urls::params_view &&params);

    [[nodiscard]] const Filter &getFilter() const;

    [[nodiscard]] const PageRequest &getPage() const;
};
