#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<utility>
#include<random>
#include<ctime>
#include<memory>
#include<cmath>


int random(int min, int max) {// make random 
	static std::random_device rd;
	static std::mt19937 gen(rd());

	std::uniform_int_distribution<> dist(min, max);
	return dist(gen);
}

int map_block_count(int lv, int x_max, int y_max) {
	int k = lv - 1; // x + y = k
	int x_min = std::max(0, k - y_max);
	int x_max_valid = std::min(x_max, k);

	if (x_min > x_max_valid) {
		return 0; // �ش� lv�� ��� ����
	}
	return x_max_valid - x_min + 1;
}

	
class Living {
protected:
	std::string name;
	int age{};
	int damage_init{};
	int hp_init{};
	int defence_init{};
	int lv{};
	bool live{};
public:
	Living(std::string name, int age, int damage_init, int hp_init, int defence_init,int lv,bool live) noexcept :
		name(std::move(name)),age(age), damage_init(damage_init), hp_init(hp_init), defence_init(defence_init),lv(lv),live(live) {}

	virtual ~Living() = default;

	const std::string& get_name() const noexcept { return name; }
	int get_age() const noexcept { return age; }
	int get_damage_init() const noexcept { return damage_init; }
	int get_hp_init() const noexcept { return hp_init; }
	int get_defence_init() const noexcept { return defence_init; }
	int get_lv() const noexcept { return lv; }
	bool get_live() const noexcept{ return live; }
	
	void set_name(std::string new_name) noexcept { name = new_name; }
	void set_age(int new_age) noexcept { age = new_age; }
	void set_damage_init(int new_damage_init) noexcept { damage_init = new_damage_init; }
	void set_hp_init(int new_hp_init) noexcept { hp_init = new_hp_init; }
	void set_defence_init(int new_defence_init) noexcept { defence_init = new_defence_init; }
	void set_lv(int new_lv) noexcept { lv = new_lv; }
	void set_live(bool new_live) { live = new_live; }
};

enum class CharacterType {
	Warrior = 1,
	Wizard = 2,
	Knight = 3,
	Archer = 4,
};

struct Skill {
	std::string skill_name{};
	std::string skill_exp{};
	int damage_increase{};
	int hp_increase{};
	int defence_increase{};

	std::vector<std::shared_ptr<Skill>>children;
};

class Character : public Living {
private:
	std::vector<std::shared_ptr<Skill>> skills;
	CharacterType character_type{};
	int exp{};
	int next_exp{};
	int first_damage_init, first_hp_init, first_defence_init;
	int status_point{};
	
public:
	Character(std::string name, int age, int damage_init, int hp_init, int defence_init,int lv,bool live, CharacterType character_type,std::vector<std::shared_ptr<Skill>> skills,int exp,int next_exp, int first_damage_init, int first_hp_init, int first_defence_init,int status_point) noexcept :
		Living(std::move(name), age, damage_init, hp_init, defence_init, lv,live), character_type(character_type),skills(skills),exp(exp),next_exp(next_exp), first_damage_init(first_damage_init), first_hp_init(first_hp_init), first_defence_init(first_defence_init),status_point(status_point) {
	}

	CharacterType get_character_type() const noexcept { return character_type; }
	std::vector<std::shared_ptr<Skill>>& get_skill() noexcept { return skills; }
	int get_exp() noexcept { return exp; }
	int get_next_exp() noexcept { return next_exp; }
	int get_first_damage_init() noexcept { return first_damage_init; }
	int get_first_hp_init() noexcept { return first_hp_init; }
	int get_first_defence_init() noexcept { return first_defence_init; }
	int get_status_point() { return status_point; }
	void set_character_type(CharacterType new_character_type) noexcept { character_type = new_character_type; }
	void set_skill(std::vector<std::shared_ptr<Skill>> new_skills) noexcept { skills = std::move(new_skills); }
	void set_exp(int new_exp) noexcept { exp = new_exp; }
	void set_next_exp(int new_next_exp) noexcept { next_exp = new_next_exp; }
	void set_status_point(int new_status_point) { status_point = new_status_point; }
	void learn_skill(std::shared_ptr<Skill> skill) noexcept { skills.push_back(skill); }
};

enum class MonsterType {
	Undead = 1,
	Humanoid = 2,
	Animal = 3,
	Dragon = 4,
};

class Monster : public Living {
private:
	MonsterType monster_type{};
	int exp_pd{};
public:
	Monster(std::string name, int age, int damage_init, int hp_init, int defence_init, int lv, bool live, MonsterType monster_type, int exp_pd) noexcept :
		Living(std::move(name), age, damage_init, hp_init, defence_init, lv,live), monster_type(monster_type),exp_pd(exp_pd) {
	}
	MonsterType get_monster_type() const noexcept { return monster_type; }
	int get_exp_pd() { return exp_pd; }
	void set_monster_type(MonsterType new_monster_type) noexcept { monster_type = new_monster_type; }
	void set_exp_pd(int new_exp_pd) { exp_pd = new_exp_pd; }
};

class Character_Manager {
private:
	std::vector<std::unique_ptr<Character>> characters;
public:
	Character* make_character(std::string name, int age, CharacterType character_type) {
		int damage_init, hp_init, defence_init,lv = 1,exp = 0,live = true;
		switch (character_type) {
		case CharacterType::Warrior: damage_init = 10; hp_init = 20; defence_init = 5; break;
		case CharacterType::Wizard: damage_init = 15; hp_init = 15; defence_init = 15; break;
		case CharacterType::Knight: damage_init = 8; hp_init = 25; defence_init = 8; break;
		case CharacterType::Archer: damage_init = 12; hp_init = 18; defence_init = 10; break;
		}
		characters.push_back(std::make_unique<Character>(
			std::move(name), age, damage_init, hp_init, defence_init, lv, live,
			character_type, std::vector<std::shared_ptr<Skill>>{},
			exp, 51, // next_exp = lv�� + 50 = 51
			damage_init, hp_init, defence_init,0));
		return characters.back().get();
	}
	const std::vector<std::unique_ptr<Character>>& all() const noexcept { return characters; }

	Character* Find_by_name(const std::string& name) {
		for (int i = 0; i<characters.size(); i++) {
			if (characters[i]->get_name() == name) { return characters[i].get(); }
		}
		return nullptr;
	}
	
};

class Monster_Manager {
	std::vector<std::unique_ptr<Monster>> monsters;
public:
	Monster* make_monster(std::string& name, int age,int damage_init,int hp_init,int defence_init,int lv,MonsterType monster_type, int exp_pd) {
		bool live = true;
		monsters.push_back(std::make_unique<Monster>(std::move(name), age, damage_init, hp_init, defence_init, lv,live,monster_type,exp_pd));
		return monsters.back().get();
	}
	std::vector<std::unique_ptr<Monster>>& all() { return monsters; }

	Monster* find_by_name(const std::string& name) {
		for (int i = 0; i < monsters.size(); i++) {
			if (monsters[i]->get_name() == name) {
				return monsters[i].get();
			}
		}
		return nullptr;
	}

	std::string to_string(MonsterType type) {
		switch (type) {
		case MonsterType::Undead:   return "Undead";
		case MonsterType::Humanoid: return "Humanoid";
		case MonsterType::Animal:   return "Animal";
		case MonsterType::Dragon:   return "Dragon";
		default: return "Unknown";
		}
	}

	Monster* make_random_monster(int lv,int x_max,int y_max) {

		std::string name_data[21] = { "Angry", "Brutal", "Cunning", "Dark", "Elder",
		"Fierce", "Giant", "Hungry", "Infernal", "Jeweled",
		"Keen", "Lethal", "Mighty", "Nimble", "Obsidian",
		"Poisonous", "Quick", "Ruthless", "Savage", "Terrifying" };

		int typenum = random(1, 100);
		if (typenum <= 40) { typenum = 1; }        // 1~40 �� Undead (40%)
		else if (typenum <= 70) { typenum = 2; }   // 41~70 �� Humanoid (30%) 
		else if (typenum <= 90) { typenum = 3; }   // 71~90 �� Animal (20%)
		else { typenum = 4; }                       // 91~100 �� Dragon (10%)
		int exp_pd = pow(map_block_count(lv, x_max, y_max), 1.2) + 50*typenum;//���Ͱ� �����ϴ� ����ġ (�ش� ������ ��ϼ�^1.2+50*Ÿ�Գѹ�)
		MonsterType monster_type = static_cast<MonsterType>(typenum);
		int damage_init, hp_init, defence_init;
		switch (monster_type) {
		case MonsterType::Undead: damage_init = 8; hp_init = 15; defence_init = 4; break;
		case MonsterType::Humanoid: damage_init = 15; hp_init = 20; defence_init = 8; break;
		case MonsterType::Animal: damage_init = 6; hp_init = 15; defence_init = 13; break;
		case MonsterType::Dragon: damage_init = 20; hp_init = 25; defence_init = 3; break;
		}
		damage_init *= 1 + (pow(lv,1.5));
		hp_init *= 1 + (pow(lv, 1.2));
		defence_init *= 1 + (pow(lv, 0.5));
		std::string name = name_data[random(0, 19)] +" "+ to_string(monster_type);
		int age = random(1, 500);
		Monster* monster = make_monster(name, age, damage_init, hp_init, defence_init, lv, monster_type,exp_pd);
		return monster;
	}
	
};

class Map {
public:
	struct Map_node {
		int x, y;
		bool visited{ false };
		bool char_here{ false };
		struct Map_node* front{ nullptr };
		struct Map_node* back{ nullptr };
		struct Map_node* left{ nullptr };
		struct Map_node* right{ nullptr };
		Monster* monster{ nullptr };
	};
private:
	Monster_Manager monster_manager;
	std::vector<std::vector<Map_node*>> grid;
public:
	Map_node* map_creation(int x,int y,Monster* monster) {
		Map_node* new_map = new Map_node;
		new_map->x = x;
		new_map->y = y;
		new_map->monster = monster;
		return new_map;
	}
	void make_map(int max_x, int max_y) {
		grid.clear();
		grid.resize(max_y);
		for (int y = 0; y < max_y; y++) {
			grid[y] = std::vector<Map_node*>(max_x, nullptr);
		}

		for (int y = 0; y < max_y; y++) {
			for (int x = 0; x < max_x; x++) {
				int monster_lv = x > y ? x : y;
				Monster* monster = monster_manager.make_random_monster(monster_lv,max_x,max_y);
				grid[y][x] = map_creation(x, y,monster);
			}
		}

		for (int y = 0; y < max_y; y++) {
			for (int x = 0; x < max_x; x++) {
				if (y > 0) {
					grid[y][x]->back = grid[y - 1][x];
				}
				if (x > 0) {
					grid[y][x]->left = grid[y][x - 1];
				}
				if (y < max_y-1) {
					grid[y][x]->front = grid[y + 1][x];
				}
				if (x < max_x-1) {
					grid[y][x]->right = grid[y][x + 1];
				}
			}
		}
	}
	
};

class Skill_Tree {
private:
	std::shared_ptr<Skill>root;
	std::vector<std::shared_ptr<Skill>> skills; // ��� ��ų ����
public:
	// �� ��ų ���� �� ��ȯ
	std::shared_ptr<Skill> make_skill(const std::string& name,
		const std::string& exp,
		int dmg, int hp, int def)
	{
		auto skill = std::make_shared<Skill>();
		skill->skill_name = name;
		skill->skill_exp = exp;
		skill->damage_increase = dmg;
		skill->hp_increase = hp;
		skill->defence_increase = def;
		skills.push_back(skill);
		if (!root) { root = skill; }
		return skill; // �ʿ��ϸ� �ۿ��� children ���� ����
	}

	std::shared_ptr<Skill> get_rood() const { return root; }

	// ����׿�: ��ü ��ų ���
	void print_skills() {
		for (auto& s : skills) {
			std::cout << s->skill_name << " - " << s->skill_exp << "\n";
		}
	}
};

class Skill_Manager {
private:
	Skill_Tree tree;
public:
	void skill_init() {
		// 0�� (��Ʈ)
		auto root = tree.make_skill("Į�� ���", "�⺻ �˼��� ���� ����", 10, 0, 0);

		// 1�� (5��, ���Һ� Į��)
		auto fire_slash = tree.make_skill("ȭ�� Į��", "�Ҳ��� �θ� �ϰ�", 20, 0, 0);
		auto water_slash = tree.make_skill("���� Į��", "����ó�� �帣�� �ϰ�", 18, 0, 5);
		auto lightning_slash = tree.make_skill("���� Į��", "������ �ӵ��� ����", 22, 0, -2);
		auto wind_slash = tree.make_skill("�ٶ��� Į��", "��ī�ο� �ٶ����� ����", 16, 0, 8);
		auto earth_slash = tree.make_skill("������ Į��", "������ ������ ���� ����ģ��", 15, 10, 10);

		root->children = { fire_slash, water_slash, lightning_slash, wind_slash, earth_slash };

		// 2�� (�� ���Ҹ��� 2�� �� �� 10��)
		auto fire_blast = tree.make_skill("ȭ�� ����", "�Ҳ��� ������ ����Ų��", 40, 0, 0);
		auto fire_guard = tree.make_skill("ȭ�� ����", "�Ҳ����� ���и� �����", 15, 20, 10);
		fire_slash->children = { fire_blast, fire_guard };

		auto water_wave = tree.make_skill("����", "�Ŵ��� �ĵ��� ���� ��ģ��", 35, 0, 15);
		auto water_guard = tree.make_skill("���� ����", "���� ������ ���и� �����", 10, 25, 20);
		water_slash->children = { water_wave, water_guard };

		auto lightning_storm = tree.make_skill("���� ��ǳ", "������ ��ǳ�� ����Ų��", 45, 0, 5);
		auto lightning_guard = tree.make_skill("���� ��ȣ��", "������ �� ��ȣ��", 20, 15, 25);
		lightning_slash->children = { lightning_storm, lightning_guard };

		auto wind_storm = tree.make_skill("ȸ���� �ٶ�", "ȸ������ ���� �۾��������", 32, 0, 12);
		auto wind_guard = tree.make_skill("�ٶ� �帷", "�ٶ����� �帷�� �� ���´�", 12, 20, 25);
		wind_slash->children = { wind_storm, wind_guard };

		auto earth_quake = tree.make_skill("���� ����", "���� ��� ���� ���ʶ߸���", 30, 0, 30);
		auto earth_guard = tree.make_skill("���� ����", "������ ���и� �����", 10, 35, 30);
		earth_slash->children = { earth_quake, earth_guard };

		// 3�� (2���� �� ��ų���� 2�� �� �� 20��)
		fire_blast->children = {
			tree.make_skill("���� ����", "���� ������ ���� ���� �Ҹ��Ų��", 80, 0, 0),
			tree.make_skill("��� ����", "������ ���� ����� �����Ų��", 70, 20, 10)
		};
		fire_guard->children = {
			tree.make_skill("�Ҳ� �庮", "�ұ��� �庮���� ���� �¿��", 60, 40, 20),
			tree.make_skill("�һ� ȿ��", "�ڽ��� �Ҳ����� ���� �ұ��� ���� ��´�", 50, 30, 30)
		};

		water_wave->children = {
			tree.make_skill("���� ����", "������ ���߽��� ���� ����", 75, 0, 25),
			tree.make_skill("���� �ĵ�", "�ĵ��� ��� ���� �����Ѵ�", 65, 10, 40)
		};
		water_guard->children = {
			tree.make_skill("���� �帷", "���� �帷���� ������ ��ȭ�Ѵ�", 40, 30, 50),
			tree.make_skill("ġ���� ����", "������ �Ʊ��� ġ���Ѵ�", 0, 80, 20)
		};

		lightning_storm->children = {
			tree.make_skill("õ���� ����", "�ϴÿ��� ���� ������ ������", 90, 0, 15),
			tree.make_skill("�ö�� ����", "�ö�� Į���� ����", 85, 0, 10)
		};
		lightning_guard->children = {
			tree.make_skill("�ڱ��� ��ȣ��", "�ڱ����� ������ ���", 20, 40, 60),
			tree.make_skill("����� �ݻ�", "������ ��� �� �ݻ��Ѵ�", 30, 20, 70)
		};

		wind_storm->children = {
			tree.make_skill("��ǳ ��ȯ", "�Ŵ��� ��ǳ�� ��ȯ�Ѵ�", 70, 20, 30),
			tree.make_skill("Į�ٶ� �ϰ�", "Į�� ���� �ٶ����� ����", 85, 0, 15)
		};
		wind_guard->children = {
			tree.make_skill("���� ����", "���� ����� ���", 15, 35, 55),
			tree.make_skill("�ٶ��� ��", "��ø���� ����Ѵ�", 40, 30, 40)
		};

		earth_quake->children = {
			tree.make_skill("��� �ر�", "����� ���ʶ߷� ���� ����", 100, 20, 20),
			tree.make_skill("���� â", "������ âó�� ���", 95, 0, 25)
		};
		earth_guard->children = {
			tree.make_skill("������ ���", "������ �ϳ� �Ǿ� ���", 30, 60, 80),
			tree.make_skill("ö��", "������ ����� ��� �¼�", 20, 80, 100)
		};

			
	}
};

class Status {
public:
	void add_damage_point(Character* character) {
		if (character->get_status_point()) {

		}
	}
};

class Level_Manager {
private:
	int next_exp_init{50};
public:
	void add_exp(Character* character,int plus_exp) {
		character->set_exp(character->get_exp() + plus_exp);
	}
	void next_exp_next(Character* character) {
		character->set_next_exp(pow(character->get_lv(), 2) + next_exp_init);
	}
	void lvup(Character* character) {
		while (character->get_exp() >= character->get_next_exp()) {
			character->set_lv(character->get_lv() + 1);
			next_exp_next(character);

			character->set_damage_init((pow(character->get_lv(), 1.1) + 1) * character->get_first_damage_init());
			character->set_hp_init((pow(character->get_lv(), 1.35) + 1) * character->get_first_hp_init());
			character->set_defence_init((pow(character->get_lv(), 0.6) + 1) * character->get_first_defence_init());
		}
		
	}
};