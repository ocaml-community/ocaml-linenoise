type completions

external add_completion : completions -> string -> unit = "ml_add_completion"

external set_completion_callback :
  (string -> completions -> unit) -> unit = "ml_set_completion_cb"
external linenoise : string -> string option = "ml_linenoise"

external history_add_ : string -> int = "ml_history_add"
external history_set_ : max_length:int -> int = "ml_history_set_maxlen"
external history_save_ : filename:string -> int = "ml_history_save"
external history_load_ : filename:string -> int = "ml_history_load"

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

type hint_color = Red | Green | Yellow | Blue | Magenta | Cyan | White
external set_hints_callback :
  (string -> (string * hint_color * bool) option) -> unit = "ml_set_hints_cb"
