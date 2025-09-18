#pragma once
#include "../Collidable.h"
#include <string>

class DialogTrigger : public Collidable
{
private:
    Texture2D dia_trigger;
    std::string text_;
    std::string name_;
    std::string portrait_path_;
    bool is_in_range_ = false;

public:
    DialogTrigger(Rectangle hitbox, std::string text, std::string name, std::string portrait);
    ~DialogTrigger() override = default;

    void Tick(float delta_time) override;
    void Draw() override;
    void On_Collision(std::shared_ptr<Collidable> other) override;
    Collision_Type Get_Collision_Type() const override;

    const std::string& GetText() const;
    const std::string& GetName() const;
    const std::string& GetPortraitPath() const;
};