#pragma once



// Based on GLFW's keycodes.

// TODO: Replace these with platform specific keycodes in the future.


#define 	RGF_KEY_UNKNOWN   -1
#define 	RGF_KEY_SPACE   32
#define 	RGF_KEY_APOSTROPHE   39 /* ' */
#define 	RGF_KEY_COMMA   44 /* , */
#define 	RGF_KEY_MINUS   45 /* - */
#define 	RGF_KEY_PERIOD   46 /* . */
#define 	RGF_KEY_SLASH   47 /* / */
#define 	RGF_KEY_0   48
#define 	RGF_KEY_1   49
#define 	RGF_KEY_2   50
#define 	RGF_KEY_3   51
#define 	RGF_KEY_4   52
#define 	RGF_KEY_5   53
#define 	RGF_KEY_6   54
#define 	RGF_KEY_7   55
#define 	RGF_KEY_8   56
#define 	RGF_KEY_9   57
#define 	RGF_KEY_SEMICOLON   59 /* ; */
#define 	RGF_KEY_EQUAL   61 /* = */
#define 	RGF_KEY_A   65
#define 	RGF_KEY_B   66
#define 	RGF_KEY_C   67
#define 	RGF_KEY_D   68
#define 	RGF_KEY_E   69
#define 	RGF_KEY_F   70
#define 	RGF_KEY_G   71
#define 	RGF_KEY_H   72
#define 	RGF_KEY_I   73
#define 	RGF_KEY_J   74
#define 	RGF_KEY_K   75
#define 	RGF_KEY_L   76
 
#define 	RGF_KEY_M   77
 
#define 	RGF_KEY_N   78
 
#define 	RGF_KEY_O   79
 
#define 	RGF_KEY_P   80
 
#define 	RGF_KEY_Q   81
 
#define 	RGF_KEY_R   82
 
#define 	RGF_KEY_S   83
 
#define 	RGF_KEY_T   84
 
#define 	RGF_KEY_U   85
 
#define 	RGF_KEY_V   86
 
#define 	RGF_KEY_W   87
 
#define 	RGF_KEY_X   88
 
#define 	RGF_KEY_Y   89
 
#define 	RGF_KEY_Z   90
 
#define 	RGF_KEY_LEFT_BRACKET   91 /* [ */
 
#define 	RGF_KEY_BACKSLASH   92 /* \ */
 
#define 	RGF_KEY_RIGHT_BRACKET   93 /* ] */
 
#define 	RGF_KEY_GRAVE_ACCENT   96 /* ` */
 
#define 	RGF_KEY_WORLD_1   161 /* non-US #1 */
 
#define 	RGF_KEY_WORLD_2   162 /* non-US #2 */
 
#define 	RGF_KEY_ESCAPE   256
 
#define 	RGF_KEY_ENTER   257
 
#define 	RGF_KEY_TAB   258
 
#define 	RGF_KEY_BACKSPACE   259
 
#define 	RGF_KEY_INSERT   260
 
#define 	RGF_KEY_DELETE   261
 
#define 	RGF_KEY_RIGHT   262
 
#define 	RGF_KEY_LEFT   263
 
#define 	RGF_KEY_DOWN   264
 
#define 	RGF_KEY_UP   265
 
#define 	RGF_KEY_PAGE_UP   266
 
#define 	RGF_KEY_PAGE_DOWN   267
 
#define 	RGF_KEY_HOME   268
 
#define 	RGF_KEY_END   269
 
#define 	RGF_KEY_CAPS_LOCK   280
 
#define 	RGF_KEY_SCROLL_LOCK   281
 
#define 	RGF_KEY_NUM_LOCK   282
 
#define 	RGF_KEY_PRINT_SCREEN   283
 
#define 	RGF_KEY_PAUSE   284
 
#define 	RGF_KEY_F1   290
 
#define 	RGF_KEY_F2   291
 
#define 	RGF_KEY_F3   292
 
#define 	RGF_KEY_F4   293
 
#define 	RGF_KEY_F5   294
 
#define 	RGF_KEY_F6   295
 
#define 	RGF_KEY_F7   296
 
#define 	RGF_KEY_F8   297
 
#define 	RGF_KEY_F9   298
 
#define 	RGF_KEY_F10   299
 
#define 	RGF_KEY_F11   300
 
#define 	RGF_KEY_F12   301
 
#define 	RGF_KEY_F13   302
 
#define 	RGF_KEY_F14   303
 
#define 	RGF_KEY_F15   304
 
#define 	RGF_KEY_F16   305
 
#define 	RGF_KEY_F17   306
 
#define 	RGF_KEY_F18   307
 
#define 	RGF_KEY_F19   308
#define 	RGF_KEY_F20   309
#define 	RGF_KEY_F21   310
#define 	RGF_KEY_F22   311
#define 	RGF_KEY_F23   312
#define 	RGF_KEY_F24   313
#define 	RGF_KEY_F25   314
#define 	RGF_KEY_KP_0   320
#define 	RGF_KEY_KP_1   321
#define 	RGF_KEY_KP_2   322
#define 	RGF_KEY_KP_3   323
#define 	RGF_KEY_KP_4   324
#define 	RGF_KEY_KP_5   325
#define 	RGF_KEY_KP_6   326
#define 	RGF_KEY_KP_7   327
#define 	RGF_KEY_KP_8   328
#define 	RGF_KEY_KP_9   329
#define 	RGF_KEY_KP_DECIMAL   330
#define 	RGF_KEY_KP_DIVIDE   331
#define 	RGF_KEY_KP_MULTIPLY   332
#define 	RGF_KEY_KP_SUBTRACT   333
#define 	RGF_KEY_KP_ADD   334
#define 	RGF_KEY_KP_ENTER   335
#define 	RGF_KEY_KP_EQUAL   336
#define 	RGF_KEY_LEFT_SHIFT   340
#define 	RGF_KEY_LEFT_CONTROL   341
#define 	RGF_KEY_LEFT_ALT   342
#define 	RGF_KEY_LEFT_SUPER   343
#define 	RGF_KEY_RIGHT_SHIFT   344
#define 	RGF_KEY_RIGHT_CONTROL   345
#define 	RGF_KEY_RIGHT_ALT   346 
#define 	RGF_KEY_RIGHT_SUPER   347
#define 	RGF_KEY_MENU   348
#define 	RGF_KEY_LAST   RGF_KEY_MENU