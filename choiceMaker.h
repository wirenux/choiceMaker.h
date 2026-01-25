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
    int scroll_offset = 0;
    const int VIEWPORT_HEIGHT = 15; // num of items to show on 1 viewport

    while (1) {
        printf("\033[H\033[J");

        printf("=== %s ===\n", title);
        printf("Items %d-%d of %zu | ↑/↓ Navigate\n",
                scroll_offset + 1,
                (int)(scroll_offset + VIEWPORT_HEIGHT > count ? count : scroll_offset + VIEWPORT_HEIGHT),
                count);
        printf("----------------------------------------\n");

        // only print items in the viewport
        for (int i = 0; i < VIEWPORT_HEIGHT; i++) {
            int idx = i + scroll_offset;
            if (idx >= (int)count) break;

            if (idx == selected) {
                printf("\033[1;32m> %s\033[0m\n", items[idx]); // green
            } else {
                printf("  %s\n", items[idx]);
            }
        }
        printf("----------------------------------------\n");

        set_raw_mode(1);
        char *key = get_key();
        set_raw_mode(0);

        if (strcmp(key, "\x1b[A") == 0) { // UP
            if (selected > 0) {
                selected--;
                // if cursor < viewport -> scroll up
                if (selected < scroll_offset) {
                    scroll_offset = selected;
                }
            } else {
                // loop to bottom
                selected = count - 1;
                scroll_offset = count - VIEWPORT_HEIGHT;
                if (scroll_offset < 0) scroll_offset = 0;
            }
        } else if (strcmp(key, "\x1b[B") == 0) { // DOWN
            if (selected < (int)count - 1) {
                selected++;
                //  if cursor > viewport -> scroll down
                if (selected >= scroll_offset + VIEWPORT_HEIGHT) {
                    scroll_offset = selected - VIEWPORT_HEIGHT + 1;
                }
            } else {
                // loop to top
                selected = 0;
                scroll_offset = 0;
            }
        } else if (key[0] == '\n' || key[0] == '\r') {
            return items[selected];
        }
    }
}