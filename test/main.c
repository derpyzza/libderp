#include <stdio.h>
#include <stdlib.h>
#include "../derp.h"
#include <assert.h>

dbuf_decl(int, nums)

void dbuf_push_int(dbuf_nums * d, int i) {
  _dbuf_push( &(int){i}, (void**) &d->data, &d->len, &d->cur, sizeof(int) );
}

int dbuf_pop_int(dbuf_nums * d) {
  int * x = (int*)_dbuf_pop( (void*)d->data, &d->cur, sizeof(int));
  return *x;
}

int dbuf_get_int(dbuf_nums * d, isize id) {
  int * x = (int*)_dbuf_get( id, (void*)d->data, &d->cur, sizeof(int) );
  return *x;
}

void dbuf_set_int(dbuf_nums * d, isize id, int i) {
  _dbuf_set( &i, id, (void*)d->data, &d->cur, sizeof(int) );
}

void dbuf_insert_int(dbuf_nums * d, int i, isize id) {
  _dbuf_insert(&i, id, (void**)&d->data, &d->len, &d->cur, sizeof(int));
}

dbuf_nums dbuf_make_init_int(char* data, isize len) {
  dbuf_nums n = { 0 };
  _dbuf_init((void**)&data, len, (void**)&n.data, &n.len, &n.cur, sizeof(int));
  return n;
}

int main(void) {
  dlog_init(DLOG_TRACE);
  dlog_trace("hello there");

  int str[6] = {'H', 'e', 'l', 'l', 'o', '\0'};
  dbuf_nums chars = dbuf_make_init_int((char*)str, 6);
  dbuf_foreach(int, _char, chars) {
    printf("char is %c\n", (char)*_char);
  }

  dbuf_nums q = { 0 };
  printf("cur: %zd, len: %zd\n", q.cur, q.len);
  dbuf_push_int(&q, 10);
  dbuf_push_int(&q, 20);
  dbuf_push_int(&q, 30);  
  // dbuf_push(q, (int){10});
  // dbuf_push(q, (int){15});
  // dbuf_push(q, (int){20});
  dbuf_foreach(int, i, q) {
    printf("DATA: %i\n", *i);
  }

  printf("===\n");
  dbuf_set_int(&q, 1, 40);

  dbuf_foreach(int, i, q) {
    printf("DATA: %i\n", *i);
  }

  printf("===\n");

  dbuf_insert_int(&q, 50, 1);

  dbuf_foreach(int, i, q) {
    printf("DATA: %i\n", *i);
  }

  printf("===\n");

  int y = dbuf_get_int(&q, 1);
  printf("int at %i is: %i\n", 1, y);

  int x = dbuf_pop_int(&q);
  printf("Popped int is: %i\n", x);

  dbuf_foreach(int, i, q) {
    printf("DATA: %i\n", *i);
  }

  d_free(chars.data);
  d_free(q.data);
  
  return 0;

  dbuf_nums d = dbuf_new_nums(10);
  for(int i = 0; i < d.len; i++) {
    d.data[i] = (i+1) * (i+1);
  }
  
  int c = 10;
  int * values = calloc(c, sizeof(int));
  dforeach(int, i, values, c) {
    *i = (int)(i-values) * (int)(i-values);
    printf("NORMAL NUM: %i\n", *i);
  }

  dforeach_id(int, val, id, values, c) {
    printf("NORMAL NUM: %i at %i\n", *val, id);
  }

  dbuf_foreach(int, it, d) {
    printf("num: %i\n", *it);
  }

  dbuf_foreach_id(int, it, j, d) {
    printf("num at %i is %i\n", j, *it);
  }

}
