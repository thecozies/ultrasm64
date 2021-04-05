#ifndef TUTORIAL_H
#define TUTORIAL_H

extern s32 render_tutorial(s32 onPause);
extern void next_tip(void);
extern void prev_tip(void);
extern void reset_tutorial(void);
extern void render_pause_hint_text(void);
extern s32 gTutorialDone;

#endif
