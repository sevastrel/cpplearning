
#include <iostream>
#include <curses.h>

#include <string>
#include <stdio.h>

#include <chrono>
#include <vector>
#include <algorithm>

#include <cstdlib> // <<< НОВОЕ: для rand() и srand()
#include <ctime>   // <<< НОВОЕ: для time()

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

    auto spawn_coin = [&]() {

        coin_x = 1 + rand() % (w1 -2);
        coin_y = 1 + rand() % (h1 -2);

    };
    int vel_x = 0; 
    int vel_y = 0;
    spawn_coin(); // first coin
    while ('q' != (c = getch())) {

        clear(); // очистка предыдущего кадра
        mvprintw(0, 0, "Use arrows. 'q' or Esc to exit p=(%d,%d) score=%d", h1, w1,score);

        // 1. ОБРАБОТКА ВВОДА (Меняем только скорость, а не координаты!)
        if (c == KEY_UP)    { vel_y = -1; vel_x = 0; }
        else if (c == KEY_DOWN)  { vel_y = 1; vel_x = 0; }
        else if (c == KEY_LEFT)  { vel_x = -1; vel_y = 0; }
        else if (c == KEY_RIGHT) { vel_x = 1; vel_y = 0; }
        // Если c == ERR (кнопка не нажата), vel_x и vel_y НЕ МЕНЯЮТСЯ, сохраняя предыдущее значение!

        // 2. ПРИМЕНЕНИЕ ДВИЖЕНИЯ (Происходит в КАЖДОМ кадре)
        p_x += vel_x;
        p_y += vel_y;
        // flushinp();
        p_x = std::clamp(p_x, 1, w1 - 2);
        p_y = std::clamp(p_y, 1, h1 - 2);

        if (rad(p_x,p_y,coin_x,coin_y)) {
            score++;
            spawn_coin();
        }
        attron(COLOR_PAIR(1)); // Можно убрать, если цвета пока не нужны
        mvaddch(coin_y, coin_x, '@');
        attroff(COLOR_PAIR(1));
        mvprintw(p_y,p_x,"<^>"); // В. Отрисовка текущего кадра в буфер
        // mvprintw(y, x, "формат", аргументы...) - это аналог printf для координат

        refresh();// Возвращаем терминал в обычный режим
    }

    endwin();
    return 0;
}