//Standard C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
// OCaml declarations
#include <caml/mlvalues.h>
#include <caml/alloc.h>
#include <caml/memory.h>
#include <caml/fail.h>
#include <caml/bigarray.h>
#include <caml/callback.h>
#include <caml/signals.h>

#include "linenoise.h"

CAMLprim value foo(value junk)
{
  printf("Called from OCaml\n");
  return Val_unit;
}
