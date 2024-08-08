#include <stdio.h>
#include <raylib.h>

int main(){
  const int windowWidth = 800;
  const int windowHeight = 450;

  InitWindow(windowWidth, windowHeight, "Asteroids");

  SetTargetFPS(160);
  
  while(!WindowShouldClose()){
    BeginDrawing();
      
      ClearBackground(RAYWHITE);

      DrawText("HUJ", 190, 200, 20, LIGHTGRAY);

    EndDrawing();
  }

  CloseWindow();
  
  return 0;
}
