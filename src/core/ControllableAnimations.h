//
// Created by Kruse on 01/09/2025.
//

#pragma once

#include <raylib.h>
#include <memory>
#include <vector>

class ControllableAnimations {
private:
    int current_Frame = 0;
    int frame_Count;
    int sprites_per_line;
    Vector2 size;
    Rectangle target;
    Texture2D spritesheet;
    std::vector<int> frame_timings_;
    int frame_progress_counter_ = 0;
    bool is_finished_ = false;
    bool is_looping_ = false;

public:
    ControllableAnimations(Vector2 sprite_size, const char* filename, int frame_count, int sprites_per_line, std::vector<int> frame_timings, bool loops = false);

    void Next_Frame();
    void Draw_Current_Frame(Vector2 pos, Color tint = WHITE, float scale = 1.0f);

    void SetProgress(int frame, int frame_progress);
    int Get_Frame_Progress() const { return frame_progress_counter_; }
    int Get_Frame_Count() const { return frame_Count; }
    void Reset();

    bool Is_Finished() const;
    int Get_Current_Frame() const { return current_Frame; }
};