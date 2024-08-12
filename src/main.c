#include <math.h>
#include <raylib.h>
#include <stdio.h>

#define MAX_SMALL_METEORS 20
#define MAX_MEDIUM_METEORS 10
#define MAX_BIG_METEORS 5

typedef struct Ship {
  Vector2 position;
  Vector2 cords[6];
  Vector2 lines[6];
  Vector2 collisionPoint;
  Vector2 speed;
  int rotation;
  float acceleration;
} Ship;

typedef struct Jet {
  Vector2 cords[3];
  Vector2 lines[3];
} Jet;

typedef struct Meteor {
  Vector2 position;
  Vector2 cords[9];
  Vector2 lines[9];
  Vector2 speed;
  int rotation;
} Meteor;

const float twoPI = (2 * 3.14);

static Ship ship = {0};
static Jet jet = {0};
static Meteor smallMeteor[MAX_SMALL_METEORS] = {0};

// Screen size
const int windowWidth = 1280;
const int windowHeight = 720;

int frameCounter = 0;
bool gameOver = false;

void StartGame() {
  frameCounter = 0;

  /*
   *
   * STARTING VALUES OF SHIP
   *
   */
  ship.position = (Vector2){windowWidth / 2, windowHeight / 2};
  ship.speed = (Vector2){0, 0};
  ship.rotation = 0;
  ship.acceleration = 0;

  Vector2 shipCords[] = {{0, -25},  {20, 20},  {10, 10},
                         {-10, 10}, {-20, 20}, {0, -25}};

  for (int sC = 0; sC < sizeof(shipCords) / sizeof(shipCords[0]); sC++) {
    ship.cords[sC] = shipCords[sC];
  }

  Vector2 shipLines[] = {{0}, {0}, {0}, {0}, {0}, {0}};

  for (int sL = 0; sL < sizeof(shipLines) / sizeof(shipLines[0]); sL++) {
    ship.lines[sL].x =
        ((ship.position.x + ship.cords[sL].x) - ship.position.x) *
            cos(ship.rotation * twoPI) -
        ((ship.position.y + ship.cords[sL].y) - ship.position.y) *
            sin(ship.rotation * twoPI) +
        ship.position.x;

    ship.lines[sL].y =
        ((ship.position.x + ship.cords[sL].x) - ship.position.x) *
            sin(ship.rotation * twoPI) +
        ((ship.position.y + ship.cords[sL].y) - ship.position.y) *
            cos(ship.rotation * twoPI) +
        ship.position.y;
  }

  /*
   *
   * STARTING VALUES OF JET
   *
   */
  Vector2 jetCords[] = {{10, 10}, {0, 30}, {-10, 10}};

  for (int jC = 0; jC < sizeof(jetCords) / sizeof(jetCords[0]); jC++) {
    jet.cords[jC] = jetCords[jC];
  }

  Vector2 jetLines[] = {{0}, {0}, {0}};

  for (int jL = 0; jL < sizeof(jetLines) / sizeof(jetLines[0]); jL++) {
    jet.lines[jL].x = ((ship.position.x + jet.cords[jL].x) - ship.position.x) *
                          cos(ship.rotation * twoPI) -
                      ((ship.position.y + jet.cords[jL].y) - ship.position.y) *
                          sin(ship.rotation * twoPI) +
                      ship.position.x;

    jet.lines[jL].y = ((ship.position.x + jet.lines[jL].x) - ship.position.x) *
                          sin(ship.rotation * twoPI) +
                      ((ship.position.y + jet.lines[jL].y) - ship.position.y) *
                          cos(ship.rotation * twoPI) +
                      ship.position.y;
  };

  /*
   *
   * STARTING VALUES OF SMALL METEOR
   *
   */
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
  if (!gameOver) {
    frameCounter++;

    /*
     *
     * UPDATING SHIP
     *
     */
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

    for (int sL = 0; sL < sizeof(ship.lines) / sizeof(ship.lines[0]); sL++) {
      ship.lines[sL].x =
          ((ship.position.x + ship.cords[sL].x) - ship.position.x) *
              cos(ship.rotation * twoPI) -
          ((ship.position.y + ship.cords[sL].y) - ship.position.y) *
              sin(ship.rotation * twoPI) +
          ship.position.x;

      ship.lines[sL].y =
          ((ship.position.x + ship.cords[sL].x) - ship.position.x) *
              sin(ship.rotation * twoPI) +
          ((ship.position.y + ship.cords[sL].y) - ship.position.y) *
              cos(ship.rotation * twoPI) +
          ship.position.y;
    }

    /*
     *
     * UPDATING JET
     *
     */
    for (int jL = 0; jL < sizeof(jet.lines) / sizeof(jet.lines[0]); jL++) {
      jet.lines[jL].x =
          ((ship.position.x + jet.cords[jL].x) - ship.position.x) *
              cos(ship.rotation * twoPI) -
          ((ship.position.y + jet.cords[jL].y) - ship.position.y) *
              sin(ship.rotation * twoPI) +
          ship.position.x;

      jet.lines[jL].y =
          ((ship.position.x + jet.cords[jL].x) - ship.position.x) *
              sin(ship.rotation * twoPI) +
          ((ship.position.y + jet.cords[jL].y) - ship.position.y) *
              cos(ship.rotation * twoPI) +
          ship.position.y;
    };

    /*
     *
     * UPDATING SMALL METEOR
     *
     */
    for (int sM = 0; sM < MAX_SMALL_METEORS; sM++) {
      // Moving meteors into random direction
      smallMeteor[sM].speed.x = sin(smallMeteor[sM].rotation * twoPI);
      smallMeteor[sM].speed.y = cos(smallMeteor[sM].rotation * twoPI);

      smallMeteor[sM].position.x += smallMeteor[sM].speed.x;
      smallMeteor[sM].position.y -= smallMeteor[sM].speed.y;

      // Update the meteor lines
      for (int l = 0; l < 9; l++) {
        smallMeteor[sM].lines[l].x =
            smallMeteor[sM].position.x + smallMeteor[sM].cords[l].x;
        smallMeteor[sM].lines[l].y =
            smallMeteor[sM].position.y + smallMeteor[sM].cords[l].y;
      }

      // Collision detection
      for (int l = 0;
           l < sizeof(smallMeteor[sM].lines) / sizeof(smallMeteor[sM].lines[0]);
           l++) {
        for (int sL = 0; sL < sizeof(ship.lines) / sizeof(ship.lines[0]);
             sL++) {
          if (CheckCollisionLines(
                  smallMeteor[sM].lines[l],
                  smallMeteor[sM]
                      .lines[(l + 1) % sizeof(smallMeteor[sM].lines) /
                             sizeof(smallMeteor[sM].lines[0])],
                  ship.lines[sL],
                  ship.lines[(sL + 1) % sizeof(ship.lines) /
                             sizeof(ship.lines[0])],
                  &ship.collisionPoint)) {
            gameOver = true;
            break;
          }
        }
        if (gameOver)
          break;
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
  } else {
    if (IsKeyPressed(KEY_ENTER)) {
      StartGame();
      gameOver = false;
    }
  }
}

void DrawGame() {

  BeginDrawing();

  // Background color
  ClearBackground(BLACK);

  if (!gameOver) {
    // Draw ship
    DrawLineStrip(ship.lines, 6, WHITE);

    // Draw Jet
    if (IsKeyDown(KEY_UP)) {
      DrawLineStrip(jet.lines, 3, WHITE);
    }

    // Drawing Meteors
    for (int sM = 0; sM < MAX_SMALL_METEORS; sM++) {
      DrawLineStrip(smallMeteor[sM].lines, 9, WHITE);
    }

    DrawText(TextFormat("Score: %0.2f", (float)frameCounter / 160), 10, 10, 15,
             WHITE);

  } else {
    DrawText("PRESS ENTER TO START NEW GAME!",
             windowWidth / 2 -
                 MeasureText("PRESS ENTER TO START NEW GAME!", 20) / 2,
             windowHeight / 2, 20, RED);

    DrawText(TextFormat("Your score was: %0.2f", (float)frameCounter / 160),
             windowWidth / 2 - MeasureText("Your score was: %0.2f", 20) / 2,
             windowHeight / 2 - 40, 20, RED);
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
