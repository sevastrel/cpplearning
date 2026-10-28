
#include <iostream>
#include <curses.h>

#include <string>
#include <stdio.h>

#include <chrono>
#include <vector>
#include <algorithm>
int main() {
    setlocale(LC_ALL, "");
    initscr();            // Запуск режима curses
    cbreak();             // Отключаем буферизацию ввода (не нужно жать Enter)
    noecho();             // Отключаем эхо (символы не печатаются сами при нажатии)
    curs_set(FALSE);          // Прячем мигающий курсор терминала
    keypad(stdscr, TRUE); // Разрешае
    timeout(50);
    

    int h1, w1;
    getmaxyx(stdscr, h1, w1);
    int p_y=h1 / 2;;
    int p_x=w1 / 2;
    int c;
    
    while ('q' != (c = getch())) {

        clear(); // очистка предыдущего кадра
        mvprintw(0, 0, "Use arrows. 'q' or Esc to exit123123123. p=(%d,%d)", p_y, p_x);

        if (c==KEY_UP) {
            p_y--;
        }
        else if (c==KEY_DOWN) {
            p_y++;
        }
        else if (c==KEY_LEFT) {
            p_x--;
        }
        else if (c==KEY_RIGHT) {
            p_x++;
        }

        flushinp();

        p_x = std::clamp(p_x, 1, w1 - 2);
        p_y = std::clamp(p_y, 1, h1 - 2);

        mvprintw(p_y,p_x,"<^>"); // В. Отрисовка текущего кадра в буфер
        // mvprintw(y, x, "формат", аргументы...) - это аналог printf для координат

        refresh();// Возвращаем терминал в обычный режим
    }

    endwin();
    return 0;
}