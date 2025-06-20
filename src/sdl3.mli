type window
type renderer
type texture
type surface

module Scancode : sig
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
end

type event =
  | Unhandled_event
  | Mouse_motion of int * int
  | Key_down_event of Scancode.t
  | Event_Quit

val init : unit -> unit
val quit : unit -> unit

module Render : sig
  val create_window_and_renderer : int * int -> window * renderer
  val set_draw_color : renderer -> int * int * int -> unit
  val fill_rect : renderer -> int * int * int * int -> unit
  val draw_rect : renderer -> int * int * int * int -> unit
  val render_texture : renderer -> texture -> int * int * int * int -> int * int * int * int -> unit
  val clear : renderer -> unit
  val present : renderer -> unit
  val destroy_renderer : renderer -> unit
  val destroy_window : window -> unit
end

module Events : sig
  val poll_event : unit -> event option
end

module Timer : sig
  val get_ticks : unit -> int
  val delay : int -> unit
end

module Surface : sig
  val load_bmp : file:string -> surface
  val destroy : surface -> unit
  val set_color_key : surface -> int * int * int -> unit
end

module Texture : sig
  val create_texture_from_surface : renderer -> surface -> texture
  val destroy : texture -> unit
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

module MakeApp : functor (App : APP) -> sig
  val run : unit -> unit
end

