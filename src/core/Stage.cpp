#include <memory>
#include <map>
#include "Stage.h"
#include "Renderer.h"


game::core::Stage::Stage(const std::string& new_Scene_Name, std::unique_ptr<game::core::Scene> scene)
{
    this->next_Scene_ = std::move(scene);
    this->scenes_.insert(std::make_pair(new_Scene_Name, this->next_Scene_));
}

void game::core::Stage::SwitchToScene(const std::string& new_Scene_Name)
{
    auto it = this->scenes_.find(new_Scene_Name);
    if(it != this->scenes_.end())
        this->next_Scene_ = it->second;
}

void game::core::Stage::SwitchToNewScene(const std::string &new_Scene_Name, std::shared_ptr<game::core::Scene> scene)
{
    this->next_Scene_ = std::move(scene);
    this->scenes_.insert(std::make_pair(new_Scene_Name, this->next_Scene_));
}

void game::core::Stage::ReplaceWithNewScene(const std::string& old_Scene_Name,
    const std::string& new_Scene_Name, std::shared_ptr<game::core::Scene> scene)
{
    this->scenes_.erase(old_Scene_Name);

    this->next_Scene_ = std::move(scene);
    this->scenes_.insert(std::make_pair(new_Scene_Name, this->next_Scene_));
}

void game::core::Stage::ReplaceWithExistingScene(const std::string &old_Scene_Name, const std::string &new_Scene_Name)
{
    auto it = this->scenes_.find(new_Scene_Name);

    if(it != this->scenes_.end())
    {
        this->scenes_.erase(old_Scene_Name);
        this->next_Scene_ = it->second;
    }
}

const std::shared_ptr<game::core::Scene> &game::core::Stage::Scene() const
{
    return this->scene_;
}

const std::map<std::string, std::shared_ptr<game::core::Scene>> &game::core::Stage::Scenes() const
{
    return this->scenes_;
}

void game::core::Stage::Update()
{
    if (this->next_Scene_)
    {
        this->scene_ = std::move(this->next_Scene_);
    }

    this->scene_->Update();
}

void game::core::Stage::Draw()
{
    ClearBackground(WHITE);

    for (auto const& [key, val] : this->scene_->actors)
    {
        val->sprite()->Update();

        if(val->sprite()->visible)
            game::core::Renderer::DrawTexture(val->sprite());
    }

    this->scene_->Draw();
}
