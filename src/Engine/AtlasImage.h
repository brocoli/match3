#pragma once

#include "Renderable.h"

#include <memory>

#include "SDL.h"

#include "Resources/Atlas.h"


namespace Match3 {

using Int2D = std::pair<int, int>;

class AtlasImage : public Renderable {
public:
    AtlasImage(const std::string& atlasName, const std::string& imageName, int x, int y, bool visible);

    void Render(SDL_Renderer* renderer) override;
    void SetX(int x) override { dstRect_.x = x; }
    void SetY(int y) override { dstRect_.y = y; }

    void SetImage(const std::string& imageName);

    void SetVisible(bool visible) { visible_ = visible; }
    void SetXY(Int2D xy) { dstRect_.x = xy.first; dstRect_.y = xy.second; }

    bool IsVisible() const { return visible_; }

    Int2D GetXY() const { return Int2D(dstRect_.x, dstRect_.y); }
    int GetX() const { return dstRect_.x; }
    int GetY() const { return dstRect_.y; }

private:
    std::shared_ptr<const Atlas> atlas_;
    SDL_Rect srcRect_;
    SDL_Rect dstRect_;

    bool visible_;
};

} // namespace Match3
