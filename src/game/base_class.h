//
// Created by Kruse on 23/05/2025.
//

#ifndef RAYLIBSTARTER_BASE_CLASS_H
#define RAYLIBSTARTER_BASE_CLASS_H


class base_class {
protected:
    int hp = 0;
    float speed = 0.0f;
    int damage = 0;

public:
    virtual ~base_class() = default;

    virtual void ATTACK() = 0;
    virtual void UPDATE(float delta_time) = 0;

    int GET_HP() const { return hp; }
    float GET_SPEED() const { return speed; }
    int GET_DAMAGE() const { return damage; }
};


#endif //RAYLIBSTARTER_BASE_CLASS_H
