/* sdl3 interface for ocaml
 * Copyright (c) 2025 Florent Monnier
 * To the extent permitted by law, you can use, modify and redistribute
 * this file.
 */
#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define CAML_NAME_SPACE 1
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


/* Option values */

static value
Val_some(value v)
{
    CAMLparam1(v);
    CAMLlocal1(some);
    some = caml_alloc(1, 0);
    Store_field(some, 0, v);
    CAMLreturn(some);
}


/* Convert Events to ocaml values */

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

static value
Val_event_key_down(SDL_KeyboardEvent *key)
{
    SDL_Scancode scancode = key->scancode;
    switch (scancode) {
        /* Directions */
        case SDL_SCANCODE_LEFT:
            return Val_key_down_event(0);
        case SDL_SCANCODE_RIGHT:
            return Val_key_down_event(1);
        case SDL_SCANCODE_UP:
            return Val_key_down_event(2);
        case SDL_SCANCODE_DOWN:
            return Val_key_down_event(3);

        case SDL_SCANCODE_SPACE:
            return Val_key_down_event(40);

        case SDL_SCANCODE_PAGEUP:
            return Val_key_down_event(41);

        case SDL_SCANCODE_PAGEDOWN:
            return Val_key_down_event(42);

        default: {
            switch (key->key) {
            case SDLK_A: return Val_key_down_event(4);
            case SDLK_B: return Val_key_down_event(5);
            case SDLK_C: return Val_key_down_event(6);
            case SDLK_D: return Val_key_down_event(7);
            case SDLK_E: return Val_key_down_event(8);
            case SDLK_F: return Val_key_down_event(9);
            case SDLK_G: return Val_key_down_event(10);
            case SDLK_H: return Val_key_down_event(11);
            case SDLK_I: return Val_key_down_event(12);
            case SDLK_J: return Val_key_down_event(13);
            case SDLK_K: return Val_key_down_event(14);
            case SDLK_L: return Val_key_down_event(15);
            case SDLK_M: return Val_key_down_event(16);
            case SDLK_N: return Val_key_down_event(17);
            case SDLK_O: return Val_key_down_event(18);
            case SDLK_P: return Val_key_down_event(19);
            case SDLK_Q: return Val_key_down_event(20);
            case SDLK_R: return Val_key_down_event(21);
            case SDLK_S: return Val_key_down_event(22);
            case SDLK_T: return Val_key_down_event(23);
            case SDLK_U: return Val_key_down_event(24);
            case SDLK_V: return Val_key_down_event(25);
            case SDLK_W: return Val_key_down_event(26);
            case SDLK_X: return Val_key_down_event(27);
            case SDLK_Y: return Val_key_down_event(28);
            case SDLK_Z: return Val_key_down_event(29);

            case SDLK_0: return Val_key_down_event(30);
            case SDLK_1: return Val_key_down_event(31);
            case SDLK_2: return Val_key_down_event(32);
            case SDLK_3: return Val_key_down_event(33);
            case SDLK_4: return Val_key_down_event(34);
            case SDLK_5: return Val_key_down_event(35);
            case SDLK_6: return Val_key_down_event(36);
            case SDLK_7: return Val_key_down_event(37);
            case SDLK_8: return Val_key_down_event(38);
            case SDLK_9: return Val_key_down_event(39);
            }
        }
    }
}


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
caml_SDL_Quit(value u)
{
    SDL_Quit();
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

CAMLprim value
caml_SDL_PollEvent(value u)
{
    SDL_Event event;
    bool r = SDL_PollEvent(&event);
    if (!r) return Val_none;
    switch (event.type) {
    case SDL_EVENT_QUIT:
        return Val_some(Val_int(1));
    case SDL_EVENT_MOUSE_MOTION:
        return Val_some(Val_mouse_motion_event(event.motion.x, event.motion.y));
    default:
        return Val_some(Val_int(0));
    }
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


SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    value val_event;
    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;

    case SDL_EVENT_KEY_DOWN:
      { SDL_Scancode scancode = event->key.scancode;
        val_event = Val_int(0);
        switch (scancode) {
        /* Quit */
        case SDL_SCANCODE_ESCAPE:
        //case SDL_SCANCODE_Q:
            return SDL_APP_SUCCESS;
            break;
        default:
            val_event = Val_event_key_down(&(event->key));
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

