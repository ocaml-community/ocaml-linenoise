(** OCaml bindings to linenoise, functions that can fail use result
    type *)

(** Abstract type of completions, given to your completion callback *)
type completions

(** This function is used by the callback function registered by the
    user in order to add completion options given the input string
    when the user typed <TAB>. *)
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
val history_add : string -> (unit, string) result

(** Set the maximum length for the history. This function can be
    called even if there is already some history, the function will
    make sure to retain just the latest 'len' elements if the new
    history length value is smaller than the amount of items already
    inside the history. *)
val history_set : max_length:int -> (unit, string) result

(** Save the history in the specified file *)
val history_save : filename:string -> (unit, string) result

(** Load the history from the specified file. *)
val history_load : filename:string -> (unit, string) result

(** Clear the screen; used to handle CTRL+L *)
val clear_screen : unit -> unit

(** If [true], [ctrl-c] during a call to {!linenoise}
    will raise [Sys.Break] instead of returning an empty string.
    @since 1.1 *)
val catch_break : bool -> unit

(** Set if to use or not use the multi line mode. *)
val set_multiline : bool -> unit

(** This special mode is used by linenoise in order to print scan
    codes on screen for debugging / development purposes. *)
val print_keycodes : unit -> unit

(** What color you want the hints to be. *)
type hint_color = Red | Green | Yellow | Blue | Magenta | Cyan | White

(** Set a hints callback, callback gets a string, aka the line input,
    and you get a chance to give a hint to the user. Example, imagine
    if user types git remote add, then you can give a hint of <this is
    where you add a remote name> <this is where you add the remote's
    URL>, see animated gif in source repo for clear example. Returned
    tuple represents the hint message, color, and whether it ought to
    be bold. *)
val set_hints_callback : (string -> (string * hint_color * bool) option) -> unit
