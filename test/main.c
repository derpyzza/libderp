#include <stddef.h>
#include <stdio.h>
#include "../derp.h"
#include <assert.h>

dbuf_decl(int, nums)

void dbuf_print_nums( dbuf_nums dbuf) {
  const char* name = "dbuf_nums";
  printf("[%s]:\n   > cur: %zd,\n   > len: %zd\n", name, dbuf.cur, dbuf.len);
  dbuf_foreach_id(int, i, j, dbuf) {
    printf("\x1b[31m[%0.2X]\x1b[0m: %i\n", j, *i);
  }
}

#define print_size(type) printf("sizeof[%s]: %zu\n", #type, sizeof(type));

int main(void) {

  dlog_init(DLOG_TRACE);
  dlog_trace("hello there");

  dstr f = dstr_from("HELLO THERE");

  dlog_debug("dstr: %.*s", dstr_fmt(f));
  dstr_to_lower(f);
  dlog_debug("dstr: %.*s", dstr_fmt(f));
  dstr_to_upper(f);
  dlog_debug("dstr: %.*s", dstr_fmt(f));

  dstr_replace(&f, "THERE", "[YOUR NAME]");
  dlog_debug("dstr: %.*s", dstr_fmt(f));


  return 0;

  int str[6] = {'H', 'e', 'l', 'l', 'o', '\0'};
  dbuf_nums chars = dbuf_nums_init(str, 6);
  dbuf_foreach(int, _char, chars) {
    printf("char is %c\n", (char)*_char);
  }

  dbuf_nums q = { 0 };
  q.alloc = def_allocator;
  printf("=== PUSHING INTS ===\n");
  dbuf_nums_push(&q, 10);
  dbuf_nums_push(&q, 20);
  dbuf_nums_push(&q, 30);  
  dbuf_nums_push(&q, 2026);
  dbuf_push(&q, int, 6405);

  dbuf_print_nums(q);

  printf("=== SETTING INT ===\n");
  dbuf_nums_set(&q, 1, 40);

  dbuf_print_nums(q);

  printf("=== INSERTING INT ===\n");

  dbuf_nums_insert(&q, 50, 1);
  dbuf_insert(&q, int, 444, 1);

  dbuf_print_nums(q);

  printf("=== REMOVING INT ===\n");
  dbuf_nums_remove(&q, 3);

  dbuf_print_nums(q);

  printf("=== GETTING INT ===\n");

  int y = dbuf_nums_get(&q, 1);
  printf("int at %i is: %i\n", 1, y);

  printf("=== POPPING INT ===\n");
  int x = dbuf_nums_pop(&q);
  printf("Popped int is: %i\n", x);
  x = *(int*)dbuf_pop(&q, int); 
  printf("Popped int is: %i\n", x);

  dbuf_print_nums(q);

  printf("=== APPENDING INTS ===\n");

  int ints[5] = {2, 4, 6, 8, 10};
  dbuf_nums_append(&q, ints, 5);

  dbuf_print_nums(q);

  printf("=== PREPENDING INTS ===\n");

  dbuf_nums_prepend(&q, ints, 5);

  dbuf_print_nums(q);

  printf("=== CLONENING INTS ===\n");

  dbuf_nums new = dbuf_nums_clone(q);

  dbuf_print_nums(new);

  printf("=== DBUF END ===\n");

  // dbuf_dstr f = dfile_read_lines("input.txt");
  // dbuf_foreach(dstr, st, f) {
  //   printf("line: %.*s\n", dstr_fmt(*st));
  // }

  // dbuf_foreach(dstr, st, f) {
  //   d_free(st->cptr);
  // }

  // dbuf_free(&q);
  // dbuf_free(&chars);  
  // dbuf_free(&f);
}
