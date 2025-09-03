//
// Created by Kruse on 07/07/2025.
//
#include "AssetManager.h"
#include "RepeatAnimation.h"
RepeatAnimation::RepeatAnimation(Vector2 sprite_size,const char* filename,int FC,int spl) {
    this->spritesheet = AssetManager::GetInstance().Load(filename);
    this->size=sprite_size;
    this->frame_Count=FC;
    this->sprites_per_line=spl;
    this->target=Rectangle{1,1, this->size.x, this->size.y};
}
void RepeatAnimation::First_Frame() {
    target.x=1;
    target.y=1;
    this->current_Frame=0;
}
void RepeatAnimation::Next_Frame() {
    this->current_Frame++;

    if (this->frame_Count > this->current_Frame) {
        if (this->current_Frame % this->sprites_per_line == 0 && current_Frame != 0) {
            this->target.x = 1;
            this->target.y = this->target.y + this->size.y;
        } else {
            this->target.x = this->target.x + this->size.x;
        }
    }
    if (this->current_Frame >= this->frame_Count){
        First_Frame();
    }
}
void RepeatAnimation::Draw_Current_Frame(Vector2 pos, Color tint) {
    Vector2 origin = { this->size.x / 2, this->size.y / 2 };
    Rectangle dest = { pos.x, pos.y, this->size.x, this->size.y };

    // --- ZEICHENLOGIK ---
    BeginBlendMode(BLEND_ALPHA);
    DrawTexturePro(
        this->spritesheet,  // Die Quell-Textur (das Spritesheet)
        this->target,       // Der Quell-Ausschnitt (der aktuelle Frame)
        dest,               // Das Ziel-Rechteck auf dem Bildschirm
        origin,             // Der Rotations- und Skalierungs-Ursprung (die Mitte)
        0.0f,               // Keine Rotation
        tint                // Deine Tint-Farbe mit der korrekten Transparenz
    );
    EndBlendMode();
}