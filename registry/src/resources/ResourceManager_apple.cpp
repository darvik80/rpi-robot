//
// Created by Ivan Kishchenko on 21.04.2021.
//
#include "ResourceManager.h"
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/filesystem.hpp>
#include <boost/serialization/singleton.hpp>

#include <string>
#include <vector>

#include <CoreFoundation/CFBundle.h>

using namespace boost;

class ResourceApple : public Resource {
    std::unique_ptr<interprocess::file_mapping> _file;
    std::unique_ptr<interprocess::mapped_region> _region;
public:
    explicit ResourceApple(std::string_view filePath)
    {
        _file = std::make_unique<interprocess::file_mapping>(filePath.data(), interprocess::read_only);
        _region = std::make_unique<interprocess::mapped_region>(*_file, interprocess::read_only);
    }

    const void *getOffset() override {
        return _region->get_address();
    }

    size_t getSize() override {
        return _region->get_size();
    }

    ~ResourceApple() override {
        _region.reset();
        _file.reset();
    }
};

class ResourceManagerApple : public ResourceManager {
    std::string resourcesDirectory;
private:
    std::string getResourcesDir() override {

        CFURLRef resourceURL = CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
        char resourcePath[PATH_MAX];

        if (CFURLGetFileSystemRepresentation(resourceURL, true,
                                             (UInt8 *) resourcePath,
                                             PATH_MAX)) {
            if (resourceURL != nullptr) {
                CFRelease(resourceURL);
            }

            return resourcePath;
        }

        return std::string();
    }

public:
    ResourceManagerApple() {
        resourcesDirectory = getResourcesDir() + "/";
    }
    std::vector<std::string> getResourceNames() override {
        std::vector<std::string> result;
        filesystem::path dir(getResourcesDir());
        for (auto &itr : filesystem::directory_iterator(dir)) {
            // If it's not a directory, list it. If you want to list directories too, just remove this check.
            if (is_regular_file(itr)) {
                // assign current file name to current_file and echo it out to the console.
                result.emplace_back(itr.path().string());
            }
        }

        return result;
    }

    Resource::Ptr getResource(std::string_view name) override {
        return std::make_shared<ResourceApple>(resourcesDirectory + name.data());
    }
};

ResourceManager& ResourceManager::instance() {
    return boost::serialization::singleton<ResourceManagerApple>::get_mutable_instance();
}