#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;

Color green ={185, 229, 232, 255}; // Light Blue
Color darkGreen = {58, 89, 209, 255}; // Dark Blue

int cellSize = 25;
int cellCount = 25;
int offSet = 75;
const char* highScoreFile = "highscore.txt";

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
        direction = {1, 0};
        nextDirection = {1, 0};
        pendingDirection = {0, 0}; 
        addSegment = false;
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
    bool running = false;
    int score = 0;
    bool paused = false;
    int highScore = 0;
    const char* gameOverReason = "";
    bool firstMoveMade = false;
    float moveInterval = 0.2f;       // Starting speed
    float minInterval = 0.06f;       // Fastest allowed
    Sound eatSound;
    Sound wallSound;

    Game(){
        InitAudioDevice();
        eatSound = LoadSound("Sounds/eat.mp3");
        wallSound = LoadSound("Sounds/wall.mp3");
    }

    ~Game(){
        UnloadSound(eatSound);
        UnloadSound(wallSound);
        CloseAudioDevice();
    }

    void Draw(){
        food.Draw();
        snake.Draw();
        if (!running) {
            DrawText("Press arrow key to start", 100, 100, 30, DARKGRAY);
            if (gameOverReason && gameOverReason[0] != '\0') {
                int fontSize = 40;
                int textWidth = MeasureText(gameOverReason, fontSize);
                int screenCenterX = GetScreenWidth() / 2;
                DrawText(gameOverReason, screenCenterX - textWidth / 2, 375, fontSize, MAROON);
            }
        }
        if (paused) {
            DrawText("Paused", offSet + 225, offSet + cellSize * cellCount + 10, 40, RED);
        }
    }

    void Update(){
        if(running){
            snake.Update();
            if (firstMoveMade) {
                CheckCollisionWithFood();
                CheckCollisionWithEdges();
                CheckCollisionWithTail();
            }
        }
    }

    void CheckCollisionWithFood(){
        if (Vector2Equals(snake.body[0], food.position)) {
            food.position = food.generateRandomPos(snake.body);
            snake.addSegment = true;
            score++;
            PlaySound(eatSound);

            // Increase speed every 5 points
            if (score % 5 == 0 && moveInterval > minInterval) {
                moveInterval -= 0.02f; // decrease interval, increase speed
                if (moveInterval < minInterval) moveInterval = minInterval;
            }
        }
    }

    void CheckCollisionWithEdges(){
        if(snake.body[0].x == cellCount || snake.body[0].x == -1){
            gameOverReason = "Wall Bang!";
            GameOver();
        }
        if(snake.body[0].y == cellCount || snake.body[0].y == -1){
            gameOverReason = "Wall Bang!";
            GameOver();
        }
    }

    void GameOver(){
        if (score > highScore) {
            highScore = score;
            SaveHighScore();
        }
        PlaySound(wallSound);
        score = 0;
        running = false;
        firstMoveMade = false;
        snake.Reset();
        food.position = food.generateRandomPos(snake.body);
        moveInterval = 0.2f;
    }

    void CheckCollisionWithTail(){
        deque<Vector2> headLessBody = snake.body;
        headLessBody.pop_front();
        if(ElementInDeque(snake.body[0], headLessBody)){
            gameOverReason = "Tail Eat!";
            GameOver();
        }
    }

    void LoadHighScore() {
        FILE* file = fopen(highScoreFile, "r");
        if (file != nullptr) {
            fscanf(file, "%d", &highScore);
            fclose(file);
        }
    }

    void SaveHighScore() {
        FILE* file = fopen(highScoreFile, "w");
        if (file != nullptr) {
            fprintf(file, "%d", highScore);
            fclose(file);
        }
    }
};

int main () {

    cout << "Starting the game..." << endl;
    InitWindow(2 * offSet + cellSize * cellCount, 2 * offSet + cellSize * cellCount, "Retro Snake");
    SetTargetFPS(60);

    Game game = Game();
    game.LoadHighScore();

    while(WindowShouldClose() == false){
        BeginDrawing();

        if (IsKeyPressed(KEY_UP) && game.snake.pendingDirection.y != 1) {
            game.snake.pendingDirection = {0, -1};
            game.running = true;
            game.firstMoveMade = true;
            game.gameOverReason = "";
        }
        if (IsKeyPressed(KEY_DOWN) && game.snake.pendingDirection.y != -1) {
            game.snake.pendingDirection = {0, 1};
            game.running = true;
            game.firstMoveMade = true;
            game.gameOverReason = "";
        }
        if (IsKeyPressed(KEY_LEFT) && game.snake.pendingDirection.x != 1) {
            game.snake.pendingDirection = {-1, 0};
            game.running = true;
            game.firstMoveMade = true;
            game.gameOverReason = "";
        }
        if (IsKeyPressed(KEY_RIGHT) && game.snake.pendingDirection.x != -1) {
            game.snake.pendingDirection = {1, 0};
            game.running = true;
            game.firstMoveMade = true;
            game.gameOverReason = "";   
        }

        if (IsKeyPressed(KEY_SPACE)) {
            game.paused = !game.paused;
        }

        if(eventTriggered(game.moveInterval) && !game.paused){
            game.Update();
            game.CheckCollisionWithFood();
        }

        ClearBackground(green);
        DrawRectangleLinesEx(Rectangle{(float)offSet - 5, (float)offSet - 5, (float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10}, 5, darkGreen);
        DrawText("Retro Snake", offSet - 5, 20, 40, darkGreen);
        DrawText(TextFormat("Score: %i", game.score), offSet - 5, offSet + cellSize * cellCount + 10, 30, darkGreen);
        DrawText(TextFormat("High Score: %i", game.highScore), offSet + 430, offSet + cellSize * cellCount + 10, 30, darkGreen);
        game.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}