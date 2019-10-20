#include <Siv3D.hpp> // OpenSiv3D v0.4.1

class playable_character {
public:
	Circle circle;
	Vec2 velocity;
	Vec2 before_pos;
	void draw(double& camera_pos) {
		circle.movedBy(0, camera_pos).draw(Palette::Orange);
	}
	void move(Array<Line>& scaffold, double& camera_pos, Array<Circle>& effect) {
		before_pos = circle.center;
		circle.center += velocity;
		if (circle.y >= 680) {
			circle.y = 680;
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
		for (Line& l : scaffold) {
			l.movedBy(0, camera_pos).draw(5, Palette::Blue);
			if (l.intersects(circle) && velocity.y >= 0 && before_pos.y <= l.begin.y - circle.r) {
				velocity = Vec2(0, 0);
				circle.y = l.begin.y - circle.r;
				if (MouseL.down()) {
					accelerate(camera_pos);
					effect.push_back(Circle(Cursor::Pos(), 2));
				}
			}
		}
	}
	void gravity() {
		velocity.y += 0.5;
	}
	void accelerate(double& camera_pos) {
		velocity = (Cursor::PosF().movedBy(0, -camera_pos) - circle.center) / (Cursor::PosF().movedBy(0, -camera_pos) - circle.center).length() * 10;
		velocity.y *= 2;
	}
};

void Main() {
	Scene::SetBackground(ColorF(0.8, 0.9, 1.0));
	Window::Resize(800, 800);
	const Rect ground(0, 700, 600, 200);
	const Rect left_wall(0, 0, 50, 800);
	const Rect right_wall(550, 0, 50, 800);
	Array<Rect>barrier;
	Array<Line>scaffold;
	Array<Circle>effect;
	playable_character p;
	double camera_pos = 0;
	p.circle = Circle(300, 680, 20);
	p.velocity = Vec2(0, 0);
	for (int i = 0; i <= 50000; i += Random(100, 500)) {
		barrier.push_back(Rect(Random(50, 500), -i, 50, 50));
	}
	for (int i = -600; i <= 50000; i += Random(40, 180)) {
		int t = Random(50, 550);
		scaffold.push_back(Line(Max(50, t - Random(40, 80)), -i, Min(550, t + Random(40, 80)), -i));
	}
	while (System::Update()) {
		p.draw(camera_pos);
		p.move(scaffold, camera_pos, effect);
		p.gravity();
		if (MouseL.down() && p.circle.y == 680) {
			p.accelerate(camera_pos);
			effect.push_back(Circle(Cursor::Pos(), 2));
		}
		//for (Rect& r : barrier) {
		//	r.movedBy(0, camera_pos).draw(Palette::Red);
		//}		
		
		for (Circle& c : effect) {
			c.draw(Color(255, 0, 0, 255 / c.r));
			c.r++;
		}
		if (!effect.empty()&&effect.front().r == 40) {
			effect.pop_front();
		}
		ground.movedBy(0, camera_pos).draw(Palette::Blue);
		left_wall.draw(Palette::Black);
		right_wall.draw(Palette::Black);
		if (p.circle.y + camera_pos < 500) {
			camera_pos = 500 - p.circle.y;
		}
		if (p.circle.y + camera_pos > 700) {
			camera_pos = 700 - p.circle.y;
		}
	}
}
