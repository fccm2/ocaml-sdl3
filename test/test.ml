open Sdl3

let assoc_c = [
    Scancode.A, 'A'; Scancode.B, 'B'; Scancode.C, 'C'; Scancode.D, 'D';
    Scancode.E, 'E'; Scancode.F, 'F'; Scancode.G, 'G'; Scancode.H, 'H';
    Scancode.I, 'I'; Scancode.J, 'J'; Scancode.K, 'K'; Scancode.L, 'L';
    Scancode.M, 'M'; Scancode.N, 'N'; Scancode.O, 'O'; Scancode.P, 'P';
    Scancode.Q, 'Q'; Scancode.R, 'R'; Scancode.S, 'S'; Scancode.T, 'T';
    Scancode.U, 'U'; Scancode.V, 'V'; Scancode.W, 'W'; Scancode.X, 'X';
    Scancode.Y, 'Y'; Scancode.Z, 'Z';
    Scancode.N_0, '0';  Scancode.N_1, '1';  Scancode.N_2, '2';
    Scancode.N_3, '3';  Scancode.N_4, '4';  Scancode.N_5, '5';
    Scancode.N_6, '6';  Scancode.N_7, '7';  Scancode.N_8, '8';
    Scancode.N_9, '9';
  ]

module UserApp = struct
  type state = unit

  type app_calls = {
    app_init : unit -> window * renderer * state;
    app_iterate : renderer -> state -> unit;
    app_event : event -> state -> state;
    app_quit : window -> renderer -> state -> unit;
  }

  let app = {
    app_init = (fun () ->
      Sdl3.init ();
      let window, renderer = Render.create_window_and_renderer (200, 140) in
      (window, renderer, ())
    );
    app_event = (fun e () ->
      begin
        match e with
        | Unhandled_event -> ()
        | Mouse_motion (x, y) ->
            Printf.printf " %d %d\n%!" x y;
        | Key_down_event c ->
            begin match c with
            | Scancode.Left  -> Printf.printf " Left \n%!";
            | Scancode.Right -> Printf.printf " Right \n%!";
            | Scancode.Up    -> Printf.printf " Up \n%!";
            | Scancode.Down  -> Printf.printf " Down \n%!";
            | Scancode.A -> Printf.printf " A \n%!";
            | Scancode.B -> Printf.printf " B \n%!";
            | Scancode.C -> Printf.printf " C \n%!";
            | Scancode.Z -> Printf.printf " Z \n%!";
            | c -> Printf.printf " %c \n%!" (List.assoc c assoc_c)
            end
        | _ -> ()
      end;
      ()
    );
    app_quit = (fun w r state ->
      Render.destroy_renderer r;
      Render.destroy_window w;
      ()
    );
    app_iterate = (fun r state ->
      Render.set_draw_color r (210, 80, 20);
      Render.clear r;
      Render.set_draw_color r (20, 210, 80);
      Render.fill_rect r (20, 20, 20, 20);
      Render.present r;
      ()
    );
  }
end

module SomeApp = MakeApp(UserApp) ;;
SomeApp.run () ;;

