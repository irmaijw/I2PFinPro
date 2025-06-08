#include "UI/Component/TextInput.hpp"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h> // Make sure this is included for ALLEGRO_KEY_...
#include <cctype> // For std::toupper

namespace Engine {
    TextInput::TextInput(float x, float y, float w, float h, ALLEGRO_FONT* font) :
        IObject(x, y), font(font), width(w), height(h), selected(false), cursorTimer(0) {
        textColor = al_map_rgb(0, 0, 0);
        bgColor = al_map_rgb(255, 255, 255);
        borderColor = al_map_rgb(100, 100, 100);
    }

    void TextInput::Draw() const {
        // Draw background and border
        al_draw_filled_rectangle(Position.x, Position.y, Position.x + width, Position.y + height, bgColor);
        al_draw_rectangle(Position.x, Position.y, Position.x + width, Position.y + height,
                          selected ? al_map_rgb(0, 0, 255) : borderColor, 2);

        // Draw text
        // Ensure text is truncated if it exceeds the width
        float textWidth = al_get_text_width(font, text.c_str());
        std::string displayString = text;
        if (textWidth > width - 10) { // Add some padding
            // Simple truncation: keep removing characters from the start until it fits
            // A more sophisticated approach might be to keep the end of the string
            // and add "..." at the beginning, but this is simpler for now.
            while (al_get_text_width(font, displayString.c_str()) > width - 10 && !displayString.empty()) {
                displayString = displayString.substr(1);
            }
        }


        const std::string& actualDisplayText = displayString.empty() && !selected ? "Enter name..." : displayString;
        ALLEGRO_COLOR displayColor = (displayString.empty() && !selected) ? al_map_rgb(150, 150, 150) : textColor;
        al_draw_text(font, displayColor, Position.x + 5, Position.y + height/2 - al_get_font_line_height(font)/2,
                     0, actualDisplayText.c_str());

        // Draw cursor if selected
        if (selected && static_cast<int>(cursorTimer * 2) % 2 == 0) {
            // Use the full text for cursor position
            float cursorX = Position.x + 5 + al_get_text_width(font, text.c_str());
            al_draw_line(cursorX, Position.y + 5,
                         cursorX, Position.y + height - 5,
                         textColor, 2);
        }
    }

    void TextInput::Update(float deltaTime) {
        if (selected) {
            cursorTimer += deltaTime;
            if (cursorTimer > 1.0) cursorTimer = 0;
        }
    }

    void TextInput::OnKeyDown(int keycode) {
        if (!selected) return;

        if (keycode >= ALLEGRO_KEY_A && keycode <= ALLEGRO_KEY_Z) {
            // Convert keycode to uppercase character directly
            text += static_cast<char>('A' + (keycode - ALLEGRO_KEY_A));
        } else if (keycode >= ALLEGRO_KEY_0 && keycode <= ALLEGRO_KEY_9) {
            // Convert keycode to digit character
            text += static_cast<char>('0' + (keycode - ALLEGRO_KEY_0));
        } else if (keycode == ALLEGRO_KEY_SPACE) {
            text += ' ';
        } else if (keycode == ALLEGRO_KEY_BACKSPACE && !text.empty()) {
            text.pop_back();
        }
        // Add more special characters if needed, e.g.,
        // else if (keycode == ALLEGRO_KEY_MINUS) { text += '-'; }
        // else if (keycode == ALLEGRO_KEY_EQUALS) { text += '='; }
        // ... and so on for other symbols you want to support.
        // This will become quite tedious for comprehensive support.

        // After any input, reset cursor timer for immediate visibility
        cursorTimer = 0;
    }

    void TextInput::OnMouseDown(int button, int mx, int my) {
        // Toggle selected state based on mouse click
        selected = IsMouseOver(mx, my);
        cursorTimer = 0; // Reset cursor timer when selection changes
    }

    // Removed OnChar method completely
    // void TextInput::OnChar(int unicode) { ... }

    bool TextInput::IsMouseOver(int mx, int my) const {
        return mx >= Position.x && mx <= Position.x + width &&
               my >= Position.y && my <= Position.y + height;
    }

    std::string TextInput::GetText() const {
        return text;
    }
}