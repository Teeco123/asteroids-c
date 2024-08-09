#include <stdio.h>
#include <raylib.h>
#include <math.h>

typedef struct Ship{
  Vector2 position;
  Vector2 speed;
  int rotation;
  float acceleration;
} Ship;

const float twoPI = (2 * 3.14);

static Ship ship = { 0 };

//Screen size
const int windowWidth = 1280;
const int windowHeight = 720;

void StartGame(){
  ship.position = (Vector2){windowWidth/2,windowHeight/2};
  ship.speed = (Vector2){0,0};
  ship.rotation = 0;
  ship.acceleration = 0;
};

void UpdateGame(){

  //Ship rotation
  if(IsKeyDown(KEY_RIGHT)){
    ship.rotation -= 5;
  }
  if(IsKeyDown(KEY_LEFT)){
    ship.rotation += 5;
  }

  //Ship speed
  ship.speed.x = sin(ship.rotation * (2 * 3.14));
  ship.speed.y = cos(ship.rotation * (2 * 3.14));

  //Ship acceleration
  if(IsKeyDown(KEY_UP)){
    if(ship.acceleration < 1.5) ship.acceleration += 0.02f;
  }else if(ship.acceleration > 0.01f && !IsKeyDown(KEY_UP)){
    ship.acceleration -= 0.01f;
  }
  
  //Calculate position of ship
  ship.position.x += (ship.speed.x * ship.acceleration);
  ship.position.y -= (ship.speed.y * ship.acceleration);
  
}

void DrawGame(){

  BeginDrawing();
    
    //Background color
    ClearBackground(BLACK);

    //Draw ship
    const Vector2 lines[] = {
      (Vector2){
       (ship.position.x - ship.position.x) * cos(ship.rotation * twoPI) - ((ship.position.y - 25) - ship.position.y) * sin(ship.rotation * twoPI) + ship.position.x,
       (ship.position.x - ship.position.x) * sin(ship.rotation * twoPI) + ((ship.position.y - 25) - ship.position.y) * cos(ship.rotation * twoPI) + ship.position.y
      },
      (Vector2){
       ((ship.position.x + 20) - ship.position.x) * cos(ship.rotation * twoPI) - ((ship.position.y + 20) - ship.position.y) * sin(ship.rotation * twoPI) + ship.position.x,
       ((ship.position.x + 20) - ship.position.x) * sin(ship.rotation * twoPI) + ((ship.position.y + 20) - ship.position.y) * cos(ship.rotation * twoPI) + ship.position.y
      },
      (Vector2){
       ((ship.position.x + 5) - ship.position.x) * cos(ship.rotation * twoPI) - ((ship.position.y + 10) - ship.position.y) * sin(ship.rotation * twoPI) + ship.position.x,
       ((ship.position.x + 5) - ship.position.x) * sin(ship.rotation * twoPI) + ((ship.position.y + 10) - ship.position.y) * cos(ship.rotation * twoPI) + ship.position.y
      },
      (Vector2){
       ((ship.position.x - 5) - ship.position.x) * cos(ship.rotation * twoPI) - ((ship.position.y + 10) - ship.position.y) * sin(ship.rotation * twoPI) + ship.position.x,
       ((ship.position.x - 5) - ship.position.x) * sin(ship.rotation * twoPI) + ((ship.position.y + 10) - ship.position.y) * cos(ship.rotation * twoPI) + ship.position.y
      },
      (Vector2){
       ((ship.position.x - 20) - ship.position.x) * cos(ship.rotation * twoPI) - ((ship.position.y + 20) - ship.position.y) * sin(ship.rotation * twoPI) + ship.position.x,
       ((ship.position.x - 20) - ship.position.x) * sin(ship.rotation * twoPI) + ((ship.position.y + 20) - ship.position.y) * cos(ship.rotation * twoPI) + ship.position.y
      },
      (Vector2){
       (ship.position.x - ship.position.x) * cos(ship.rotation * twoPI) - ((ship.position.y - 25) - ship.position.y) * sin(ship.rotation * twoPI) + ship.position.x,
       (ship.position.x - ship.position.x) * sin(ship.rotation * twoPI) + ((ship.position.y - 25) - ship.position.y) * cos(ship.rotation * twoPI) + ship.position.y
      }
    };
    DrawLineStrip(lines, 6, WHITE);
    DrawPixel(ship.position.x, ship.position.y, RED);

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

