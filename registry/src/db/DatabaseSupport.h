//
// Created by Ivan Kishchenko on 21/03/2023.
//

#pragma once

#include "SqlBuilder.h"
#include "DataSource.h"
#include "http/HttpRequestParams.h"

class Repository {
public:
    virtual ~Repository() = default;
};

template<typename T>
class TRepository : public Repository {
protected:
    DataSource &_source;
protected:
    template<typename M = sql::SelectModel>
    M &applyFilter(M &model, const Filter &filter) {
        for (const auto &item: filter) {
            switch (item.operation) {
                case Filter::Operation::op_like:
                    if (item.type == Filter::Type::type_string) {
                        model.where(sql::column(item.name).like(std::get<std::string>(item.value)));
                    }
                    break;
                case Filter::Operation::op_nlike:
                    if (item.type == Filter::Type::type_string) {
                        model.where(sql::column(item.name).not_like(std::get<std::string>(item.value)));
                    }
                    break;
                case Filter::Operation::op_gt:
                    if (item.type == Filter::Type::type_long) {
                        model.where(sql::column(item.name) > (std::get<long>(item.value)));
                    } else if (item.type == Filter::Type::type_double) {
                        model.where(sql::column(item.name) > (std::get<double>(item.value)));
                    }
                    break;
                case Filter::Operation::op_lt:
                    if (item.type == Filter::Type::type_long) {
                        model.where(sql::column(item.name) < (std::get<long>(item.value)));
                    } else if (item.type == Filter::Type::type_double) {
                        model.where(sql::column(item.name) < (std::get<double>(item.value)));
                    }
                    break;
                case Filter::Operation::op_gte:
                    if (item.type == Filter::Type::type_long) {
                        model.where(sql::column(item.name) >= (std::get<long>(item.value)));
                    }
                    break;
                case Filter::Operation::op_lte:
                    if (item.type == Filter::Type::type_long) {
                        model.where(sql::column(item.name) <= (std::get<long>(item.value)));
                    } else if (item.type == Filter::Type::type_double) {
                        model.where(sql::column(item.name) <= (std::get<double>(item.value)));
                    }
                    break;
                case Filter::Operation::op_eq:
                default:
                    if (item.type == Filter::Type::type_string) {
                        model.where(sql::column(item.name) == (std::get<std::string>(item.value)));
                    } else if (item.type == Filter::Type::type_long) {
                        model.where(sql::column(item.name) == (std::get<long>(item.value)));
                    } else if (item.type == Filter::Type::type_double) {
                        model.where(sql::column(item.name) == (std::get<double>(item.value)));
                    }
                    break;
            }
        }

        return model;
    }

public:
    explicit TRepository(DataSource &source) : _source(source) {}

    virtual std::optional<T> findById(long id) {
        return findOne(Filter::create().add("id", Filter::Operation::op_eq, id));
    }

    virtual std::optional<T> findOne(const Filter &filter) {
        auto res = findAll(filter, PageRequest::of(0, 1, false));
        return res->data.empty() ? std::optional<T>{} : (*res->data.begin());
    }

    virtual typename Page<T>::Ptr findAll(const Filter &filter, const PageRequest &page) = 0;

    virtual long insert(const T &model) = 0;

    virtual void update(const T &model) = 0;

    virtual void deleteByFilter(const Filter &filter) = 0;

    virtual void deleteById(long id) {
        deleteByFilter(Filter::create().add("id", Filter::Operation::op_eq, id));
    }
};
