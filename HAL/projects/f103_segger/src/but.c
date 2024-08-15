#include "but.h"
#include <string.h>


button_ctx Button_Init(GPIO_PIN* pin, uint8_t buttonId)
{
  button_ctx ctx;
  memset (&ctx, 0, sizeof ctx);
  ctx.buttonId = buttonId;
  ctx.pin      = pin;
  ctx.lastClick = 0;
  ctx.n_same_state = 0;
  ctx.lastDown     = 0;
  ctx.durDown      = 0;

  GPIO_Setup_In_Pullup(pin);
  Button_PullUp(&ctx);
  return ctx;
}

void Button_PullDown(button_ctx* ctx) {
  GPIO_Setup_In_Pulldown(ctx->pin);
  ctx->currentState  = FALSE;
  ctx->activeHigh    = TRUE;
  ctx->lastReadValue = ctx->currentState;
}

void Button_PullUp(button_ctx* ctx) {
  GPIO_Setup_In_Pullup(ctx->pin);
  ctx->currentState  = TRUE;
  ctx->activeHigh    = FALSE;
  ctx->lastReadValue = ctx->currentState;
}

BOOL Button_IsPinSet(button_ctx* ctx) {
  return GPIO_IsSet(ctx->pin);
}

BOOL Button_IsDown(button_ctx* ctx) {
  return (ctx->currentState == ctx->activeHigh);
}

BOOL Button_IsUp(button_ctx* ctx) {
  return !Button_IsDown(ctx);
}
