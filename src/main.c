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
  Vector2 lines[9];
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

  // Starting values of small meteor
  for (int sM = 0; sM < MAX_SMALL_METEORS; sM++) {
    smallMeteor[sM].position.x = GetRandomValue(0, windowWidth);
    smallMeteor[sM].position.y = GetRandomValue(0, windowHeight);

    Vector2 cords[] = {
        {GetRandomValue(-15, -15), GetRandomValue(15, 15)},
        {GetRandomValue(0, 0), GetRandomValue(5, 15)},
        {GetRandomValue(5, 15), GetRandomValue(5, 15)},
        {GetRandomValue(5, 15), GetRandomValue(0, 0)},
        {GetRandomValue(5, 15), GetRandomValue(-15, -5)},
        {GetRandomValue(0, 0), GetRandomValue(-15, -5)},
        {GetRandomValue(-15, -5), GetRandomValue(-15, -5)},
        {GetRandomValue(-15, -5), GetRandomValue(0, 0)},
        {GetRandomValue(-15, -15), GetRandomValue(15, 15)},
    };

    for (int c = 0; c < sizeof(cords) / sizeof(cords[0]); c++) {
      smallMeteor[sM].cords[c] = cords[c];
    };

    Vector2 lines[] = {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};

    for (int l = 0; l < sizeof(lines) / sizeof(lines[0]); l++) {

      smallMeteor[sM].lines[l].x =
          smallMeteor[sM].position.x + smallMeteor[sM].cords[l].x;
      smallMeteor[sM].lines[l].y =
          smallMeteor[sM].position.y + smallMeteor[sM].cords[l].y;
    }

    smallMeteor[sM].rotation = GetRandomValue(-1000, 1000);
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

  // Meteors mechanics
  for (int sM = 0; sM < MAX_SMALL_METEORS; sM++) {
    // Moving meteors into random direction
    smallMeteor[sM].speed.x = sin(smallMeteor[sM].rotation * (2 * 3.14));
    smallMeteor[sM].speed.y = cos(smallMeteor[sM].rotation * (2 * 3.14));

    smallMeteor[sM].position.x += (smallMeteor[sM].speed.x * 1);
    smallMeteor[sM].position.y -= (smallMeteor[sM].speed.y * 1);

    // Calculating meteor lines
    for (int l = 0;
         l < sizeof(smallMeteor[sM].lines) / sizeof(smallMeteor[sM].lines[0]);
         l++) {

      smallMeteor[sM].lines[l].x =
          smallMeteor[sM].position.x + smallMeteor[sM].cords[l].x;
      smallMeteor[sM].lines[l].y =
          smallMeteor[sM].position.y + smallMeteor[sM].cords[l].y;
    }

    // Meteor wall mechanic
    if (smallMeteor[sM].position.x < 0) {
      smallMeteor[sM].position.x = windowWidth;
    } else if (smallMeteor[sM].position.x > windowWidth) {
      smallMeteor[sM].position.x = 0;
    }

    if (smallMeteor[sM].position.y < 0) {
      smallMeteor[sM].position.y = windowHeight;
    } else if (smallMeteor[sM].position.y > windowHeight) {
      smallMeteor[sM].position.y = 0;
    }
  }
}

void DrawGame() {

  BeginDrawing();

  // Background color
  ClearBackground(BLACK);

  // Draw ship
  Vector2 shipCords[] = {{0, -25},  {20, 20},  {10, 10},
                         {-10, 10}, {-20, 20}, {0, -25}};

  Vector2 shipLines[] = {{0}, {0}, {0}, {0}, {0}, {0}};

  for (int i = 0; i < sizeof(shipLines) / sizeof(shipLines[0]); ++i) {
    shipLines[i].x = ((ship.position.x + shipCords[i].x) - ship.position.x) *
                         cos(ship.rotation * twoPI) -
                     ((ship.position.y + shipCords[i].y) - ship.position.y) *
                         sin(ship.rotation * twoPI) +
                     ship.position.x;

    shipLines[i].y = ((ship.position.x + shipCords[i].x) - ship.position.x) *
                         sin(ship.rotation * twoPI) +
                     ((ship.position.y + shipCords[i].y) - ship.position.y) *
                         cos(ship.rotation * twoPI) +
                     ship.position.y;
  }

  DrawLineStrip(shipLines, 6, WHITE);

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
  for (int sM = 0; sM < MAX_SMALL_METEORS; sM++) {
    DrawLineStrip(smallMeteor[sM].lines, 9, WHITE);
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
