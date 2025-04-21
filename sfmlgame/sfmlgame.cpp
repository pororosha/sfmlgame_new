#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <random>

using namespace sf;
using namespace std;

//bool isInMass(int mas[], int element) {
//	for (int i = 0; i < sizeof(mas); i++) { if (mas[i] == element) return true; }
//	return false;
//}

Font font;

struct Hero {

	int hits = 10, max_hits = 10, CD = 12, atk = 5, dmgb = 3, dmgd = 4;
};
Hero hero1;

struct Guard {
	Guard() {}
	void changeAll(bool os, double x0, double y0, int L, bool napr) {
		texture.loadFromFile("guard.png");
		image.setOrigin(50, 50);
		image.setTexture(texture);
		this->os = os;
		this->coords = Vector2f(x0, y0);
		this->napr = napr;
		if (os) this->minmax = Vector2f(x0 - L * (napr-1), x0 + L * napr);
		else this->minmax = Vector2f(y0 - L * (napr - 1), y0 + L * napr);
		cout << minmax.x << "  " << minmax.y << "\n";
		//задача картинок и текстур
	}
	bool Hero_check(Vector2f pos) {
		if (os) return (abs(pos.y - coords.y) < (hall_width/2 + 30)/*ширина героя*/) and (pos.x > minmax.x-100 and pos.x < minmax.y+100) and ((pos.x - coords.x) * (2*napr-1) > 0);
		else return (abs(pos.x - coords.x) < (hall_width/2 + 30)) and (pos.y > minmax.x-100 and pos.y < minmax.y+100) and ((pos.y - coords.y) * (2*napr-1) > 0);
	}
	void walk(double time) {
		if (os) {
			coords.x += base_speed * time;
			if (not napr) coords.x -= 2 * base_speed * time;
		}
		else {
			coords.y += base_speed * time;
			if (not napr) coords.y -= 2 * base_speed * time;
		}
	}
	bool HSM(double time) {
		timer_H += time;
		if (timer_H >= wait_time) {
			timer_H = 0;
			return true;
		}
		else return false;
	}
	bool wait(double time) {
		timer_w += time;
		if (timer_w >= wait_time) {
			timer_w = 0;
			napr = not napr;
			return false;
		}
		else return true;
	}
	bool turn(Vector2f pos, double time) {
		if (is_wait) {
			is_wait = wait(time);
		}
		else {
			walk(time);
			x = coords.x * os + coords.y * (not os);
			is_wait = (x <= minmax.x and not napr) or (x >= minmax.y and napr);
		}
		if (Hero_check(pos)) { if (HSM(time)) return true; }
		else timer_H = 0;
		image.setPosition(coords);
		image.setRotation(360-180*napr-os*90);
		return false;
	}
	Sprite image;
	Texture texture;
	Vector2f coords;//координаты
	Vector2f minmax;
	float base_speed = 0.21;
	float wait_time = 1000; //время в милисекундах перед разворотом (простой)
	float HSM_time = 1000; //время в милисекундах перед атакой
	float timer_H = 0; //текущее время
	float timer_w = 0; //:)
	bool os;//при создании true - горизонталь, false - вертикаль
	bool napr = true;//идёт он изначально по координате или против (по умолчанию по)
	bool is_HSM = false;
	bool is_wait = false;
	float hall_width = 250;
	bool isActive = true;
	int x; //для промежуточных значений
};

bool fight(bool aboba) {
	//cout << "GOOOOAAAAAAAAL" << "\n";
	return true;
}

struct NPC {
	int dia_n = 0;
	bool gender = true, dia = false;
	bool q1 = false;
	bool q2 = false;
	string name;
	Sprite image;
	Texture texture;
	Vector2f coords;
	SoundBuffer  buffer;
	Sound sound;
	NPC() {}
	NPC(string nam, bool gen, int x, int y, int rad) {
		name = nam;
		coords.x = x;
		coords.y = y;
		if (texture.loadFromFile(name + ".png")) cout << name + ".png" " - success\n";
		image.setTexture(texture);
		image.setTextureRect(IntRect(0, 0, 100, 100));
		image.setOrigin(rad, rad);
		image.setPosition(coords);
		gender = gen;
		if (gender) buffer.loadFromFile("male_voice.mp3");
		else buffer.loadFromFile("female_voice.mp3");
		sound.setBuffer(buffer);
		sound.setVolume(10);
	}
	void changeAll(string nam, bool gen, int x, int y, int rad) {
		name = nam;
		coords.x = x;
		coords.y = y;
		if (texture.loadFromFile(name + ".png")) cout << name + ".png" " - success\n";
		image.setTexture(texture);
		image.setTextureRect(IntRect(0, 0, 100, 100));
		image.setOrigin(rad, rad);
		image.setPosition(coords);
		gender = gen;
		if (gender) buffer.loadFromFile("male_voice.mp3");
		else buffer.loadFromFile("female_voice.mp3");
		sound.setBuffer(buffer);
		sound.setVolume(10);
	}
	bool dialog() {
		RenderWindow d_window(sf::VideoMode(750, 750), this->name, Style::Default);
		Vector2u size = d_window.getSize();
		Vector2i mp = Mouse::getPosition(d_window);
		bool spspr, sps = false, choose = false, ext = false;
		string line;
		Texture texture;
		texture.loadFromFile("fight_icon.png");
		Sprite image;
		image.setTexture(texture);
		image.setPosition(675, 340);
		ifstream in(this->name + "_std.txt");
		if (this->dia_n > 2) {
			in.close();
			in.open(this->name + "_hsm.txt");
		}
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
		mf.setString("What happened?  Strangers?    Bye!  ");
		mf.setCharacterSize(50);
		mf.setFillColor(sf::Color::Black);
		mf.setPosition(10, size.y - 110);
		Sprite background;
		Texture txt;
		if (not txt.loadFromFile(this->name + "_dia.png")) return 1;
		background.setTexture(txt);
		sound.play();
		if (dia_n > 0) {
			for (int i = 0; i < n; i++) {
				getline(in, line);
			}
		}
		while (d_window.isOpen()) {

			Event event;
			while (d_window.pollEvent(event)) {
				if (event.type == Event::Closed) d_window.close();
				else if (event.type == Event::Resized) {
					d_window.setSize(size);
				}
			}

			spspr = sps;
			mp = Mouse::getPosition(d_window);
			sps = (Keyboard::isKeyPressed(Keyboard::Space) or (Mouse::isButtonPressed(Mouse::Left) and (mp.x > 0 and mp.y > 0 and mp.x < size.x and mp.y < size.y)));

			if (Mouse::isButtonPressed(Mouse::Left) and mp.x>675 and mp.x<750 and mp.y>340 and mp.y<415) {
				return true;
			}

			if (choose) {
				if (Keyboard::isKeyPressed(Keyboard::Num1) or (Mouse::isButtonPressed(Mouse::Left) and (mp.x > 0 and mp.y > size.y-220 and mp.x < size.x/3 and mp.y < size.y))) {
					getline(in, line);
					it = stoi(line);
					for (int i = 0; i < it; i++) {
						getline(in, line);
					}
					choose = false;
					sps = true;
					spspr = false;
				}
				else if (Keyboard::isKeyPressed(Keyboard::Num2) or (Mouse::isButtonPressed(Mouse::Left) and (mp.x > size.x/3 and mp.y > size.y - 220 and mp.x < size.x*2/3 and mp.y < size.y))) {
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
				else if (Keyboard::isKeyPressed(Keyboard::Num3) or (Mouse::isButtonPressed(Mouse::Left) and (mp.x > size.x*2/3 and mp.y > size.y - 220 and mp.x < size.x and mp.y < size.y))) {
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
				if (line == "4") { 
					d_window.close(); 
					this->dia = true;
					this->dia_n += 1;
					return false;
				}
				else if (line == "5") {
					d_window.close();
					this->dia = true;
					this->dia_n += 1;
					return false;
				}
				else if (line == "0") {
					in.close();
					in.open(this->name + "_std.txt");
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
					this->sound.play();
				}
				else if (line == "2") {
					getline(in, line);
					phrase.setString(line);
					phrase.setPosition(10, size.y - 110);
				}
				else if (line == "3") {
					choose = true;
					getline(in, line);
					phrase.setString(line);
					phrase.setPosition(10, 10);
					this->sound.play();
					getline(in, line);
					int lish = stoi(line);
					for (int i = 0; i < lish; i++) {
						getline(in, line);
					}
				}
				else {
					phrase.setString(line);
					if (phrase.getPosition().y == 10) this->sound.play();
				}
			}

			d_window.clear(Color::Blue);
			d_window.draw(background);
			d_window.draw(phrase);
			d_window.draw(image);
			if (choose) d_window.draw(mf);
			d_window.display();
		}
	}
	void set_frame(int frame) {
		this->image.setTextureRect(IntRect(frame * 100, 0, 100, 100));
	}
};

struct Item {
	int rad = 50;
	Vector2f coords;
	string type, name = "null";
	int image_num;
	Sprite image;
	Texture texture;
	/*Item(int x, int y, string type) {
		image_num = rand() % 3 + 1;
		coords = Vector2f(x, y);
		this->type = type;
		texture.loadFromFile(type + "_" + to_string(image_num) + ".png");
		image.setOrigin(rad / 2, rad / 2);
		image.setTexture(texture);
		image.setPosition(coords);
	}
	Item(int x, int y, string type, string name) {
		image_num = rand() % 3 + 1;
		coords = Vector2f(x,y);
		this->type = type;
		this->name = name;
		texture.loadFromFile(type + "_" + to_string(image_num) + ".png");
		image.setOrigin(rad / 2, rad / 2);
		image.setTexture(texture);
		image.setPosition(coords);
	}*/
	Item() {};
	void changeAll(int x, int y, string typ, string nam) {
		image_num = rand() % 3 + 1;
		coords = Vector2f(x, y);
		type = typ;
		name = nam;
		texture.loadFromFile(type + "_" + to_string(image_num) + ".png");
		image.setOrigin(rad / 2, rad / 2);
		image.setTexture(texture);
		image.setPosition(coords);
	}
	void changeAll(int x, int y, string typ) {
		image_num = rand() % 3 + 1;
		coords = Vector2f(x, y);
		type = typ;
		texture.loadFromFile(type + "_" + to_string(image_num) + ".png");
		image.setOrigin(rad / 2, rad / 2);
		image.setTexture(texture);
		image.setPosition(coords);
	}
	//если бомба - выводит -1: надо начать наводиться/ письмо - выводит письмо и выводит 0/ хилка - выводит исцеляемое значение
	int use() {
		if (type == "letter") {
			RenderWindow i_window(sf::VideoMode(608, 857), L"Inventory", Style::None);
			Text text;
			ifstream in(name + ".txt");
			string line;
			int y = 10;
			text.setFont(font);
			text.setCharacterSize(50);
			text.setFillColor(sf::Color::Black);
			text.setPosition(15, 10);

			Sprite paper;
			Texture bumaga;
			if (not bumaga.loadFromFile("letter.png")) return -1;
			paper.setTexture(bumaga);
			paper.setPosition(0, 0);
			i_window.clear(Color::Blue);
			i_window.draw(paper);

			while (getline(in, line)) {
				text.setString(line);
				text.setPosition(15, y);
				y += 60;
				i_window.draw(text);
			}
			i_window.display();
			while (i_window.isOpen()) {
				if (Keyboard::isKeyPressed(Keyboard::Escape)) i_window.close();
			}
			return 0;
		}
		else if (type == "healing") {
			return rand() % 4 + rand() % 4 + 3;
		}
		else {
			return -1;
		}
	}
	void pick() {
		if (type == "letter") {
			RenderWindow i_window(sf::VideoMode(608, 857), L"letter", Style::None);
			Text text;
			ifstream in(name + ".txt");
			string line;
			int y = 10;
			text.setFont(font);
			text.setCharacterSize(50);
			text.setFillColor(sf::Color::Black);
			text.setPosition(15, 10);

			Sprite paper;
			Texture bumaga;
			if (not bumaga.loadFromFile("letter.png")) {}
			paper.setTexture(bumaga);
			paper.setPosition(0, 0);
			i_window.clear(Color::Blue);
			i_window.draw(paper);

			while (getline(in, line)) {
				text.setString(line);
				text.setPosition(15, y);
				y += 60;
				i_window.draw(text);
			}
			i_window.display();
			while (i_window.isOpen()) {
				if (Keyboard::isKeyPressed(Keyboard::Escape)) i_window.close();
			}
		}
	}
};

struct Room {// номер комнаты = её индекс в массиве
	int rad = 50; //радиус персонажа, главное не забыть)))
	Vector2i coord0;
	Vector2i coord1;
	bool isObs = false;
	vector<vector<int>> obstackles;
	vector<NPC> characters;
	bool isPerso = false;
	vector<Item> items;
	bool isItems = false;
	vector<vector<int>> escapes;
	// 0 - сверху/справа/снизу/слева (1/2/3/4)  
	// 1 - начальная координата относительно левого верхнего угла комнаты
	// 2 - и его ширина
	// 3 - в какую комнату переходим
	void changeAll(int x0, int y0, int x, int y) {
		this->coord0 = { x0, y0 };
		this->coord1 = { x,y };
	}
	void addEscape(int position, int doors, int dist, int next_room) {
		escapes.push_back({ position, doors , dist, next_room });
	}
	void addObstackle(int x0, int y0, int x, int y) {
		isObs = true;
		obstackles.push_back({ x0, y0, x, y });
	}
	void addNPC(NPC ind) {
		isPerso = true;
		characters.push_back(ind);
	}
	void addItem(Item thing) {
		isItems = true;
		items.push_back(thing);
	}
	Vector3i isAbleToGo(Vector2f curPosition, Vector2f nextPosition) {
		bool x = true;
		bool y = true;
		for (size_t i = 0; i < obstackles.size(); i++) {
			if (x and (curPosition.y + rad > obstackles[i][1] and curPosition.y - rad < obstackles[i][3]) and (nextPosition.x + rad > obstackles[i][0] and nextPosition.x - rad < obstackles[i][2])) x = false;
			if (y and (curPosition.x + rad > obstackles[i][0] and curPosition.x - rad < obstackles[i][2]) and (nextPosition.y + rad > obstackles[i][1] and nextPosition.y - rad < obstackles[i][3])) y = false;
			if (not (y and x)) break;
		}
		return { 0 + (nextPosition.x - rad > coord0.x and nextPosition.x + rad < coord1.x and x), 0 + (nextPosition.y - rad > coord0.y and nextPosition.y + rad < coord1.y and y), isDoor(nextPosition) };
	}
	int isDoor(Vector2f nextPosition) {
		for (size_t i = 0; i < escapes.size(); i++) {
			if ((escapes[i][0] == 1 and nextPosition.y - rad < coord0.y and (nextPosition.x - rad > coord0.x + escapes[i][2] and nextPosition.x + rad < coord0.x + escapes[i][2] + escapes[i][1]))
				or (escapes[i][0] == 2 and nextPosition.x + rad > coord1.x and (nextPosition.y - rad > coord0.y + escapes[i][2] and nextPosition.y + rad < coord0.y + escapes[i][2] + escapes[i][1]))
				or (escapes[i][0] == 3 and nextPosition.y + rad > coord1.y and (nextPosition.x - rad > coord0.x + escapes[i][2] and nextPosition.x + rad < coord0.x + escapes[i][2] + escapes[i][1]))
				or (escapes[i][0] == 4 and nextPosition.x - rad < coord0.x and (nextPosition.y - rad > coord0.y + escapes[i][2] and nextPosition.y + rad < coord0.y + escapes[i][2] + escapes[i][1]))) {
				//cout << "YES" << "\n";
				return escapes[i][3];
			}
		}
		return -1;
	}
	bool isInRoom(Vector2f Position) {
		bool is = Position.x < coord1.x and Position.y < coord1.y and Position.x > coord0.x and Position.y > coord0.y;
		//if (not is) cout << Position.y << "\n";
		return (is);
	}
	Room() {}
};

int inventory(vector<Item> items, int hits, int max_hits) {
	RenderWindow i_window(sf::VideoMode(460, 110 * (int(items.size() / 4) + (items.size() % 4 > 0) + 1) + 50), L"Inventory", Style::None);
	/*attack.setFillColor(Color::Green);
	RectangleShape damage(Vector2f(600.f, 300.f));
	damage.setFillColor(Color::Red);
	damage.setPosition(0.f, 300.f);*/
	Vector2i mouse_pos;
	Text text_atk;
	text_atk.setFont(font);
	text_atk.setString("hits: " + to_string(hits) + "/" + to_string(max_hits));
	text_atk.setCharacterSize(50);
	text_atk.setFillColor(sf::Color::Black);
	text_atk.setPosition(10, i_window.getSize().y - 50);
	i_window.clear(Color::Blue);
	Texture texture;
	i_window.clear(Color::Black);
	texture.loadFromFile("for_inventory.png");
	Sprite image;
	image.setTexture(texture);
	int n = 0;
	while (n < i_window.getSize().y) {
		image.setPosition(0, n);
		i_window.draw(image);
		n += 110;
	}
	texture.loadFromFile("for_inventory_hits.png");
	image.setTexture(texture);
	image.setPosition(0, i_window.getSize().y - 50);
	i_window.draw(image);
	i_window.draw(text_atk);
	Sprite img;
	for (int i = 0; i < items.size(); i++) {
		img.setTexture(items[i].texture);
		img.setPosition(40 + (i % 4) * 110, 30 + (i / 4 - (i % 4 == 3)) * 110);
		i_window.draw(img);
	}
	i_window.display();
	while (i_window.isOpen()) {

		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			i_window.close();
			return -1;
		}

		//Mouse::isButtonPressed(Mouse::Left);

		if (Mouse::isButtonPressed(Mouse::Left)) {
			mouse_pos = Mouse::getPosition(i_window);
			for (size_t i = 0; i < items.size(); i++) {
				if (mouse_pos.x > 20 + 110 * (i % 4)
					and mouse_pos.x < 110 * (i % 4 + 1)
					and mouse_pos.y > 10 + 110 * (i / 4 - (i % 4 == 4))
					and mouse_pos.y < 100 + 110 * (i / 4 - (i % 4 == 4))) {
					return i;
				}
			}
		}
	}
}

//	void fillRoomsMassive() 

int main() {
	Vector2i mouse_pos; //текущие координаты курсора
	Vector2u size = { 1000, 1000 };
	Image icon;
	Sprite load;
	Texture load_t;
	if (not load_t.loadFromFile("loading.png")) return -1;
	load.setTexture(load_t);
	load.setPosition(0, 0);
	if (!icon.loadFromFile("icon.png")) return 1;
	RenderWindow window(sf::VideoMode(size.x, size.y), L"Illusion", Style::Default);//создаём окно
	window.setVerticalSyncEnabled(true);//изначально окно вертикально, чтоб без рандомных углов
	window.setIcon(50, 50, icon.getPixelsPtr());
	window.draw(load);
	window.display();
	//крч в ближайших нескольких строках обитает подключение звука
	Music music;
	music.openFromFile("main_theme.mp3");
	music.setVolume(25);
	music.play();
	music.setLoop(true);
	//вот до сюда)
	if (!font.loadFromFile("font.ttf")) return 0;
	Vector2u size_old = size; //размеры окна до последнего изменения
	View view(FloatRect(0, 0, size.x, size.y));//создаём камеру, размером с окно
	Text text_main;
	text_main.setFont(font);
	text_main.setString("You can't use it outside of a fight");
	text_main.setCharacterSize(50);
	text_main.setFillColor(sf::Color::Red);
	text_main.setPosition(10, size.y - 110);
	float warning_color = 0;
	int rad = 50;// радиус круга-модельки персонажа
	Sprite hero;
	Texture SF;
	if (not SF.loadFromFile("zxc.png")) return -1;
	hero.setOrigin(rad, rad);
	hero.setTexture(SF);
	hero.setTextureRect(IntRect(0, 0, 100, 100));
	Sprite map;
	Texture minimap_1;
	if (not minimap_1.loadFromFile("map_1.png")) return -1;
	Texture minimap_2;
	if (not minimap_2.loadFromFile("map_2.png")) return -1;
	map.setTexture(minimap_1);
	map.setPosition(0, 0);
	float vx = 0; //скорость персонажа
	float vy = 0;
	Vector2f hero_pos = { 2500, 2100 };
	double len = 1;//задача переменной, которая впоследствие будет нужна для рассчёта скорости персонажа
	double base_speed = 0.42;
	double movespeed; //скорость героя
	Vector2i mouse_old; //координаты курсора кадра назад
	Vector2i viewcenter = Vector2i(size.x / 2, size.y / 2); //центр камеры (вид)
	hero.setPosition(viewcenter.x, viewcenter.y);//начальная позиция героя
	bool isckm = false;//проверка, нажата ли центральная кнопка мыши
	bool isckm_old = false; //была ли нажата ли центральная кнопка мыши в прошлом кадре
	bool islkm_old = false; //была ли нажата ли левая кнопка мыши в прошлом кадре
	int dx = 0;//перемещение камеры по х
	int dy = 0;//по у
	bool is_mouse_in_window;//в окне ли курсор?
	bool is_floor;//находится ли курсор в допустимой для перемещения области
	int check;
	int count = 0;
	int frame = 0, frames = 0, hod = 0; //всё про анимации
	int tek_room = 0;
	Vector3i results;

	vector<Item> selectedIems; 

	// и дальше весёлые задавушки))))))))
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int floor = 0; //этаж
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Clock time = Clock();
	unsigned long gltime = 0;
	short deltatime = 0;
	Guard guards[2][1];
	guards[0][0].changeAll(true, 1570, 1320, 1845, true);
	guards[1][0].changeAll(true, 1570, 1320, 1845, true);
	vector<vector<Room>> rooms(2, vector<Room>(0));
	//задача комнат:
	{
		string line;
		int stage = 1;
		int tek_i = -1;
		int tek_j = 0;
		int x0, y0, x, y;
		ifstream in("rooms.txt");
		while (getline(in, line)) {
			istringstream iss(line);
			if (iss >> x0 >> y0 >> x >> y) {
				switch (stage) {
				case 1:
					rooms[tek_j].push_back(Room());
					rooms[tek_j][tek_i].changeAll(x0, y0, x, y);
					break;
				case 2:
					rooms[tek_j][tek_i].addEscape(x0, y0, x, y);
					break;
				case 3:
					rooms[tek_j][tek_i].addObstackle(x0, y0, x, y);
					break;
				}
			}
			else {
				if (line == "new room:") {
					stage = 1;
					tek_i++;
				}
				else if (line == "new floor:") {
					tek_i = -1;
					tek_j++;
				}
				else if (line == "escapes:") stage = 2;
				else if (line == "obstackles:") stage = 3;
			}
		}
		in.close();
	} 
	cout << rooms.size() << ": {" << rooms[0].size() << ", " << rooms[1].size() << "}\n";
	//задача нпс и распихивание по комнатам:
	{
		string line, name;
		int posX, posY, gender, room, tek_j = 0;
		ifstream in("characters.txt");
		while (getline(in, line)) {
			stringstream ss(line);
			if (ss >> room >> posX >> posY >> gender >> name) {
				rooms[floor][room].addNPC(NPC());
				rooms[floor][room].characters.back().changeAll(name, gender, posX, posY, rad);
			}
			else if (line == "new floor:") tek_j = 1;
		}
	}
	//задача предметов и распихивание по комнатам:
	{
		string line, name, types[3] = {"letter", "potion", "bomb"};
		int posX, posY, type, room, tek_j = 0;
		ifstream in("items.txt");
		while (getline(in, line)) {
			stringstream ss(line);
			if (ss >> room >> posX >> posY >> type) {
				rooms[floor][room].addItem(Item());
				if (ss >> name) {
					rooms[floor][room].items.back().changeAll(posX, posY, types[type], name);
				}
				else {
					rooms[floor][room].items.back().changeAll(posX, posY, types[type]);
				}
			}
			else if (line == "new floor:") tek_j = 1;
		}
	}

	while (window.isOpen()) {
		Event event;
		deltatime = time.getElapsedTime().asMilliseconds() - gltime;
		gltime = time.getElapsedTime().asMilliseconds();
		//cout << gltime << "\n";//проверка, которая показала, что время работает
		movespeed = base_speed * deltatime;
		is_floor = true;
		mouse_pos = Mouse::getPosition(window); //смотрим, где курсор
		is_mouse_in_window = mouse_pos.x >= 0 and mouse_pos.x <= size.x and mouse_pos.y >= 0 and mouse_pos.y <= size.y;
		frames++;

		////////////////////////////////////////////СОБЫТИЯ В ОКНЕ/////////////////////////////////////////////////////////////////////////////////////
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) window.close();
			else if (event.type == Event::Resized) {
				size = window.getSize();
				view.setSize(size.x, size.y);
				dx = dx - size.x / 2 + size_old.x / 2;//меняем восприятие мыши при изменении размера окна
				dy = dy - size.y / 2 + size_old.y / 2;
				size_old = size;
				if (viewcenter.x < size.x / 2) {
					dx -= viewcenter.x - size.x / 2;
					viewcenter.x = size.x / 2;
				}
				else if (viewcenter.x + size.x / 2 > minimap_1.getSize().x) {
					dx -= viewcenter.x - (minimap_1.getSize().x - size.x / 2);
					viewcenter.x = minimap_1.getSize().x - size.x / 2;
				}
				if (viewcenter.y < size.y / 2) {
					dy -= viewcenter.y - size.y / 2;
					viewcenter.y = size.y / 2;
				}
				else if (viewcenter.y + size.y / 2 > minimap_1.getSize().y) {
					dy -= viewcenter.y - (minimap_1.getSize().y - size.y / 2);
					viewcenter.y = minimap_1.getSize().y - size.y / 2;
				}
				view.setCenter(viewcenter.x, viewcenter.y);
				text_main.setPosition(10 + dx, size.y - 110 + dy);
			}
		}

		isckm_old = isckm;
		if (Mouse::isButtonPressed(Mouse::Middle) and is_mouse_in_window) {
			isckm = true; //нажата ли центральная кнопка мыши

		}
		else isckm = false;

		if (isckm and isckm_old) {
			check = viewcenter.x - mouse_pos.x + mouse_old.x;
			if (check >= size.x / 2 and check + size.x / 2 <= map.getTexture()->getSize().x) {
				viewcenter.x = check;
				dx += (mouse_old.x - mouse_pos.x);
			}
			check = viewcenter.y - mouse_pos.y + mouse_old.y;
			if (check >= size.y / 2 and check + size.y / 2 <= map.getTexture()->getSize().y) {
				viewcenter.y = check;
				dy += (mouse_old.y - mouse_pos.y);
			}
			text_main.setPosition(10 + dx, size.y - 110 + dy);
			view.setCenter(viewcenter.x, viewcenter.y);
			text_main.setPosition(10 + dx, size.y - 110 + dy);
		} //если центральная кнопка мыши удерживается - двигаем камеру за курсором

		//////////////////////////////////////////////ПЕРЕДВИЖЕНИЕ/////////////////////////////////////////////////////////////////////////////////////

		if (Mouse::isButtonPressed(Mouse::Right) and is_mouse_in_window) {
			len = sqrt(pow((mouse_pos.x - hero_pos.x + dx), 2) + pow((mouse_pos.y - hero_pos.y + dy), 2));
			vx = (mouse_pos.x - hero_pos.x + dx) / len;
			vy = movespeed * (mouse_pos.y - hero_pos.y + dy) / len;
			if (vy <= 0) hero.setRotation(asin(vx) * 57.3);
			else hero.setRotation(180 - asin(vx) * 57.3);
			vx *= movespeed;
			hod = 100;
			hero.setTextureRect(IntRect(frame * 100, hod, 100, 100));
		}//по теореме пифагора и подобиям задаём скорости по х и у, найдя синусы косинусы там...
		else if (Keyboard::isKeyPressed(Keyboard::S) and is_mouse_in_window) {
			vx = 0;
			vy = 0;
			hod = 0;
			hero.setTextureRect(IntRect(frame * 100, hod, 100, 100));
		}
		results = rooms[floor][tek_room].isAbleToGo(hero_pos, { hero_pos.x + vx, hero_pos.y + vy });
		//if (is_floor and len > rad) { hero_pos.x += vx; hero_pos.y += vy; len -= movespeed; }//двигаем, если не стенка
		if (len > rad) {
			hero_pos.x += vx * (results.x or results.z != -1);
			hero_pos.y += vy * (results.y or results.z != -1);
			len -= movespeed;
			len *= (results.x and results.y or results.z != -1);
		}
		if(results.x or results.y or results.z != -1) {
			hod = 0;
			hero.setTextureRect(IntRect(frame * 100, hod, 100, 100));
		}
		if ((not rooms[floor][tek_room].isInRoom(hero_pos)) and results.z != -1) {
			tek_room = results.z;
			cout << results.z <<"\n";
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		check = sqrt(pow((mouse_pos.x + dx - hero_pos.x), 2) + pow((mouse_pos.y + dy - hero_pos.y), 2));
		if (Mouse::isButtonPressed(Mouse::Left) and not islkm_old and is_mouse_in_window) {
			count++;
			if (count % 2 == 1)	cout << "{" << mouse_pos.x + dx << "," << mouse_pos.y + dy << ",";
			else cout << mouse_pos.x + dx << "," << mouse_pos.y + dy << "},\n";
			if (sqrt(pow((mouse_pos.x + dx - 2500), 2) + pow((mouse_pos.y + dy - 2200), 2)) < 700 and check < 700) {
				floor = 1 - floor;
				if (floor == 0) map.setTexture(minimap_1);
				else map.setTexture(minimap_2);
			}
			else if (check < 150) {
				for (int i = 0; i < rooms[floor][tek_room].characters.size(); i++) {
					if (abs(rooms[floor][tek_room].characters[i].coords.x - mouse_pos.x - dx) < rad and abs(rooms[floor][tek_room].characters[i].coords.y - mouse_pos.y - dy) < rad) {
						if (rooms[floor][tek_room].characters[i].dialog()) {
							if (fight(true)) {
								auto iter = rooms[floor][tek_room].characters.cbegin();
								rooms[floor][tek_room].characters.erase(iter + i);
							}
						}
					}
				}
				for (int i = 0; i < rooms[floor][tek_room].items.size(); i++) {
					if (abs(rooms[floor][tek_room].items[i].coords.x - mouse_pos.x - dx) < rad and abs(rooms[floor][tek_room].items[i].coords.y - mouse_pos.y - dy) < rad) {
						rooms[floor][tek_room].items[i].pick();
						selectedIems.push_back(rooms[floor][tek_room].items[i]);
						auto iter = rooms[floor][tek_room].items.cbegin();
						rooms[floor][tek_room].items.erase(iter + i);
					}
				}
			}
		}
		islkm_old = Mouse::isButtonPressed(Mouse::Left);


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		mouse_old = mouse_pos;

		if (frames % 50 == 0) {
			frame = 1 - frame;
			hero.setTextureRect(IntRect(frame * 100, hod, 100, 100));
			for (size_t i = 0; i < rooms[floor][tek_room].characters.size(); i++) {
				rooms[floor][tek_room].characters[i].set_frame(frame);
			}
		}
		hero.setPosition(hero_pos); //возможно возникает вопрос: а зачем задавать координаты, если можно
		//использовать функцию .move, а я отвечу - да по фану, мне просто координаты нужны для
		//расчётов выше, поэтому они есть.

		window.setView(view); //переводимся на вид с камеры
		window.clear(); //чистим, чтобы не накладывались рисунки
		window.draw(map);
		window.draw(hero); //рисуем собсна героя
		if (tek_room < 4) {
			for (int i = 0; i < 1; i++) {
				if (guards[floor][i].isActive) {
					if (guards[floor][i].turn(hero_pos, deltatime)) {
						if (fight(true)) {
							guards[floor][i].isActive = false;
						}
					}
					window.draw(guards[i][floor].image);
				}
			}
		}
		//рисуем персонажей и предметы в комнате:
		for (size_t i = 0; i < rooms[floor][tek_room].characters.size(); i++) {
			window.draw(rooms[floor][tek_room].characters[i].image);
		}
		for (size_t i = 0; i < rooms[floor][tek_room].items.size(); i++) {
			window.draw(rooms[floor][tek_room].items[i].image);
		}

		if (Keyboard::isKeyPressed(Keyboard::I) and is_mouse_in_window) {
			int n = inventory(selectedIems, hero1.hits, hero1.max_hits);
			if (n > -1) { 
				n = selectedIems[n].use(); 
				if (n > 0) {
					hero1.hits = min(hero1.hits+n, hero1.max_hits);
				}
				else if (n = -1) {
					warning_color = 255;
				}
			}
			text_main.setString("Hits: " + to_string(hero1.hits) + "/" + to_string(hero1.max_hits));
		}
		warning_color -= deltatime / 3.0;
		text_main.setFillColor(Color{255, 0, 0, Uint8(max(int(warning_color), 0))});
		window.draw(text_main);
		window.display();//так надо
	}
	return 0;
}