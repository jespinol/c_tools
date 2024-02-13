//
// Modified from https://github.com/1OOyan/Snake
//

#include <ctype.h>
#include <math.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "../include/my_free.h"
#include "../include/my_malloc.h"

#define TIME_OUT 1000
typedef unsigned short int num;

num lives = 3;

const char* empty = " ";
const char* body = "O";
const char* apple = "@";
const char* wall = "#";
const char* snake_head = "O";

num rows, cols;
num nrows, ncols;
num num_apples = 0;
num score = 0;
int time_out = TIME_OUT;

char** disp_arr;

typedef struct ListNode {
  void* data;
  struct ListNode* next;
} ListNode;

typedef struct List {
  int size;
  int (*equals)(const void* key1, const void* key2);
  void (*destroy)(void* data);
  ListNode* head;
  ListNode* tail;
} List;

typedef struct SnakeNode {
  num x;
  num y;
  bool head;
} SnakeNode;

typedef struct Node {
  num x;
  num y;
} Node;

typedef struct WallSegment {
  num start_x;
  num start_y;
  num end_x;
  num end_y;
} WallSegment;

typedef struct Level {
  num number;
  char name[64];
  num apples_to_eat;
  num penalty_time;
  num wall_count;
  WallSegment* walls;
} Level;

Level levels_array[16];

typedef Node Apple;

typedef enum e_dir { up, down, left, right } e_dir;

typedef struct Snake {
  List* body;
  e_dir dir;
  num size;
} Snake;

typedef struct Apples {
  List* list;
  num count;
} Apples;

Snake* snake = NULL;
Apples* papples = NULL;

#define list_size(list) ((list)->size)
#define list_head(list) ((list)->head)
#define list_tail(list) ((list)->tail)
#define list_is_head(list, element) ((element) == (list)->head ? 1 : 0)
#define list_is_tail(element) ((element)->next == NULL ? 1 : 0)
#define list_data(element) ((element)->data)
#define list_next(element) ((element)->next)

int list_rem_next(List* list, ListNode* element, void** data);
int collision(Snake* psnake);
void get_random_apple(Snake* psnake, Apples* apples);

int list_init(List** list, void (*destroy)(void* data), int (*equals)(const void* key1, const void* key2)) {
  if (*list == NULL) {
    if ((*list = (List*)my_malloc(sizeof(List))) == NULL) return -1;
  }
  (*list)->size = 0;
  (*list)->destroy = destroy;
  (*list)->equals = equals;
  (*list)->head = NULL;
  (*list)->tail = NULL;
  return 0;
}

void list_destroy(List** list) {
  void* data = NULL;
  if (*list) {
    while ((*list)->size > 0) {
      list_rem_next((*list), NULL, (void**)&data);
    }
    my_free(*list);
    (*list) = NULL;
  }
  return;
}

int list_ins_next(List* list, ListNode* element, const void* data) {
  ListNode* new_element;

  if ((new_element = (ListNode*)my_malloc(sizeof(ListNode))) == NULL) return -1;
  new_element->data = (void*)data;

  if (element == NULL) {
    if (list_size(list) == 0) list->tail = new_element;
    new_element->next = list->head;
    list->head = new_element;
  } else {
    if (element->next == NULL) list->tail = new_element;
    new_element->next = element->next;
    element->next = new_element;
  }
  list->size++;
  return 0;
}

int list_rem_next(List* list, ListNode* element, void** data) {
  ListNode* old_element;

  if (list_size(list) == 0) return -1;

  if (element == NULL) {
    *data = list->head->data;
    old_element = list->head;
    list->head = list->head->next;
    if (list_size(list) == 1) list->tail = NULL;
  } else {
    if (element->next == NULL) return -1;
    *data = element->next->data;
    old_element = element->next;
    element->next = element->next->next;
    if (element->next == NULL) list->tail = element;
  }
  if (list->destroy != NULL) {
    list->destroy(old_element->data);
  }
  my_free(old_element);
  list->size--;
  return 0;
}

int delete_element(List* list, void* key, void** deleted) {
  if (key == NULL || list == NULL || list->head == NULL) return -1;
  ListNode* current = list->head;
  ListNode* old = NULL;
  if (list->equals(key, current->data) == 0) {
    *deleted = current->data;
    list->head = list->head->next;
    current->next = NULL;
    list->size--;
    my_free(current);
    return 0;
  }
  while (current->next != NULL) {
    if (list->equals(key, current->next->data) == 0) {
      old = current->next;
      *deleted = old->data;
      current->next = old->next;
      old->next = NULL;
      list->size--;
      my_free(old);
      return 0;
    }
    current = current->next;
  }

  return -1;
}

int delete_tail(List* list) {
  ListNode* current = NULL;
  if (list) {
    current = list->head;
    while (current->next != list->tail) {
      current = current->next;
    }
    my_free(current->next->data);
    my_free(current->next);
    current->next = NULL;
    list->size--;
    list->tail = current;
    return 0;
  }
  return -1;
}

int find_element(List* list, void* key, void** found) {
  if (key == NULL || list == NULL) return -1;
  ListNode* current = list->head;
  while (current != NULL) {
    if (list->equals(key, current->data) == 0) {
      *found = current->data;
      return 0;
    }
    current = current->next;
  }
  return -1;
}

void iterate(List* list, void (*iterator)(void* data)) {
  if (list == NULL) return;
  ListNode* current = list->head;
  while (current != NULL) {
    iterator(current->data);
    current = current->next;
  }
}

void display() {
  int i, j;
  for (i = 0; i < rows; i++)
    for (j = 0; j < cols; j++) {
      mvprintw(i, j, empty);
    }
}
void clear_display() {
  int i, j;
  for (i = 0; i < rows; i++)
    for (j = 0; j < cols; j++) {
      mvprintw(i, j, empty);
    }
}

void destroy(void* data) {
  if (data != NULL) {
    my_free(data);
  }
}

int equals(const void* key1, const void* key2) {
  Node* n1 = (Node*)key1;
  Node* n2 = (Node*)key2;
  if (n1->x == n2->x && n1->y == n2->y) return 0;
  return -1;
}

int init_apples(Apples** apples) {
  if ((*apples) == NULL) {
    if ((*apples = (Apples*)my_malloc(sizeof(Apples))) == NULL) return -1;
    (*apples)->count = 0;
    (*apples)->list = NULL;

    if ((*apples)->list == NULL) {
      list_init(&((*apples)->list), destroy, equals);
    }
  }
  return -1;
}

int init_snake(Snake** psnake, const unsigned int size) {
  if ((*psnake) == NULL) {
    if ((*psnake = (Snake*)my_malloc(sizeof(Snake))) == NULL) return -1;
    (*psnake)->size = size;
    (*psnake)->dir = left;
    (*psnake)->body = NULL;

    if ((*psnake)->body == NULL) {
      list_init(&((*psnake)->body), destroy, equals);
      unsigned int i;
      unsigned int mid_y = rows / 2;
      unsigned int mid_x = cols / 2;
      for (i = mid_x + size; i > mid_x; i--) {
        SnakeNode* n = (SnakeNode*)my_malloc(sizeof(SnakeNode));
        if (n == NULL) return -1;
        n->x = i;
        n->y = mid_y;
        if (i == (mid_x + size)) {
          n->head = true;
        } else {
          n->head = false;
        }
        list_ins_next((*psnake)->body, NULL, (const void*)n);
      }
      return 0;
    }
  }
  return -1;
}

void draw_node(void* data) {
  SnakeNode* n = (SnakeNode*)data;
  if (n->head) {
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    attron(COLOR_PAIR(6));
    mvprintw(n->y, n->x, snake_head);
    attroff(COLOR_PAIR(6));
  } else {
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    mvprintw(n->y, n->x, body);
    attroff(COLOR_PAIR(1));
  }
}

void draw_apple(void* data) {
  Apple* a = (Apple*)data;
  init_pair(2, COLOR_RED, COLOR_BLACK);
  attron(COLOR_PAIR(2));
  attron(A_BOLD);
  mvprintw((a)->y, (a)->x, apple);
  attroff(A_BOLD);
  attroff(COLOR_PAIR(2));
}

void erase_node(void* data) {
  SnakeNode* n = (SnakeNode*)data;
  mvprintw(n->y, n->x, " ");
}

void display_snake(Snake* psnake) { iterate(psnake->body, draw_node); }

void erase_snake(Snake* psnake) { iterate(psnake->body, erase_node); }

int destroy_snake(Snake** psnake) {
  if (*psnake != NULL) {
    if ((*psnake)->body != NULL) {
      list_destroy(&((*psnake)->body));
    }
    my_free(*psnake);
    (*psnake) = NULL;
    return 0;
  }

  return -1;
}
int destroy_apples(Apples** papples_) {
  if ((*papples_) != NULL) {
    if ((*papples_)->list != NULL) {
      list_destroy(&((*papples_)->list));
    }
    my_free(*papples_);
    (*papples_) = NULL;
    return 0;
  }

  return -1;
}
int found_apple(SnakeNode* head) {
  Node* found;
  if (find_element(papples->list, head, (void**)&found) == 0)
    return 1;
  else
    return 0;
}

void eat_apple(SnakeNode* head) {
  Node* found;
  Apple* tofree;
  if (find_element(papples->list, head, (void**)&found) == 0) {
    delete_element(papples->list, head, (void**)&tofree);
    papples->count--;
    my_free(tofree);
    time_out = TIME_OUT;
  }
}

void move_snake(Snake* psnake) {
  if (psnake) {
    SnakeNode* head = (SnakeNode*)psnake->body->head->data;
    SnakeNode* new_head = NULL;
    switch (psnake->dir) {
      case up:
        if (!collision(psnake)) {
          new_head = (SnakeNode*)my_malloc(sizeof(SnakeNode));
          new_head->y = head->y - 1;
          new_head->x = head->x;
          new_head->head = true;
          head->head = false;
          list_ins_next(psnake->body, NULL, new_head);

          if (found_apple(new_head)) {
            eat_apple(new_head);
            score++;
            if (papples->count == 0) {
              get_random_apple(psnake, papples);
            }

          } else {
            delete_tail(psnake->body);
          }
        }
        break;
      case down:
        if (!collision(psnake)) {
          new_head = (SnakeNode*)my_malloc(sizeof(SnakeNode));
          new_head->y = head->y + 1;
          new_head->x = head->x;
          new_head->head = true;
          head->head = false;

          list_ins_next(psnake->body, NULL, new_head);

          if (found_apple(new_head)) {
            eat_apple(new_head);
            score++;
            if (papples->count == 0) {
              get_random_apple(psnake, papples);
            }

          } else {
            delete_tail(psnake->body);
          }
        }
        break;
      case left:
        if (!collision(psnake)) {
          new_head = (SnakeNode*)my_malloc(sizeof(SnakeNode));
          new_head->y = head->y;
          new_head->x = head->x - 1;
          new_head->head = true;
          head->head = false;

          list_ins_next(psnake->body, NULL, new_head);
          if (found_apple(new_head)) {
            eat_apple(new_head);
            score++;
            if (papples->count == 0) {
              get_random_apple(psnake, papples);
            }
          } else {
            delete_tail(psnake->body);
          }
        }
        break;
      case right:
        if (!collision(psnake)) {
          new_head = (SnakeNode*)my_malloc(sizeof(SnakeNode));
          new_head->y = head->y;
          new_head->x = head->x + 1;
          new_head->head = true;
          head->head = false;

          list_ins_next(psnake->body, NULL, new_head);
          if (found_apple(new_head)) {
            eat_apple(new_head);
            score++;
            if (papples->count == 0) {
              get_random_apple(psnake, papples);
            }

          } else {
            delete_tail(psnake->body);
          }
        }
        break;
    }
  }
}

void display_apples() { iterate(papples->list, draw_apple); }

int self_collision(Snake* psnake, int x, int y) {
  SnakeNode n;
  SnakeNode* found;
  n.x = x;
  n.y = y;
  if (find_element(psnake->body, &n, (void**)&found) == 0)
    return 1;
  else
    return 0;
}

int collision(Snake* psnake) {
  if (psnake) {
    SnakeNode* head = (SnakeNode*)psnake->body->head->data;
    switch (psnake->dir) {
      case up:
        if (head->y - 1 == -1) return 1;
        if (self_collision(psnake, head->x, head->y - 1)) return 1;
        return 0;
        break;
      case down:
        if (head->y + 1 == rows) return 1;
        if (self_collision(psnake, head->x, head->y + 1)) return 1;
        return 0;
        break;
      case right:
        if (head->x + 1 == cols) return 1;
        if (self_collision(psnake, head->x + 1, head->y)) return 1;
        return 0;
        break;
      case left:
        if (head->x - 1 == -1) return 1;
        if (self_collision(psnake, head->x - 1, head->y)) return 1;
        return 0;
        break;
    }
  }
  return 0;
}

void get_input(Snake* psnake) {
  int ch = getch();
  switch (ch) {
    case KEY_LEFT:
      if (psnake->dir != right) psnake->dir = left;
      break;
    case KEY_DOWN:
      if (psnake->dir != up) psnake->dir = down;
      break;
    case KEY_UP:
      if (psnake->dir != down) psnake->dir = up;
      break;
    case KEY_RIGHT:
      if (psnake->dir != left) psnake->dir = right;
      break;
  }
}

void get_random_apple(Snake* psnake, Apples* apples) {
  Node n;
  Node* found;
  unsigned int y;
  unsigned int x;

  do {
    y = (rand() % rows - 1) + 1;
    x = (rand() % cols - 1) + 1;
    if (y <= 0) y = 1;
    if (y >= rows - 1) y = rows - 2;
    if (x <= 0) x = 1;
    if (x >= cols - 1) x = cols - 2;
    n.x = x;
    n.y = y;
  } while (find_element(psnake->body, &n, (void**)&found) == 0 || find_element(apples->list, &n, (void**)&found) == 0);

  Apple* papple = (Apple*)my_malloc(sizeof(Apple));
  if (papple) {
    papple->x = x;
    papple->y = y;
    list_ins_next(apples->list, NULL, (void*)papple);
    apples->count++;
  }
}

void check_timeout(Snake* psnake, Apples* apples) {
  if (time_out <= 0) {
    get_random_apple(psnake, apples);
    get_random_apple(psnake, apples);
    time_out = TIME_OUT;
  }
}

void display_walls() {
  unsigned int i;
  init_pair(3, COLOR_WHITE, COLOR_BLACK);
  attron(COLOR_PAIR(3));
  for (i = 0; i < cols; i++) {
    mvprintw(0, i, wall);
    // mvprintw(rows - 1, i, wall);
  }
  for (i = 0; i < rows; i++) {
    mvprintw(i, 0, wall);
    mvprintw(i, cols - 1, wall);
  }
  attroff(COLOR_PAIR(3));
}

void display_timeout() {
  unsigned int i;
  float step = cols / 1000.0;
  unsigned int x = (1000 - time_out) * step;

  init_pair(7, COLOR_RED, COLOR_BLACK);
  for (i = 0; i < cols; i++) {
    if (i < x) {
      attron(COLOR_PAIR(7));
      mvprintw(rows - 1, i, wall);
      attroff(COLOR_PAIR(7));
    } else {
      attron(COLOR_PAIR(3));
      mvprintw(rows - 1, i, wall);
      attroff(COLOR_PAIR(3));
    }
  }
}

void display_score() {
  char buf[100];
  if (cols >= 40) {
    sprintf(buf, " [ score: %d lives: %d ] ", score, lives);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);
    attron(COLOR_PAIR(5));
    mvprintw(0, 3, buf);
    attroff(COLOR_PAIR(5));
  }
}

void display_final_score() {
  clear_display();
  char buf[30];
  sprintf(buf, "Your score is: %d", score);
  init_pair(5, COLOR_CYAN, COLOR_BLACK);
  attron(COLOR_PAIR(5));
  mvprintw(rows / 2, cols / 2 - 8, buf);
  attroff(COLOR_PAIR(5));
  refresh();
  sleep(2);
}

void display_game_over() {
  clear_display();
  init_pair(5, COLOR_CYAN, COLOR_BLACK);
  attron(COLOR_PAIR(5));
  mvprintw(rows / 2, cols / 2 - 5, "Game Over!");
  attroff(COLOR_PAIR(5));
  refresh();
  sleep(3);
}

void display_title() {
  init_pair(5, COLOR_CYAN, COLOR_BLACK);
  attron(COLOR_PAIR(5));
  mvprintw(rows / 2 - 1, cols / 2 - 6, "Snake Curses");
  attroff(COLOR_PAIR(5));
  refresh();
  sleep(3);
}

void display_lives_left() {
  clear_display();
  char buf[30];
  sprintf(buf, "Lives left: %d", lives);
  init_pair(5, COLOR_CYAN, COLOR_BLACK);
  attron(COLOR_PAIR(5));
  mvprintw(rows / 2 - 1, cols / 2 - 7, buf);
  attroff(COLOR_PAIR(5));
  refresh();
  sleep(2);
}

int main() {
  initscr();
  cbreak();
  nodelay(stdscr, TRUE);
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  getmaxyx(stdscr, rows, cols);
  srand(time(NULL));
  start_color();
  display_title();

  while (lives > 0) {
    init_snake(&snake, cols / 4);
    init_apples(&papples);
    clear_display();
    get_random_apple(snake, papples);
    time_out = TIME_OUT;

    while (!collision(snake)) {
      getmaxyx(stdscr, nrows, ncols);
      if (rows != nrows || cols != ncols) {
        clear_display();
        rows = nrows;
        cols = ncols;
        destroy_apples(&papples);
        destroy_snake(&snake);
        init_snake(&snake, cols / 4);
        init_apples(&papples);
        get_random_apple(snake, papples);
      }
      get_input(snake);
      time_out -= floor(sqrt(score) * 2);
      check_timeout(snake, papples);
      clear_display();
      display_walls();
      display_timeout();
      display_score();
      display_apples();
      move_snake(snake);
      display_snake(snake);
      refresh();
      usleep(100000 - score * 100);
    }
    destroy_snake(&snake);
    destroy_apples(&papples);
    lives--;
    if (lives) {
      display_lives_left();
    }
  }
  display_final_score();
  display_game_over();
  clear();
  curs_set(2);
  endwin();
  return 0;
}