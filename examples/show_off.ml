let rec user_input prompt cb =
  match LNoise.linenoise prompt with
  | None -> ()
  | Some v -> cb v; user_input prompt cb

let () =
  (fun from_user -> Printf.sprintf "Got: %s" from_user)
  |> user_input "test_program> "
