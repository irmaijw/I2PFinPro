#include <algorithm> // For std::sort
#include <fstream>   // For std::ifstream
#include <iostream>  // For std::cout, std::cerr
#include <sstream>   // For std::stringstream
#include <functional> // For std::bind
#include <string>    // For std::string
#include <vector>    // For std::vector

// No need for <map> or <utility> anymore with the unified file!
// #include <map>
// #include <utility>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "PlayScene.hpp" // Keep if PlayScene might be referenced (e.g., MapId check in Update)
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "ScoreScene.hpp"
#include "Engine/Resources.hpp"

// Destructor and other general scene functions remain as per the last working version.
ScoreScene::~ScoreScene() {
    // Handled by IScene::Terminate()
}

void ScoreScene::Initialize() {
    ticks = 0;
    currentPage = 0;

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    AddNewObject(new Engine::Label("Score Board", "pirulen.ttf", 48, halfW, halfH / 4 - 10, 255, 255, 255, 255, 0.5, 0.5));

    Engine::ImageButton *btnBack;
    btnBack = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, h - 100, 400, 100);
    btnBack->SetOnClickCallback(std::bind(&ScoreScene::BackOnClick, this, 2));
    AddNewControlObject(btnBack);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, h - 50, 0, 0, 0, 255, 0.5, 0.5));

    Engine::ImageButton *btnPrev;
    btnPrev = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 300, h - 200, 120, 60);
    btnPrev->SetOnClickCallback(std::bind(&ScoreScene::OnPrevPageClick, this));
    AddNewControlObject(btnPrev);
    AddNewObject(new Engine::Label("Prev", "pirulen.ttf", 32, halfW - 240, h - 170, 0, 0, 0, 255, 0.5, 0.5));

    Engine::ImageButton *btnNext;
    btnNext = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW + 180, h - 200, 120, 60);
    btnNext->SetOnClickCallback(std::bind(&ScoreScene::OnNextPageClick, this));
    AddNewControlObject(btnNext);
    AddNewObject(new Engine::Label("Next", "pirulen.ttf", 32, halfW + 240, h - 170, 0, 0, 0, 255, 0.5, 0.5));

    pageLabel = new Engine::Label("", "pirulen.ttf", 24, halfW, h - 140, 255, 255, 255, 255, 0.5, 0.5);
    AddNewObject(pageLabel);

    LoadScores();
    DisplayScores();
    bgmId = AudioHelper::PlayAudio("win.wav");
}

void ScoreScene::Terminate() {
    AudioHelper::StopBGM(bgmId);
    IScene::Terminate();
}

void ScoreScene::Update(float deltaTime) {
    ticks += deltaTime;
    if (ticks > 4 && ticks < 100 &&
        dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetScene("play")) &&
        dynamic_cast<PlayScene *>(Engine::GameEngine::GetInstance().GetScene("play"))->MapId == 2) {
        ticks = 100;
        bgmId = AudioHelper::PlayBGM("happy.ogg");
    }
}

void ScoreScene::BackOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}

// Helper to trim whitespace from a string
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, (last - first + 1));
}

void ScoreScene::LoadScores() {
    scores.clear(); // Clear any existing scores

    std::ifstream scoreFile("../Resource/scoreboard.txt");
    if (!scoreFile.is_open()) {
        std::cerr << "Error: Failed to open ../Resource/scoreboard.txt\n";
        // Optionally, you could display a message to the user here
        return; // Nothing to load if the file isn't open
    }

    std::cout << "\n--- ScoreScene: Reading unified scoreboard.txt ---\n";
    std::string line;
    int lineNum = 0;
    while (std::getline(scoreFile, line)) {
        lineNum++;
        std::cout << "Scoreboard Line " << lineNum << " (raw): '" << line << "'\n"; // Debug: show raw line

        std::stringstream ss(line);
        std::string moneyPart, timestampPart, namePart;
        int money_val = 0;

        // Parse: [money] - read until the first comma
        if (!std::getline(ss, moneyPart, ',')) {
            std::cerr << "  Error: Invalid format (money missing) in line: '" << line << "'\n";
            continue; // Skip this malformed line
        }
        moneyPart = trim(moneyPart);
        try {
            money_val = std::stoi(moneyPart);
        } catch (const std::exception& e) {
            std::cerr << "  Error parsing money value: " << e.what() << " in line: '" << line << "'\n";
            continue; // Skip this malformed line
        }

        // Parse: [date] - read until the second comma
        if (!std::getline(ss, timestampPart, ',')) {
            std::cerr << "  Error: Invalid format (timestamp missing) in line: '" << line << "'\n";
            continue; // Skip this malformed line
        }
        timestampPart = trim(timestampPart);

        // Parse: [name] - read the rest of the line
        if (std::getline(ss, namePart)) {
            namePart = trim(namePart);
        } else {
            namePart = "Unknown Player"; // Fallback if name is missing (e.g., malformed line)
        }

        std::cout << "  Parsed: Name: '" << namePart << "', Money: " << money_val << ", Timestamp: '" << timestampPart << "'\n";
        scores.push_back({namePart, money_val, timestampPart}); // Add to the vector of scores
    }
    scoreFile.close(); // Close the file after reading all entries

    // Sort scores from highest money to lowest (assuming ScoreEntry::operator> is defined)
    std::sort(scores.begin(), scores.end(), std::greater<ScoreEntry>());
    std::cout << "\n--- ScoreScene: LoadScores() completed ---\n";
}

void ScoreScene::ClearScoreDisplay() {
    // Remove all existing score labels from the scene
    for (auto label : scoreLabels) {
        RemoveObject(label->GetObjectIterator());
    }
    scoreLabels.clear(); // Clear the vector of pointers
}

void ScoreScene::DisplayScores() {
    ClearScoreDisplay(); // Clear old labels before displaying new ones

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int halfW = w / 2;
    int startY = 200;       // Starting Y position for the first score
    const int lineHeight = 40; // Vertical spacing between scores

    // Calculate the range of scores to display for the current page
    int startIndex = currentPage * entriesPerPage;
    int endIndex = std::min(startIndex + entriesPerPage, static_cast<int>(scores.size()));

    for (int i = startIndex; i < endIndex; ++i) {
        const ScoreEntry& entry = scores[i];
        // Format for display: [NAME] $[MONEY]
        std::string scoreText = entry.name + " $" + std::to_string(entry.money);
        
        // Create and add the label
        Engine::Label* scoreLabel = new Engine::Label(scoreText, "pirulen.ttf", 28, 
                                                   halfW, startY + (i - startIndex) * lineHeight, 
                                                   255, 255, 255, 255, 0.5, 0.5);
        AddNewObject(scoreLabel);
        scoreLabels.push_back(scoreLabel); // Store pointer for later removal
    }

    // Update the page number label
    int totalPages = (scores.empty() ? 1 : (scores.size() + entriesPerPage - 1) / entriesPerPage);
    std::string pageText = "Page " + std::to_string(currentPage + 1) + " / " + std::to_string(totalPages);
    pageLabel->Text = pageText;
}

void ScoreScene::OnPrevPageClick() {
    if (currentPage > 0) {
        currentPage--;
        DisplayScores(); // Re-display scores for the new page
    }
}

void ScoreScene::OnNextPageClick() {
    // Calculate total pages dynamically
    int totalPages = (scores.empty() ? 1 : (scores.size() + entriesPerPage - 1) / entriesPerPage);
    if (currentPage < totalPages - 1) {
        currentPage++;
        DisplayScores(); // Re-display scores for the new page
    }
}