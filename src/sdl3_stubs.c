/* sdl3 interface for ocaml
 * Copyright (c) 2025 Florent Monnier
 * To the extent permitted by law, you can use, modify and redistribute
 * this file.
 */
#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <caml/mlvalues.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/custom.h>
#include <caml/alloc.h>
#include <caml/fail.h>

/* Converting Types */

/* Creating an ocaml value encapsulating a pointer to the SDL_Window */
static value Val_sdlwindow(SDL_Window *w)
{
    value v = caml_alloc(1, Abstract_tag);
    *((SDL_Window **) Data_abstract_val(v)) = w;
    return v;
}

#define SDL_Window_val(v) \
  *((SDL_Window **) Data_abstract_val(v))


/* Creating an ocaml value encapsulating a pointer to the SDL_Renderer */
static value Val_sdlrenderer(SDL_Renderer *r)
{
    value v = caml_alloc(1, Abstract_tag);
    *((SDL_Renderer **) Data_abstract_val(v)) = r;
    return v;
}

#define SDL_Renderer_val(v) \
  *((SDL_Renderer **) Data_abstract_val(v))


/* Function Bindings */

CAMLprim value
caml_SDL_Init(value u)
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        caml_failwith("SDL_Init()");
    }
    return Val_unit;
}

CAMLprim value
caml_SDL_SetRenderDrawColor(value r, value c)
{
    SDL_SetRenderDrawColor(SDL_Renderer_val(r),
       	Int_val(Field(c, 0)),
       	Int_val(Field(c, 1)),
       	Int_val(Field(c, 2)), SDL_ALPHA_OPAQUE);

    return Val_unit;
}

CAMLprim value
caml_SDL_RenderClear(value r)
{
    SDL_RenderClear(SDL_Renderer_val(r));
    return Val_unit;
}

CAMLprim value
caml_SDL_RenderPresent(value r)
{
    SDL_RenderPresent(SDL_Renderer_val(r));
    return Val_unit;
}


CAMLprim value
caml_SDL_CreateWindowAndRenderer(value size)
{
    CAMLparam1(size);
    CAMLlocal1(win_n_rnd);
 
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_WindowFlags window_flags = 0;
    int width  = Int_val(Field(size, 0));
    int height = Int_val(Field(size, 1));

    if (!SDL_CreateWindowAndRenderer("sdl3", width, height, window_flags, &window, &renderer)) {
        caml_failwith("SDL_CreateWindowAndRenderer()");
    }

    win_n_rnd = caml_alloc(2, 0);

    Store_field(win_n_rnd, 0, Val_sdlwindow(window) );
    Store_field(win_n_rnd, 1, Val_sdlrenderer(renderer) );
 
    CAMLreturn(win_n_rnd);
}

CAMLprim value
caml_SDL_RenderFillRect(value rn, value rc)
{
    SDL_FRect r;
    r.x = Int_val(Field(rc, 0));
    r.y = Int_val(Field(rc, 1));
    r.w = Int_val(Field(rc, 2));
    r.h = Int_val(Field(rc, 3));
    SDL_RenderFillRect(SDL_Renderer_val(rn), &r);
    return Val_unit;
}

CAMLprim value
caml_SDL_DestroyRenderer(value r)
{
    SDL_DestroyRenderer(SDL_Renderer_val(r));
    return Val_unit;
}

CAMLprim value
caml_SDL_DestroyWindow(value w)
{
    SDL_DestroyWindow(SDL_Window_val(w));
    return Val_unit;
}

CAMLprim value
caml_SDL_GetTicks(value u)
{
    Uint64 t = SDL_GetTicks();
    return Val_long(t);
}

CAMLprim value
caml_SDL_Delay(value ms)
{
    SDL_Delay(Long_val(ms));
    return Val_unit;
}


/* Caml Callbacks */

static const value * closure_init = NULL;
static const value * closure_iterate = NULL;
static const value * closure_event = NULL;
static const value * closure_quit = NULL;


/* SDL3 Callbacks */

SDL_AppResult SDL_AppIterate(void *appstate)
{
    caml_callback(*closure_iterate, Val_unit);
    return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    char *caml_argv[] = { "caml_argv", NULL };
    caml_main(caml_argv);

    closure_init = caml_named_value("init-callback");
    closure_iterate = caml_named_value("iterate-callback");
    closure_event = caml_named_value("event-callback");
    closure_quit = caml_named_value("quit-callback");

    caml_callback(*closure_init, Val_unit);

    return SDL_APP_CONTINUE;
}


static value
Val_mouse_motion_event(int x, int y)
{
    CAMLparam0();
    CAMLlocal1(m);
    m = caml_alloc(2, 0);
    Store_field(m, 0, Val_int(x));
    Store_field(m, 1, Val_int(y));
    CAMLreturn(m);
}

static value
Val_key_down_event(int c)
{
    CAMLparam0();
    CAMLlocal1(m);
    m = caml_alloc(1, 1);
    Store_field(m, 0, Val_int(c));
    CAMLreturn(m);
}


SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    value val_event;
    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;

    case SDL_EVENT_KEY_DOWN:
      { SDL_Scancode key_code = event->key.scancode;
        val_event = Val_int(0);
        switch (key_code) {
        /* Quit */
        case SDL_SCANCODE_ESCAPE:
        //case SDL_SCANCODE_Q:
            return SDL_APP_SUCCESS;

        /* Directions */
        case SDL_SCANCODE_LEFT:
            val_event = Val_key_down_event(0);
            break;
        case SDL_SCANCODE_RIGHT:
            val_event = Val_key_down_event(1);
            break;
        case SDL_SCANCODE_UP:
            val_event = Val_key_down_event(2);
            break;
        case SDL_SCANCODE_DOWN:
            val_event = Val_key_down_event(3);
            break;

        case SDL_SCANCODE_SPACE:
            val_event = Val_key_down_event(40);
            break;

        case SDL_SCANCODE_PAGEUP:
            val_event = Val_key_down_event(41);
            break;

        case SDL_SCANCODE_PAGEDOWN:
            val_event = Val_key_down_event(42);
            break;

        default: {
            switch (event->key.key) {
            case SDLK_A: val_event = Val_key_down_event(4); break;
            case SDLK_B: val_event = Val_key_down_event(5); break;
            case SDLK_C: val_event = Val_key_down_event(6); break;
            case SDLK_D: val_event = Val_key_down_event(7); break;
            case SDLK_E: val_event = Val_key_down_event(8); break;
            case SDLK_F: val_event = Val_key_down_event(9); break;
            case SDLK_G: val_event = Val_key_down_event(10); break;
            case SDLK_H: val_event = Val_key_down_event(11); break;
            case SDLK_I: val_event = Val_key_down_event(12); break;
            case SDLK_J: val_event = Val_key_down_event(13); break;
            case SDLK_K: val_event = Val_key_down_event(14); break;
            case SDLK_L: val_event = Val_key_down_event(15); break;
            case SDLK_M: val_event = Val_key_down_event(16); break;
            case SDLK_N: val_event = Val_key_down_event(17); break;
            case SDLK_O: val_event = Val_key_down_event(18); break;
            case SDLK_P: val_event = Val_key_down_event(19); break;
            case SDLK_Q: val_event = Val_key_down_event(20); break;
            case SDLK_R: val_event = Val_key_down_event(21); break;
            case SDLK_S: val_event = Val_key_down_event(22); break;
            case SDLK_T: val_event = Val_key_down_event(23); break;
            case SDLK_U: val_event = Val_key_down_event(24); break;
            case SDLK_V: val_event = Val_key_down_event(25); break;
            case SDLK_W: val_event = Val_key_down_event(26); break;
            case SDLK_X: val_event = Val_key_down_event(27); break;
            case SDLK_Y: val_event = Val_key_down_event(28); break;
            case SDLK_Z: val_event = Val_key_down_event(29); break;

            case SDLK_0: val_event = Val_key_down_event(30); break;
            case SDLK_1: val_event = Val_key_down_event(31); break;
            case SDLK_2: val_event = Val_key_down_event(32); break;
            case SDLK_3: val_event = Val_key_down_event(33); break;
            case SDLK_4: val_event = Val_key_down_event(34); break;
            case SDLK_5: val_event = Val_key_down_event(35); break;
            case SDLK_6: val_event = Val_key_down_event(36); break;
            case SDLK_7: val_event = Val_key_down_event(37); break;
            case SDLK_8: val_event = Val_key_down_event(38); break;
            case SDLK_9: val_event = Val_key_down_event(39); break;
            }
          } break;
        }
      } break;

    case SDL_EVENT_MOUSE_MOTION:
        val_event = Val_mouse_motion_event(event->motion.x, event->motion.y);
        break;

    default:
        val_event = Val_int(0);
        break;
    }

    caml_callback(*closure_event, val_event);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    caml_callback(*closure_quit, Val_unit);
}

