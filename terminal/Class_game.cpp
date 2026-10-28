
#include <iostream>
#include <curses.h>

#include <string>
#include <stdio.h>

#include <chrono>
#include <vector>
#include <algorithm>

#include <cstdlib> // <<< НОВОЕ: для rand() и srand()
#include <ctime>   // <<< НОВОЕ: для time()

#include <memory>
using steady_clock_t = std::chrono::steady_clock;
using time_point_t = std::chrono::time_point<steady_clock_t>;
bool rad(int p_x,int p_y, int coin_x, int coin_y) {
    int radius = 2; // Радиус сбора в клетках
    int dy = p_y - coin_y; // Разница по вертикали
    int dx = p_x - coin_x; // Разница по горизонтали
    // Если квадрат расстояния меньше или равен квадрату радиуса, мы подобрали монету!
    return (dx * dx + dy * dy <= radius * radius);
}

class GameObject;
class Bullet;

struct GameContext {
    int max_y, max_x;
    float delta_time;
    int& score;
    bool& is_running;

    // <<< НОВОЕ: Сюда объекты складывают указатели на себя, если хотят умереть
    std::vector<GameObject*> to_remove;
    // Список всех объектов для проверки коллизий
    std::vector<GameObject*> active_objects; 
    //Сюда складываются новые объекты, которые нужно добавить в мир
    std::vector<std::unique_ptr<GameObject>> to_add;
    // Метод, который вызывает объект, чтобы попросить об удалении
    void mark_for_removal(GameObject* obj) {
        to_remove.push_back(obj);
    }
};
struct RenderContext {
    void draw_char(int y, int x, char c, int color_pair) const {
        attron(COLOR_PAIR(color_pair));
        mvaddch(y, x, c);
        attroff(COLOR_PAIR(color_pair));
    }
    void draw_string(int y, int x, const std::string& s, int color_pair) const {
        attron(COLOR_PAIR(color_pair));
        mvprintw(y, x, "%s", s.c_str());
        attroff(COLOR_PAIR(color_pair));
    }
};

class GameObject {
public:
    virtual ~GameObject() = default;

    virtual void update(GameContext& ctx) = 0;

    virtual void render(const RenderContext& ctx) const = 0;
    
    virtual void handle_input(int key, GameContext& ctx) {};

    virtual int get_x() const = 0;
    virtual int get_y() const = 0;

    virtual void hit_by_bullet(GameContext& ctx) {
        ctx.mark_for_removal(this); // По умолчанию: объект просто исчезает
    }

    // ПОЛИМОРФИЗМ В ДЕЙСТВИИ: Объект сам решает, что будет, если его коснулся враг
    virtual void bit_by_enemy(GameContext& ctx) {
        // По умолчанию: ничего (например, другая пуля или стена просто игнорируют это)
    }
};
class Bullet : public GameObject {
private:
    int x;
    int y;
    float time_accumulator;     // Накопитель времени для шага
    float move_interval;        // Сколько секунд между сдвигами (0.05 = 20 клеток/сек)
    int color_id;
public:
        Bullet(int start_x, int start_y, int color)
        : x(start_x), y(start_y),
          time_accumulator(0.0f), move_interval(0.05f), color_id(color) {}

    void handle_input(int key, GameContext& ctx) override {}

    void update(GameContext& ctx) override {
        time_accumulator += ctx.delta_time;
        while (time_accumulator>=move_interval) {
            y-=1;
            time_accumulator-=move_interval;
        }
                for (auto* other : ctx.active_objects) {
            if (other == this) continue; // Не проверяем сами с собой
            
            int dx = x - other->get_x();
            int dy = y - other->get_y();
            
            if (dx * dx + dy * dy <= 1) { // Попадание!
                other->hit_by_bullet(ctx); // Пусть цель решает, что делать (враг даст очки и умрет)
                // ctx.mark_for_removal(this);   // Пуля всегда умирает при попадании
                break; // Прекращаем проверку, пуля уже уничтожена
            }
        }
        // 3. Если вылетели за границы — просим движок нас удалить
        if (x < 1 || x > ctx.max_x - 2 || y < 1 || y > ctx.max_y - 2) {
            ctx.mark_for_removal(this);
        }
    }
    void render(const RenderContext& ctx) const override {
        ctx.draw_char(y,x,'o',color_id);
    }
    int get_x() const override { return x; }
    int get_y() const override { return y; }

};

class Player : public GameObject {
private: 
    int x, y;
    int color_id;
    int vel_x, vel_y;

public: 
        Player(int start_x, int start_y, int color) 
        : x(start_x), y(start_y), color_id(color), vel_x(0), vel_y(0) {}
    void handle_input(int key, GameContext& ctx) override {
        if (key == KEY_UP)    y--;
        else if (key == KEY_DOWN)  y++;
        else if (key == KEY_LEFT)  x--;
        else if (key == KEY_RIGHT) x++;
        if (key == ' ') {
            ctx.to_add.push_back(std::make_unique<Bullet>(x, y, 1));
        }
    }

    void update(GameContext& ctx) override {
        // 2. Ограничение границ (используем данные из контекста)
        x = std::clamp(x, 1, ctx.max_x - 2);
        y = std::clamp(y, 1, ctx.max_y - 2);
    }
    void render(const RenderContext& ctx) const override {
        ctx.draw_string(y,x,"<^>", color_id);
    }
    void bit_by_enemy(GameContext& ctx) override {
        ctx.is_running = false; // GAME OVER! Враг коснулся игрока.
    }
    
    // Игрок игнорирует свои пули (чтобы не убить себя случайно)
    void hit_by_bullet(GameContext& ctx) override {
        // Ничего не делаем
    }
    int get_x() const override { return x; }
    int get_y() const override { return y; }

};
class Enemy : public GameObject {
private:
    int x;
    int y;
    float time_accumulator;
    float move_interval;
    int color_id;
public:
    Enemy(int start_x, int start_y, int color)
        : x(start_x), y(start_y), time_accumulator(0.0f), move_interval(0.3f), color_id(color) {}
    
    void handle_input(int, GameContext&) override {}

    void update(GameContext& ctx) override {
        time_accumulator += ctx.delta_time;
        if (time_accumulator>=move_interval) {
            y++;
            time_accumulator -=move_interval;
        }
        for (auto* other : ctx.active_objects) {
            if (other==this) continue;
            if (rad(x,other->get_x(),y,other->get_y())) {
                other->bit_by_enemy(ctx);
                ctx.mark_for_removal(this);
                break;
            }
            
        }
        if (y >= ctx.max_y - 1) {
            ctx.mark_for_removal(this);
        }

    }
    void render(const RenderContext& ctx) const override {
        ctx.draw_string(y,x,"W",color_id);
    }
    int get_x() const override { return x; }
    int get_y() const override { return y; }
    // Если в врага попала пуля, он даёт очки и умирает
    void hit_by_bullet(GameContext& ctx) override {
        ctx.score += 10;
        ctx.mark_for_removal(this);
    }

};
class EnemySpawner : public GameObject {
private:
    float time_accumulator;
    float spawn_interval;

public:
    EnemySpawner(float interval) : time_accumulator(0.0f), spawn_interval(interval) {}

    void handle_input(int, GameContext&) override {}

    void update(GameContext& ctx) override {
        time_accumulator += ctx.delta_time;
        if (time_accumulator >= spawn_interval) {
            // Спавним врага в случайной колонке на самой верхней строке (y = 1)
            int spawn_x = 1 + rand() % (ctx.max_x - 2);
            // 4 - это COLOR_MAGENTA (пурпурный), мы добавим его в инициализацию
            ctx.to_add.push_back(std::make_unique<Enemy>(spawn_x, 1, 4));
            time_accumulator -= spawn_interval;
        }
    }

    void render(const RenderContext&) const override {
        // Спавнер невидим, он не рисует ничего
    }

    int get_x() const override { return 0; }
    int get_y() const override { return 0; }
};
class Game {
private:
    std::vector<std::unique_ptr<GameObject>> objects; // ТРЕБОВАНИЕ: unique_ptr
    int score = 0;
    bool is_running = true;
public:
    void run() {
        setlocale(LC_ALL, "");
        initscr();            // Запуск режима curses
        cbreak();             // Отключаем буферизацию ввода (не нужно жать Enter)
        noecho();             // Отключаем эхо (символы не печатаются сами при нажатии)
        curs_set(FALSE);          // Прячем мигающий курсор терминала
        keypad(stdscr, TRUE); // Разрешае
        timeout(50);

        start_color();
        use_default_colors();
        init_pair(1, COLOR_YELLOW, -1); // Для интерфейса
        init_pair(2, COLOR_CYAN, -1); 

        // Find screen max sizes
        int h1, w1;
        getmaxyx(stdscr, h1, w1);

        //создаём игрока в центре и добавляем в вектор
        objects.push_back(std::make_unique<Player>(w1/2, h1/2, 2)); 
        
        objects.push_back(std::make_unique<EnemySpawner>(1.5f)); 
        
        auto last_time = std::chrono::steady_clock::now();
        int c;
        bool is_running = true;
        while (is_running) {
            if ('q' == (c = getch()) || c==27) {
                is_running=false;
                break;
            }
            auto current_time = std::chrono::steady_clock::now();
            std::chrono::duration<float> frame_duration = current_time - last_time;
            last_time = current_time;
            float dt = frame_duration.count();

            // 2. Подготовка контекстов
            GameContext game_ctx{h1, w1, dt, score,is_running};
            RenderContext render_ctx;

            clear();

            // int ch = getch();
            // if (ch == 'q' || ch==27) is_running = false;

            for (auto& obj : objects) {
                obj->handle_input(c,game_ctx);
            }
            // Заполняем список активных объектов
            game_ctx.active_objects.clear();
            for (auto& obj : objects) {
                game_ctx.active_objects.push_back(obj.get());
            }

            // 5. ОБНОВЛЕНИЕ СОСТОЯНИЯ
            for (auto& obj : objects) {
                obj->update(game_ctx);
            }
            // 6. <<< НОВОЕ: ДОБАВЛЕНИЕ НОВЫХ ОБЪЕКТОВ (пули от игрока)
            for (auto& new_obj : game_ctx.to_add) {
                objects.push_back(std::move(new_obj));
            }


            // 7. <<< НОВОЕ: БЕЗОПАСНОЕ УДАЛЕНИЕ МЁРТВЫХ ОБЪЕКТОВ
            // Это и есть решение "подвоха" лабы про мутацию контейнера
            if (!game_ctx.to_remove.empty()) {
                objects.erase(
                    std::remove_if(objects.begin(), objects.end(),
                        [&game_ctx](const std::unique_ptr<GameObject>& obj) {
                            for (auto* ptr : game_ctx.to_remove) {
                                if (obj.get() == ptr) return true;
                            }
                            return false;
                        }),
                    objects.end()
                );
                game_ctx.to_remove.clear();
            }
            // 8. ОТРИСОВКА
            for (const auto& obj : objects) {
                obj->render(render_ctx);
            }

            // 9. Интерфейс
            render_ctx.draw_string(0, 0, "Score: " + std::to_string(score) + " | Press 'q' to exit", 1);

            refresh();

        }
        endwin();
        std::cout << "Game finished successfully.\n";
    }

};

int main() {
    Game game;
    game.run();
    return 0;
}