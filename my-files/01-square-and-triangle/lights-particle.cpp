#include <Siv3D.hpp>

struct Particle : IEffect {

    // パーティクルの生存時間（秒）
	static constexpr double m_durationSec = 3.0;
    // パーティクルが移動する距離
    static constexpr double m_distance = 150.0;
    // パーティクルのサイズ
    static constexpr double m_size = 15.0;
    // パーティクルの色
	// static constexpr ColorF m_color = ColorF(1.0);
    // パーティクルのぼかしサイズ
	static constexpr double m_blurSize = 6.0;

    Vec2 m_pos;
    Vec2 m_dir;
    ColorF m_color;

    Particle(const Vec2& pos, const Vec2& dir, const ColorF& color)
        : m_pos(pos), m_dir(dir), m_color(color) {}
    
    bool update(double t) override {

        t /= m_durationSec;
        const Vec2 pos = m_pos + m_dir * t * m_distance;
        const double r = (1.0 - t) * m_size;
        Circle(pos, r).drawShadow(Vec2(0, 0), m_blurSize, 0, m_color); // 小さくなる

        return t < 1.0;
    }
};

void Main() {
    // 毎秒のパーティクル生成量
    constexpr int32 EffectAmount = 15;
    // パーティクル生成の周期
    constexpr std::chrono::duration EmitterDuration = 1.0s / EffectAmount;

    Effect effect;
    Stopwatch stopwatch(true);

    while (System::Update()) {
        if (stopwatch >= EmitterDuration) {
            const HSV hsv(Random(360.0), Random(0.4, 0.8), 1.0);
            effect.add<Particle>(Vec2(320, 240), RandomVec2(), hsv);
            stopwatch.set(stopwatch.elapsedF() - EmitterDuration);
        }
        // 加算ブレンド. 色が重なる
		RenderStateBlock2D blend(BlendState::Additive);
        effect.update();
    }

}