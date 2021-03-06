#pragma once

#ifdef __APPLE__
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif
#include <unordered_map>

#include <SDL.h>
#include <SDL_image.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "Atlas.h"


namespace Match3 {

class AtlasHandler {
public:
    AtlasHandler(const fs::path& currentDirectory, SDL_Renderer* renderer) :
        currentDirectory_(currentDirectory),
        renderer_(renderer)
    {}

    void Load(const std::string& filename);
    const std::shared_ptr<const Atlas> Get(const std::string& filename) {
        return atlas_[filename];
    }

private:
    fs::path currentDirectory_;
    std::unordered_map<std::string, std::shared_ptr<Atlas>> atlas_;

    SDL_Renderer* renderer_;
};

} // namespace Match3
