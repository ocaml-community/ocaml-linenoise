// OCaml declarations
#include <caml/alloc.h>
#include <caml/memory.h>
#include <caml/callback.h>
#include <caml/fail.h>

#include <errno.h>
#include <assert.h>

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

/* if true, raise Sys.Break on ctrl-c */
static int raise_sys_break = 0;

CAMLprim value ml_catch_break(value flag)
{
  CAMLparam1(flag);
  raise_sys_break = Bool_val(flag);
  CAMLreturn(Val_unit);
}

CAMLprim value ml_add_completion(value completions, value new_completion)
{
  CAMLparam2(completions, new_completion);
  linenoiseAddCompletion((linenoiseCompletions *)completions,
			 caml_strdup(String_val(new_completion)));
  CAMLreturn(Val_unit);
}

static int has_completion_cb = 0, has_hints_cb = 0; /* callbacks set? */
static value completion_cb, hints_cb;

static void completion_bridge(const char *buf, linenoiseCompletions *lc)
{
  assert (has_completion_cb);
  caml_callback2(completion_cb, caml_copy_string(buf), (value)lc);
}

static char *hints_bridge(const char *buf, int *color, int *bold)
{
  CAMLparam0();
  CAMLlocal1(cb_result);
  assert(has_hints_cb);
  cb_result = caml_callback(hints_cb, caml_copy_string(buf));
  if (cb_result == Val_none) {
    CAMLreturnT(char *,NULL);
  } else {
    char *msg = caml_strdup(String_val(Field(Field(cb_result, 0), 0)));
    *color = Int_val(Field(Field(cb_result, 0), 1)) + 31;
    *bold = Bool_val(Field(Field(cb_result, 0), 2));
    CAMLreturnT(char *,msg);
  }
}

__attribute__((constructor))
void set_free_hints(void) { linenoiseSetFreeHintsCallback(free); }

/* Callbacks must be registered as global GC roots,
 * search "global" in https://caml.inria.fr/pub/docs/manual-ocaml/intfc.html#sec439
 */

CAMLprim value ml_set_completion_cb(value completions)
{
  CAMLparam1(completions);
  if (has_completion_cb) {
    caml_modify_generational_global_root(&completion_cb, completions);
  } else {
    has_completion_cb = 1;
    completion_cb = completions;
    caml_register_generational_global_root(&completion_cb);
  }
  linenoiseSetCompletionCallback(completion_bridge);
  CAMLreturn(Val_unit);
}

CAMLprim value ml_set_hints_cb(value hints)
{
  CAMLparam1(hints);
  if (has_hints_cb) {
    caml_modify_generational_global_root(&hints_cb, hints);
  } else {
    has_hints_cb = 1;
    hints_cb = hints;
    caml_register_generational_global_root(&hints);
  }
  linenoiseSetHintsCallback(hints_bridge);
  CAMLreturn(Val_unit);
}


CAMLprim value ml_linenoise(value prompt)
{
  CAMLparam1(prompt);
  CAMLlocal1(lnoise_result);
  linenoiseWasInterrupted = 0; // reset
  const char *result = linenoise(caml_strdup(String_val(prompt)));
  if (!result) {
    if (linenoiseWasInterrupted && raise_sys_break) {
      caml_raise_constant(*caml_named_value("sys_break"));
    } else {
      CAMLreturn(Val_none);
    }
  }
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
