#pragma once
#include "../Collidable.h"
#include <string>

class Trigger1 : public Collidable
{
private:
    Texture2D dia_trigger1;
    std::string text_;
    std::string name_;
    std::string portrait_path_;
    bool is_in_range_ = false;

public:
    Trigger1(Rectangle hitbox);
    ~Trigger1() override = default;

    void Tick(float delta_time) override;
    void Draw() override;
    void On_Collision(std::shared_ptr<Collidable> other) override;
    Collision_Type Get_Collision_Type() const override;

    const std::string& GetText() const;
    const std::string& GetName() const;
    const std::string& GetPortraitPath() const;
};