#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snake_utils.h"
#include "state.h"

/* Helper function definitions */
static char get_board_at(game_state_t* state, int x, int y);
static void set_board_at(game_state_t* state, int x, int y, char ch);
static bool is_tail(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static int incr_x(char c);
static int incr_y(char c);
static void find_head(game_state_t* state, int snum);
static char next_square(game_state_t* state, int snum);
static void update_tail(game_state_t* state, int snum);
static void update_head(game_state_t* state, int snum);

/* Helper function to get a character from the board (already implemented for you). */
static char get_board_at(game_state_t* state, int x, int y) {
  return state->board[y][x];
}

/* Helper function to set a character on the board (already implemented for you). */
static void set_board_at(game_state_t* state, int x, int y, char ch) {
  state->board[y][x] = ch;
}

/* Task 1 */
game_state_t* create_default_state() {
  //自己再写一遍。。。。
  // TODO: Implement this function.
  game_state_t* temp = (game_state_t *) malloc(sizeof(game_state_t));
  temp->x_size = 14;
  temp->y_size = 10;
  unsigned int width = temp->x_size;
  unsigned int height = temp->y_size;
  temp->board = (char **)malloc(sizeof(char*) * height);
  for (int i = 0; i < height; i++) {
    temp->board[i] = (char *)malloc(sizeof(char) * (width + 1));
    for(int j = 0; j < width; j++) {
      if (i == 0 || j == 0 ||i == height - 1 || j == width - 1) {
        temp->board[i][j] = '#';
      } else {
        temp->board[i][j] = ' ';
      }
    }
    temp->board[i][width] = 0;
  }
  set_board_at(temp,9,2,'*');
  set_board_at(temp,4,4,'d');
  set_board_at(temp,5,4,'>');
  temp->num_snakes = 1;
  snake_t * snake = (snake_t *)malloc(sizeof(snake_t) * temp->num_snakes);
  snake[0].head_x = 5;
  snake[0].head_y = 4;
  snake[0].tail_x = 4;
  snake[0].tail_y = 4;
  snake->live = true;
  temp->snakes = snake;
  return temp;
}

/* Task 2 */
void free_state(game_state_t* state) {
  // TODO: Implement this function.

  // for (int j = 0; j < state->num_snakes; j++) {
  free(state->snakes);
    // state->snakes[i] = NULL;
  // }
  // free(state->snakes);
  state->snakes = NULL;
  for (int i = 0; i < state->y_size; i++) {
    free(state->board[i]);
    state->board[i] = NULL;
  }
  free(state->board);
  state->board = NULL;
  free(state);
  state = NULL;
  return;
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
  // TODO: Implement this function.
  for(int i = 0; i < state->y_size; i++) {
    for(int j = 0; j < state->x_size; j++) {
      fprintf(fp, "%c", state->board[i][j]);
    }
    fprintf(fp,"%c",'\n');
  }
  // fclose(fp);
  return;
}

/* Saves the current state into filename (already implemented for you). */
void save_board(game_state_t* state, char* filename) {
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */
static bool is_tail(char c) {
  // TODO: Implement this function.
  return (c == 'a'||c=='w'||c=='s'||c=='d');
}

static bool is_snake(char c) {
  // TODO: Implement this function.
  return (c=='^'||c=='<'||c=='>'||c=='v'||c=='x'||is_tail(c));
}

static char body_to_tail(char c) {
  // TODO: Implement this function.
  char temp;
  switch(c) {
    case '<': temp = 'a'; break;
    case '>': temp = 'd'; break;
    case '^': temp = 'w'; break;
    case 'v': temp = 's'; break;
    default: ;
  }
  return temp;
}

static int incr_x(char c) {
  // TODO: Implement this function.
  if(c=='>'||c=='d') {
    return 1;
  } else if (c=='<'||c=='a') {
    return -1;
  }
  return 0;
}

static int incr_y(char c) {
  // TODO: Implement this function.
  if(c=='v'||c=='s'){
    return 1;
  } else if(c=='^'||c=='w'){
    return -1;
  }
  return 0;
}

/* Task 4.2 */
static char next_square(game_state_t* state, int snum) {
  // TODO: Implement this function.
  snake_t temp = state->snakes[snum];
  char head = get_board_at(state, temp.head_x, temp.head_y);
  // if(head == '^'||head == 'v') {
  //   return get_board_at(state,temp.head_x, temp.head_y + incr_y(head));
  // } else if(head=='<'||head=='>'){
  //   return get_board_at(state,temp.head_x + incr_x(head), temp.head_y);
  // }
  return get_board_at(state, temp.head_x + incr_x(head), temp.head_y + incr_y(head));
  // return ' ';
}

/* Task 4.3 */
static void update_head(game_state_t* state, int snum) {
  // TODO: Implement this function.
  snake_t* temp = &(state->snakes[snum]);
  char head = get_board_at(state, temp->head_x, temp->head_y);
  temp->head_x += incr_x(head);
  temp->head_y += incr_y(head);
  set_board_at(state, temp->head_x, temp->head_y, head);
  return;
}

/* Task 4.4 */
static void update_tail(game_state_t* state, int snum) {
  // TODO: Implement this function.
  snake_t* temp = &(state->snakes[snum]);
  char tail = get_board_at(state, temp->tail_x, temp->tail_y);
  set_board_at(state, temp->tail_x, temp->tail_y, ' ');
  temp->tail_x += incr_x(tail);
  temp->tail_y += incr_y(tail);
  char next_tail = get_board_at(state, temp->tail_x, temp->tail_y);
  set_board_at(state,temp->tail_x,temp->tail_y,body_to_tail(next_tail));
  return;
}

/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
  // TODO: Implement this function.
  for(int i = 0; i < state->num_snakes; i++) {
    char next_sq = next_square(state, i);
    if(next_sq == ' ') {
      update_head(state, i);//先update head，后update tail
      update_tail(state, i);
    } else if(next_sq == '*') {
      update_head(state, i);
      add_food(state);
    } else if (next_sq == '#' ||is_snake(next_sq)) {
      set_board_at(state, state->snakes[i].head_x, state->snakes[i].head_y, 'x');
      state->snakes[i].live = false;
    }
  }
  return;
}

/* Task 5 */
game_state_t* load_board(char* filename) {
  // TODO: Implement this function.
  FILE *fp = fopen(filename, "r");
  game_state_t *temp = (game_state_t *)malloc(sizeof(game_state_t));
  char *str = (char *)malloc(sizeof(char) * 100 + 1);
  int y_size = 1;
  str = fgets(str, 100, fp);//str = 可以删除？
  int x_size = strlen(str) - 1;//会把'\n'读入str，strlen会计算'\n'字符 
  while(fgets(str, 100, fp)) {
    y_size++;
  }
  rewind(fp);
  temp->x_size = x_size;
  temp->y_size = y_size;
  temp->board = (char **)malloc(sizeof(char *) * y_size);
  char to_read;
  for(int i = 0; i < y_size; i++) {
    temp->board[i] = (char *)malloc(sizeof(char) * x_size);
    for(int j = 0; j < x_size + 1; j++) {
      to_read = (char)fgetc(fp);
      if(to_read=='\n'){
        break;
      }
      temp->board[i][j] = to_read;
    }
  }
  fclose(fp);
  return temp;
}

/* Task 6.1 */
static void find_head(game_state_t* state, int snum) {
  // TODO: Implement this function.
  snake_t* temp = &(state->snakes[snum]);//这里必须根据地址访问
  int head_x = temp->tail_x;
  int head_y = temp->tail_y;
  char body = get_board_at(state, head_x, head_y);
  char next_body = get_board_at(state, head_x + incr_x(body), head_y + incr_y(body));
  while(next_body=='<'||next_body=='>'||next_body=='v'||next_body=='^') {
    head_x += incr_x(body);
    head_y += incr_y(body);
    body = next_body;
    next_body = get_board_at(state, head_x + incr_x(body), head_y + incr_y(body));
  }
  temp->head_x = head_x;
  temp->head_y = head_y;
  return;
}

/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
  // TODO: Implement this function.
  int tail[10][2];
  int k = 0;
  state->num_snakes = 0;
  for(int i = 0; i < state->y_size; i++) {
    for(int j = 0; j < state->x_size; j++) {
      char p = get_board_at(state, j, i);
      if(p=='w'||p=='a'||p=='s'||p=='d'){
        (state->num_snakes)++;
        tail[k][0] = i;
        tail[k][1] = j;
        k++;
      }
    }
  }
  k = 0;
  state->snakes = (snake_t *)malloc(sizeof(snake_t) * state->num_snakes);
  for(int num = 0; num < state->num_snakes; num++) {
    state->snakes[num].tail_x = tail[num][1];
    state->snakes[num].tail_y = tail[num][0];
    find_head(state, num);
    state->snakes[num].live = true;
  }
  // state->snakes = snakes;
  return state;
}
