#ifndef TEXTINPUT_HPP
#define TEXTINPUT_HPP

#include <string>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include "Engine/IObject.hpp"
#include "Engine/IControl.hpp"

namespace Engine {
    class TextInput : public IObject, public IControl {
    private:
        std::string text;
        ALLEGRO_FONT* font;
        float width, height;
        bool selected;
        double cursorTimer;
        ALLEGRO_COLOR textColor;
        ALLEGRO_COLOR bgColor;
        ALLEGRO_COLOR borderColor;

    public:
        TextInput(float x, float y, float w, float h, ALLEGRO_FONT* font);
        void Draw() const override;
        void Update(float deltaTime) override;

        // IControl interface
        void OnKeyDown(int keycode) override; // This will now handle all input
        void OnMouseDown(int button, int mx, int my) override;

        // OnChar(int unicode); // <<< REMOVE THIS LINE
        std::string GetText() const;
        bool IsMouseOver(int mx, int my) const;
    };
}
#endif