#include <stdio.h>
#include <raylib.h>

typedef struct Ship{
  Vector2 position;
} Ship;

static Ship ship = { 0 };

//Screen size
const int windowWidth = 1280;
const int windowHeight = 720;

void StartGame(){
  ship.position = (Vector2){windowWidth/2,windowHeight/2};
};

void DrawGame(){

  BeginDrawing();
    
    ClearBackground(BLACK);

    //Draw ship
    const Vector2 lines[] = {
      (Vector2){ship.position.x, ship.position.y - 25},
      (Vector2){ship.position.x + 20, ship.position.y + 20},
      (Vector2){ship.position.x + 5, ship.position.y + 10},
      (Vector2){ship.position.x - 5, ship.position.y + 10},
      (Vector2){ship.position.x - 20, ship.position.y + 20},
      (Vector2){ship.position.x, ship.position.y - 25}
    };

    DrawLineStrip(lines, 6, WHITE);

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

