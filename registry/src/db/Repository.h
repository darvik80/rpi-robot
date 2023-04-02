//
// Created by Ivan Kishchenko on 21/03/2023.
//

#pragma once

#include <vector>
#include <string>

namespace db {

    enum class Operation {
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

    class Filter {
    public:
        struct Field {
            std::string _name;
            Operation _operation;
            std::string _value;

        private:
            template<typename T>
            static std::string join(const std::vector<T> &v, const std::string &delim, const std::function<std::string(const T&)>& trans) {
                std::string ret;
                auto it = std::begin(v);
                if (it != std::end(v)) {
                    ret = trans(v);
                    while (++it != std::end(v)) ret += delim + trans(*it);
                }
                return ret;
            }
        public:
            template<class T>
            static Field of(std::string_view name, Operation op, const T &val) {
                Field res{._name=name.data(), ._operation = op};
                if constexpr (std::is_same<std::string, T>::value) {
                    res._value = val;
                } else if constexpr (std::is_arithmetic_v<T>) {
                    res._value = std::to_string(val);
                }

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

                Field res{._name=name.data(), ._operation = op};
                if constexpr (std::is_same<std::string, T>::value) {
                    res._value = join(val, ", ", [](const T& v) {
                        return "'" + v + "'";
                    });
                } else if constexpr (std::is_arithmetic_v<T>) {
                    res._value = join(val, ", ", [](const T& v) {
                        return std::to_string(v);
                    });
                }

                return res;
            }
        };


        template<class T>
        void add(std::string_view name, Operation op, const T& val) {
            _fields.push_back(Field::of(name, op, val));
        }

    private:
        std::vector<Field> _fields;
    };


    class Repository {
    public:
        virtual ~Repository() = default;
    };
}
