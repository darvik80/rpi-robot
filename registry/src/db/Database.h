//
// Created by Ivan Kishchenko on 21/03/2023.
//

#pragma once

#include <BaseService.h>

#include "Repository.h"
#include "DatabaseProperties.h"
#include "DataSource.h"
#include <typeindex>

class Database : public BaseService {
    DataSource::Ptr _dataSource;
    std::unordered_map<std::type_index, std::unique_ptr<Repository>> _repositories;
public:

    const char *name() override;

    int order() override;

    void postConstruct(Registry &registry) override;

    void preDestroy(Registry &registry) override;

    template<class R>
    R &getRepository() {
        if (auto iter = _repositories.find(typeid(R)); iter != std::end(_repositories)) {
            return *static_cast<R*>(iter->second.get());
        }

        throw std::invalid_argument(std::string("The repository has not been registered ") + typeid(R).name());
    }

    template<class R>
    R& createRepository() {
        return *static_cast<R*>(_repositories.emplace(typeid(R), std::make_unique<R>(*_dataSource)).first->second.get());
    }
};
