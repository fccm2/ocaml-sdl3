open Sdl3

let () =
  Sdl3.init ();
  let w, r = Render.create_window_and_renderer (340, 220) in

  let rec event_loop () =
    let e = Events.poll_event () in
    match e with
    | None -> ()
    | Some ev ->
        match ev with
        | Event_Quit -> Sdl3.quit (); exit 0
        | _ -> event_loop ()
  in
  let rec aux () =
    event_loop ();
    Render.set_draw_color r (20, 80, 210);
    Render.clear r;
    Render.present r;
    Timer.delay 600;
    aux ()
  in
  aux ()
;;
