#include <stdio.h>
#include "../derp.h"
#include <assert.h>

dbuf_decl(int, nums)

int main(void) {
  dlog_init(DLOG_TRACE);
  dlog_trace("hello there");

  int str[6] = {'H', 'e', 'l', 'l', 'o', '\0'};
  dbuf_nums chars = dbuf_init_nums(str, 6);
  dbuf_foreach(int, _char, chars) {
    printf("char is %c\n", (char)*_char);
  }

  dbuf_nums q = { 0 };
  q.alloc = def_allocator;
  printf("=== PUSHING INTS ===\n");
  printf("cur: %zd, len: %zd\n", q.cur, q.len);
  dbuf_push_nums(&q, 10);
  dbuf_push_nums(&q, 20);
  dbuf_push_nums(&q, 30);  
  dbuf_push_nums(&q, 2026);
  dbuf_push(&q, int, 6405);

  dbuf_foreach(int, i, q) {
    printf("DATA: %i\n", *i);
  }

  printf("=== SETTING INT ===\n");
  dbuf_set_nums(&q, 1, 40);

  dbuf_foreach(int, i, q) {
    printf("DATA: %i\n", *i);
  }

  printf("=== INSERTING INT ===\n");

  dbuf_insert_nums(&q, 50, 1);
  dbuf_insert(&q, int, 1, 444);

  dbuf_foreach(int, i, q) {
    printf("DATA: %i\n", *i);
  }

  printf("=== GETTING INT ===\n");

  int y = dbuf_get_nums(&q, 1);
  printf("int at %i is: %i\n", 1, y);

  printf("=== POPPING INT ===\n");
  int x = dbuf_pop_nums(&q);
  (void)dbuf_pop(&q, int); 
  printf("Popped int is: %i\n", x);

  dbuf_foreach(int, i, q) {
    printf("DATA: %i\n", *i);
  }

  dbuf_dstr f = dfile_read_lines("input.txt");
  dbuf_foreach(dstr, st, f) {
    printf("line: %.*s\n", dstr_fmt(*st));
  }

  dbuf_foreach(dstr, st, f) {
    d_free(st->cptr);
  }

  dbuf_free(&q);
  dbuf_free(&chars);  
  dbuf_free(&f);
}
