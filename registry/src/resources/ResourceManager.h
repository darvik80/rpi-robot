//
// Created by Ivan Kishchenko on 21.04.2021.
//

#pragma once

#include <cstddef>
#include <memory>
#include <vector>
#include <string>
#include <optional>

class Resource {
public:
    typedef std::shared_ptr<Resource> Ptr;
public:
    virtual const void *getOffset() = 0;

    virtual size_t getSize() = 0;

    virtual ~Resource() = default;
};

class ResourceManager {
public:
    typedef std::shared_ptr<ResourceManager> Ptr;
public:
    virtual std::vector<std::string> getResourceNames() = 0;

    virtual Resource::Ptr getResource(std::string_view name) = 0;

    virtual std::optional<std::string> getResourceAsString(std::string_view name) {
        if (auto resource = getResource(name); resource) {
            return std::string((const char *) resource->getOffset(), resource->getSize());
        }

        return {};
    }

    static ResourceManager &instance();

    virtual std::string getResourcesDir() = 0;
};
