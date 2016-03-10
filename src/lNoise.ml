type completions

external add_completion : completions -> string -> unit = "ml_add_completion"

external set_completion_callback :
  (string -> completions -> unit) -> unit = "ml_set_completion_cb"
external linenoise : string -> string option = "ml_linenoise"

external history_add : string -> int = "ml_history_add"
external history_set : max_length:int -> int = "ml_history_set_maxlen"
external history_save : filename:string -> int = "ml_history_save"
external history_load : filename:string -> int = "ml_history_load"

external clear_screen : unit -> unit = "ml_clearscreen"
external set_multiline : bool -> unit = "ml_set_multiline"
external print_keycodes : unit -> unit = "ml_printkeycodes"
