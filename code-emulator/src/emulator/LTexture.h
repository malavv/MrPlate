#ifndef _LTEXTURE_H_
#define _LTEXTURE_H_

#include <string>
#include "SDL_ttf.h"

class LTexture {
public:
  LTexture();
  ~LTexture() { free(); }

  bool loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font* font, SDL_Renderer* renderer);
  void render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip, SDL_Renderer* renderer);

  int getWidth() { return mWidth; }
  int getHeight() { return mHeight; }

  void setColor(Uint8 red, Uint8 green, Uint8 blue) { SDL_SetTextureColorMod(mTexture, red, green, blue); }
  void setAlpha(Uint8 alpha) { SDL_SetTextureAlphaMod(mTexture, alpha); }

private:
  void free();

  SDL_Texture * mTexture;

  int mWidth;
  int mHeight;
};

#endif // _LTEXTURE_H_