(* sdl3 interface for ocaml
 * Copyright (c) 2025 Florent Monnier
 * To the extent permitted by law, you can use, modify and redistribute
 * this file.
 *)

type window
type renderer
type texture
type surface

module Scancode = struct
type t =
  | Left
  | Right
  | Up
  | Down
  | A | B | C | D | E | F
  | G | H | I | J | K | L
  | M | N | O | P | Q | R
  | S | T | U | V | W | X
  | Y | Z
  | N_0 | N_1 | N_2 | N_3 | N_4
  | N_5 | N_6 | N_7 | N_8 | N_9
  | Space
  | PageUp
  | PageDown
  | Escape
end

type event =
  | Unhandled_event
  | Mouse_motion of int * int
  | Key_down_event of Scancode.t
  | Event_Quit

external init : unit -> unit
  = "caml_SDL_Init"

external quit : unit -> unit
  = "caml_SDL_Quit"


module Render = struct

  external clear : renderer -> unit
    = "caml_SDL_RenderClear"

  external present : renderer -> unit
    = "caml_SDL_RenderPresent"

  external set_draw_color : renderer -> int * int * int -> unit
    = "caml_SDL_SetRenderDrawColor"

  external create_window_and_renderer : int * int -> window * renderer
    = "caml_SDL_CreateWindowAndRenderer"

  external fill_rect : renderer -> int * int * int * int -> unit
    = "caml_SDL_RenderFillRect"

  external draw_rect : renderer -> int * int * int * int -> unit
    = "caml_SDL_RenderRect"

  external render_texture : renderer -> texture ->
      int * int * int * int ->
      int * int * int * int -> unit
    = "caml_SDL_RenderTexture"

  external destroy_renderer : renderer -> unit
    = "caml_SDL_DestroyRenderer"

  external destroy_window : window -> unit
    = "caml_SDL_DestroyWindow"
end

module Timer = struct
  external get_ticks : unit -> int
    = "caml_SDL_GetTicks"

  external delay : int -> unit
    = "caml_SDL_Delay"
end

module Events = struct
  external poll_event : unit -> event option
    = "caml_SDL_PollEvent"
end

module Surface = struct
  external load_bmp : file:string -> surface
    = "caml_SDL_LoadBMP"

  external destroy : surface -> unit
    = "caml_SDL_DestroySurface"

  external set_color_key : surface -> int * int * int -> unit
    = "caml_SDL_SetSurfaceColorKey"
end

module Texture = struct
  external create_texture_from_surface : renderer -> surface -> texture
    = "caml_SDL_CreateTextureFromSurface"
  external destroy : texture -> unit
    = "caml_SDL_DestroyTexture"
end


module type APP = sig
  type state
  type app_calls = {
    app_init : unit -> window * renderer * state;
    app_iterate : renderer -> state -> unit;
    app_event : event -> state -> state;
    app_quit : window -> renderer -> state -> unit;
  }
  val app : app_calls
end



module MakeApp (App : APP) = struct
  let _state = ref (None : App.state option)
  let _wr = ref (None : (window * renderer) option)

  let some_r v = match v with Some (_, r) -> r | None -> assert false
  let some_wr v = match v with Some (w, r) -> (w, r) | None -> assert false

  let init_callback () =
    let w, r, state = App.app.app_init () in
    _state := Some (state);
    _wr := Some (w, r);
  ;;

  let iterate_callback () =
    match !_state with None -> ()
    | Some state -> App.app.app_iterate (some_r !_wr) state;
  ;;

  let quit_callback () =
    match !_state with None -> ()
    | Some state ->
        let w, r = some_wr !_wr in
        App.app.app_quit w r state;
  ;;

  let event_callback e =
    match !_state with None -> ()
    | Some state ->
        let state = App.app.app_event e state in
        _state := Some (state);
  ;;

  let run () =
    Callback.register "init-callback" (init_callback : unit -> unit) ;;
    Callback.register "iterate-callback" (iterate_callback : unit -> unit) ;;
    Callback.register "event-callback" (event_callback : event -> unit) ;;
    Callback.register "quit-callback" (quit_callback : unit -> unit) ;;
  ;;
end

