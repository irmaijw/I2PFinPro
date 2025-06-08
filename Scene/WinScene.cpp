#include <functional>
#include <string>
#include <fstream>
#include <iostream>
#include <allegro5/allegro_ttf.h>
#include <ctime>      // Required for std::time, std::localtime, std::strftime
#include <filesystem> // Required for std::filesystem::current_path (optional debug)

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "PlayScene.hpp" // Crucial: Include PlayScene.hpp to access its members/methods
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/TextInput.hpp"
#include "UI/Component/Label.hpp"
#include "WinScene.hpp"
#include "Engine/Resources.hpp"

void WinScene::Initialize() {
    ticks = 0;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    AddNewObject(new Engine::Image("win/benjamin-sad.png", halfW, halfH + 30, 0, 0, 0.5, 0.5));
    AddNewObject(new Engine::Label("You Win!", "pirulen.ttf", 48, halfW, halfH / 4 - 10, 255, 255, 255, 255, 0.5, 0.5));

    // --- Optional: Debugging current working directory ---
    // Make sure you have included <filesystem> for this to work.
    try {
        std::filesystem::path currentPath = std::filesystem::current_path();
        std::cout << "Current working directory: " << currentPath << std::endl;
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error getting current working directory: " << e.what() << std::endl;
    }
    // --- End of optional debug ---

    // Create text input
    textInput = new Engine::TextInput(
        halfW - 150, halfH - 250, 300, 40,
        Engine::Resources::GetInstance().GetFont("pirulen.ttf", 24).get()
    );
    AddNewControlObject(textInput);

    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
    // When this button is clicked, BackOnClick will now also save the score
    btn->SetOnClickCallback(std::bind(&WinScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));
    bgmId = AudioHelper::PlayAudio("win.wav");
}


void WinScene::Terminate() {
    IScene::Terminate();
    AudioHelper::StopBGM(bgmId);
}
void WinScene::Update(float deltaTime) {
    ticks += deltaTime;
    // This part of the logic remains unchanged. It handles background music
    // based on MapId from PlayScene.
    if (ticks > 4 && ticks < 100 &&
        dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetScene("play")) &&
        dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetScene("play"))->MapId == 2) {
        ticks = 100;
        bgmId = AudioHelper::PlayBGM("happy.ogg");
    }
}


void WinScene::BackOnClick(int stage) {
    // Get the player's name from the text input
    std::string playerName = textInput->GetText();
    if (playerName.empty()) {
        playerName = "Anonymous"; // Default to "Anonymous" if no name is entered
    }

    // --- Retrieve the winning money from PlayScene ---
    int winningMoney = 0;
    // Get a pointer to the active PlayScene instance
    PlayScene* playScene = dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"));
    if (playScene) {
        // Access the money using the GetMoney() public getter
        winningMoney = playScene->GetMoney();
    } else {
        std::cerr << "Warning: PlayScene not found or not active. Cannot retrieve winning money. Using 0.\n";
    }
    // --- End of score retrieval ---

    // Open scoreboard.txt in append mode
    std::ofstream outFile("../Resource/scoreboard.txt", std::ios::app);
    if (outFile.is_open()) {
        // Get current timestamp
        std::time_t now = std::time(nullptr);
        char timestamp[100];
        // Format the timestamp consistently (YYYY-MM-DD HH:MM:SS)
        std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

        // Write the unified record: money , timestamp , player name
        outFile << winningMoney << " , " << timestamp << " , " << playerName << "\n";
        outFile.close();
        std::cout << "Successfully saved score to scoreboard.txt: " << winningMoney << " , " << timestamp << " , " << playerName << std::endl;
    } else {
        std::cerr << "Error: Failed to open ../Resource/scoreboard.txt for writing. Check file path and permissions.\n";
    }

    // Change scene to stage-select
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
