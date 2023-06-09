#pragma once

#define isDown(key) input->keyState[key].bIsDown
#define pressed(key) (input->keyState[key].bIsDown && input->keyState[key].bHasChanged)
#define released(key) (!input->keyState[key].bIsDown && input->keyState[key].bHasChanged)

float playerPosition_x = 60.f;
float playerPosition_y = 30.f;
float playerSpeed = 50.f; // units per second
