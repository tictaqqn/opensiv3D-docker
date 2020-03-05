# include <Siv3D.hpp>

namespace s3d
{ // 字下げスタイルは「BSD/オールマン」

	namespace detail
	{
		// 補助的な関数やクラスはここに
	}

	namespace Geometry2D
	{
		[[nodiscard]]bool Intersect(const Ellipse& a, const Ellipse& b)
		{
            const auto ox = b.center.x - a.center.x;
            const auto oy = b.center.y - a.center.y;
            const auto rx_pow2 = 1 / (a.a * a.a);
            const auto ry_pow2 = 1 / (a.b * a.b);
            const auto A = rx_pow2 * b.a * b.a;
            const auto B = ry_pow2 * b.b * b.b;
            const auto E = 2 * rx_pow2 * b.a * ox;
            const auto F = 2 * ry_pow2 * b.b * oy;
            const auto G = (ox/a.a) * (ox/a.a) + (oy/a.b) * (oy/a.b);

            const auto tmp1 = 1 / (- 4 * A * B);
            const auto h = -2 * E * B * tmp1;
            const auto k = -2 * A * F * tmp1;

            auto KK = A * h * h + B * k * k - E * h - F * k + G;
            if (KK > 0)
            {
                KK = 0;
            }
            const auto Rx_tt = 1 + sqrt(-KK / A);
            const auto Ry_tt = 1 + sqrt(-KK / B);

            const auto judge = h * h / (Rx_tt * Rx_tt) + k * k / (Ry_tt * Ry_tt);
            return judge <= 1;
		}
	}
}
void DragEllipse(Ellipse& r, const Key& k)
{
	if (k.down())
		r.center = Cursor::Pos();
	if (k.pressed())
    {
        const auto size = Cursor::Pos() - r.center;
        r.a = size.x; r.b = size.y;
    }
	if (r.a < 0)
		r.y -= r.a *= -1;
	if (r.b < 0)
		r.x -= r.b *= -1;
}

void Main()
{
	Window::Resize(1280, 720);

	//
	// テストはコーナーケースに注意し、
	// 1 つの関数について複数バリエーション用意するのが好ましい
	//

    Ellipse el1(0, 0, 100, 200);

	while (System::Update())
	{
		ClearPrint();

		//const Triangle shape(Cursor::Pos(), 100, 40_deg);
		const Ellipse shape(Cursor::Pos(), 100, 200);

		// 実装済みの Rect vs Circle のテスト例
        const auto intersection = Geometry2D::Intersect(el1, shape);
		if (intersection)
		{
			el1.draw(Palette::Orange);
			shape.draw(ColorF(1.0, 0.0, 0.0, 0.7));

			Print << intersection;
		}
		else
		{
			el1.draw();
			shape.draw(Palette::Yellow);
		}
	}
}