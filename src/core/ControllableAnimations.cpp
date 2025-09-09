//
// Created by Kruse on 01/09/2025.
//

#include "ControllableAnimations.h"
#include "AssetManager.h"

ControllableAnimations::ControllableAnimations(Vector2 sprite_size, const char* filename, int FC, int spl, std::vector<int> ft, bool loops) {
    // Lade die Textur über den AssetManager, um Duplikate zu vermeiden.
    this->spritesheet = AssetManager::GetInstance().Load(filename);
    this->size = sprite_size;
    this->frame_Count = FC;
    this->sprites_per_line = spl;
    this->target = Rectangle{1, 1, this->size.x, this->size.y};
    this->frame_timings_ = ft;
    this->is_looping_ = loops;
}

void ControllableAnimations::Reset() {
    target.x = 1;
    target.y = 1;
    this->current_Frame = 0;
    this->frame_progress_counter_ = 0;
    this->is_finished_ = false;
}

void ControllableAnimations::Next_Frame() {
    // Wenn die Animation fertig ist, bleibt sie auf dem letzten Frame stehen, bis sie resettet wird.
    if (is_finished_) return;

    frame_progress_counter_++;

    // Prüfe, ob die Anzeigedauer für den aktuellen Frame erreicht ist.
    if (current_Frame < frame_timings_.size() && frame_progress_counter_ >= frame_timings_[current_Frame]) {
        current_Frame++;
        frame_progress_counter_ = 0; // Zähler für den nächsten Frame zurücksetzen

        // Prüfe, ob die Animation mit diesem Frame zu Ende ist.
        if (current_Frame >= frame_Count) {
            if (is_looping_) {
                Reset(); // Wenn es ein Loop ist, fange von vorne an.
            } else {
                is_finished_ = true; // Ansonsten beende die Animation.
            }
            return;
        }

        // Gehe zum nächsten Frame im Spritesheet
        if (current_Frame % sprites_per_line == 0 && current_Frame != 0) {
            target.x = 1;
            target.y = target.y + size.y;
        } else {
            target.x = target.x + size.x;
        }
    }
}

    void ControllableAnimations::Draw_Current_Frame(Vector2 pos, Color tint, float scale) {
    // Diese Draw-Methode ist robust und funktioniert mit dem Nebel-Shader.
    Rectangle dest = { pos.x, pos.y, this->size.x * scale, this->size.y * scale };
    Vector2 origin = { (this->size.x * scale) / 2, (this->size.y * scale) / 2 };

    BeginBlendMode(BLEND_ALPHA);
    DrawTexturePro(
        this->spritesheet,
        this->target,
        dest,
        origin,
        0.0f,
        tint // Benutze die übergebene tint-Variable
    );
    EndBlendMode();
}

bool ControllableAnimations::Is_Finished() const {
    return this->is_finished_;
}

void ControllableAnimations::SetProgress(int frame, int frame_progress) {
    if (frame >= 0 && frame < this->frame_Count) {
        this->current_Frame = frame;
        this->frame_progress_counter_ = frame_progress;

        // Aktualisiere das 'target' Rechteck, damit es zum neuen Frame passt.
        int row = frame / this->sprites_per_line;
        int col = frame % this->sprites_per_line;
        this->target.x = 1 + col * this->size.x;
        this->target.y = 1 + row * this->size.y;
    }
}