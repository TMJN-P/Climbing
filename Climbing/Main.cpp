#include <Siv3D.hpp> // OpenSiv3D v0.4.1

class playable_character {
public:
	Circle circle;
	Vec2 velocity;
	Vec2 before_pos;
	void draw(double& camera_pos) {
		circle.movedBy(0, camera_pos).draw(Palette::Orange);
	}
	void move(Array<Line>& scaffold, double& camera_pos, Array<Circle>& effect, int& t) {
		before_pos = circle.center;
		circle.center += velocity;
		if (circle.y >= 480) {
			circle.y = 480;
			velocity.x = 0;
			velocity.y = 0;
		}
		if (circle.x < 70) {
			circle.x = 71;
			velocity.x *= -0.3;
		}
		if (circle.x > 530) {
			circle.x = 529;
			velocity.x *= -0.3;
		}
		velocity.x *= 0.95;
		velocity.x += ((Cursor::PosF().x - circle.x) > 0 ? 0.3 : -0.3);
		for (Line& l : scaffold) {
			l.movedBy(0, camera_pos).draw(5, Palette::Blue);
			if (l.intersects(circle) && velocity.y >= 0 && before_pos.y <= l.begin.y - circle.r) {
				velocity = Vec2(0, 0);
				circle.y = l.begin.y - circle.r;
				//if (MouseL.down()) {
					accelerate(camera_pos, t);
					effect.push_back(Circle(Cursor::Pos(), 2));
				//}
			}
		}
	}
	void gravity() {
		velocity.y += 0.5;
	}
	void accelerate(double& camera_pos, int& t) {
		velocity.y = -t;
	}
};

void Main() {
	Scene::SetBackground(Palette::Black);
	Window::Resize(800, 600);
	const Rect ground(0, 500, 600, 100);
	const Rect left_wall(0, 0, 50, 600);
	const Rect right_wall(550, 0, 50, 600);
	Array<Rect>barrier;
	Array<Rect>boost;
	Array<Line>scaffold;
	Array<Circle>effect;
	playable_character p;
	double camera_pos = 0;
	int t = 15;
	p.circle = Circle(300, 480, 20);
	p.velocity = Vec2(0, 0);
	for (int i = 0; i <= 500000; i += Random(700, 1200)) {
		boost.push_back(Rect(Random(50, 500), -i, 50, 50));
	}
	for (int i = 0; i <= 500000; i += Random(700, 1200)) {
		barrier.push_back(Rect(Random(50, 500), -i, 50, 50));
	}
	for (int i = -400; i <= 500000; i += Random(100, 150)) {
		int t = Random(50, 550);
		scaffold.push_back(Line(Max(50, t - Random(50, 100)), -i, Min(550, t + Random(50, 100)), -i));
	}
	while (System::Update()) {
		p.draw(camera_pos);
		p.move(scaffold, camera_pos, effect, t);
		p.gravity();
		if (MouseL.down() && p.circle.y == 480) {
			p.accelerate(camera_pos, t);
			effect.push_back(Circle(Cursor::Pos(), 2));
		}
		for (Rect& r : barrier) {
			r.movedBy(0, camera_pos).draw(Palette::Red);
		}
		for (int i : step(barrier.size())) {
			if (barrier[i].intersects(p.circle)) {
				p.velocity.y = -t;
				barrier.erase(barrier.begin() + i);
			}
		}
		for (Rect& r : boost) {
			r.movedBy(0, camera_pos).draw(Palette::Lime);
		}
		for (int i : step(boost.size())) {
			if (boost[i].intersects(p.circle)) {
				t++;
				boost.erase(boost.begin() + i);
			}
		}
		for (Circle& c : effect) {
			c.draw(Color(255, 0, 0, 255 / c.r));
			c.r++;
		}
		if (!effect.empty()&&effect.front().r == 40) {
			effect.pop_front();
		}
		ground.movedBy(0, camera_pos).draw(Palette::Blue);
		left_wall.draw(Palette::Yellow);
		right_wall.draw(Palette::Yellow);
		if (p.circle.y + camera_pos < 300) {
			camera_pos = 300 - p.circle.y;
		}
		if (p.circle.y + camera_pos > 500) {
			camera_pos = 500 - p.circle.y;
		}
	}
}
