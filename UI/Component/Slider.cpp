#include <algorithm>
#include <string>

#include "Engine/Point.hpp"
#include "Slider.hpp"

Slider::Slider(float x, float y, float w, float h)
    : ImageButton("stage-select/slider.png", "stage-select/slider-blue.png", x, y),
      Bar("stage-select/bar.png", x, y, w, h),
      End1("stage-select/end.png", x, y + h / 2, 0, 0, 0.5, 0.5),
      End2("stage-select/end.png", x + w, y + h / 2, 0, 0, 0.5, 0.5) {
    Position.x += w;
    Position.y += h / 2;
    Anchor = Engine::Point(0.5, 0.5);
}
void Slider::Draw() const {
    // TODO HACKATHON-5 (3/4): The slider's component should be drawn here.
    Bar.Draw(); // the line along which the knob (slider button) moves
    End1.Draw(); // the decorative end points of the slidebar
    End2.Draw(); // the decorative end points of the slidebar
    ImageButton::Draw(); // sliderknob inherited from Imagebutton (the actual button part), calling this makes the button visible
}
void Slider::SetOnValueChangedCallback(std::function<void(float value)> onValueChangedCallback) {
    OnValueChangedCallback = onValueChangedCallback;
}
void Slider::SetValue(float value) {
    // TODO HACKATHON-5 (4/4): Set the value of the slider and call the callback.
    this->value = std::min(std::max(value, 0.0f), 1.0f); // Clamp between 0 and 1 (preventing invalid positions that go off the bar)

    // update the slider knob position
    Position.x = Bar.Position.x + Bar.Size.x * this->value; // the slider knob moves horizontally based on the value (porpotionally along the bar's width)
    Position.y = Bar.Position.y + Bar.Size.y / 2; // It stays vertically centered on the bar (Bar.Position.y + Bar.Size.y / 2)
    if (OnValueChangedCallback) {
        OnValueChangedCallback(this->value);
    }
    // to notifies other parts of the program (e.g. volume ) that the value has changes, allowing them to react.
}
void Slider::OnMouseDown(int button, int mx, int my) {
    if ((button & 1) && mouseIn)
        Down = true;
}
void Slider::OnMouseUp(int button, int mx, int my) {
    Down = false;
}
void Slider::OnMouseMove(int mx, int my) {
    ImageButton::OnMouseMove(mx, my);
    if (Down) {
        // Clamp
        float clamped = std::min(std::max(static_cast<float>(mx), Bar.Position.x), Bar.Position.x + Bar.Size.x);
        float value = (clamped - Bar.Position.x) / Bar.Size.x * 1.0f;
        SetValue(value);
    }
}
