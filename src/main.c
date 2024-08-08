#include <stdio.h>
#include <raylib.h>

typedef struct Ship{
  Vector2 position;
  float rotation;
  float acceleration;
} Ship;

static Ship ship = { 0 };

//Screen size
const int windowWidth = 1280;
const int windowHeight = 720;

void StartGame(){
  ship.position = (Vector2){windowWidth/2,windowHeight/2};
  ship.rotation = 0;
  ship.acceleration = 0;
};

void UpdateGame(){

  //Ship rotation
  if(IsKeyDown(KEY_RIGHT)){
    ship.rotation += 0.04f;
  }
  if(IsKeyDown(KEY_LEFT)){
    ship.rotation -= 0.04f;
  }

  //ship acceleration
  if(IsKeyDown(KEY_UP)){
    ship.acceleration += 0.02f;
  }else if(ship.acceleration > 0 && !IsKeyDown(KEY_UP)){
    ship.acceleration -= 0.01f;
  }

  ship.position.y -= ship.acceleration;
}

void DrawGame(){

  BeginDrawing();
    
    //Background color
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
    UpdateGame();
  }

  CloseWindow();
  
  return 0;
}

