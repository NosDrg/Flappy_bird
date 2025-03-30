#include "Game.h"

//TTF_Font* font = nullptr;

Game::Game() : window(nullptr), renderer(nullptr), running(false),texture(nullptr) , currentMapIndex(0), mapOffset(0), mapSpeed(1000), highScore(0) {}

Game::~Game() {
    delete player1;
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s", SDL_GetError());
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        SDL_Log("SDL_image could not initialize! IMG_Error: %s", IMG_GetError());
        return false;
    }

    window = SDL_CreateWindow("Fpally bird", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("Window could not be created! SDL_Error: %s", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Renderer could not be created! SDL_Error: %s", SDL_GetError());
        return false;
    }

     loadMaps();
    if (mapTextures.empty()) {
        SDL_Log("No maps loaded!");
        return false;
    }

    SDL_QueryTexture(mapTexture, NULL, NULL, &mapWidth, &mapHeight);
    renderMenu(renderer, menuTexture);

    player1 = new Character(100, 200, renderer);
    running = true;

    return true;
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

/*void Game::Gameover() {
    endgame = true;
}
*/
void Game::update() {
    endgame = true;
//    if(endgame) return;

    player1->update();
    Attack::updateAll(player1);
    Item::updateAll(player1);
    scoreGame(0);
/*
    if(player1->health <= 0) {
        Gameover();
        return;
    }
*/
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

    if (mapOffset <= -mapWidth) {
        switchToNextMap();
    }
/*
    if (mapOffset <= -SCREEN_WIDTH) {
        currentMapIndex = (currentMapIndex + 1) % TOTAL_MAPS;
        mapOffset = 0;
    }
*/
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

void Game::loadMaps() {
    mapTextures.push_back(IMG_LoadTexture(renderer, "img//desert.png"));
    mapTextures.push_back(IMG_LoadTexture(renderer, "img//gense.png"));
    mapTextures.push_back(IMG_LoadTexture(renderer, "img//dangai.png"));
    mapTextures.push_back(IMG_LoadTexture(renderer, "img//goteijusantai.png"));
    mapTextures.push_back(IMG_LoadTexture(renderer, "img//hollow_forset.png"));
    mapTextures.push_back(IMG_LoadTexture(renderer, "img//juuichi_no_bantai.png"));
    mapTextures.push_back(IMG_LoadTexture(renderer, "img//karakura.png"));
    mapTextures.push_back(IMG_LoadTexture(renderer, "img//kiri_no_mura.png"));
}

void Game::renderMenu(SDL_Renderer* renderer, SDL_Texture* menuTexture) {

    menuTexture = IMG_LoadTexture(renderer, "img//Menu.png");
    SDL_RenderCopy(renderer, menuTexture, NULL, NULL);

    SDL_RenderPresent( renderer );
}

void Game::switchToNextMap() {
    currentMapIndex = (currentMapIndex + 1) % mapTextures.size();
    mapOffset = 0;

    SDL_QueryTexture(mapTextures[currentMapIndex], NULL, NULL, &mapWidth, &mapHeight);
}


void Game::processInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            running = false;
        }
       /* if (endgame && e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_r) {
//                resetGame();
            } else if (e.key.keysym.sym == SDLK_q) {
                running = false;
            }
        }*/
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
/*
void Game::resetGame() {
    endgame = false;
    score = 0;
    mapOffset = 0;
    currentMapIndex = 0;

    player1->health = 100;
    player1->x = 100;
    player1->z = 240;
    player1->velocityZ = 0;
    player1->invincible = false;
    player1->gravityFlip = false;

    Attack::attacks.clear();
    Item::items.clear();
}
*/
void Game::render() {
     if (inMenu) {
        renderMenu(renderer, texture);
        return;
    }

 /*   if (endgame) {
        renderEnd();
        return;
    }
*/
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_RenderClear(renderer);

    SDL_Rect mapRect = {mapOffset, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_Rect srcRect = { -mapOffset, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(renderer, mapTexture, &srcRect, &mapRect);

/*
    SDL_SetRenderDrawColor(renderer, 100 + (currentMap * 20), 100, 200, 255);
    SDL_Rect mainMap = { mapOffset, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderFillRect(renderer, &mainMap);

    int nextMapIndex = (currentMap + 1) % TOTAL_MAPS;
    SDL_SetRenderDrawColor(renderer, 100 + (nextMapIndex * 20), 100, 200, 255);
    SDL_Rect nextMap = { mapOffset + SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderFillRect(renderer, &nextMap);
*/
    SDL_Rect currentMapRect = { mapOffset, 0, mapWidth, SCREEN_HEIGHT };
    SDL_RenderCopy(renderer, mapTextures[currentMapIndex], NULL, &currentMapRect);

    if (mapOffset + SCREEN_WIDTH > mapWidth) {
        int nextMapIndex = (currentMapIndex + 1) % mapTextures.size();
        SDL_Rect nextMapRect = { mapOffset + mapWidth, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_RenderCopy(renderer, mapTextures[nextMapIndex], NULL, &nextMapRect);
    }

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


void Game::renderEnd() {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Vẽ ảnh game over nếu có
    if (endTexture) {
        SDL_RenderCopy(renderer, endTexture, NULL, NULL);
    } else {
        // Fallback nếu không có ảnh
        SDL_SetRenderDrawColor(renderer, 50, 0, 0, 255);
        SDL_Rect background = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderFillRect(renderer, &background);
    }

    // Hiển thị điểm số
    if (font) {
        SDL_Color white = {255, 255, 255, 255};

        // Hiển thị "GAME OVER"
        SDL_Surface* surface = TTF_RenderText_Solid(font, "GAME OVER", white);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect rect = {SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 60, 200, 40};
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        // Hiển thị điểm
        std::string scoreText = "Score: " + std::to_string(score);
        surface = TTF_RenderText_Solid(font, scoreText.c_str(), white);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        rect = {SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2, 200, 30};
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

    }

    SDL_RenderPresent(renderer);
}
*/

void Game::close() {
//    TTF_CloseFont(font);
    for (auto& map : mapTextures) {
        SDL_DestroyTexture(map);
    }
    mapTextures.clear();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(mapTexture);
    /*if (endTexture) {
        SDL_DestroyTexture(endTexture);
    }*/
    IMG_Quit();
//    TTF_Quit();
    SDL_Quit();
}
