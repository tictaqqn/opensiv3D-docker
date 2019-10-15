#include <Siv3D.hpp>


	/// <summary>
	/// 円座標
	/// </summary>
	template <int32 Oclock>
	struct OffsetCircularBase
	{
	private:

		[[nodiscard]] static constexpr double Clamp(const double theta) noexcept
		{
			return theta <= -Math::Pi ? theta + Math::TwoPi : theta;
		}
        
		[[nodiscard]] static constexpr double Clock() noexcept
		{
			return Oclock * (Math::TwoPi / 12);
		}

		[[nodiscard]] static constexpr double Offset(const double theta) noexcept
		{
			return Clamp(theta - Clock());
		}

	public:

		Vec2 center;

		/// <summary>
		/// 半径
		/// </summary>
		double r;

		/// <summary>
		/// 角度（ラジアン）
		/// </summary>
		double theta;

		/// <summary>
		/// デフォルトコンストラクタ
		/// </summary>
		OffsetCircularBase() = default;

		constexpr OffsetCircularBase(const Vec2& _center, double _r = 0.0, double _theta = 0.0) noexcept
			: center(_center)
			, r(_r)
			, theta(_theta) {}

		constexpr OffsetCircularBase(const Vec2& _center, Arg::r_<double> _r, Arg::theta_<double> _theta) noexcept
			: center(_center)
			, r(*_r)
			, theta(*_theta) {}

		constexpr OffsetCircularBase(const Vec2& _center, Arg::theta_<double> _theta, Arg::r_<double> _r) noexcept
			: center(_center)
			, r(*_r)
			, theta(*_theta) {}

		OffsetCircularBase(const Vec2& _center, const Vec2& target) noexcept
			: center(_center)
		{
			setTarget(target);
		}

		[[nodiscard]] constexpr OffsetCircularBase operator +(const Vec2& v) const noexcept
		{
			return movedBy(v);
		}

		[[nodiscard]] constexpr OffsetCircularBase operator -(const Vec2& v) const noexcept
		{
			return movedBy(-v);
		}

		constexpr OffsetCircularBase& operator +=(const Vec2& v) noexcept
		{
			return moveBy(v.x, v.y);
		}

		constexpr OffsetCircularBase& operator -=(const Vec2& v) noexcept
		{
			return moveBy(-v.x, -v.y);
		}

		constexpr OffsetCircularBase& moveBy(double x, double y) noexcept
		{
			center.moveBy(x, y);

			return *this;
		}

		constexpr OffsetCircularBase& moveBy(const Vec2& v) noexcept
		{
			return moveBy(v.x, v.y);
		}

		[[nodiscard]] constexpr OffsetCircularBase movedBy(double x, double y) const noexcept
		{
			return OffsetCircularBase(*this).moveBy(x, y);
		}

		[[nodiscard]] constexpr OffsetCircularBase movedBy(const Vec2& v) const noexcept
		{
			return movedBy(v.x, v.y);
		}

		constexpr OffsetCircularBase& setCenter(double x, double y) noexcept
		{
			return setCenter(Vec2(x, y));
		}

		constexpr OffsetCircularBase& setCenter(const Vec2& _center) noexcept
		{
			center = _center;

			return *this;
		}

		OffsetCircularBase& setTarget(double x, double y) noexcept
		{
			return setTarget(Vec2(x, y));
		}

		OffsetCircularBase& setTarget(const Vec2& target) noexcept
		{
			const Vec2 v = target - center;

			r = v.length();

			theta = Offset(std::atan2(v.x, -v.y));

			return *this;
		}

		[[nodiscard]] constexpr OffsetCircularBase rotated(double angle) const noexcept
		{
			return OffsetCircularBase(*this).rotate(angle);
		}

		constexpr OffsetCircularBase& rotate(double angle) noexcept
		{
			theta += angle;

			return *this;
		}

		[[nodiscard]] Vec2 toVec2() const noexcept
		{
			return center.movedBy(std::sin(theta + Clock()) * r, -std::cos(theta + Clock()) * r);
		}

		[[nodiscard]] Float2 toFloat2() const noexcept
		{
			return{ center.x + std::sin(theta + Clock()) * r, center.y - std::cos(theta + Clock()) * r };
		}

		[[nodiscard]] operator Vec2() const noexcept
		{
			return toVec2();
		}
	};

	using OffsetCircular  = OffsetCircularBase<0>;
	using OffsetCircular0 = OffsetCircularBase<0>;
	using OffsetCircular3 = OffsetCircularBase<3>;
	using OffsetCircular6 = OffsetCircularBase<6>;
	using OffsetCircular9 = OffsetCircularBase<9>;


//////////////////////////////////////////////////////////////////////////////
//
//  Formatting CircularOffsetBase
//
//  [x] Siv3D Formatter
//  [x] ostream
//  [x] wostream
//  [x] istream
//  [x] wistream
//  [x] fmtlib BasicFormatter<wchar>
//
// namespace s3d
// {
//     template <int32 Oclock>
//     inline void Formatter(FormatData& formatData, const CircularOffsetBase<Oclock>& c)
//     {
//         Formatter(formatData, Vec4(c.center, c.r, c.theta));
//     }

//     /// <summary>
//     /// 出力ストリームに円座標を渡します。
//     /// </summary>
//     /// <param name="os">
//     /// 出力ストリーム
//     /// </param>
//     /// <param name="c">
//     /// 円座標
//     /// </param>
//     /// <returns>
//     /// 渡した後の出力ストリーム
//     /// </returns>
//     template <class CharType, int32 Oclock>
//     inline std::basic_ostream<CharType>& operator <<(std::basic_ostream<CharType>& os, const CircularOffsetBase<Oclock>& c)
//     {
//         return os << CharType('(')
//             << c.center.x << CharType(',')
//             << c.center.y << CharType(',')
//             << c.r << CharType(',')
//             << c.theta << CharType(')');
//     }

//     /// <summary>
//     /// 入力ストリームに円座標を渡します。
//     /// </summary>
//     /// <param name="is">
//     /// 入力ストリーム
//     /// </param>
//     /// <param name="c">
//     /// 円座標
//     /// </param>
//     /// <returns>
//     /// 渡した後の入力ストリーム
//     /// </returns>
//     template <class CharType, int32 Oclock>
//     inline std::basic_istream<CharType>& operator >>(std::basic_istream<CharType>& is, CircularOffsetBase<Oclock>& c)
//     {
//         CharType unused;
//         return is >> unused
//             >> c.center.x >> unused
//             >> c.center.y >> unused
//             >> c.r >> unused
//             >> c.theta >> unused;
//     }
// }

// namespace fmt
// {
//     template <class ArgFormatter, s3d::int32 Oclock>
//     void format_arg(BasicFormatter<s3d::wchar, ArgFormatter>& f, const s3d::wchar*& format_str, const s3d::CircularOffsetBase<Oclock>& c)
//     {
//         const auto tag = s3d::detail::GetTag(format_str);

//         const auto fmt = S3DSTR("({") + tag + S3DSTR("},{") + tag + S3DSTR("},{") + tag + S3DSTR("},{") + tag + S3DSTR("})");

//         f.writer().write(fmt, c.center.x, c.center.y, c.r, c.theta);
//     }
// }
//
//////////////////////////////////////////////////////////////////////////////

class VisibilityMap {
    private:
        RectF m_region;
        Array<Line> m_lines;
    public:
        explicit VisibilityMap(const RectF& region = RectF(640, 480))
            : m_region(region)
        {
            add(m_region);
        }
        void add(const Triangle& s) {
            m_lines.emplace_back(s.p0, s.p1);
            m_lines.emplace_back(s.p1, s.p2);
            m_lines.emplace_back(s.p2, s.p0);
        }
        void add(const RectF& s)
	{
		m_lines.emplace_back(s.tl(), s.tr());
		m_lines.emplace_back(s.tr(), s.br());
		m_lines.emplace_back(s.br(), s.bl());
		m_lines.emplace_back(s.bl(), s.tl());
	}

	void add(const Quad& s)
	{
		m_lines.emplace_back(s.p0, s.p1);
		m_lines.emplace_back(s.p1, s.p2);
		m_lines.emplace_back(s.p2, s.p3);
		m_lines.emplace_back(s.p3, s.p0);
	}

	void add(const Circle& s, int32 quality = 8)
	{
		quality = Max(quality, 6);
		
		const double da = 2_pi / quality;

		for (auto i : step(quality))
		{
			m_lines.emplace_back(OffsetCircular(s.center, s.r, da * i), OffsetCircular(s.center, s.r, da * (i + 1)));
		}
	}

	void add(const Polygon& s)
	{
		const auto& outer = s.outer();

		for (auto i : step(outer.size()))
		{
			m_lines.emplace_back(outer[i], outer[(i + 1) % outer.size()]);
		}
	}

	const RectF& getRegion() const
	{
		return m_region;
	}

	const Array<Line>& getLines() const
	{
		return m_lines;
	}
};

void Main() {
    Window::Resize(1280, 720);

    constexpr ColorF objectColor = Palette::Deepskyblue;
    const Array<Triangle> triangles{ Triangle(120, 120, 300, 120, 120, 500) };
    const Array<RectF> rects{
        Rect(600, 40, 40, 260), RectF(440, 300, 440, 40), 
        RectF(1040, 300, 200, 40), Rect(480, 480, 240, 100)
    };
    const Array<Circle> circles{
        Circle(1000, 500, 80),  Circle(460, 180, 30),
        Circle(240, 480, 30), Circle(300, 560, 30)
    };
    // const Array<Polygon> polygons{ Shape2D::Star(60, Vec2(940, 180)) };
    VisibilityMap map(Rect(40, 40, 1200, 640));

    while (System::Update()) {
        // Cursor::RequestStyle(CursorStyle::Hidden);

        for (const auto& triangle : triangles)
            triangle.draw(objectColor);
        for (const auto& rect : rects)
            rect.draw(objectColor);
        for (const auto& circle : circles)
            circle.draw(objectColor);
        // for (const auto& polygon : polygons)
        //     polygon.draw(objectColor);

        map.getRegion().drawFrame(0, 8, objectColor);
        for (const auto& line : map.getLines()) {
            line.draw(3, Palette::Yellow);
        }
        const Vec2 eyePos = Cursor::Pos();
        Circle(eyePos, 20).draw(Palette::Orange).drawFrame(1, 2);
    }
}