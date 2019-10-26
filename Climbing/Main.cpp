#include <Siv3D.hpp> // OpenSiv3D v0.4.1

struct player_ball {
	Circle circle;
	Color color;
	Vec2 velocity;
	Vec2 before_pos;
	int jump_frame;

	player_ball() {
		before_pos = circle.center = Vec2(325, 480);
		circle.r = 20;
		color = Palette::Orange;
		jump_frame = 0;
	}

	void move(Array<Rect>& scaffold, Array<Polygon>& jump_item, Array<Polygon>& gravity_item, double& camera_pos, int& gravity_frame) {
		if (Cursor::PosF().x < circle.x - 5) {
			velocity.x -= 0.2;
		}
		else if (Cursor::PosF().x > circle.x + 5) {
			velocity.x += 0.2;
		}
		else {
			velocity.x *= 0.9;
		}
		if (circle.x < 70) {
			circle.x = 70;
			velocity.x *= -0.3;
		}
		if (circle.x > 580) {
			circle.x = 580;
			velocity.x *= -0.3;
		}
		velocity.x *= 0.95;
		if (gravity_frame) {
			gravity_frame--;
			gravity(0.275);
		}
		else {
			gravity(0.45);
		}
		for (Rect& r : scaffold) {
			if (r.intersects(circle) && before_pos.y <= r.y - circle.r && r.x <= circle.x && circle.x < r.x + r.w) {
				circle.center.y = r.y - circle.r;
				velocity = Vec2(0, 0);
				if (MouseL.down()) {
					jump_frame = 8;
				}
			}
		}
		for (int i : step(jump_item.size())) {
			if (jump_item[i].intersects(circle)) {
				jump(25);
				jump_item.erase(jump_item.begin() + i);
				break;
			}
		}		
		for (int i : step(gravity_item.size())) {
			if (gravity_item[i].intersects(circle)) {
				gravity_frame = 360;
				//velocity.y = -10;
				gravity_item.erase(gravity_item.begin() + i);
				break;
			}
		}
		if (jump_frame && MouseL.pressed()) {
			jump(5 - (9 - jump_frame) * 0.5);
			jump_frame--;
		}
		else {
			jump_frame = 0;
		}
		if (camera_pos + circle.y < 300) {
			camera_pos = 300 - circle.y;
		}
		if (camera_pos + circle.y > 500) {
			camera_pos = 500 - circle.y;
		}
		before_pos = circle.center;
		velocity.y = Max(-30.0, velocity.y);
		velocity.y = Min(30.0, velocity.y);
		circle.center += velocity;
	}
	void draw(double& camera_pos) {
		circle.movedBy(0,camera_pos).draw(color);
	}
	void gravity(double d) {
		velocity.y += d;
	}
	void jump(double d) {
		velocity.y = Min(0.0, velocity.y);
		velocity.y -= d;
	}
};

void Main() {
	Scene::SetBackground(Color(0, 0, 50));
	Window::Resize(1000, 600);
	const Rect ground(50, 500, 550, 100);
	const Rect left_wall(0, 0, 50, 600);
	const Rect right_wall(600, 0, 400, 600);
	const Rect timer_box(650, 250, 300, 100);
	const Rect instruction_box(50, 450, 400, 100);
	const Rect play_box(550, 450, 400, 100);
	const Rect difficulty_easy_box(300, 100, 400, 100);
	const Rect difficulty_medium_box(300, 250, 400, 100);
	const Rect difficulty_hard_box(300, 400, 400, 100);
	const Rect return_box(50, 500, 140, 70);
	const Array<Color>color_palette {
		Color(200, 0, 0), Color(0, 200, 0), Color(100, 100, 200), Color(200, 200, 0), Color(200, 0, 200), Color(0, 200, 200)
	};
	const Array<Shape2D> title_star{
		Shape2D::Star(13, Vec2(30, 80)), Shape2D::Star(13, Vec2(250, 340)), Shape2D::Star(13, Vec2(900, 400)), Shape2D::Star(13, Vec2(850, 100)), Shape2D::Star(13, Vec2(400, 390)), Shape2D::Star(13, Vec2(100, 300))
	};
	const Polygon logo_s {
		{Vec2(170, 30), Vec2(260, 30), Vec2(260, 60), Vec2(200, 60), Vec2(200, 90), Vec2(260, 90), Vec2(260, 180), Vec2(170, 180), Vec2(170, 150), Vec2(230, 150), Vec2(230, 120), Vec2(170, 120)}
	};
	const Polygon logo_u {
		{Vec2(290, 30), Vec2(320, 30), Vec2(320, 150), Vec2(350, 150), Vec2(350, 30), Vec2(380, 30), Vec2(380, 180), Vec2(290, 180)}
	};
	const Polygon logo_p{
		{Vec2(410, 30), Vec2(500, 30), Vec2(500, 120), Vec2(440, 120), Vec2(440, 180), Vec2(410, 180)},
		{{Vec2(440, 60), Vec2(440, 90), Vec2(470, 90), Vec2(470, 60)}}
	};
	const Polygon logo_e{
		{Vec2(530, 30), Vec2(620, 30), Vec2(620, 60), Vec2(560, 60), Vec2(560, 90), Vec2(620, 90), Vec2(620, 120), Vec2(560, 120), Vec2(560, 150), Vec2(620, 150), Vec2(620, 180), Vec2(530, 180)}
	};
	const Polygon logo_r{
		{Vec2(650, 30), Vec2(725, 30), Vec2(740, 45), Vec2(740, 90), Vec2(725, 105), Vec2(740, 120), Vec2(740, 180), Vec2(710, 180), Vec2(710, 120), Vec2(680, 120), Vec2(680, 180), Vec2(650, 180)},
		{{Vec2(680, 60), Vec2(680, 90), Vec2(710, 90), Vec2(710, 60)}}
	};
	const Polygon logo_b{
		{Vec2(410, 210), Vec2(485, 210), Vec2(500, 225), Vec2(500, 270), Vec2(485, 285), Vec2(500, 300), Vec2(500, 360), Vec2(410, 360)},
		{{Vec2(440, 240), Vec2(440, 270), Vec2(470, 270), Vec2(470, 240)}, {Vec2(440, 300), Vec2(440, 330), Vec2(470, 330), Vec2(470, 300)}}
	};
	const Polygon logo_a{
		{Vec2(560, 210), Vec2(590, 210), Vec2(620, 240), Vec2(620, 360), Vec2(590, 360), Vec2(590, 300), Vec2(560, 300), Vec2(560, 360), Vec2(530, 360), Vec2(530, 240)},
		{{Vec2(560, 240), Vec2(560, 270), Vec2(590, 270), Vec2(590, 240)}}
	};
	const Polygon logo_l{
		{Vec2(650, 210), Vec2(680, 210), Vec2(680, 330), Vec2(740, 330), Vec2(740, 360), Vec2(650, 360)}
	};
	const Polygon logo_l2 = logo_l.movedBy(120, 0);

	const Font font30_regular(30, Typeface::Regular);
	const Font font40_regular(40, Typeface::Regular);
	const Font font100_regular(100, Typeface::Regular);
	const Font font255_regular(255, Typeface::Regular);
	const Font font60_medium(60, Typeface::Medium);
	const Font font70_heavy(70, Typeface::Heavy);

	Array<Rect>scaffold;
	Array<Polygon>jump_item;
	Array<Polygon>gravity_item;
	Array<std::pair<Polygon, Color>> star;
	player_ball ball;
	int status = 0;//0:タイトル 1:せつめい 2:難易度選択 10:ゲーム開始前 11:ゲーム本体 12:結果発表
	TextReader reader(U"result.txt");
	int timer = 0;
	double highest = 480;
	double camera_pos = 0;
	int gravity_frame = 0;
	scaffold.push_back(Rect(50, 500, 550, 100));
	for (int i = -Random(400, 450); i <= 500000; i += Random(0, 200)) {
		star.push_back(std::make_pair(Shape2D::NStar(Random(5, 10), 7, 4, Vec2(Random(110, 590), -i)).asPolygon(), color_palette[Random(0, 5)]));
	}
	for (int i = -Random(300, 450); i <= 500000; i += Random(100, 200)) {
		int tmp = Random(50, 600);
		int left = Max(50, tmp - Random(80, 130));
		int right = Min(600, tmp + Random(80, 130));
		scaffold.push_back(Rect(left, -i, right - left, 10));
	}
	for (int i = Random(1000, 5000); i <= 500000; i += Random(1000, 5000)) {
		jump_item.push_back(Shape2D::Star(25, Vec2(Random(65, 585), -i)).asPolygon());
	}
	for (int i = Random(1000, 5000); i <= 500000; i += Random(1000, 5000)) {
		gravity_item.push_back(Shape2D::Star(25, Vec2(Random(65, 585), -i)).asPolygon());
	}
	while (System::Update()) {
		if (status == 0) {
			logo_s.draw();
			logo_u.draw();
			logo_p.draw();
			logo_e.draw();
			logo_r.draw();
			logo_b.draw();
			logo_a.draw();
			logo_l.draw();
			logo_l2.draw();
			for (int i = 0; i < 6; i++) {
				title_star[i].draw(color_palette[i]);
			}
			if (instruction_box.intersects(Cursor::Pos())) {
				instruction_box.drawFrame(0, 10, Palette::Yellow);
				font70_heavy(U"あそびかた").drawAt(instruction_box.center());
				if (MouseL.down()) {
					status = 1;
				}
			}
			else {
				instruction_box.drawFrame(0, 5, Palette::Blue);
				font60_medium(U"あそびかた").drawAt(instruction_box.center());
			}
			if (play_box.intersects(Cursor::Pos())) {
				play_box.drawFrame(0, 10, Palette::Yellow);
				font70_heavy(U"あそぶ").drawAt(play_box.center());
				if (MouseL.down()) {
					status = 2;
				}
			}
			else {
				play_box.drawFrame(0, 5, Palette::Red);
				font60_medium(U"あそぶ").drawAt(play_box.center());
			}
		}
		else if (status == 1) {
			font40_regular(U"<ルール>\nとにかく高く昇っていこう！\n\nマウス操作で動かします\nクリックでジャンプ\n\n  :取ると大ジャンプします\n  :取ると重力が一定時間半分になります\n\n").drawAt(500, 300);
			Shape2D::Star(25, Vec2(140, 385)).draw(Color(155, 255, 0)).drawFrame(2, Color(255, 255, 0));
			Shape2D::Star(25, Vec2(140, 440)).draw(Color(255, 50, 100)).drawFrame(2, Color(255, 255, 0));
			if (return_box.intersects(Cursor::Pos())) {
				return_box.drawFrame(0, 10);
				font40_regular(U"もどる").drawAt(return_box.center());
				if (MouseL.down()) {
					status = 0;
				}
			}
			else {
				return_box.drawFrame(0, 5);
				font30_regular(U"もどる").drawAt(return_box.center());
			}
		}
		else if (status == 2) {
			status = 11;
		}
		else if (status == 10) {

		}
		else if (status == 11) {
			ball.move(scaffold, jump_item, gravity_item, camera_pos, gravity_frame);
			for (std::pair<Polygon, Color>& p : star) {
				p.first.movedBy(0, camera_pos / 2).draw(p.second);
			}
			ball.draw(camera_pos);
			for (Rect& r : scaffold) {
				r.movedBy(0, camera_pos).draw(Color(255, 255, 200));
			}
			for (Polygon& p : jump_item) {
				p.movedBy(0, camera_pos).draw(Color(155, 255, 0)).drawFrame(2, Color(255, 255, 0));
			}
			for (Polygon& p : gravity_item) {
				p.movedBy(0, camera_pos).draw(Color(255, 50, 100)).drawFrame(2, Color(255, 255, 0));
			}
			ground.movedBy(0, camera_pos).draw(Color(0, 100, 20));
			left_wall.draw(Color(0, 0, 10));
			right_wall.draw(Color(0, 0, 10));
			timer_box.drawFrame(3);
			highest = Min(highest, ball.circle.y);
			Rect(0, 600 - gravity_frame * 5 / 3, 50, gravity_frame * 5 / 3).draw(Arg::top = Color(255, 130, 255), Arg::bottom = Color(255, 50, 255));
			font100_regular(U"{}:{:0>2}"_fmt(timer / 60 / 60, timer / 60 % 60)).drawAt(timer_box.center());
			timer++;
			ClearPrint();
			Print << ball.circle.center;
		}
		else if (status == 12) {

		}
	}
}
