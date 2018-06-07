#include "LTexture.h"

LTexture::LTexture() {
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor, TTF_Font* font, SDL_Renderer* renderer) {
	free();

	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
  if (textSurface == NULL) {
    printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    return mTexture != nullptr;
  }

	mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

	if (mTexture == nullptr) {
		printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
	}	else {
		mWidth = textSurface->w;
		mHeight = textSurface->h;
	}

	SDL_FreeSurface(textSurface);

	return mTexture != nullptr;
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip, SDL_Renderer* renderer) {
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	if (clip != nullptr) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopyEx(renderer, mTexture, clip, &renderQuad, angle, center, flip);
}

void LTexture::free() {
  if (mTexture == nullptr)
    return;

  SDL_DestroyTexture(mTexture);
  mTexture = nullptr;
  mWidth = 0;
  mHeight = 0;
}