type result = Ok

(* external linenoise : string -> string = "ml_linenoise" *)

(* external history_add : string -> result = "ml_history_add" *)
(* external history_set : max:int -> result = "ml_history_set_maxlen" *)
(* external history_save : filename:string -> result = "ml_history_save" *)
(* external history_load : filename:string -> result = "ml_history_load" *)

(* external clear_screen : unit -> unit = "ml_clearscreen" *)
(* external set_multiline : int -> unit = "ml_set_mutliline" *)
external print_keycodes : unit -> unit = "ml_printkeycodes"
