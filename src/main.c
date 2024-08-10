#include <math.h>
#include <raylib.h>
#include <stdio.h>

#define MAX_SMALL_METEORS 15
#define MAX_MEDIUM_METEORS 10
#define MAX_BIG_METEORS 5

typedef struct Ship {
  Vector2 position;
  Vector2 speed;
  int rotation;
  float acceleration;
} Ship;

typedef struct Meteor {
  Vector2 position;
  Vector2 cords[9];
  Vector2 speed;
  int rotation;
} Meteor;

const float twoPI = (2 * 3.14);

static Ship ship = {0};
static Meteor smallMeteor[MAX_SMALL_METEORS] = {0};

// Screen size
const int windowWidth = 1280;
const int windowHeight = 720;

void StartGame() {
  // Starting Values of ship
  ship.position = (Vector2){windowWidth / 2, windowHeight / 2};
  ship.speed = (Vector2){0, 0};
  ship.rotation = 0;
  ship.acceleration = 0;

  // Randomizing position and layout of meteors
  for (int i = 0; i < MAX_SMALL_METEORS; i++) {
    smallMeteor[i].position.x = GetRandomValue(0, windowWidth);
    smallMeteor[i].position.y = GetRandomValue(0, windowHeight);
    
    Vector2 cords[] ={
      {GetRandomValue(-15,-15),GetRandomValue(15,15)},
      {GetRandomValue(0,0),GetRandomValue(5,15)},
      {GetRandomValue(5,15),GetRandomValue(5,15)},
      {GetRandomValue(5,15),GetRandomValue(0,0)},
      {GetRandomValue(5,15),GetRandomValue(-15,-5)},
      {GetRandomValue(0,0), GetRandomValue(-15,-5)},
      {GetRandomValue(-15,-5),GetRandomValue(-15,-5)},
      {GetRandomValue(-15,-5),GetRandomValue(0,0)},
      {GetRandomValue(-15,-15),GetRandomValue(15,15)},
    };
    
    for(int c = 0; c < sizeof(cords) / sizeof(cords[0]); c++){
      smallMeteor[i].cords[c] = cords[c];
    };
    
    smallMeteor[i].rotation = GetRandomValue(-1000, 1000);
  };
}

void UpdateGame() {

  // Ship rotation
  if (IsKeyDown(KEY_RIGHT)) {
    ship.rotation -= 5;
  }
  if (IsKeyDown(KEY_LEFT)) {
    ship.rotation += 5;
  }

  // Ship speed
  ship.speed.x = sin(ship.rotation * (2 * 3.14));
  ship.speed.y = cos(ship.rotation * (2 * 3.14));

  // Ship acceleration
  if (IsKeyDown(KEY_UP)) {
    if (ship.acceleration < 10)
      ship.acceleration += 0.02f;
  } else if (ship.acceleration > 0 && !IsKeyDown(KEY_UP)) {
    ship.acceleration -= 0.01f;
  } else if (ship.acceleration < 0) {
    ship.acceleration = 0;
  }

  // Calculate position of ship
  ship.position.x += (ship.speed.x * ship.acceleration);
  ship.position.y -= (ship.speed.y * ship.acceleration);

  // Handle moving through screen
  if (ship.position.x < 0) {
    ship.position.x = windowWidth;
  } else if (ship.position.x > windowWidth) {
    ship.position.x = 0;
  }
  if (ship.position.y < 0) {
    ship.position.y = windowHeight;
  } else if (ship.position.y > windowHeight) {
    ship.position.y = 0;
  }

  //Moving meteors to random direction
  for(int i = 0; i < MAX_SMALL_METEORS; i++){
    smallMeteor[i].speed.x = sin(smallMeteor[i].rotation * (2 * 3.14));
    smallMeteor[i].speed.y = cos(smallMeteor[i].rotation * (2 * 3.14));

    smallMeteor[i].position.x += (smallMeteor[i].speed.x * 1);
    smallMeteor[i].position.y -= (smallMeteor[i].speed.y * 1);
  }
}

void DrawGame() {

  BeginDrawing();

  // Background color
  ClearBackground(BLACK);

  // Draw ship
  Vector2 cords[] = {{0, -25},  {20, 20},  {10, 10},
                     {-10, 10}, {-20, 20}, {0, -25}};

  Vector2 lines[] = {{0}, {0}, {0}, {0}, {0}, {0}};

  for (int i = 0; i < sizeof(lines) / sizeof(lines[0]); ++i) {
    lines[i].x = ((ship.position.x + cords[i].x) - ship.position.x) *
                     cos(ship.rotation * twoPI) -
                 ((ship.position.y + cords[i].y) - ship.position.y) *
                     sin(ship.rotation * twoPI) +
                 ship.position.x;

    lines[i].y = ((ship.position.x + cords[i].x) - ship.position.x) *
                     sin(ship.rotation * twoPI) +
                 ((ship.position.y + cords[i].y) - ship.position.y) *
                     cos(ship.rotation * twoPI) +
                 ship.position.y;
  }

  DrawLineStrip(lines, 6, WHITE);

  // Draw Thruster
  if (IsKeyDown(KEY_UP)) {
    Vector2 jetCords[] = {{10, 10}, {0, 30}, {-10, 10}};

    Vector2 jetLines[] = {{0}, {0}, {0}};

    for (int i = 0; i < sizeof(jetLines) / sizeof(jetLines[0]); ++i) {
      jetLines[i].x = ((ship.position.x + jetCords[i].x) - ship.position.x) *
                          cos(ship.rotation * twoPI) -
                      ((ship.position.y + jetCords[i].y) - ship.position.y) *
                          sin(ship.rotation * twoPI) +
                      ship.position.x;

      jetLines[i].y = ((ship.position.x + jetCords[i].x) - ship.position.x) *
                          sin(ship.rotation * twoPI) +
                      ((ship.position.y + jetCords[i].y) - ship.position.y) *
                          cos(ship.rotation * twoPI) +
                      ship.position.y;
    };

    DrawLineStrip(jetLines, 3, WHITE);
  }

  // Drawing Meteors
  for (int m = 0; m < MAX_SMALL_METEORS; m++) {
    Vector2 meteorLines[] = {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};

    for (int l = 0; l < sizeof(meteorLines) / sizeof(meteorLines[0]); l++) {

      meteorLines[l].x = smallMeteor[m].position.x + smallMeteor[m].cords[l].x;
      meteorLines[l].y = smallMeteor[m].position.y + smallMeteor[m].cords[l].y;
    }
  
    DrawLineStrip(meteorLines,9, WHITE);
  }

  EndDrawing();
}

int main() {
  InitWindow(windowWidth, windowHeight, "Asteroids");
  SetTargetFPS(160);

  StartGame();

  while (!WindowShouldClose()) {
    DrawGame();
    UpdateGame();
  }

  CloseWindow();

  return 0;
}
