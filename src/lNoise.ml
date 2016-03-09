type completions

external add_completion : completions -> unit = "ml_add_completion"

external set_completion_callback :
  (string -> completions -> unit) -> unit = "ml_set_completion_cb"

external linenoise : string -> string option = "ml_linenoise"

external history_add : string -> int = "ml_history_add"

(* Set the maximum length for the history. This function can be called even
 * if there is already some history, the function will make sure to retain
 * just the latest 'len' elements if the new history length value is smaller
 * than the amount of items already inside the history. *)
external history_set : max:int -> int = "ml_history_set_maxlen"
external history_save : filename:string -> int = "ml_history_save"
external history_load : filename:string -> int = "ml_history_load"

external clear_screen : unit -> unit = "ml_clearscreen"
(* Set if to use or not the multi line mode. *)
external set_multiline : bool -> unit = "ml_set_mutliline"
external print_keycodes : unit -> unit = "ml_printkeycodes"
