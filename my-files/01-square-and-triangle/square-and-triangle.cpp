#include <Siv3D.hpp>

void Main() {

    const RectF rect(Arg::center(320, 240), 120);
    const Triangle triangle(Vec2(320, 240), 150, 90_deg);

    Transition transition(0.3s, 0.3s);
    bool isPlaying = false;

    while (System::Update()) {

        if (MouseL.down()) {
            isPlaying = !isPlaying;
        }
        transition.update(isPlaying);
        
        const double e = transition.easeInOut(Easing::Expo);

        const Quad quad(
            triangle.p0.lerp(rect.tr(), e),
            triangle.p0.lerp(rect.br(), e),
            triangle.p1.lerp(rect.bl(), e),
            triangle.p2.lerp(rect.tl(), e)
        );
        quad.draw();
    }
}