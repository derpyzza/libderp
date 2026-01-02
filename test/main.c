#include <stdio.h>
#include <stdlib.h>
#include "../derp.h"
#include <assert.h>

dbuf_decl(int, nums)

void dbuf_push_int(dbuf_nums * d, int i) {
  dbuf_data buf = { 0 };
  buf.data = (void**)&d->data;
  buf.cur = &d->cur;
  buf.len = &d->len;
  buf.elem_size = sizeof(int);
  buf.alloc = &d->alloc;
  _dbuf_push( &(int){i}, buf );
}

int dbuf_pop_int(dbuf_nums * d) {
  dbuf_data buf = { 0 };
  buf.data = (void**)&d->data;
  buf.cur = &d->cur;
  buf.len = &d->len;
  buf.elem_size = sizeof(int);
  buf.alloc = &d->alloc;
  int * x = (int*)_dbuf_pop( buf );
  return *x;
}

int dbuf_get_int(dbuf_nums * d, isize id) {
  dbuf_data buf = { 0 };
  buf.data = (void**)&d->data;
  buf.cur = &d->cur;
  buf.len = &d->len;
  buf.elem_size = sizeof(int);
  buf.alloc = &d->alloc;
  int * x = (int*)_dbuf_get( id, buf );
  printf("GOOOT: %i\n", *x);
  return *x;
}

void dbuf_set_int(dbuf_nums * d, isize id, int i) {
  dbuf_data buf = { 0 };
  buf.data = (void**)&d->data;
  buf.cur = &d->cur;
  buf.len = &d->len;
  buf.elem_size = sizeof(int);
  buf.alloc = &d->alloc;
  _dbuf_set( id, &i, buf );
}

void dbuf_insert_int(dbuf_nums * d, int i, isize id) {
  dbuf_data buf = { 0 };
  buf.data = (void**)&d->data;
  buf.cur = &d->cur;
  buf.len = &d->len;
  buf.elem_size = sizeof(int);
  buf.alloc = &d->alloc;
  _dbuf_insert(&i, id, buf );
}

dbuf_nums dbuf_init_int( int* data, isize len) {
  dbuf_data buf = { 0 };

  dbuf_nums n = { 0 };

  buf.data = (void**)&n.data;
  buf.cur = &n.cur;
  buf.len = &n.len;
  buf.alloc = &n.alloc;
  buf.elem_size = sizeof(int);
  _dbuf_init( (void**)&data, len, buf );
  return n;
}

int main(void) {
  dlog_init(DLOG_TRACE);
  dlog_trace("hello there");

  int str[6] = {'H', 'e', 'l', 'l', 'o', '\0'};
  dbuf_nums chars = dbuf_init_int((int*)str, 6);
  dbuf_foreach(int, _char, chars) {
    printf("char is %c\n", (char)*_char);
  }

  dbuf_nums q = { 0 };
  q.alloc = def_allocator;
  printf("=== PUSHING INTS ===\n");
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

  printf("=== SETTING INT ===\n");
  dbuf_set_int(&q, 1, 40);

  dbuf_foreach(int, i, q) {
    printf("DATA: %i\n", *i);
  }

  printf("=== INSERTING INT ===\n");

  dbuf_insert_int(&q, 50, 1);

  dbuf_foreach(int, i, q) {
    printf("DATA: %i\n", *i);
  }

  printf("=== GETTING INT ===\n");

  int y = dbuf_get_int(&q, 1);
  printf("int at %i is: %i\n", 1, y);

  printf("=== POPPING INT ===\n");
  int x = dbuf_pop_int(&q);
  printf("Popped int is: %i\n", x);

  dbuf_foreach(int, i, q) {
    printf("DATA: %i\n", *i);
  }

  // q.alloc.free(q.data, (d_alloc_data){0});
  // dbuf_free(&chars);
  dbuf_free(&q);
  dbuf_free(&chars);
  
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
