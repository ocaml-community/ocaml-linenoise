#define CAML_NAME_SPACE
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

#include "linenoise_src.h"

CAMLprim value ml_add_completion(value completions)
{
  CAMLparam1(completions);

  CAMLreturn(Val_unit);
}

CAMLprim value ml_set_completion_cb(value callback)
{
  CAMLparam1(callback);

  CAMLreturn(Val_unit);
}

CAMLprim value ml_linenoise(value prompt)
{
  CAMLparam1(prompt);
  const char *fn = caml_strdup(String_val(prompt));
  CAMLreturn(caml_copy_string("42"));
}

CAMLprim value ml_history_add(value line)
{
  CAMLparam1(line);
  CAMLreturn(linenoiseHistoryAdd(caml_strdup(String_val(line))));
}

CAMLprim value ml_history_set_maxlen(value max)
{
  CAMLparam1(max);
  CAMLreturn(caml_copy_nativeint(linenoiseHistorySetMaxLen(Int_val(max))));
}

CAMLprim value ml_history_save(value filename)
{
  CAMLparam1(filename);
  CAMLreturn(linenoiseHistorySave(caml_strdup(String_val(filename))));
}

CAMLprim value ml_history_load(value filename)
{
  CAMLparam1(filename);
  CAMLreturn(linenoiseHistoryLoad(caml_strdup(String_val(filename))));
}

CAMLprim value ml_clearscreen(void)
{
  CAMLparam0();
  linenoiseClearScreen();
  CAMLreturn(Val_unit);
}

CAMLprim value ml_set_multiline(value use_multiline)
{
  CAMLparam1(use_multiline);
  linenoiseSetMultiLine(Bool_val(use_multiline));
  CAMLreturn(Val_unit);
}

CAMLprim value ml_printkeycodes(void)
{
  CAMLparam0();
  linenoisePrintKeyCodes();
  CAMLreturn(Val_unit);
}

