open Sdl3

type direction = int * int

type element = {
  dir: direction;
  pos: int * int;
  sp_index: int;
}

type sprite = {
  sp: int * int * int * int;  (* (x, y, w, h) *)
  name: string;
}

(* sprites *)

let sprites = [|
  { sp = (48, 34, 15, 16); name = "abs-12a" }; (* beige *)
  { sp = (37,  0, 16, 17); name = "abs-12b" }; (* plain-green *)
  { sp = (20, 17, 16, 17); name = "abs-12c" }; (* orange-split2 *)
  { sp = (20,  0, 17, 17); name = "abs-12d" }; (* beige-broken *)
  { sp = ( 0, 50, 16, 15); name = "abs-12e" }; (* broken-darkblue *)
  { sp = ( 0, 18, 15, 17); name = "abs-12f" }; (* collectible-01 *)
  { sp = (15, 34, 17, 16); name = "abs-12g" }; (* darkgray *)
  { sp = ( 0,  0, 20, 18); name = "abs-12h" }; (* turkoise *)
  { sp = (32, 34, 16, 16); name = "abs-12i" }; (* split2-red *)
  { sp = (36, 17, 16, 17); name = "abs-12j" }; (* collectible-02 *)
|]

let directions = [|
  (1, 0);
  (0, 1);
  (-1, 0);
  (0, -1);
|]

let rand_take arr =
  let n = Array.length arr in
  let i = Random.int n in
  arr.(i)

(* dst-rectangle *)
let dst (x, y) (_, _, w, h) =
  (x, y, w, h)

(* draw-bg *)
let draw_bg r () =
  Render.set_draw_color r (52, 54, 56);
  Render.clear r;
;;

(* draw-sprite *)
let draw_sprite r t pos sprite =
  Render.render_texture r t (sprite.sp) (dst pos sprite.sp);
;;

(* main-draw *)
let display r t es () =
  draw_bg r ();
  Array.iter (fun e ->
    draw_sprite r t (e.pos) sprites.(e.sp_index);
  ) es;
  Render.present r;
;;

(* event-handlers *)

type key_change =
  | KeyDown
  | KeyUp

type mouse_change =
  | MouseDown
  | MouseUp

let mouse_event b (x, y) =
  match b with
  | MouseDown -> Printf.printf " %d %d \n%!" x y;
  | _ -> ()

let key_event m c =
  match m, c with
  | KeyDown, Scancode.A -> Printf.printf " A \n%!";
  | KeyDown, Scancode.Escape -> Sdl3.quit (); exit 0
  | _ -> ()

(* inits *)

let init_data r () =
  let s = Surface.load_bmp ~file:"./atlas-2025-06-13_1.bmp" in
  Surface.set_color_key s (255, 0, 255);
  let t = Texture.create_texture_from_surface r s in
  (t)

let rand_pos () =
  (Random.int 340,
   Random.int 220)

let init_elements () =
  Random.self_init ();
  Array.init 2 (fun i ->
    match i with
    | 0 -> {
        dir = rand_take directions;
        pos = rand_pos ();
        sp_index = 0;
      }
    | 1 -> {
        dir = rand_take directions;
        pos = rand_pos ();
        sp_index = 6;
      }
    | _ -> assert false
  )

let init r () =
  let t = init_data r () in
  let es = init_elements () in
  (t, es)

(* updates *)

let step_elem (x, y) (dx, dy) =
  (x + dx, y + dy)

let wrap_elem (x, y) =
  let x = if x > 340 then -20 else x in
  let y = if y > 220 then -20 else y in
  let x = if x < -20 then 340 else x in
  let y = if y < -20 then 220 else y in
  (x, y)

let dir_elem e =
  if Random.int 120 < 6
  then (rand_take directions)
  else e.dir

let update_elem e =
  let pos = step_elem e.pos e.dir in
  let pos = wrap_elem pos in
  let dir = dir_elem e in
  { e with pos; dir; }

let update es =
  let es = Array.map update_elem es in
  (es)

(* main *)
let () =
  Sdl3.init ();
  let w, r = Render.create_window_and_renderer (340, 220) in
  let t, es = init r () in

  let rec event_loop () =
    let e = Events.poll_event () in
    match e with
    | None -> ()
    | Some ev ->
        match ev with
        | Quit_event -> Sdl3.quit (); exit 0
        | Key_down_event c -> key_event KeyDown c; event_loop ()
        | Mouse_button_event (x, y, true) -> mouse_event MouseDown (x, y); event_loop ()
        | _ -> event_loop ()
  in
  let rec aux (es) =
    event_loop ();
    display r t es ();
    let es = update es in
    Timer.delay 80;
    aux (es)
  in
  aux (es)
;;
