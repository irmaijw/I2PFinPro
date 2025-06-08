#ifndef SCORESCENE_HPP
#define SCORESCENE_HPP
#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>
#include <vector>
#include <string>
#include <map> // Might be useful for internal correlation

namespace Engine {
    class Label;
}

// Struct to hold each score entry
struct ScoreEntry {
    std::string name;
    int money;
    std::string timestamp;

    bool operator>(const ScoreEntry& other) const {
        if (money != other.money) {
            return money > other.money;
        }
        return name < other.name; // Alphabetical for ties
    }
};

class ScoreScene final : public Engine::IScene {
private:
    float ticks;
    ALLEGRO_SAMPLE_ID bgmId;
    std::vector<ScoreEntry> scores; // Final sorted list for display
    int currentPage;
    const int entriesPerPage = 8;

    std::vector<Engine::Label*> scoreLabels;
    Engine::Label* pageLabel;

    void LoadScores();
    void DisplayScores();
    void ClearScoreDisplay();

public:
    explicit ScoreScene() = default;
    ~ScoreScene();
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void BackOnClick(int stage);
    void OnPrevPageClick();
    void OnNextPageClick();
};

#endif   // SCORESCENE_HPP