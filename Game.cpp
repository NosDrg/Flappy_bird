#include "Game.h"

//TTF_Font* font = nullptr;

Game::Game() : window(nullptr), renderer(nullptr), running(false), currentMap(0), mapOffset(0), mapSpeed(1000), highScore(0) {
    player1 = new Character(200, 150, renderer);
}

Game::~Game() {
    delete player1;
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }else{
        cout <<"Init video run normally."<<endl;
        }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
        return false;
    }else{
        cout <<"Init img run normally."<< endl;
    }
//    if (TTF_Init() == -1) return false;

    window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        return false;
    }else{
        cout <<"Window run normally."<<endl;
        }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        return false;
    }else{
        cout<<"Renderer run normally."<<endl;
        }

//    font = TTF_OpenFont("arial.ttf", 24);
//    if (!font) return false;
    cout<<endl;
    running = true;
    return true;
}

void renderEnd(SDL_Renderer* renderer, SDL_Texture* endmenu)
{

}

void Game::run() {
    while (running) {
        processInput();
        if(!inMenu) {
            update();
        }
        render();
        SDL_Delay(16);
    }
}

void Game::update() {
    player1->update();
    Attack::updateAll(player1);
    Item::updateAll(player1);
    scoreGame(0);

    if(score < 600){
        mapOffset -= ( 3 + score / 100);
    }else{
        if(score < 1600) {
            mapOffset -= mapSpeed / 100;
        }else{
            mapOffset -= mapSpeed / 100 + score / 800;
        }
    }
    if (score > highScore) {
        highScore = score;
    }

    if (mapOffset <= -SCREEN_WIDTH) {
        currentMap = (currentMap + 1) % TOTAL_MAPS;
        mapOffset = 0;
    }

    if (rand() % 1000000 < ((600 + score/30 <= 20000) ? ( 600 + score/60) : 20000) && score > 170) {
        Attack::spawnRandomAttack();
    }

    if (rand() % 1000000 < ((400 + score/50 <= 10000) ? (400 + score/50) : 10000) && score > 800) {
        Attack::spawnLaser();
    }

    if (rand() % 1000000 < 100 && score > 400 ) {
        Item::spawnRandomItem();
    }
    if (score % 100 == 0) cout << score << endl;
}

void Game::scoreGame(int value){
    score += mapSpeed / 1000 + value;

}

void Game::renderMenu(SDL_Renderer* renderer, SDL_Texture* menu) {

    menu = IMG_LoadTexture(renderer, "img//Menu.png");
    SDL_RenderCopy( renderer, menu, NULL, NULL);

    SDL_RenderPresent( renderer );
}

void Game::processInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            running = false;
        }
        if (e.type == SDL_KEYDOWN) {
            if (inMenu && e.key.keysym.sym == SDLK_UP) {
                inMenu = false;
            }
        }
        if (!inMenu) {
            player1->handleEvent(e);
        }
    }
}

void Game::render() {
     if (inMenu) {
        renderMenu(renderer, menu);
        return;
    }

    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 100 + (currentMap * 20), 100, 200, 255);
    SDL_Rect mainMap = { mapOffset, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderFillRect(renderer, &mainMap);

    int nextMapIndex = (currentMap + 1) % TOTAL_MAPS;
    SDL_SetRenderDrawColor(renderer, 100 + (nextMapIndex * 20), 100, 200, 255);
    SDL_Rect nextMap = { mapOffset + SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderFillRect(renderer, &nextMap);

    player1->render(renderer);
    Attack::renderAll(renderer);
    Item::renderAll(renderer);

    SDL_RenderPresent(renderer);

//    std::string scoreText = "Score: " + std::to_string(score);
//    std::string highScoreText = "High Score: " + std::to_string(highScore);

//    this->renderText(renderer, font, scoreText, 20, 20);
//    this->renderText(renderer, font, highScoreText, 20, 50);

}
/*
void Game::renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y) {
    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dstRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
*/

void Game::close() {
//    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
//    TTF_Quit();
    SDL_Quit();
}
