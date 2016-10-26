// OCaml declarations
#include <caml/alloc.h>
#include <caml/memory.h>
#include <caml/callback.h>

#include "linenoise_src.h"

// Ripped from ctypes
#define Val_none Val_int(0)
#define Some_val(v) Field(v, 0)

static value Val_some(value v)
{
  CAMLparam1(v);
  CAMLlocal1(some);
  some = caml_alloc(1, 0);
  Store_field(some, 0, v);
  CAMLreturn(some);
}

CAMLprim value ml_add_completion(value completions, value new_completion)
{
  CAMLparam2(completions, new_completion);
  linenoiseAddCompletion((linenoiseCompletions *)completions,
			 caml_strdup(String_val(new_completion)));
  CAMLreturn(Val_unit);
}

static value completion_cb, hints_cb;

static void completion_bridge(const char *buf, linenoiseCompletions *lc)
{
  caml_callback2(completion_cb, caml_copy_string(buf), (value)lc);
}

static char *hints_bridge(const char *buf, int *color, int *bold)
{
  CAMLlocal1(cb_result);
  cb_result = caml_callback(hints_cb, caml_copy_string(buf));
  if (cb_result == Val_none) {
    return NULL;
  } else {
    const char *msg = caml_strdup(String_val(Field(Field(cb_result, 0), 0)));
    *color = Int_val(Field(Field(cb_result, 0), 1)) + 31;
    *bold = Bool_val(Field(Field(cb_result, 0), 2));
    return (char *)msg;
  }
}

__attribute__((constructor))
void set_free_hints(void) { linenoiseSetFreeHintsCallback(free); }

CAMLprim value ml_set_completion_cb(value completions)
{
  CAMLparam1(completions);
  completion_cb = completions;
  linenoiseSetCompletionCallback(completion_bridge);
  CAMLreturn(Val_unit);
}

CAMLprim value ml_set_hints_cb(value hints)
{
  CAMLparam1(hints);
  hints_cb = hints;
  linenoiseSetHintsCallback(hints_bridge);
  CAMLreturn(Val_unit);
}


CAMLprim value ml_linenoise(value prompt)
{
  CAMLparam1(prompt);
  CAMLlocal1(lnoise_result);
  const char *result = linenoise(caml_strdup(String_val(prompt)));
  if (!result) CAMLreturn(Val_none);
  lnoise_result = caml_copy_string(result);
  linenoiseFree((void*)result);
  CAMLreturn(Val_some(lnoise_result));
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

CAMLprim value ml_clearscreen(__attribute__((unused))value unit)
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
