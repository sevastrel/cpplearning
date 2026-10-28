
#include <iostream>
#include <curses.h>

#include <string>
#include <stdio.h>

#include <chrono>
#include <vector>
#include <algorithm>

#include <cstdlib> // <<< НОВОЕ: для rand() и srand()
#include <ctime>   // <<< НОВОЕ: для time()

// Используем удобные алиасы для времени
using steady_clock_t = std::chrono::steady_clock;
using time_point_t = std::chrono::time_point<steady_clock_t>;

struct Bullet {
    int x, y;           // Позиция
    int vx, vy;         // Направление (скорость)
    time_point_t last_move; // Время последнего движения
    int move_interval_ms;   // Интервал движения в мс (чем меньше, тем быстрее пуля)
};


bool rad(int p_x,int p_y, int coin_x, int coin_y) {
    int radius = 2; // Радиус сбора в клетках
    int dy = p_y - coin_y; // Разница по вертикали
    int dx = p_x - coin_x; // Разница по горизонтали
    // Если квадрат расстояния меньше или равен квадрату радиуса, мы подобрали монету!
    return (dx * dx + dy * dy <= radius * radius);
}
int main() {
    setlocale(LC_ALL, "");
    initscr();            // Запуск режима curses
    cbreak();             // Отключаем буферизацию ввода (не нужно жать Enter)
    noecho();             // Отключаем эхо (символы не печатаются сами при нажатии)
    curs_set(FALSE);          // Прячем мигающий курсор терминала
    keypad(stdscr, TRUE); // Разрешае
    timeout(50);
    
    start_color(); // 1. Включаем поддержку цветов
    // 2. Настраиваем пару №1: Жёлтый текст (COLOR_YELLOW) на Чёрном фоне (COLOR_BLACK)
    use_default_colors();  
    init_pair(1, COLOR_YELLOW, -1); 
    init_pair(2, COLOR_CYAN, -1); 
    srand(static_cast<unsigned int>(time(nullptr)));

    // Find screen max sizes
    int h1, w1;
    getmaxyx(stdscr, h1, w1);
    int p_y=h1 / 2;;
    int p_x=w1 / 2;
    int c;
    
    int score = 0;
    int coin_x = 0;
    int coin_y= 0;
    int vel_x=0;
    int vel_y=0;
    std::vector<Bullet> bullets; // Контейнер для всех пуль
    auto spawn_coin = [&]() {

        coin_x = 1 + rand() % (w1 -2);
        coin_y = 1 + rand() % (h1 -2);

    };
    spawn_coin(); // first coin
    while ('q' != (c = getch())) {

        clear(); // очистка предыдущего кадра
        mvprintw(0, 0, "Use arrows. 'q' or Esc to exit p=(%d,%d) score=%d", h1, w1,score);

        if (c==KEY_UP) p_y--;
        else if (c==KEY_DOWN) p_y++;
        else if (c==KEY_LEFT) p_x--;
        else if (c==KEY_RIGHT) p_x++;
        
        // Bullet
        if (c == ' ') {
            Bullet new_bullet;
            new_bullet.x = p_x;
            new_bullet.y = p_y;
            // Стреляем в том направлении, куда смотрит игрок (или просто вверх по умолчанию)
            new_bullet.vx = (vel_x == 0 && vel_y == 0) ? 0 : vel_x; 
            new_bullet.vy = (vel_x == 0 && vel_y == 0) ? -1 : vel_y; // По умолчанию вверх
            new_bullet.last_move = steady_clock_t::now();
            new_bullet.move_interval_ms = 100; // Пуля двигается каждые 100 мс (10 клеток в сек)
            
            bullets.push_back(new_bullet);
        }
        time_point_t now = steady_clock_t::now();
        for (auto& b : bullets) {
            // Проверяем, прошло ли достаточно времени для движения пули
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - b.last_move).count();
            if (elapsed >= b.move_interval_ms) {
                b.x += b.vx;
                b.y += b.vy;
                b.last_move = now; // Сбрасываем таймер
            }
        }
        // flushinp();
        for (auto& b : bullets) {
            if (rad(b.x,b.y,coin_x,coin_y)) {
                score++;
                spawn_coin();
            }
        }
        bullets.erase(
            std::remove_if(bullets.begin(), bullets.end(), [&](const Bullet& b) {
            // Удаляем, если пуля улетела за границы или была "убита" (x == -999)
            return b.x < 1 || b.x > w1 - 2 || b.y < 1 || b.y > h1 - 2 || b.x == -999;
        }),
        bullets.end()
        );
        p_x = std::clamp(p_x, 1, w1 - 2);
        p_y = std::clamp(p_y, 1, h1 - 2);

        if (rad(p_x,p_y,coin_x,coin_y)) {
            score++;
            spawn_coin();
        }
        attron(COLOR_PAIR(1)); // Можно убрать, если цвета пока не нужны
        mvaddch(coin_y, coin_x, '@');
        attroff(COLOR_PAIR(1));
        attron(COLOR_PAIR(2));
        mvprintw(p_y,p_x,"<^>"); // В. Отрисовка текущего кадра в буфер
        attroff(COLOR_PAIR(2));
        for (const auto& b : bullets) {
            mvaddch(b.y, b.x, 'o'); // Рисуем пули
        }
        refresh();// Возвращаем терминал в обычный режим
    }

    endwin();
    std::cout << "Game over. Your score: " <<score << "\n";
    return 0;
}