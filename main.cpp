#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};

int cellSize = 25;
int cellCount = 25;
int offSet = 75;

double lastUpdateTime = 0;

bool ElementInDeque(Vector2 element, deque<Vector2> dq){
    for(unsigned i = 0; i < dq.size(); i++){
        if(Vector2Equals(dq[i], element)) return true;
    }
    return false;
}

bool eventTriggered(double interval){
    double currentTime = GetTime();
    if(currentTime - lastUpdateTime >= interval){
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

class Snake{
    public :
    deque<Vector2> body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
    Vector2 direction = {1,0};
    Vector2 nextDirection = {1, 0}; 
    bool addSegment = false;
    Vector2 pendingDirection = {0, 0};  

    void Draw(){
        for(unsigned i=0; i<body.size(); i++){
            float x = body[i].x;
            float y = body[i].y;
            Rectangle segment = Rectangle{offSet + x * cellSize, offSet + y * cellSize,(float) cellSize,(float) cellSize};
            DrawRectangleRounded(segment, 0.4, 6, darkGreen);
        }
    }

    void Update(){
        direction = pendingDirection;  // Apply latest buffered input
        body.push_front(Vector2Add(body[0], direction));
        if(addSegment == true){
            addSegment = false;
        }
        else{
            direction = nextDirection;  
            body.pop_back();
        }   
    }

    void Reset(){
        body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
        direction = {1,0};
    }
};

class Food{
    public : 
    Vector2 position;
    Texture2D texture;

    // Food Counstructor
    Food(deque<Vector2> snakeBody){
        Image image = LoadImage("Graphics/red_food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = generateRandomPos(snakeBody);
    }

    // Food Distructor
    ~Food(){
        UnloadTexture(texture);
    }

    void Draw(){
        //DrawRectangle(position.x * cellSize, position.y * cellSize, cellSize, cellSize, darkGreen);
        DrawTexture(texture, offSet + position.x * cellSize, offSet + position.y * cellSize, WHITE);
    }

    Vector2 generateRandomCell(){
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);
        return Vector2{x,y};
    }

    Vector2 generateRandomPos(deque<Vector2> snakeBody){
        Vector2 position = generateRandomCell();
        while(ElementInDeque(position, snakeBody)){
            position = generateRandomCell();
        }
        return position;
    }
};

class Game{
    public : 
    Snake snake = Snake();
    Food food = Food(snake.body);
    bool running = true;
    int score = 0;

    void Draw(){
        food.Draw();
        snake.Draw();
        if (!running) {
            DrawText("Press arrow key to start", 100, 100, 30, DARKGRAY);
        }
    }

    void Update(){
        if(running){
            snake.Update();
            CheckCollisionWithFood();
            CheckCollisionWithEdges();
            CheckCollisionWithTail();
        }
    }

    void CheckCollisionWithFood(){
        if(Vector2Equals(snake.body[0], food.position)){
            food.position = food.generateRandomPos(snake.body);
            snake.addSegment = true;
            score++;
        }
    }

    void CheckCollisionWithEdges(){
        if(snake.body[0].x == cellCount || snake.body[0].x == -1){
            GameOver();
        }
        if(snake.body[0].y == cellCount || snake.body[0].y == -1){
            GameOver();
        }
    }

    void GameOver(){
        snake.Reset();
        food.position = food.generateRandomPos(snake.body);
        running = false;
        score = 0;
    }

    void CheckCollisionWithTail(){
        deque<Vector2> headLessBody = snake.body;
        headLessBody.pop_front();
        if(ElementInDeque(snake.body[0], headLessBody)){
            GameOver();
        }
    }
};

int main () {

    cout << "Starting the game..." << endl;
    InitWindow(2 * offSet + cellSize * cellCount, 2 * offSet + cellSize * cellCount, "Retro Snake");
    SetTargetFPS(60);

    Game game = Game();

    while(WindowShouldClose() == false){
        BeginDrawing();

        if (IsKeyPressed(KEY_UP) && game.snake.pendingDirection.y != 1) {
            game.snake.pendingDirection = {0, -1};
            game.running = true;
        }
        if (IsKeyPressed(KEY_DOWN) && game.snake.pendingDirection.y != -1) {
            game.snake.pendingDirection = {0, 1};
            game.running = true;
        }
        if (IsKeyPressed(KEY_LEFT) && game.snake.pendingDirection.x != 1) {
            game.snake.pendingDirection = {-1, 0};
            game.running = true;
        }
        if (IsKeyPressed(KEY_RIGHT) && game.snake.pendingDirection.x != -1) {
            game.snake.pendingDirection = {1, 0};
            game.running = true;
        }

        if(eventTriggered(0.2)){
            game.Update();
            game.CheckCollisionWithFood();
        }

        ClearBackground(green);
        DrawRectangleLinesEx(Rectangle{(float)offSet - 5, (float)offSet - 5, (float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10}, 5, darkGreen);
        DrawText("Retro Snake", offSet - 5, 20, 40, darkGreen);
        DrawText(TextFormat("%i", game.score), offSet - 5, offSet + cellSize * cellCount + 10, 40, darkGreen);
        game.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}