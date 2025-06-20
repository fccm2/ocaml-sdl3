open Sdl3

type sprite = {
  x: int;
  y: int;
  w: int;
  h: int;
  name: string;
}

let sprites = [|
  { x = 48; y = 34; w = 15; h = 16; name = "abs-12a" };
  { x = 37; y =  0; w = 16; h = 17; name = "abs-12b" };
  { x = 20; y = 17; w = 16; h = 17; name = "abs-12c" };
  { x = 20; y =  0; w = 17; h = 17; name = "abs-12d" };
  { x =  0; y = 50; w = 16; h = 15; name = "abs-12e" };
  { x =  0; y = 18; w = 15; h = 17; name = "abs-12f" };
  { x = 15; y = 34; w = 17; h = 16; name = "abs-12g" };
  { x =  0; y =  0; w = 20; h = 18; name = "abs-12h" };
  { x = 32; y = 34; w = 16; h = 16; name = "abs-12i" };
  { x = 36; y = 17; w = 16; h = 17; name = "abs-12j" };
|]

let rect s =
  (s.x, s.y, s.w, s.h)

let dst (x, y) s =
  (x, y, s.w, s.h)

let () =
  Sdl3.init ();
  let w, r = Render.create_window_and_renderer (340, 220) in

  let s = Surface.load_bmp ~file:"./atlas-2025-06-13_1.bmp" in
  Surface.set_color_key s (255, 0, 255);
  let t = Texture.create_texture_from_surface r s in

  let draw_sprite pos sprite =
    Render.render_texture r t (rect sprite) (dst pos sprite);
  in

  let rec event_loop () =
    let e = Events.poll_event () in
    match e with
    | None -> ()
    | Some ev ->
        match ev with
        | Quit_event -> Sdl3.quit (); exit 0
        | Key_down_event c ->
            begin match c with
            | Scancode.A -> Printf.printf " A \n%!";
            | Scancode.Escape -> Sdl3.quit (); exit 0
            | _ -> event_loop ()
            end
        | _ -> event_loop ()
  in
  let rec aux () =
    event_loop ();
    Render.set_draw_color r (20, 80, 210);
    Render.clear r;
    Array.iteri (fun i sprite ->
      draw_sprite (10 + (i * 28), 10) sprite;
    ) sprites;
    Render.present r;
    Timer.delay 600;
    aux ()
  in
  aux ()
;;
