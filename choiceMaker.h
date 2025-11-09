#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

static void set_raw_mode(int enable) {
    static struct termios oldt;
    struct termios newt;

    if (enable) {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}

static char *get_key() {
    static char buffer[4];
    memset(buffer, 0, sizeof(buffer));
    read(STDIN_FILENO, buffer, 1);
    if (buffer[0] == '\x1b') {
        read(STDIN_FILENO, buffer + 1, 2);
    }
    return buffer;
}

static const char* choice_from_array(const char *title, const char *items[], size_t count) {
    int selected = 0;
    while (1) {
        system("clear");
        printf("%s\n", title);
        printf("↑/↓ Navigate | Enter = Select\n");
        printf("----------------------------------------\n");

        for (size_t i = 0; i < count; i++) {
            printf("%s %s\n",
                   (i == (size_t)selected ? "> " : "  "),
                   items[i]);
        }

        set_raw_mode(1);
        char *key = get_key();
        set_raw_mode(0);

        if (strcmp(key, "\x1b[A") == 0) {
            selected = (selected - 1 + count) % count;
        } else if (strcmp(key, "\x1b[B") == 0) {
            selected = (selected + 1) % count;
        } else if (key[0] == '\n' || key[0] == '\r') {
            return items[selected];
        }
    }
}