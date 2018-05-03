open Result

type completions

external add_completion : completions -> string -> unit = "ml_add_completion"

external linenoise : string -> string option = "ml_linenoise"

external history_add_ : string -> int = "ml_history_add"
external history_set_ : max_length:int -> int = "ml_history_set_maxlen"
external history_save_ : filename:string -> int = "ml_history_save"
external history_load_ : filename:string -> int = "ml_history_load"

external catch_break : bool -> unit = "ml_catch_break"

external setup_bridges : unit -> unit = "ml_setup_bridges"

type hint_color = Red | Green | Yellow | Blue | Magenta | Cyan | White

let completion_cb = ref (fun _ _ -> ())
let hints_cb = ref (fun _ -> None)

let set_completion_callback (f:string->completions->unit) : unit =
  completion_cb := f;
  Callback.register "lnoise_completion_cb" f

let set_hints_callback (f:string -> (string*hint_color*bool) option) : unit =
  hints_cb := f;
  Callback.register "lnoise_hints_cb" f

(* initialization: register [Sys.Break] and enable catch-break *)
let () =
  setup_bridges();
  set_completion_callback !completion_cb;
  set_hints_callback !hints_cb;
  Callback.register_exception "sys_break" Sys.Break;
  catch_break true

let history_add h =
  if history_add_ h = 0 then Error "Couldn't add to history"
  else Ok ()

let history_set ~max_length =
  if history_set_ ~max_length = 0
  then Error "Couldn't set the max length of history"
  else Ok ()

let history_save ~filename =
  if history_save_ ~filename = 0 then Ok ()
  else Error "Couldn't save"

let history_load ~filename =
  if history_load_ ~filename = 0 then Ok ()
  else Error "Couldn't load the file"

external clear_screen : unit -> unit = "ml_clearscreen"
external set_multiline : bool -> unit = "ml_set_multiline"
external print_keycodes : unit -> unit = "ml_printkeycodes"
