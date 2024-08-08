#include <stdio.h>
#include <raylib.h>

typedef struct Ship{
  Vector2 position;
} Ship;

static Ship ship = { 0 };

//Screen size
const int windowWidth = 800;
const int windowHeight = 450;

void StartGame(){
  ship.position = (Vector2){windowWidth/2,windowHeight/2};
};

void DrawGame(){

  BeginDrawing();
    
    ClearBackground(BLACK);

    //Draw ship
    DrawPolyLines(ship.position, 4, 10, 45, WHITE);

  EndDrawing();
}

int main(){
  InitWindow(windowWidth, windowHeight, "Asteroids");
  SetTargetFPS(160);
  
  StartGame();

  while(!WindowShouldClose()){
    DrawGame();
  }

  CloseWindow();
  
  return 0;
}

