// OCaml declarations
#include <caml/alloc.h>
#include <caml/memory.h>
#include <caml/callback.h>
#include <caml/fail.h>
#include <caml/threads.h>

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
  char* c_new_completion = caml_stat_strdup(String_val(new_completion));
  linenoiseAddCompletion((linenoiseCompletions *)completions, c_new_completion);
  caml_stat_free(c_new_completion);
  CAMLreturn(Val_unit);
}

// this bridge runs with the runtime lock acquired
static void completion_bridge_inner(const char *buf, linenoiseCompletions *lc)
{
  CAMLparam0();
  CAMLlocal1(str_copy);
  str_copy = caml_copy_string(buf);
  caml_callback2(*caml_named_value("lnoise_completion_cb"), str_copy, (value)lc);
  CAMLreturn0;
}

static void completion_bridge(const char *buf, linenoiseCompletions *lc)
{
  caml_acquire_runtime_system();
  completion_bridge_inner(buf, lc);
  caml_release_runtime_system();
}

static char *hints_bridge_inner(const char *buf, int *color, int *bold)
{
  CAMLparam0();
  CAMLlocal2(str_copy, cb_result);

  str_copy = caml_copy_string(buf);

  cb_result = caml_callback(*caml_named_value("lnoise_hints_cb"), str_copy);
  if (cb_result == Val_none) {
    CAMLreturnT(char *,NULL);
  } else {
    char* msg = caml_stat_strdup(String_val(Field(Field(cb_result, 0), 0)));
    *color = Int_val(Field(Field(cb_result, 0), 1)) + 31;
    *bold = Bool_val(Field(Field(cb_result, 0), 2));
    CAMLreturnT(char *,msg);
  }
}

static char *hints_bridge(const char *buf, int *color, int *bold)
{
  caml_acquire_runtime_system();
  char* res = hints_bridge_inner(buf, color, bold);
  caml_release_runtime_system();
  return res;
}


static void free_hints_bridge(void* data) {
  caml_acquire_runtime_system();
  caml_stat_free(data);
  caml_release_runtime_system();
}

__attribute__((constructor))
void set_free_hints(void) { linenoiseSetFreeHintsCallback(free); }

CAMLprim value ml_setup_bridges(value unit) {
  CAMLparam1(unit);
  linenoiseSetCompletionCallback(completion_bridge);
  linenoiseSetHintsCallback(hints_bridge);
  linenoiseSetFreeHintsCallback(free_hints_bridge);
  CAMLreturn(Val_unit);
}

CAMLprim value ml_linenoise(value prompt)
{
  CAMLparam1(prompt);
  CAMLlocal1(lnoise_result);

  linenoiseWasInterrupted = 0; // reset
  char* c_prompt = caml_stat_strdup(String_val(prompt));

  caml_release_runtime_system();
  const char *result = linenoise(c_prompt);
  caml_acquire_runtime_system();

  caml_stat_free(c_prompt);
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
  char* c_line = caml_stat_strdup(String_val(line));
  int res = linenoiseHistoryAdd(c_line);
  caml_stat_free(c_line);
  CAMLreturn(Val_int(res));
}

CAMLprim value ml_history_set_maxlen(value max)
{
  CAMLparam1(max);
  CAMLreturn(Val_int(linenoiseHistorySetMaxLen(Int_val(max))));
}

CAMLprim value ml_history_save(value filename)
{
  CAMLparam1(filename);
  char* c_filename = caml_stat_strdup(String_val(filename));
  int res = linenoiseHistorySave(c_filename);
  caml_stat_free(c_filename);
  CAMLreturn(Val_int(res));
}

CAMLprim value ml_history_load(value filename)
{
  CAMLparam1(filename);
  char* c_filename= caml_stat_strdup(String_val(filename));
  int res = linenoiseHistoryLoad(c_filename);
  caml_stat_free(c_filename);
  CAMLreturn(Val_int(res));
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
