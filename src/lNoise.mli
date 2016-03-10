(** OCaml bindings to linenoise. *)

(** Abstract type of completions, given to your completion callback *)
type completions

(** This function is used by the callback function registered by the
    user in order to add completion options given the input string
    when the user typed <tab>. *)
val add_completion : completions -> string -> unit

(** Register the callback function that is called for upon
    tab-completion, aka when <TAB> is hit in the terminal *)
val set_completion_callback : (string -> completions -> unit) -> unit

(** The high level function that is the main API of the linenoise
    library. This function checks if the terminal has basic
    capabilities, just checking for a blacklist of stupid terminals,
    and later either calls the line editing function or uses dummy
    fgets() so that you will be able to type something even in the
    most desperate of the conditions. *)
val linenoise : string -> string option

(** Add a string to the history *)
val history_add : string -> int

(** Set the maximum length for the history. This function can be
    called even if there is already some history, the function will
    make sure to retain just the latest 'len' elements if the new
    history length value is smaller than the amount of items already
    inside the history. *)
val history_set : max_length:int -> int

(** Save the history in the specified file. On success 0 is returned
    otherwise -1 is returned. *)
val history_save : filename:string -> int

(** Load the history from the specified file. If the file does not
    exist zero is returned and no operation is performed.  If the file
    exists and the operation succeeded 0 is returned, otherwise on
    error -1 is returned. *)
val history_load : filename:string -> int

(** Clear the screen. Used to handle ctrl+l *)
val clear_screen : unit -> unit

(** Set if to use or not the multi line mode. *)
val set_multiline : bool -> unit

(** This special mode is used by linenoise in order to print scan
    codes on screen for debugging / development purposes. *)
val print_keycodes : unit -> unit
