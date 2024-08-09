#include <math.h>
#include <raylib.h>
#include <stdio.h>

typedef struct Ship {
  Vector2 position;
  Vector2 speed;
  int rotation;
  float acceleration;
} Ship;

const float twoPI = (2 * 3.14);

static Ship ship = {0};

// Screen size
const int windowWidth = 1280;
const int windowHeight = 720;

void StartGame() {
  ship.position = (Vector2){windowWidth / 2, windowHeight / 2};
  ship.speed = (Vector2){0, 0};
  ship.rotation = 0;
  ship.acceleration = 0;
};

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
}

void DrawGame() {

  BeginDrawing();

  // Background color
  ClearBackground(BLACK);

  // Draw ship
  Vector2 cords[] = {{0, -25}, {20, 20},  {5, 10},
                     {-5, 10}, {-20, 20}, {0, -25}};

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
  DrawPixel(ship.position.x, ship.position.y, RED);

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
