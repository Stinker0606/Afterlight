#pragma once
#include "../Collidable.h"
#include "../config.h.in"
#include <vector>

/**
 * @brief Eine temporäre Hitbox für Nahkampfangriffe (Sweeps).
 * Sie existiert nur für eine kurze Dauer, verursacht einmalig Schaden
 * und zerstört sich dann selbst.
 */
class MeleeHitbox : public Collidable
{
private:
    float lifetime_;      // Wie lange die Hitbox in Sekunden aktiv ist.
    Collision_Type owner_type_; // Wer hat diese Hitbox erzeugt (Player oder Enemy)
    int damage_;
    int hit_counter_ = 4; // Zähler für maximale Treffer
    std::vector<std::shared_ptr<Collidable>> already_hit_; // Verhindert Mehrfachschaden

public:
    /**
     * @param hitbox Das Rechteck, das die Position und Größe des Angriffs definiert.
     * @param lifetime Die Dauer in Sekunden, wie lange die Hitbox aktiv bleibt.
     * @param damage Der Schaden, den die Hitbox verursacht.
     * @param owner_type Der Kollisionstyp des Angreifers (PLAYER oder ENEMY).
     */
    MeleeHitbox(Rectangle hitbox, float lifetime, int damage, Collision_Type owner_type);
    ~MeleeHitbox() override = default;

    void Tick(float delta_time) override;
    void On_Collision(std::shared_ptr<Collidable> other) override;

    void Draw() override {
        if (game::Config::kDebugShowHitboxes) {
            DrawRectangleLinesEx(this->hitbox, 1.0f, RED);
        }
    };

    Collision_Type Get_Collision_Type() const override;
};