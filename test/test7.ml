open Sdl3

let () =
  Sdl3.init ();
  let w, r = Render.create_window_and_renderer (340, 220) in

  let running = ref 0 in
  while !running < 100 do
    Printf.printf " %d%!" !running;
    Render.set_draw_color r (20, 80, 210);
    Render.clear r;
    Render.present r;
    incr running;
  done;
  Printf.printf "\n";
;;
