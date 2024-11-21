#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace sf;
using namespace std;

Vector2i mouse_pos; //текущие координаты курсора
Font font;

struct hero {
	int hits = 10, max_hits = 10, CD = 12, atk = 5, dmgb = 3, dmgd = 4;
};
hero hero1;

struct NPC {
	NPC() {}
	void changeAll(string name, bool gender, int x, int y, int rad) {
		this->name = name;
		this->coords.x = x;
		this->coords.y = y;
		this->texture.loadFromFile(name + ".png");
		this->image.setTexture(texture);
		this->image.setOrigin(rad, rad);
		this->image.setPosition(coords);
		this->gender = gender;
	}
	int dia_num = 0;
	bool gender = true;
	string name;
	Sprite image;
	Texture texture;
	Vector2f coords;
};

int dialog(NPC pp) {
	RenderWindow d_window(sf::VideoMode(600, 600), L"Dialog", Style::Default);
	Vector2u size = d_window.getSize();
	bool spspr, sps = false, choose = false, ext = false;
	string line;
	ifstream in(pp.name+"_std.txt");
	getline(in, line);
	int n = stoi(line), it = 0;
	getline(in, line);
	Text phrase, mf;
	phrase.setFont(font);
	phrase.setString(line);
	phrase.setCharacterSize(50);
	phrase.setFillColor(sf::Color::Black);
	phrase.setPosition(10, 10);
	mf.setFont(font);
	mf.setString("Visitors    stranges    nothing");
	mf.setCharacterSize(50);
	mf.setFillColor(sf::Color::Black);
	mf.setPosition(10, size.y - 110);
	Sprite background;
	Texture txt;
	if (not txt.loadFromFile(pp.name + "_dia.png")) return 1;
	background.setTexture(txt);	
	while (d_window.isOpen()) {
		Event event;
		while (d_window.pollEvent(event)) {
			if (event.type == Event::Closed) d_window.close();
			else if (event.type == Event::Resized) {
				d_window.setSize(size);
			}
		}

		spspr = sps;
		sps = Keyboard::isKeyPressed(Keyboard::Space);

		if (choose) {
			if (Keyboard::isKeyPressed(Keyboard::Num1)) {
				getline(in, line);
				it = stoi(line);
				for (int i = 0; i < it; i++) {
					getline(in, line);
				}
				choose = false;
				sps = true;
				spspr = false;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Num2)) {
				getline(in, line);
				getline(in, line);
				it = stoi(line);
				for (int i = 0; i < it; i++) {
					getline(in, line);
				}
				choose = false;
				sps = true;
				spspr = false;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Num3)) {
				getline(in, line);
				getline(in, line);
				getline(in, line);
				it = stoi(line);
				for (int i = 0; i < it; i++) {
					getline(in, line);
				}
				choose = false;
				sps = true;
				spspr = false;
			}
		}
		if ((not choose and sps and not spspr) or ext) {
			ext = false;
			getline(in, line);
			if (line == "4") return 1;
			else if (line == "0") {
				in.close();
				in.open(pp.name + "_std.txt");
				for (int i = 0; i < n; i++) {
					getline(in, line);
				}
				ext = true;
				continue;
			}
			else if (line == "1") {
				getline(in, line);
				phrase.setString(line);
				phrase.setPosition(10, 10);
			}
			else if (line == "2") {
				getline(in, line);
				phrase.setString(line);
				phrase.setPosition(10, size.y-110);
			}
			else if (line == "3") {
				choose = true;
				getline(in, line);
				phrase.setString(line);
				phrase.setPosition(10, 10);
			}
		}

		d_window.clear(Color::Blue);
		d_window.draw(background);
		d_window.draw(phrase);
		if (choose) d_window.draw(mf);
		d_window.display();
	}
	return 1;
}

void inventory() {
	RenderWindow i_window(sf::VideoMode(600, 600), L"Inventory", Style::Default);
	Vector2u size = i_window.getSize();
	bool mpr, mt = false;
	RectangleShape attack(Vector2f(600.f, 300.f));
	attack.setFillColor(Color::Green);
	RectangleShape damage(Vector2f(600.f, 300.f));
	damage.setFillColor(Color::Red);
	damage.setPosition(0.f, 300.f);
	Text text_atk;
	text_atk.setFont(font);
	text_atk.setString("attack bonus: " + to_string(hero1.atk));
	text_atk.setCharacterSize(50);
	text_atk.setFillColor(sf::Color::Black);
	text_atk.setPosition(10, 10);
	while (i_window.isOpen()) {
		Event event;
		while (i_window.pollEvent(event)) {
			if (event.type == Event::Closed) i_window.close();
			else if (event.type == Event::Resized) {
				i_window.setSize(size);
			}
		}

		mpr = mt; mt = Mouse::isButtonPressed(Mouse::Left);

		if (mt and not mpr) {
			mouse_pos = Mouse::getPosition(i_window);
			mpr = false;
			if (mouse_pos.x > 0 and mouse_pos.x < 600 and mouse_pos.y > 0 and mouse_pos.y < 600) {
				hero1.atk -= 1;
				text_atk.setString("attack bonus: " + to_string(hero1.atk));
				cout << 1;
			}
		}
		i_window.clear(Color::Blue);
		i_window.draw(attack);
		i_window.draw(damage);
		i_window.draw(text_atk);
		i_window.display();
	}
}

int main() {
	SoundBuffer buffer;//крч в ближайших нескольких строках обитает подключение звука
	if (!buffer.loadFromFile("phonk.mp3")) return -1;
	Sound sound;
	sound.setBuffer(buffer);
	sound.setVolume(5);
	sound.play();
	sound.setLoop(true);
	if (!font.loadFromFile("font.ttf")) return 0;
	Vector2u razm;//размеры окна по х и у
	razm.x = 1000;
	razm.y = 1000;
	Vector2u razm_old = razm; //размеры окна до последнего изменения
	RenderWindow window(sf::VideoMode(razm.x, razm.y), L"Illusion", Style::Default);//создаём окно
	window.setVerticalSyncEnabled(true);//изначально окно вертикально, чтоб без рандомных углов
	View view(FloatRect(0, 0, razm.x, razm.y));//создаём камеру, размером с окно
	Text text_main;
	text_main.setFont(font);
	text_main.setString("Hits: " + to_string(hero1.hits) + "/" + to_string(hero1.max_hits));
	text_main.setCharacterSize(100);
	text_main.setFillColor(sf::Color::Red);
	text_main.setPosition(10, razm.y - 110);
	int rad = 50;// радиус круга-модельки персонажа
	Sprite hero;
	Texture SF;
	if (not SF.loadFromFile("zxc.png")) return -1;
	hero.setOrigin(rad, rad);
	hero.setTexture(SF);
	Sprite map;
	Texture minimap;
	if (not minimap.loadFromFile("map.png")) return -1;
	map.setTexture(minimap);
	map.setPosition(0, 0);
	double vx = 0; //скорость персонажа
	double vy = 0;
	double x = 500; //координаты персонажа
	//(не вектор, потому что они были первые и обошлось просто х и у)
	double y = 500;
	double len = 1;//задача переменной, которая впоследствие будет нужна для рассчёта скорости персонажа
	double movespeed = 7.0; //скорость героя
	Vector2i mouse_old; //координаты курсора кадра назад
	Vector2i viewcenter; //центр камеры (вид)
	viewcenter.x = razm.x / 2; //изначально в центре
	viewcenter.y = razm.y / 2;
	hero.setPosition(viewcenter.x, viewcenter.y);//начальная позиция героя
	bool isckm = false;//проверка, нажата ли левая кнопка мыши
	bool isckm_old = false; //была ли нажата ли левая кнопка мыши в прошлом кадре
	int dx = 0;//перемещение камеры по х
	int dy = 0;//по у
	bool is_mouse_in_window;//в окне ли курсор?
	bool is_floor;//находится ли курсор в допустимой для перемещения области
	const int num_obj = 4;
	int check;

	int borders[num_obj][4]{//массив стенок
	{0,0,50,2200},
	{0,0,2200,50},
	{2150,0,2200,2200},
	{0,2150,2200,2200}
	}; // весёлые задавушки))) и дальше тоже))))))))
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int npc_c = 1;
	NPC mas[1];
	mas[0].changeAll("zxc", true, 100, 100, rad);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	while (window.isOpen()) {
		Event event;

		is_floor = true;
		mouse_pos = Mouse::getPosition(window); //смотрим, где курсор
		is_mouse_in_window = mouse_pos.x >= 0 and mouse_pos.x <= razm.x and mouse_pos.y >= 0 and mouse_pos.y <= razm.y;

		////////////////////////////////////////////СОБЫТИЯ В ОКНЕ/////////////////////////////////////////////////////////////////////////////////////
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) window.close();
			else if (event.type == Event::Resized) {
				razm = window.getSize();
				view.setSize(razm.x, razm.y);
				dx -= (razm.x - razm_old.x) / 2;//меняем восприятие мыши при изменении размера окна
				dy -= (razm.y - razm_old.y) / 2;//не знаю, как это работает, но работает => не трогаем
				razm_old = razm;
				text_main.setPosition(10 + dx, razm.y - 110 + dy);
			}
		}

		isckm_old = isckm;
		if (Mouse::isButtonPressed(Mouse::Middle) and is_mouse_in_window) isckm = true; //нажата ли левая кнопка мыши
		else isckm = false;

		if (isckm and isckm_old) {
			check = viewcenter.x - mouse_pos.x + mouse_old.x;
			if (check >= razm.x / 2 and check + razm.x / 2 <= map.getTexture()->getSize().x) {
				viewcenter.x = check;
				dx += (mouse_old.x - mouse_pos.x);
			}
			check = viewcenter.y - mouse_pos.y + mouse_old.y;
			if (check >= razm.y / 2 and check + razm.y / 2 <= map.getTexture()->getSize().y) {
				viewcenter.y = check;
				dy += (mouse_old.y - mouse_pos.y);
			}
			text_main.setPosition(10 + dx, razm.y - 110 + dy);
			view.setCenter(viewcenter.x, viewcenter.y);
			text_main.setPosition(10 + dx, razm.y - 110 + dy);
		} //если центральная кнопка мыши удерживается - двигаем камеру за курсором

		//////////////////////////////////////////////ПЕРЕДВИЖЕНИЕ/////////////////////////////////////////////////////////////////////////////////////

		if (Mouse::isButtonPressed(Mouse::Right) and is_mouse_in_window) {
			len = sqrt(pow((mouse_pos.x - x + dx), 2) + pow((mouse_pos.y - y + dy), 2));
			vx = (mouse_pos.x - x + dx) / len;
			vy = movespeed * (mouse_pos.y - y + dy) / len;
			if (vy <= 0) hero.setRotation(asin(vx) * 57.3);
			else hero.setRotation(180 - asin(vx) * 57.3);
			vx *= movespeed;
		}//по теореме пифагора и подобиям задаём скорости по х и у, найдя синусы косинусы там...
		else if (Keyboard::isKeyPressed(Keyboard::S) and is_mouse_in_window) {
			vx = 0;
			vy = 0;
		}

		for (int i = 0; i < num_obj; i++) {
			if (borders[i][0] <= (x + vx) and borders[i][2] >= (x + vx) and borders[i][1] <= (y + vy) and borders[i][3] >= (y + vy)) {
				is_floor = false;
				break;
			}
		}

		if (is_floor and len > rad) { x += vx; y += vy; len -= movespeed; }//двигаем, если не стенка

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if (Mouse::isButtonPressed(Mouse::Left) and is_mouse_in_window) {
			for (int i = 0; i < npc_c; i++) {
				if (abs(mas[i].coords.x - mouse_pos.x - dx) < rad and abs(mas[i].coords.y - mouse_pos.y - dy) < rad) {
					dialog(mas[i]);
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		mouse_old = mouse_pos;

		hero.setPosition(x, y); //возможно возникает вопрос: а зачем задавать координаты, если можно
		//использовать функцию .move, а я отвечу - да по фану, мне просто координаты нужны для
		//расчётов выше, поэтому они есть.

		window.setView(view); //переводимся на вид с камеры
		window.clear(Color::Blue); //чистим, чтобы не накладывались рисунки
		window.draw(map);
		window.draw(hero); //рисуем собсна героя
		window.draw(text_main);
		for (int i = 0; i < npc_c; i++) {
			window.draw(mas[i].image);
		}
		window.display();//хз, так надо

		if (Keyboard::isKeyPressed(Keyboard::I) and is_mouse_in_window) {
			inventory();
			text_main.setString("Hits: " + to_string(hero1.hits) + "/" + to_string(hero1.max_hits));
			cout << "allow";
		}
	}
	return 0;
}